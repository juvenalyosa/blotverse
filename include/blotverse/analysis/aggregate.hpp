#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/analysis/multiverse.hpp"

namespace blotverse {

enum class Verdict { Robust, Fragile, NotSupported };
const char* verdict_label(Verdict v);

// Joint (specification-curve) inference: how many universes are significant
// in their dominant direction, compared with the same count after permuting
// group labels within each blot and rerunning the statistical part.
struct JointTest {
  std::size_t observed = 0;               // max(#sig & lfc > 0, #sig & lfc < 0)
  std::vector<std::size_t> null_counts;   // one per permutation
  double p = kNaN;                        // (1 + #{null >= observed}) / (1 + permutations)
  double distinct_relabelings = 0.0;      // within-blot labelings that exist for this design
};

struct ContrastSummary {
  Contrast contrast;
  std::size_t n_universes = 0;  // evaluated (after exclusions)
  std::size_t n_valid = 0;
  std::size_t n_excluded = 0;

  bool default_valid = false;
  double default_lfc = kNaN;
  double default_p = kNaN;

  // R = fraction of valid universes with the default's sign and p < alpha.
  double robustness = kNaN;
  double opposite_fraction = kNaN;              // lfc sign opposite to the default
  double opposite_significant_fraction = kNaN;  // ...and significant
  double significant_fraction = kNaN;
  double agreement = kNaN;  // same significance decision (and sign when significant)

  double median_lfc = kNaN;
  double lfc_q05 = kNaN;
  double lfc_q95 = kNaN;
  double p_min = kNaN;
  double p_max = kNaN;

  std::array<double, kNumDecisions> eta2{};  // variance of lfc explained per decision
  std::optional<Decision> most_influential;

  JointTest joint;
  Verdict verdict = Verdict::NotSupported;
  std::vector<std::string> notes;
};

// Proportion of variance in the effect explained by each decision: drop-one
// sums of squares from an OLS fit of lfc on the decision dummies.
std::array<double, kNumDecisions> decision_influence(const std::vector<Universe>& universes,
                                                     const std::vector<double>& lfc,
                                                     const std::vector<bool>& valid);

std::size_t dominant_significant_count(const std::vector<UniverseOutcome>& outcomes, double alpha);

// Runs the label-permutation joint test for every contrast.
std::vector<JointTest> joint_test(const MultiverseSpec& spec, const MultiverseData& data,
                                  const MultiverseResult& result, std::size_t permutations,
                                  unsigned threads);

ContrastSummary summarize_contrast(const MultiverseSpec& spec, const MultiverseResult& result,
                                   std::size_t contrast_index, JointTest joint);

Verdict decide_verdict(const VerdictSpec& v, double robustness, double joint_p);

}  // namespace blotverse
