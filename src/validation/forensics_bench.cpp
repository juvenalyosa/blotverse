#include "blotverse/validation/forensics_bench.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <stdexcept>

#include "blotverse/core/numeric.hpp"
#include "blotverse/core/rng.hpp"
#include "blotverse/io/image_file.hpp"
#include "blotverse/validation/metrics.hpp"

namespace blotverse {

const char* tamper_label(Tamper t) {
  switch (t) {
    case Tamper::none: return "none";
    case Tamper::copy: return "copy";
    case Tamper::copy_gain: return "copy_gain";
    case Tamper::copy_flip: return "copy_flip";
    case Tamper::copy_noise: return "copy_noise";
    case Tamper::cross_copy: return "cross_copy";
    case Tamper::splice: return "splice";
    case Tamper::erase: return "erase";
  }
  return "?";
}

namespace {

double pixel_noise(const ImageU16& img) {
  std::vector<double> d;
  d.reserve(img.size());
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x + 1 < img.width(); ++x) d.push_back(std::fabs(double(img(x + 1, y)) - img(x, y)));
  return d.empty() ? 0.0 : 1.4826 * median(std::move(d)) / std::sqrt(2.0);
}

constexpr int kMargin = 8;

Rect random_rect(Rng& rng, int width, int height, int w, int h) {
  const int x = kMargin + int(rng.uniform_int(std::uint64_t(std::max(1, width - w - 2 * kMargin))));
  const int y = kMargin + int(rng.uniform_int(std::uint64_t(std::max(1, height - h - 2 * kMargin))));
  return {x, y, x + w, y + h};
}

// A rectangle centred on a bright, unsaturated pixel (a band), as a copied or
// erased band would be; random when the image has no such pixel.
Rect bright_rect(Rng& rng, const RawImage& raw, int w, int h) {
  const ImageU16& img = raw.pixels;
  std::vector<std::uint16_t> sorted = img.data();
  const auto q = sorted.begin() + static_cast<std::ptrdiff_t>(sorted.size() * 9 / 10);
  std::nth_element(sorted.begin(), q, sorted.end());
  const std::uint16_t limit = *q;
  for (int attempt = 0; attempt < 2000; ++attempt) {
    const int x = int(rng.uniform_int(std::uint64_t(img.width())));
    const int y = int(rng.uniform_int(std::uint64_t(img.height())));
    const std::uint16_t v = img(x, y);
    if (v < limit || v >= raw.meta.detector_max) continue;
    const int x0 = std::clamp(x - w / 2, kMargin, img.width() - w - kMargin);
    const int y0 = std::clamp(y - h / 2, kMargin, img.height() - h - kMargin);
    return {x0, y0, x0 + w, y0 + h};
  }
  return random_rect(rng, img.width(), img.height(), w, h);
}

bool separated(const Rect& a, const Rect& b, int gap) {
  return a.x1 + gap <= b.x0 || b.x1 + gap <= a.x0 || a.y1 + gap <= b.y0 || b.y1 + gap <= a.y0;
}

double area(const Rect& r) { return r.empty() ? 0.0 : double(r.width()) * r.height(); }

// Fraction of `truth` covered by `found`.
double covered(const Rect& found, const Rect& truth) {
  const Rect i{std::max(found.x0, truth.x0), std::max(found.y0, truth.y0), std::min(found.x1, truth.x1),
               std::min(found.y1, truth.y1)};
  return area(truth) > 0 ? area(i) / area(truth) : 0.0;
}

std::uint16_t to_pixel(double v, std::uint32_t max) {
  return static_cast<std::uint16_t>(std::clamp(std::round(v), 0.0, double(std::min<std::uint32_t>(max, 65535))));
}

std::string fmt(double v, int digits = 3) {
  std::ostringstream s;
  s << std::fixed << std::setprecision(digits) << v;
  return s.str();
}

struct Loaded {
  RawImage raw;
  std::string name, group;
  double noise = 1.0;
};

}  // namespace

