#include <doctest/doctest.h>

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <random>

#include "blotverse/io/tiff.hpp"
#include "blotverse/validation/exposure.hpp"
#include "blotverse/validation/harness.hpp"

using namespace blotverse;
namespace fs = std::filesystem;

TEST_CASE("harness outputs can be reloaded and give the same metrics") {
  const MultiverseSpec spec = load_spec(BLOTVERSE_SPEC_PATH);
  HarnessConfig cfg;
  cfg.experiments = 8;
  cfg.gradient_experiments = 2;
  cfg.joint_permutations = 20;
  cfg.seed = 3;
  const HarnessReport report = run_harness(spec, cfg);
  CHECK(report.records.size() == 10);
  CHECK(report.metrics.failed == 0);

  const fs::path dir = fs::temp_directory_path() / "blotverse_tests" / "harness";
  fs::remove_all(dir);
  write_harness_outputs(dir, report);
  const HarnessReport again = read_harness_outputs(dir);
  REQUIRE(again.records.size() == report.records.size());
  CHECK(again.spec_sha256 == report.spec_sha256);
  const HarnessMetrics m = compute_metrics(again.records, spec.alpha, cfg.seed);
  CHECK(m.e3_null_robust.estimate == report.metrics.e3_null_robust.estimate);
  CHECK(m.e2_auc.lo == doctest::Approx(report.metrics.e2_auc.lo));
  for (std::size_t i = 0; i < again.records.size(); ++i) {
    CHECK(again.records[i].verdict == report.records[i].verdict);
    CHECK(again.records[i].most_influential == report.records[i].most_influential);
    CHECK(again.records[i].true_lfc == report.records[i].true_lfc);
    if (std::isfinite(report.records[i].robustness))
      CHECK(again.records[i].robustness == doctest::Approx(report.records[i].robustness).epsilon(1e-9));
  }
}

TEST_CASE("paired E5 criterion") {
  auto make = [](std::array<double, kNumDecisions> base, std::array<double, kNumDecisions> planted) {
    ExperimentRecord r;
    r.set = "background";
    r.paired = true;
    r.baseline_eta2 = base;
    r.eta2 = planted;
    return r;
  };
  std::vector<ExperimentRecord> recs;
  // Background + band rise (0.10 + 0.05) more than loading (0.02): hit.
  for (int i = 0; i < 30; ++i) recs.push_back(make({0.1, 0.2, 0.4, 0, 0, 0}, {0.2, 0.25, 0.42, 0, 0, 0}));
  // Loading rises most: miss (a minority).
  for (int i = 0; i < 5; ++i) recs.push_back(make({0.1, 0.2, 0.4, 0, 0, 0}, {0.1, 0.2, 0.6, 0, 0, 0}));
  const HarnessMetrics m = compute_metrics(recs, 0.05, 1);
  CHECK(m.e5.n == 35);
  CHECK(m.e5.hit_rate.estimate == doctest::Approx(30.0 / 35.0));
  CHECK(m.e5.mean_handling.lo > 0.0);
  CHECK(m.e5_pass());
  // If loading usually rises most, the average criterion fails.
  for (int i = 0; i < 60; ++i) recs.push_back(make({0.1, 0.2, 0.4, 0, 0, 0}, {0.1, 0.2, 0.6, 0, 0, 0}));
  CHECK_FALSE(compute_metrics(recs, 0.05, 1).e5_pass());
}

TEST_CASE("Wilson interval matches reference values") {
  // Reference: statsmodels proportion_confint(5, 100, method="wilson").
  const Interval w = wilson_interval(5, 100);
  CHECK(w.estimate == doctest::Approx(0.05));
  CHECK(w.lo == doctest::Approx(0.021543).epsilon(1e-4));
  CHECK(w.hi == doctest::Approx(0.111750).epsilon(1e-4));
  const Interval zero = wilson_interval(0, 500);
  CHECK(zero.lo == 0.0);
  CHECK(zero.hi == doctest::Approx(0.007625).epsilon(1e-3));
}

TEST_CASE("bootstrap intervals are reproducible and cover the estimate") {
  std::vector<double> v;
  for (int i = 0; i < 200; ++i) v.push_back(i % 17);
  const Interval a = bootstrap_median(v, 7), b = bootstrap_median(v, 7);
  CHECK(a.lo == b.lo);
  CHECK(a.hi == b.hi);
  CHECK(a.lo <= a.estimate);
  CHECK(a.estimate <= a.hi);
  const std::vector<double> pos{0.9, 0.8, 0.85, 0.7, 0.95}, neg{0.1, 0.3, 0.2, 0.75, 0.05};
  const Interval auc = bootstrap_auc(pos, neg, 3);
  CHECK(auc.estimate == doctest::Approx(0.96));
  CHECK(auc.lo <= auc.estimate);
}

