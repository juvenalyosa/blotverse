#include <doctest/doctest.h>

#include <cmath>
#include <filesystem>
#include <fstream>

#include "blotverse/pipeline.hpp"
#include "blotverse/report/report.hpp"
#include "blotverse/sim/simulator.hpp"

using namespace blotverse;
namespace fs = std::filesystem;

namespace {

AnalysisOptions options(unsigned threads, std::size_t permutations) {
  AnalysisOptions o;
  o.threads = threads;
  o.joint_permutations = permutations;
  return o;
}

const MultiverseSpec& repo_spec() {
  static const MultiverseSpec spec = load_spec(BLOTVERSE_SPEC_PATH);
  return spec;
}

AnalysisResult analyze_sim(ArtifactLevel level, double lfc, std::uint64_t seed, bool total_protein = true,
                           std::size_t permutations = 200, unsigned threads = 0) {
  SimParams p = sim_preset(level);
  p.n_gels = 3;
  p.reps_per_gel = 2;
  p.true_lfc = {0.0, lfc};
  p.seed = seed;
  p.total_protein = total_protein;
  AnalysisOptions opts;
  opts.joint_permutations = permutations;
  opts.threads = threads;
  return analyze(to_experiment(simulate(p)), repo_spec(), opts);
}

}  // namespace

TEST_CASE("a strong real effect on a clean blot is Robust") {
  const auto r = analyze_sim(ArtifactLevel::Clean, 1.0, 21);
  REQUIRE(r.summaries.size() == 1);
  const auto& s = r.summaries[0];
  CHECK(r.multiverse.plan.universes.size() == 432);
  CHECK(s.n_valid == 432);
  CHECK(s.default_lfc == doctest::Approx(1.0).epsilon(0.2));
  CHECK(s.robustness > 0.9);
  CHECK(s.joint.p < 0.05);
  CHECK(s.verdict == Verdict::Robust);
}

TEST_CASE("a null effect is not supported") {
  const auto r = analyze_sim(ArtifactLevel::Moderate, 0.0, 22);
  const auto& s = r.summaries[0];
  CHECK(std::fabs(s.median_lfc) < 0.3);
  CHECK(s.verdict != Verdict::Robust);
}

TEST_CASE("without total protein only 216 universes run") {
  const auto r = analyze_sim(ArtifactLevel::Clean, 0.5, 23, false, 50);
  CHECK(r.multiverse.plan.universes.size() == 216);
  CHECK(r.multiverse.plan.excluded.size() == 216);
}

TEST_CASE("results do not depend on the number of threads") {
  const auto a = analyze_sim(ArtifactLevel::Severe, 0.5, 24, true, 100, 1);
  const auto b = analyze_sim(ArtifactLevel::Severe, 0.5, 24, true, 100, 8);
  REQUIRE(a.multiverse.outcomes[0].size() == b.multiverse.outcomes[0].size());
  for (std::size_t i = 0; i < a.multiverse.outcomes[0].size(); ++i) {
    const auto& x = a.multiverse.outcomes[0][i];
    const auto& y = b.multiverse.outcomes[0][i];
    CHECK(x.valid == y.valid);
    if (x.valid) {
      CHECK(x.lfc == y.lfc);
      CHECK(x.p == y.p);
    }
  }
  CHECK(a.summaries[0].joint.p == b.summaries[0].joint.p);
}

TEST_CASE("saturated lanes are excluded from every universe") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 2;
  p.reps_per_gel = 3;
  p.true_lfc = {0.0, 1.0};
  p.peak_per_unit = 40000;  // treatment bands (x2) exceed the detector ceiling
  p.seed = 5;
  const auto r = analyze(to_experiment(simulate(p)), repo_spec(), options(0, 20));
  CHECK(r.data.observations.size() < 12);
  bool flagged = false;
  for (const auto& w : r.warnings) flagged = flagged || w.find("saturated") != std::string::npos;
  CHECK(flagged);
}