ForensicsBenchResult run_forensics_bench(const ForensicsBenchConfig& config,
                                         const std::function<void(std::size_t, std::size_t)>& progress) {
  const auto start = std::chrono::steady_clock::now();
  std::vector<Loaded> images;
  for (const auto& p : config.images) {
    Loaded l;
    l.raw = read_image(p);
    l.name = p.filename().string();
    l.group = p.parent_path().filename().string();
    l.noise = std::max(1.0, pixel_noise(l.raw.pixels));
    images.push_back(std::move(l));
  }
  ForensicsBenchResult result;
  result.options = config.options;
  const int P = config.options.patch;
  const std::size_t total = images.size() * std::size(kTampers) * config.repeats;
  std::size_t done = 0;
  for (std::size_t i = 0; i < images.size(); ++i) {
    const Loaded& base = images[i];
    const int W = base.raw.pixels.width(), H = base.raw.pixels.height();
    for (std::size_t ti = 0; ti < std::size(kTampers); ++ti) {
      const Tamper tamper = kTampers[ti];
      for (std::size_t rep = 0; rep < config.repeats; ++rep) {
        Rng rng(derive_seed(config.seed, (i * 64 + ti) * 4096 + rep));
        ForensicsTrial trial;
        trial.image = base.name;
        trial.tamper = tamper;
        trial.repeat = rep;
        RawImage img = base.raw;
        const std::uint32_t top = img.meta.detector_max;
        // Donors come from another study, so the pair shares no membrane.
        std::vector<std::size_t> donors;
        for (std::size_t j = 0; j < images.size(); ++j)
          if (images[j].group != base.group) donors.push_back(j);
        const Loaded* donor = nullptr;
        if (tamper == Tamper::cross_copy || tamper == Tamper::splice) {
          if (tamper == Tamper::splice)
            donors.erase(std::remove_if(donors.begin(), donors.end(),
                                        [&](std::size_t j) { return images[j].raw.pixels.height() < H; }),
                         donors.end());
          if (donors.empty()) {
            ++done;
            continue;
          }
          donor = &images[donors[std::size_t(rng.uniform_int(donors.size()))]];
          trial.donor = donor->name;
        }

        Orientation orient = Orientation::identity;
        if (tamper == Tamper::copy || tamper == Tamper::copy_gain || tamper == Tamper::copy_flip ||
            tamper == Tamper::copy_noise || tamper == Tamper::cross_copy) {
          const int w = 24 + int(rng.uniform_int(41)), h = 16 + int(rng.uniform_int(17));
          const RawImage& from = donor ? donor->raw : base.raw;
          trial.source = bright_rect(rng, from, std::min(w, from.pixels.width() - 2 * kMargin),
                                     std::min(h, from.pixels.height() - 2 * kMargin));
          const int cw = trial.source.width(), ch = trial.source.height();
          for (int attempt = 0; attempt < 200; ++attempt) {
            trial.truth = random_rect(rng, W, H, cw, ch);
            if (donor || separated(trial.truth, trial.source, P)) break;
          }
          double gain = 1.0, offset = 0.0, sigma = 0.0;
          if (tamper == Tamper::copy_gain) {
            gain = rng.uniform(0.6, 1.5);
            offset = rng.uniform(-5.0, 5.0) * base.noise;
            trial.detail = "gain " + fmt(gain) + " offset " + fmt(offset, 1);
          }
          if (tamper == Tamper::copy_flip) {
            const Orientation o[] = {Orientation::flip_h, Orientation::flip_v, Orientation::rotate_180};
            orient = o[rng.uniform_int(3)];
            trial.detail = orientation_label(orient);
          }
          if (tamper == Tamper::copy_noise) {
            sigma = 0.5 * base.noise;
            trial.detail = "added noise sd " + fmt(sigma, 1);
          }
          const bool fx = orient == Orientation::flip_h || orient == Orientation::rotate_180;
          const bool fy = orient == Orientation::flip_v || orient == Orientation::rotate_180;
          for (int v = 0; v < ch; ++v)
            for (int u = 0; u < cw; ++u) {
              const int sx = trial.source.x0 + (fx ? cw - 1 - u : u), sy = trial.source.y0 + (fy ? ch - 1 - v : v);
              double value = gain * from.pixels(sx, sy) + offset;
              if (sigma > 0) value += rng.normal(0.0, sigma);
              img.pixels(trial.truth.x0 + u, trial.truth.y0 + v) = to_pixel(value, top);
            }
          trial.detail += (trial.detail.empty() ? "" : "; ") + std::to_string(cw) + "x" + std::to_string(ch);
        } else if (tamper == Tamper::splice) {
          const int w = std::min(40 + int(rng.uniform_int(81)), std::min(W, donor->raw.pixels.width()) - 2 * 16);
          const int x0 = 16 + int(rng.uniform_int(std::uint64_t(std::max(1, W - w - 32))));
          const int dx = int(rng.uniform_int(std::uint64_t(std::max(1, donor->raw.pixels.width() - w))));
          for (int y = 0; y < H; ++y)
            for (int u = 0; u < w; ++u) img.pixels(x0 + u, y) = to_pixel(donor->raw.pixels(dx + u, y), top);
          trial.truth = {x0, 0, x0 + w, H};
          trial.detail = "strip " + std::to_string(w) + " px";
        } else if (tamper == Tamper::erase) {
          const int w = 32 + int(rng.uniform_int(33)), h = 16 + int(rng.uniform_int(17));
          trial.truth = bright_rect(rng, base.raw, w, h);
          std::vector<double> ring;
          const Rect& t = trial.truth;
          for (int x = t.x0 - 1; x <= t.x1; ++x) {
            ring.push_back(base.raw.pixels(x, t.y0 - 1));
            ring.push_back(base.raw.pixels(x, t.y1));
          }
          for (int y = t.y0; y < t.y1; ++y) {
            ring.push_back(base.raw.pixels(t.x0 - 1, y));
            ring.push_back(base.raw.pixels(t.x1, y));
          }
          const std::uint16_t fill = to_pixel(median(ring), top);
          for (int y = t.y0; y < t.y1; ++y)
            for (int x = t.x0; x < t.x1; ++x) img.pixels(x, y) = fill;
          trial.detail = "filled with " + std::to_string(fill) + "; " + std::to_string(t.width()) + "x" +
                         std::to_string(t.height());
        }
        img.meta.sha256 = pixel_fingerprint(img.pixels);

        std::vector<RawImage> set{img};
        std::vector<std::string> names{base.name};
        if (tamper == Tamper::cross_copy) {
          set.push_back(donor->raw);
          names.push_back(donor->name);
        }
        const ForensicsResult found = analyze_forensics(set, names, config.options);

        // Score: every finding either matches the injected manipulation or is false.
        std::vector<std::string> kinds;
        for (const auto& r : found.duplicates) {
          bool match = false;
          const bool copy_type = tamper == Tamper::copy || tamper == Tamper::copy_gain || tamper == Tamper::copy_flip ||
                                 tamper == Tamper::copy_noise || tamper == Tamper::cross_copy;
          if (copy_type && (r.kind == DuplicateKind::copy || tamper == Tamper::copy_noise)) {
            const Rect* in_tampered = nullptr;
            const Rect* in_other = nullptr;
            if (r.target_image == 0) {
              in_tampered = &r.target;
              in_other = &r.source;
            }
            if (r.source_image == 0 && r.target_image != 0) {
              in_tampered = &r.source;
              in_other = &r.target;
            }
            if (in_tampered) {
              if (tamper == Tamper::cross_copy)
                match = covered(*in_tampered, trial.truth) >= 0.25 && covered(*in_other, trial.source) >= 0.25;
              else
                match = (covered(r.target, trial.truth) >= 0.25 && covered(r.source, trial.source) >= 0.25) ||
                        (covered(r.source, trial.truth) >= 0.25 && covered(r.target, trial.source) >= 0.25);
            }
          }
          if (match)
            trial.detected = true;
          else
            kinds.push_back(r.kind == DuplicateKind::copy ? "copy" : "same_scene");
        }
        for (const auto& s : found.seams) {
          const bool match = tamper == Tamper::splice && s.vertical &&
                             (std::abs(s.position - trial.truth.x0) <= 2 || std::abs(s.position - trial.truth.x1) <= 2);
          if (match)
            trial.detected = true;
          else
            kinds.push_back("seam");
        }
        for (const auto& f : found.flats) {
          const bool match = tamper == Tamper::erase && covered(f.rect, trial.truth) >= 0.25;
          // A strip from a quieter acquisition is flat relative to the host image: related, not false.
          const bool related = tamper == Tamper::splice && f.rect.intersects(trial.truth);
          if (match)
            trial.detected = true;
          else if (!related)
            kinds.push_back("flat");
        }
        if (!found.identical_files.empty()) kinds.push_back("identical");
        trial.false_findings = kinds.size();
        std::sort(kinds.begin(), kinds.end());
        kinds.erase(std::unique(kinds.begin(), kinds.end()), kinds.end());
        for (const auto& k : kinds) trial.false_kinds += (trial.false_kinds.empty() ? "" : ";") + k;
        result.trials.push_back(std::move(trial));
        if (progress) progress(++done, total);
      }
    }
  }
  result.seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  return result;
}