namespace {

// Four lanes whose band amplitudes double from lane to lane, imaged at 1, 2,
// 4 and 16 "seconds": signal scales with time and clips at `clip`, so lane 4
// clips only at the longest exposure. With clip below the detector maximum
// the clipping is invisible to the saturation rule (as after a flat-field
// correction that rescales saturated pixels).
ExposureResult synthetic_exposure_series(const fs::path& dir, double clip) {
  const MultiverseSpec spec = load_spec(BLOTVERSE_SPEC_PATH);
  fs::remove_all(dir);
  fs::create_directories(dir);
  const std::array<double, 4> lane_x{50, 110, 170, 230}, amplitude{1000, 2000, 4000, 8000};
  const std::array<double, 4> seconds{1, 2, 4, 16};
  std::mt19937 gen(7);
  std::ofstream cfg(dir / "series.yaml");
  cfg << "name: synthetic\nlane_width: 36\nband_half_height: 6\nexposures:\n";
  for (double t : seconds) {
    ImageU16 img(280, 120);
    for (int y = 0; y < img.height(); ++y)
      for (int x = 0; x < img.width(); ++x) {
        double v = 500 + 2.0 * x / 10 + static_cast<double>(gen() % 21) - 10;
        for (std::size_t i = 0; i < lane_x.size(); ++i)
          if (std::fabs(x - lane_x[i]) < 16) v += t * amplitude[i] * std::exp(-0.5 * std::pow((y - 60) / 3.0, 2));
        img(x, y) = static_cast<std::uint16_t>(std::clamp(v, 0.0, clip));
      }
    const std::string name = "t" + std::to_string(static_cast<int>(t)) + ".tif";
    write_tiff16(dir / name, img);
    cfg << "  - {image: " << name << ", seconds: " << t << "}\n";
  }
  cfg << "lanes: [{x: 50}, {x: 110}, {x: 170}, {x: 230}]\nbands:\n  - {name: band, y: 60}\n";
  cfg.close();
  const ExposureConfig config = load_exposure_config(dir / "series.yaml");
  ExposureResult result = analyze_exposure(config, spec);
  write_exposure_outputs(dir / "out", config, spec, result);
  return result;
}

}  // namespace

TEST_CASE("exposure bench: ratios are invariant until saturation, which the QC rule flags") {
  const fs::path dir = fs::temp_directory_path() / "blotverse_tests" / "exposure";
  const ExposureResult result = synthetic_exposure_series(dir, 65535);
  REQUIRE(result.bands.size() == 1);
  const ExposureBand& band = result.bands.front();
  CHECK(band.reference == 2);  // 4 s: the longest exposure without a saturated lane
  CHECK(band.points[3][3].saturated_px > 0);
  CHECK(band.points[3][0].saturated_px == 0);
  for (const auto& br : band.branches) {
    if (br.background == "local" || br.band_definition != "rect") continue;
    CAPTURE(br.background);
    CHECK(br.clean_n == 15);  // all 6 pairs at 1 and 2 s, the 3 pairs without lane 4 at 16 s
    CHECK(br.clean_q95_abs < 0.02);
    CHECK(br.fn == 0);        // every distorted pair is flagged
    CHECK(br.tp > 0);
    CHECK(br.flagged_median_abs > 0.2);
  }
  CHECK(band.any_flagged[3]);
  CHECK(band.compressive_default[3]);
  CHECK(band.qc_misses == 0);
  CHECK(fs::exists(dir / "out" / "exposure.md"));
  CHECK(fs::exists(dir / "out" / "exposure_pairs.csv"));
}

TEST_CASE("exposure bench: clipping below the detector maximum is reported as a QC miss") {
  const ExposureResult result =
      synthetic_exposure_series(fs::temp_directory_path() / "blotverse_tests" / "exposure_hidden", 60000);
  const ExposureBand& band = result.bands.front();
  CHECK(band.reference == 3);  // nothing reaches the detector maximum, so 16 s becomes the reference
  CHECK_FALSE(band.any_flagged[3]);
  // The clipped reference compresses lane 4's ratios relative to every
  // shorter exposure; the distortion is blamed on it and it carries no flag.
  CHECK(band.compressive_default[3]);
  CHECK(band.qc_misses == 1);
}
