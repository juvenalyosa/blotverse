#include <doctest/doctest.h>

#include <cmath>

#include "blotverse/analysis/aggregate.hpp"
#include "blotverse/analysis/multiverse.hpp"
#include "blotverse/analysis/normalization.hpp"
#include "blotverse/analysis/spec.hpp"

using namespace blotverse;

namespace {
const MultiverseSpec& repo_spec() {
  static const MultiverseSpec spec = load_spec(BLOTVERSE_SPEC_PATH);
  return spec;
}
}  // namespace

TEST_CASE("repository spec: 432 universes with total protein, 216 without") {
  const auto& spec = repo_spec();
  CHECK(spec.full_size() == 432);
  CHECK(plan_universes(spec, {true, true}).universes.size() == 432);
  CHECK(plan_universes(spec, {false, true}).universes.size() == 216);
  CHECK(plan_universes(spec, {false, false}).universes.size() == 162);
  const auto plan = plan_universes(spec, {true, true});
  REQUIRE(plan.default_index);
  CHECK(spec.describe(plan.universes[*plan.default_index]) == "rb2/rect/hk/fixed/log/welch");
  for (const auto& d : spec.decisions)
    for (const auto& b : d.branches) CHECK_FALSE(b.references.empty());
}

TEST_CASE("enumeration is lexicographic with the test decision fastest") {
  const auto all = repo_spec().enumerate();
  CHECK(all.front() == Universe{0, 0, 0, 0, 0, 0});
  CHECK(all[1] == Universe{0, 0, 0, 0, 0, 1});
  CHECK(all.back() == Universe{3, 2, 1, 2, 1, 2});
}

TEST_CASE("invalid specs are rejected with a clear message") {
  CHECK_THROWS_WITH_AS(parse_spec("name: x\n"), doctest::Contains("decisions"), std::runtime_error);
  const std::string no_ref = R"(
decisions:
  background: [{id: a, method: local}]
  band: [{id: b, method: fixed_rectangle, references: [x]}]
  loading: [{id: c, method: housekeeping, references: [x]}]
  normalization: [{id: d, method: blot_sum, references: [x]}]
  scale: [{id: e, method: log, references: [x]}]
  test: [{id: f, method: welch, references: [x]}]
default_universe: {background: a, band: b, loading: c, normalization: d, scale: e, test: f}
)";
  CHECK_THROWS_WITH(parse_spec(no_ref), doctest::Contains("no literature reference"));
  std::string valid = no_ref;
  valid.replace(valid.find("method: local}"), 14, "method: local, references: [x]}");
  CHECK_NOTHROW(parse_spec(valid));
  std::string wrong = valid;
  wrong.replace(wrong.find("method: welch"), 13, "method: local");
  CHECK_THROWS_WITH(parse_spec(wrong), doctest::Contains("does not belong"));
}

TEST_CASE("fixed-point normalisation sets each blot's control mean to 1") {
  const std::vector<double> y{2, 4, 6, 10, 20, 30};
  const std::vector<int> blot{0, 0, 0, 1, 1, 1};
  const std::vector<int> group{0, 0, 1, 0, 0, 1};
  const auto out = normalize_replicates(Method::FixedPoint, y, blot, group, 2, 2);
  REQUIRE(out);
  CHECK((*out)[0] + (*out)[1] == doctest::Approx(2.0));
  CHECK((*out)[2] == doctest::Approx(2.0));
  CHECK((*out)[5] == doctest::Approx(2.0));
  // A blot without controls makes fixed-point undefined.
  const std::vector<int> no_ctrl{0, 0, 1, 1, 1, 1};
  CHECK_FALSE(normalize_replicates(Method::FixedPoint, y, blot, no_ctrl, 2, 2));
}

TEST_CASE("optimal alignment removes per-blot scale factors exactly") {
  // True condition means 1 and 3; blot 2 is scaled by 5.
  const std::vector<double> y{1, 1, 3, 3, 5, 5, 15, 15};
  const std::vector<int> blot{0, 0, 0, 0, 1, 1, 1, 1};
  const std::vector<int> group{0, 0, 1, 1, 0, 0, 1, 1};
  const auto out = normalize_replicates(Method::OptimalAlignment, y, blot, group, 2, 2);
  REQUIRE(out);
  CHECK((*out)[0] == doctest::Approx((*out)[4]));
  CHECK((*out)[2] / (*out)[0] == doctest::Approx(3.0));
  const auto sum = normalize_replicates(Method::BlotSum, y, blot, group, 2, 2);
  REQUIRE(sum);
  CHECK((*sum)[0] == doctest::Approx((*sum)[4]));
}

