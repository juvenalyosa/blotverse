#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace blotverse {

// The six analysis decisions, in the fixed order used everywhere
// (enumeration, reports, influence analysis).
enum class Decision : std::uint8_t { Background, Band, Loading, Normalization, Scale, Test };
inline constexpr std::size_t kNumDecisions = 6;
inline constexpr std::array<Decision, kNumDecisions> kDecisions = {
    Decision::Background, Decision::Band,  Decision::Loading,
    Decision::Normalization, Decision::Scale, Decision::Test};

const char* decision_key(Decision d);    // YAML key, e.g. "background"
const char* decision_label(Decision d);  // human label, e.g. "Background subtraction"

// Method identifiers. Each branch of a decision names one method.
enum class Method : std::uint8_t {
  RollingBall, LocalBackground,                    // background
  FixedRectangle, ProfilePeak, ThresholdMask,      // band definition
  Housekeeping, TotalProtein,                      // loading control
  FixedPoint, BlotSum, OptimalAlignment,           // replicate normalisation
  LinearRatio, LogRatio,                           // effect scale
  Welch, MixedModel, Permutation                   // statistical test
};

const char* method_key(Method m);
Decision method_decision(Method m);

struct Branch {
  std::string id;
  Method method{};
  double radius_factor = 0.0;  // rolling ball only: radius = factor * band height
  std::vector<std::string> references;
  std::string rationale;
};

struct DecisionSpec {
  Decision decision{};
  std::vector<Branch> branches;
};

// Facts about an experiment that decide which combinations are incoherent.
enum class ExclusionCondition : std::uint8_t { OverlappingNeighbours, TotalProteinMissing };

struct ExclusionRule {
  std::string id;
  Decision decision{};
  std::string branch_id;  // the rule applies to universes using this branch
  ExclusionCondition condition{};
  std::string reason;
};

struct QcSpec {
  std::size_t saturation_min_pixels = 1;
  double linear_range_min_slope = 0.8;
};

struct VerdictSpec {
  double robust_min_r = 0.9;
  double fragile_min_r = 0.5;
  double joint_alpha = 0.05;
};

struct InferenceSpec {
  std::size_t joint_permutations = 1000;   // label permutations for the joint test
  std::size_t test_max_permutations = 10000;  // permutation-test branch (exact when fewer)
  std::uint64_t seed = 20261024;
};

using Universe = std::array<std::uint8_t, kNumDecisions>;  // branch index per decision

struct MultiverseSpec {
  std::string name;
  std::string version;
  double alpha = 0.05;
  std::array<DecisionSpec, kNumDecisions> decisions;
  Universe default_universe{};
  std::vector<ExclusionRule> exclusions;
  QcSpec qc;
  VerdictSpec verdict;
  InferenceSpec inference;
  std::string source_text;  // verbatim YAML, embedded in the reproducibility file

  const DecisionSpec& decision(Decision d) const { return decisions[static_cast<std::size_t>(d)]; }
  const Branch& branch(Decision d, std::size_t index) const { return decision(d).branches.at(index); }
  std::size_t full_size() const;  // product of branch counts (before exclusions)
  std::vector<Universe> enumerate() const;  // lexicographic, background outermost
  std::string describe(const Universe& u) const;  // "rb2/profile/hk/fixed/log/welch"
};

MultiverseSpec parse_spec(const std::string& yaml_text);
MultiverseSpec load_spec(const std::filesystem::path& path);

struct ExperimentFacts {
  bool has_total_protein = false;
  bool local_background_valid = true;
};

// Returns the reason when the universe is excluded for this experiment.
std::optional<std::string> exclusion_reason(const MultiverseSpec& spec, const Universe& u,
                                            const ExperimentFacts& facts);

}  // namespace blotverse