void write_forensics_bench(const std::filesystem::path& dir, const ForensicsBenchResult& result) {
  std::filesystem::create_directories(dir);
  {
    std::ofstream f(dir / "forensics_bench_trials.csv");
    f << "image,tamper,repeat,donor,truth_x0,truth_y0,truth_x1,truth_y1,source_x0,source_y0,source_x1,source_y1,"
         "detail,detected,false_findings,false_kinds\n";
    for (const auto& t : result.trials)
      f << t.image << ',' << tamper_label(t.tamper) << ',' << t.repeat << ',' << t.donor << ',' << t.truth.x0 << ','
        << t.truth.y0 << ',' << t.truth.x1 << ',' << t.truth.y1 << ',' << t.source.x0 << ',' << t.source.y0 << ','
        << t.source.x1 << ',' << t.source.y1 << ",\"" << t.detail << "\"," << (t.detected ? 1 : 0) << ','
        << t.false_findings << ',' << t.false_kinds << '\n';
  }
  std::set<std::string> names;
  for (const auto& t : result.trials) names.insert(t.image);
  const auto& o = result.options;
  std::ofstream md(dir / "forensics_bench.md");
  md << "# Image-integrity screening: injection bench\n\n"
     << names.size() << " real raw images; " << result.trials.size()
     << " trials. Settings: patch " << o.patch << ", min correlation " << o.min_ncc << ", copy error <= "
     << o.copy_noise_ratio << " x noise, min patches " << o.min_patches << ", seam z >= " << o.seam_z
     << ", seam length >= " << o.seam_min_length << ", flat ratio " << o.flat_ratio << ".\n\n"
     << "| Manipulation | Trials | Detected | Rate (Wilson 95 %) | Trials with a false finding | Rate (Wilson 95 %) "
        "| False findings |\n|---|---|---|---|---|---|---|\n";
  std::size_t all_n = 0, all_false = 0;
  for (Tamper tp : kTampers) {
    std::size_t n = 0, hit = 0, with_false = 0, false_total = 0;
    for (const auto& t : result.trials) {
      if (t.tamper != tp) continue;
      ++n;
      hit += t.detected;
      with_false += t.false_findings > 0;
      false_total += t.false_findings;
    }
    if (n == 0) continue;
    all_n += n;
    all_false += with_false;
    const Interval d = wilson_interval(hit, n), f = wilson_interval(with_false, n);
    md << "| " << tamper_label(tp) << " | " << n << " | " << (tp == Tamper::none ? "-" : std::to_string(hit))
       << " | "
       << (tp == Tamper::none ? "-" : fmt(d.estimate, 2) + " (" + fmt(d.lo, 2) + "-" + fmt(d.hi, 2) + ")") << " | "
       << with_false << " | " << fmt(f.estimate, 3) << " (" << fmt(f.lo, 3) << "-" << fmt(f.hi, 3) << ") | "
       << false_total << " |\n";
  }
  const Interval f = wilson_interval(all_false, all_n);
  md << "\nAll trials: " << all_false << " / " << all_n << " with at least one false finding ("
     << fmt(f.estimate, 3) << ", Wilson 95 % " << fmt(f.lo, 3) << "-" << fmt(f.hi, 3) << ").\n\n"
     << "* `copy`: a band region (24-64 x 16-32 px) pasted elsewhere in the same image. `copy_gain`: the same "
        "with brightness/contrast changed (gain 0.6-1.5, offset +-5 noise). `copy_flip`: flipped or rotated "
        "180 degrees. `copy_noise`: with Gaussian noise of 0.5 x the image noise added (outside the stated "
        "scope of the copy test; reported to show the limit).\n"
     << "* `cross_copy`: a band region of an image from another study pasted into this one; both images are "
        "screened together.\n"
     << "* `splice`: a full-height strip (40-120 px) replaced by the same columns of an image from another "
        "study.\n"
     << "* `erase`: a band region (32-64 x 16-32 px) filled with the median of its border.\n"
     << "* A detection needs a finding of the right kind on the injected region (copies: at least 25 % of both "
        "the pasted and the source region; seams: within 2 px of a strip edge; flat: 25 % of the erased area). "
        "Flat regions inside a spliced strip are related findings, not false. Any other finding is false.\n\nRuntime " << fmt(result.seconds, 0) << " s.\n";
}

}  // namespace blotverse
