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
  const HarnessMetrics m = compute_metrics(again.records, spec.alpha);
  CHECK(m.e3_null_robust_rate == report.metrics.e3_null_robust_rate);
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
  recs.push_back(make({0.1, 0.2, 0.4, 0, 0, 0}, {0.2, 0.25, 0.42, 0, 0, 0}));
  // Loading rises most: miss.
  recs.push_back(make({0.1, 0.2, 0.4, 0, 0, 0}, {0.1, 0.2, 0.6, 0, 0, 0}));
  // Background-handling influence falls: miss.
  recs.push_back(make({0.3, 0.3, 0.2, 0, 0, 0}, {0.2, 0.2, 0.2, 0, 0, 0}));
  const HarnessMetrics m = compute_metrics(recs, 0.05);
  CHECK(m.e5_paired_hit_rate == doctest::Approx(1.0 / 3.0));
  CHECK(m.e5_rate() == m.e5_paired_hit_rate);
}
