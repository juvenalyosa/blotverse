#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/core/numeric.hpp"

namespace blotverse {

// One quantified lane that passed quality control.
struct Observation {
  int blot = 0;
  int lane = 0;
  int group = 0;  // index into MultiverseData::groups; 0 is the control
};

// Raw signals per observation for one (background, band) branch pair.
// `total` is empty when the experiment has no total-protein images.
struct QuantValues {
  std::vector<double> target;
  std::vector<double> reference;
  std::vector<double> total;
};

// Everything the statistical part of the multiverse needs. Image work is
// done once per branch pair (the cache); universes only combine numbers.
struct MultiverseData {
  std::vector<Observation> observations;
  int n_blots = 0;
  std::vector<std::string> groups;
  std::vector<std::vector<QuantValues>> quant;  // [background branch][band branch]
  ExperimentFacts facts;

  std::vector<int> labels() const;  // group of each observation
};

struct Contrast {
  int treatment = 1;  // compared against group 0
  std::string label;
};

struct UniverseOutcome {
  double effect = kNaN;  // treatment - control on the analysed scale
  double lfc = kNaN;     // log2 fold change (common scale for every universe)
  double p = kNaN;
  bool valid = false;
  std::string note;      // why the universe is invalid, if it is
  std::uint16_t n_control = 0;
  std::uint16_t n_treatment = 0;
};

struct ExcludedUniverse {
  Universe universe{};
  std::string reason;
};

struct UniversePlan {
  std::vector<Universe> universes;  // to evaluate, in enumeration order
  std::vector<ExcludedUniverse> excluded;
  std::optional<std::size_t> default_index;
};

UniversePlan plan_universes(const MultiverseSpec& spec, const ExperimentFacts& facts);

std::vector<Contrast> make_contrasts(const std::vector<std::string>& groups);

// Called as work items finish (from worker threads): done out of total.
using ItemProgress = std::function<void(std::size_t done, std::size_t total)>;

// Evaluates every universe for every contrast with the given group labels
// (the observed labels, or a permutation of them for the joint test).
// Result is indexed [contrast][universe].
std::vector<std::vector<UniverseOutcome>> evaluate_universes(
    const MultiverseSpec& spec, const MultiverseData& data, const std::vector<Universe>& universes,
    const std::vector<Contrast>& contrasts, const std::vector<int>& labels, unsigned threads,
    const ItemProgress& progress = {});

struct MultiverseResult {
  UniversePlan plan;
  std::vector<Contrast> contrasts;
  std::vector<std::vector<UniverseOutcome>> outcomes;  // [contrast][universe]
};

MultiverseResult run_multiverse(const MultiverseSpec& spec, const MultiverseData& data,
                                unsigned threads, const ItemProgress& progress = {});

}  // namespace blotverse
