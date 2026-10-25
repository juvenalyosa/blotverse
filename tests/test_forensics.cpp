#include <doctest/doctest.h>

#include <algorithm>
#include <cmath>

#include "blotverse/core/rng.hpp"
#include "blotverse/forensics/forensics.hpp"
#include "blotverse/io/tiff.hpp"

using namespace blotverse;

namespace {

// A blot-like image: background, two rows of identical bands, Poisson-like noise.
RawImage synthetic_blot(std::uint64_t seed, int width = 320, int height = 200) {
  Rng rng(seed);
  ImageU16 img(width, height);
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      double v = 1000.0;
      for (int lane = 0; lane < 6; ++lane) {
        const double cx = 30 + lane * 48.0;
        for (double cy : {60.0, 140.0}) {
          const double dx = (x - cx) / 14.0, dy = (y - cy) / 4.0;
          if (std::fabs(dx) < 1.0) v += 8000.0 * std::exp(-0.5 * dy * dy);
        }
      }
      v += rng.normal(0.0, std::sqrt(v) + 10.0);
      img(x, y) = static_cast<std::uint16_t>(std::clamp(std::round(v), 0.0, 65000.0));
    }
  return make_raw_image(std::move(img), "synthetic");
}

ForensicsResult screen(const std::vector<RawImage>& images) {
  std::vector<std::string> names;
  for (std::size_t i = 0; i < images.size(); ++i) names.push_back("img" + std::to_string(i));
  return analyze_forensics(images, names, ForensicsOptions{});
}

void paste(RawImage& dst, const RawImage& src, Rect from, int x, int y, double gain = 1.0, bool flip = false) {
  for (int v = 0; v < from.height(); ++v)
    for (int u = 0; u < from.width(); ++u) {
      const int su = flip ? from.width() - 1 - u : u;
      dst.pixels(x + u, y + v) =
          static_cast<std::uint16_t>(std::round(gain * src.pixels(from.x0 + su, from.y0 + v)));
    }
  dst.meta.sha256 = pixel_fingerprint(dst.pixels);
}

std::size_t copies(const ForensicsResult& r) {
  return static_cast<std::size_t>(std::count_if(r.duplicates.begin(), r.duplicates.end(),
                                                [](const DuplicateRegion& d) { return d.kind == DuplicateKind::copy; }));
}

}  // namespace

TEST_CASE("forensics: an untouched image has no findings, even with identical band shapes") {
  const auto r = screen({synthetic_blot(1)});
  CHECK(r.duplicates.empty());
  CHECK(r.seams.empty());
  CHECK(r.flats.empty());
}

TEST_CASE("forensics: a pasted band is found, with gain change and flipped") {
  const RawImage clean = synthetic_blot(2);
  const Rect band{20, 45, 60, 75};

  RawImage exact = clean;
  paste(exact, clean, band, 200, 100);
  auto r = screen({exact});
  REQUIRE(copies(r) == 1);
  const auto& d = r.duplicates[0];
  CHECK(d.orientation == Orientation::identity);
  CHECK(d.identical_fraction > 0.9);
  const bool forward = d.target.intersects(band) && d.source.intersects(Rect{200, 100, 240, 130});
  const bool backward = d.source.intersects(band) && d.target.intersects(Rect{200, 100, 240, 130});
  CHECK((forward || backward));

  RawImage scaled = clean;
  paste(scaled, clean, band, 200, 100, 0.7);
  r = screen({scaled});
  REQUIRE(copies(r) == 1);
  // The pair is reported from the earlier patch, here the original: gain 1 / 0.7.
  const double g = r.duplicates[0].gain;
  CHECK((std::fabs(g - 0.7) < 0.03 || std::fabs(g - 1 / 0.7) < 0.06));

  RawImage flipped = clean;
  paste(flipped, clean, band, 200, 100, 1.0, true);
  r = screen({flipped});
  REQUIRE(copies(r) == 1);
  CHECK(r.duplicates[0].orientation == Orientation::flip_h);
}

TEST_CASE("forensics: two captures of the same scene are a scene match, not a copy") {
  // Same signal and the same fixed texture, independent photon noise.
  RawImage a = synthetic_blot(3), b = synthetic_blot(4);
  Rng rng(9);
  ImageU16 texture(a.pixels.width(), a.pixels.height());
  for (auto& v : texture.data()) v = static_cast<std::uint16_t>(rng.uniform_int(400));
  for (std::size_t i = 0; i < a.pixels.size(); ++i) {
    a.pixels.data()[i] = static_cast<std::uint16_t>(a.pixels.data()[i] + texture.data()[i]);
    b.pixels.data()[i] = static_cast<std::uint16_t>(b.pixels.data()[i] + texture.data()[i]);
  }
  a.meta.sha256 = pixel_fingerprint(a.pixels);
  b.meta.sha256 = pixel_fingerprint(b.pixels);
  const auto r = screen({a, b});
  CHECK(copies(r) == 0);
  REQUIRE(r.duplicates.size() == 1);
  CHECK(r.duplicates[0].kind == DuplicateKind::same_scene);
}

TEST_CASE("forensics: a strip from another image leaves a seam; a painted area is flat") {
  RawImage spliced = synthetic_blot(5);
  RawImage other = synthetic_blot(6);
  for (auto& v : other.pixels.data()) v = static_cast<std::uint16_t>(v + 300);  // different background
  for (int y = 0; y < spliced.pixels.height(); ++y)
    for (int x = 100; x < 160; ++x) spliced.pixels(x, y) = other.pixels(x, y);
  spliced.meta.sha256 = pixel_fingerprint(spliced.pixels);
  auto r = screen({spliced});
  CHECK(r.duplicates.empty());
  REQUIRE(!r.seams.empty());
  for (const auto& s : r.seams) {
    CHECK(s.vertical);
    CHECK((std::abs(s.position - 100) <= 1 || std::abs(s.position - 160) <= 1));
  }

  RawImage erased = synthetic_blot(7);
  for (int y = 50; y < 82; ++y)
    for (int x = 64; x < 112; ++x) erased.pixels(x, y) = 1000;
  erased.meta.sha256 = pixel_fingerprint(erased.pixels);
  r = screen({erased});
  REQUIRE(r.flats.size() == 1);
  CHECK(r.flats[0].rect.intersects(Rect{64, 50, 112, 82}));
}
