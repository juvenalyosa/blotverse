#include <doctest/doctest.h>

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
