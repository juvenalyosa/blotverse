#include <doctest/doctest.h>

#include <cmath>
#include <numbers>

#include "blotverse/imaging/background.hpp"
#include "blotverse/imaging/detect.hpp"
#include "blotverse/imaging/qc.hpp"
#include "blotverse/imaging/quantify.hpp"
#include "blotverse/sim/simulator.hpp"

using namespace blotverse;

namespace {

// Gaussian band of the given peak at (xc, yc) added to an image.
void add_gaussian(ImageF& img, double xc, double yc, double sx, double sy, double peak) {
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x) {
      const double u = (x - xc) / sx, v = (y - yc) / sy;
      img(x, y) += static_cast<float>(peak * std::exp(-0.5 * (u * u + v * v)));
    }
}

Annotation two_lane_annotation() {
  Annotation ann;
  ann.lanes = {{40, 30}, {90, 30}};
  ann.target_y = {40, 40};
  ann.reference_y = {100, 100};
  ann.band_half_height = 10;
  return ann;
}

}  // namespace

TEST_CASE("rolling ball: flat background is recovered and bands are removed") {
  // Radii at least as large as the band height (as the spec prescribes).
  for (double radius : {20.0, 40.0, 80.0}) {
    ImageF img(160, 140, 500.0f);
    add_gaussian(img, 80, 70, 7, 3, 10000);
    const ImageF bg = rolling_ball_background(img, radius);
    // Background under the band stays near the flat level.
    CHECK(bg(80, 70) == doctest::Approx(500.0).epsilon(0.02));
    CHECK(bg(10, 10) == doctest::Approx(500.0).epsilon(0.01));
    const ImageF corrected = subtract(img, bg);
    CHECK(corrected(80, 70) == doctest::Approx(10000.0).epsilon(0.03));
  }
}

TEST_CASE("rolling ball smaller than the band eats into it (why radius is a decision)") {
  ImageF img(160, 140, 500.0f);
  add_gaussian(img, 80, 70, 7, 3, 10000);
  const ImageF bg = rolling_ball_background(img, 8.0);
  CHECK(bg(80, 70) > 700.0f);
}

TEST_CASE("rolling ball follows a linear gradient without smoothing") {
  ImageF img(120, 100);
  for (int y = 0; y < 100; ++y)
    for (int x = 0; x < 120; ++x) img(x, y) = static_cast<float>(1000 + 20 * x);
  const ImageF bg = rolling_ball_background(img, 20.0, false);
  // Interior pixels of a pure ramp: the ball touches everywhere.
  CHECK(bg(60, 50) == doctest::Approx(img(60, 50)).epsilon(0.01));
  for (std::size_t i = 0; i < img.size(); ++i) CHECK(bg.data()[i] <= img.data()[i] + 1e-3f);
}

TEST_CASE("quantification methods integrate a known band") {
  const auto ann = two_lane_annotation();
  ImageF img(130, 140, 0.0f);
  const double sx = 5, sy = 3, peak = 1000;
  add_gaussian(img, 40, 40, sx, sy, peak);
  add_gaussian(img, 40, 100, sx, sy, 2 * peak);
  const double integral = peak * 2 * std::numbers::pi * sx * sy;

  const double rect = quantify_band(img, ann, 0, BandKind::Target, BandMethod::FixedRectangle);
  CHECK(rect == doctest::Approx(integral).epsilon(0.01));
  const double ref = quantify_band(img, ann, 0, BandKind::Reference, BandMethod::FixedRectangle);
  CHECK(ref / rect == doctest::Approx(2.0).epsilon(1e-6));
  CHECK(quantify_band(img, ann, 0, BandKind::Target, BandMethod::ProfilePeak) ==
        doctest::Approx(integral).epsilon(0.03));  // baseline removes part of the tails
  const double mask = quantify_band(img, ann, 0, BandKind::Target, BandMethod::ThresholdMask);
  CHECK(mask > 0.5 * integral);
  CHECK(mask <= rect + 1e-6);
  // Empty lane gives (near) zero signal.
  CHECK(quantify_band(img, ann, 1, BandKind::Target, BandMethod::FixedRectangle) == doctest::Approx(0.0));
}