TEST_CASE("reports and the reproducibility file round-trip") {
  SimParams p = sim_preset(ArtifactLevel::Moderate);
  p.n_gels = 2;
  p.seed = 31;
  const fs::path dir = fs::temp_directory_path() / "blotverse_tests" / "pipeline_repro";
  fs::remove_all(dir);
  write_simulation(dir / "sim", simulate(p));
  const Experiment ex = load_experiment(dir / "sim" / "experiment.yaml");
  const auto result = analyze(ex, repo_spec(), options(0, 50));

  const std::string html = render_html_report(repo_spec(), ex, result);
  CHECK(html.find("Specification curve") != std::string::npos);
  CHECK(html.find("best universe") == std::string::npos);
  write_universes_csv(dir / "universes.csv", repo_spec(), result);
  write_summary_csv(dir / "summary.csv", result);
  write_repro_file(dir / "repro.yaml", repo_spec(), ex, dir / "sim", result);

  const ReproRecord record = read_repro_file(dir / "repro.yaml");
  CHECK(record.joint_permutations == 50);
  const Experiment again = experiment_from_repro(record);
  CHECK(again.blots[0].annotation.has_value());
  const auto rerun = analyze(again, parse_spec(record.spec_text), options(3, 50));
  CHECK(compare_with_repro(record, rerun).empty());

  // Tampering with an image breaks the fingerprint check.
  ImageU16 tampered = ex.blots[0].image.pixels;
  tampered(5, 5) = static_cast<std::uint16_t>(tampered(5, 5) + 1);
  write_tiff16(dir / "sim" / ex.blots[0].image_file, tampered);
  CHECK_THROWS_WITH(experiment_from_repro(record), doctest::Contains("fingerprint"));
}

TEST_CASE("3-blot experiment runs well under the 2-minute target (G1)") {
  const auto r = analyze_sim(ArtifactLevel::Moderate, 0.5, 40, true, 1000);
  CHECK(r.seconds < 120.0);
  MESSAGE("3 blots, 432 universes, 1000 joint permutations: " << r.seconds << " s");
}

TEST_CASE("3 gels x 1 replicate: design rules apply and the verdict is Not evaluable") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 3;
  p.reps_per_gel = 1;
  p.true_lfc = {0.0, 1.0};
  p.seed = 77;
  const auto r = analyze(to_experiment(simulate(p)), repo_spec(), options(0, 100));
  CHECK_FALSE(r.data.facts.permutation_can_reach_alpha);
  CHECK_FALSE(r.data.facts.every_blot_has_two_lanes_per_group);
  CHECK(r.multiverse.plan.universes.size() == 96);
  CHECK(r.summaries[0].verdict == Verdict::NotEvaluable);
  CHECK(r.summaries[0].robustness > 0.5);  // R itself is still reported
}

TEST_CASE("joint test enumerates every relabelling when they fit the budget") {
  const auto r = analyze_sim(ArtifactLevel::Clean, 1.0, 21, true, 500);  // 3 gels x (2+2): 216
  const auto& j = r.summaries[0].joint;
  CHECK(j.exact);
  CHECK(j.distinct_relabelings == 216.0);
  CHECK(j.null_counts.size() == 216);
  CHECK(j.p >= 2.0 / 216.0);  // observed labelling and its mirror
  const auto mc = analyze_sim(ArtifactLevel::Clean, 1.0, 21, true, 100);
  CHECK_FALSE(mc.summaries[0].joint.exact);
}

TEST_CASE("dilution lanes outside the proportional range are flagged") {
  // Every lane really holds the same amount, but the experiment declares a
  // 2-fold dilution series: the signal is flat, no step is proportional, and
  // every declared lane must be flagged.
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 1;
  p.reps_per_gel = 3;
  p.true_lfc = {0.0, 0.0};
  p.seed = 8;
  Experiment ex = to_experiment(simulate(p));
  const std::vector<double> loads{1, 2, 4, 8, 16, 32};
  for (std::size_t i = 0; i < ex.blots[0].lanes.size(); ++i) ex.blots[0].lanes[i].load = loads[i];
  const auto r = analyze(ex, repo_spec(), options(0, 10));
  std::size_t flagged = 0;
  for (const auto& w : r.warnings)
    if (w.find("outside the proportional range") != std::string::npos) ++flagged;
  CHECK(flagged == loads.size());
}
