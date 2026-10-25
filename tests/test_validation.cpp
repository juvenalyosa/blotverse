#include <doctest/doctest.h>

#include <array>
#include <cmath>
#include <filesystem>

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