TEST_CASE("local background removes a constant offset per band") {
  const auto ann = two_lane_annotation();
  ImageF img(130, 140, 300.0f);
  add_gaussian(img, 40, 40, 5, 3, 1000);
  ImageF clean(130, 140, 0.0f);
  add_gaussian(clean, 40, 40, 5, 3, 1000);
  const ImageF corrected = local_background_bands(img, ann);
  const double a = quantify_band(corrected, ann, 0, BandKind::Target, BandMethod::FixedRectangle);
  const double b = quantify_band(clean, ann, 0, BandKind::Target, BandMethod::FixedRectangle);
  CHECK(a == doctest::Approx(b).epsilon(0.01));
  CHECK(check_local_background(ann, 130, 140).valid);
}

TEST_CASE("local background is flagged when bands crowd each other") {
  auto ann = two_lane_annotation();
  ann.reference_y = {58, 58};  // strips of the target overlap the reference window
  const auto check = check_local_background(ann, 130, 140);
  CHECK_FALSE(check.valid);
  CHECK(check.reason.find("overlap") != std::string::npos);
}

TEST_CASE("detection finds simulated lanes and bands") {
  for (auto level : {ArtifactLevel::Clean, ArtifactLevel::Moderate}) {
    SimParams p = sim_preset(level);
    p.n_gels = 1;
    p.reps_per_gel = 3;
    p.seed = 11;
    const auto sim = simulate(p);
    const auto& blot = sim.blots[0];
    BandHints hints;
    hints.target_y = p.target_y;
    hints.reference_y = p.reference_y;
    const Annotation ann = detect_annotation(to_float(blot.image), blot.lanes.size(), hints);
    REQUIRE(ann.lane_count() == blot.lanes.size());
    for (std::size_t i = 0; i < ann.lane_count(); ++i) {
      CHECK(std::fabs(ann.lanes[i].x_center - blot.lanes[i].x) < 3.0);
      CHECK(std::fabs(ann.target_y[i] - blot.lanes[i].target_y) <= 2.0);
      CHECK(std::fabs(ann.reference_y[i] - blot.lanes[i].reference_y) <= 2.0);
    }
    CHECK(ann.band_half_height >= 6);
  }
}

TEST_CASE("detection without hints on a clean blot") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 1;
  p.seed = 5;
  const auto sim = simulate(p);
  const Annotation ann = detect_annotation(to_float(sim.blots[0].image), sim.blots[0].lanes.size(), {});
  CHECK(std::fabs(ann.target_y[0] - p.target_y) <= 2.0);
  CHECK(std::fabs(ann.reference_y[0] - p.reference_y) <= 2.0);
}

TEST_CASE("saturated bands are flagged and their lanes excluded") {
  const auto ann = two_lane_annotation();
  ImageU16 px(130, 140, 100);
  px(40, 40) = 65535;
  const RawImage raw = make_raw_image(px, "sat");
  const BlotQc qc = check_blot("b", raw, nullptr, ann, 1);
  CHECK(qc.saturated_bands == 1);
  CHECK(qc.lanes[0].excluded);
  CHECK_FALSE(qc.lanes[1].excluded);
  CHECK(count_saturated(px, Rect{0, 0, 130, 140}, 65535) == 1);
}

TEST_CASE("linear range flags the flattening part of a dilution series") {
  const std::vector<double> load{1, 2, 4, 8, 16, 32};
  const std::vector<double> signal{10, 20, 40, 78, 100, 110};
  const auto flags = linear_range_flags(load, signal, 0.8);
  CHECK(flags == std::vector<bool>{false, false, false, false, true, true});
}