TEST_CASE("decision influence attributes variance to the decision that matters") {
  const auto& spec = repo_spec();
  const auto universes = spec.enumerate();
  std::vector<double> lfc;
  std::vector<bool> valid(universes.size(), true);
  for (const auto& u : universes) lfc.push_back(u[0] * 0.5 + u[5] * 0.01);  // background dominates
  const auto eta = decision_influence(universes, lfc, valid);
  CHECK(eta[0] > 0.95);
  CHECK(eta[5] < 0.01);
  CHECK(eta[1] == doctest::Approx(0.0).epsilon(1e-12));
}

TEST_CASE("verdict thresholds") {
  const VerdictSpec v;
  CHECK(decide_verdict(v, 0.95, 0.01) == Verdict::Robust);
  CHECK(decide_verdict(v, 0.7, 0.01) == Verdict::Fragile);
  CHECK(decide_verdict(v, 0.3, 0.01) == Verdict::NotSupported);
  CHECK(decide_verdict(v, 0.99, 0.2) == Verdict::NotSupported);
  CHECK(decide_verdict(v, kNaN, 0.01) == Verdict::NotSupported);
}

TEST_CASE("multiverse on hand-made numbers") {
  const auto& spec = repo_spec();
  MultiverseData data;
  data.groups = {"ctrl", "treat"};
  data.n_blots = 2;
  data.facts = {false, true};
  // 2 blots x (3 ctrl + 3 treat); treatment doubles the target.
  for (int b = 0; b < 2; ++b)
    for (int l = 0; l < 6; ++l) data.observations.push_back({b, l, l % 2});
  data.quant.assign(4, std::vector<QuantValues>(3));
  for (std::size_t k = 0; k < 4; ++k)
    for (std::size_t m = 0; m < 3; ++m) {
      auto& q = data.quant[k][m];
      for (std::size_t i = 0; i < data.observations.size(); ++i) {
        const auto& o = data.observations[i];
        const double noise = 1.0 + 0.03 * static_cast<double>((i * 7) % 5) - 0.06;
        q.target.push_back(1000.0 * (o.group ? 2.0 : 1.0) * noise * (1.0 + o.blot));
        q.reference.push_back(500.0 * (1.0 + o.blot));
      }
    }
  const auto result = run_multiverse(spec, data, 2);
  REQUIRE(result.plan.universes.size() == 216);
  const auto joint = joint_test(spec, data, result, 50, 2);
  const auto s = summarize_contrast(spec, result, 0, joint[0]);
  CHECK(s.n_valid == 216);
  CHECK(s.default_lfc == doctest::Approx(1.0).epsilon(0.05));
  CHECK(s.median_lfc == doctest::Approx(1.0).epsilon(0.05));
  CHECK(s.robustness > 0.6);
  CHECK(s.opposite_fraction == 0.0);
}

TEST_CASE("design rules exclude underpowered and anti-conservative branches") {
  const auto& spec = repo_spec();
  ExperimentFacts small;
  small.has_total_protein = true;
  small.permutation_can_reach_alpha = false;
  small.every_blot_has_two_lanes_per_group = false;
  const auto plan = plan_universes(spec, small);
  // Excluded: perm (144) or sum/align (288); overlap 96 -> 336 excluded, 96 remain.
  CHECK(plan.universes.size() == 96);
  CHECK(plan.excluded.size() == 336);
  REQUIRE(plan.default_index);  // the default (fixed / welch) is never excluded
}

TEST_CASE("Not evaluable when the joint test cannot reach alpha") {
  JointTest small;
  small.distinct_relabelings = 8;  // 3 blots x (1 ctrl + 1 treat)
  CHECK_FALSE(joint_test_evaluable(small, 0.05));
  JointTest big;
  big.distinct_relabelings = 216;
  CHECK(joint_test_evaluable(big, 0.05));
  CHECK(decide_verdict(VerdictSpec{}, 1.0, 0.25, false) == Verdict::NotEvaluable);
}
