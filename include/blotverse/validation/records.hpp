#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <string>

#include "blotverse/analysis/aggregate.hpp"
#include "blotverse/sim/simulator.hpp"

namespace blotverse {

// Outcome of one simulated validation experiment (one row of records.csv).
struct ExperimentRecord {
  std::size_t id = 0;
  std::string set;  // "main" or "background" (E5 paired set; "gradient" in old runs)
  ArtifactLevel level = ArtifactLevel::Clean;
  double true_lfc = 0.0;
  int n_gels = 0;
  int reps_per_gel = 0;
  bool failed = false;
  std::string error;
  double robustness = kNaN;
  double joint_p = kNaN;
  Verdict verdict = Verdict::NotSupported;
  double default_lfc = kNaN;
  double default_p = kNaN;
  double median_lfc = kNaN;
  double p_min = kNaN;
  double p_max = kNaN;
  double significant_fraction = kNaN;  // share of valid universes with p < alpha
  std::size_t n_valid = 0;
  std::optional<Decision> most_influential;
  std::array<double, kNumDecisions> eta2{};
  // E5 only: influence in the same experiment without the planted artefact.
  bool paired = false;
  std::array<double, kNumDecisions> baseline_eta2{};
};

}  // namespace blotverse
