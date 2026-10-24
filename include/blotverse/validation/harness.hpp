#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/analysis/aggregate.hpp"
#include "blotverse/analysis/spec.hpp"
#include "blotverse/sim/simulator.hpp"

namespace blotverse {

// Level-1 validation (plan section 8): simulated experiments with known truth
// run through the complete tool, exactly as a laboratory would use it.
struct HarnessConfig {
  std::size_t experiments = 2000;          // E1-E4 (half null)
  std::size_t gradient_experiments = 200;  // E5 (planted background gradient)
  std::vector<double> effect_sizes{0.3, 0.5, 1.0};
  std::vector<ArtifactLevel> levels{ArtifactLevel::Clean, ArtifactLevel::Moderate, ArtifactLevel::Severe};
  double planted_gradient = 2.0;           // E5 gradient amplitude (x peak signal)
  std::size_t joint_permutations = 200;
  std::uint64_t seed = 1;
  unsigned threads = 0;
};

struct ExperimentRecord {
  std::size_t id = 0;
  std::string set;  // "main" or "gradient"
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
  std::size_t n_valid = 0;
  std::optional<Decision> most_influential;
  std::array<double, kNumDecisions> eta2{};
};

struct HarnessMetrics {
  // E1: dispersion of p-values across universes, log10(p_max / p_min), for
  // real effects under moderate artefacts (Gassmann compared procedures on a
  // blot with a real difference; null p-values are bounded near [0.01, 1]).
  double e1_median_log10_p_range = kNaN;
  double e1_fraction_over_2_orders = kNaN;
  double e1_null_median = kNaN;  // reference: moderate nulls
  std::vector<std::pair<std::string, double>> e1_median_by_level;  // real effects
  // E2: AUC of R separating real effects from nulls.
  // E2 and E4 are judged on experiments where the tool issues a verdict
  // (not "Not evaluable"); the all-design values are reported alongside.
  double e2_auc = kNaN;             // evaluable designs
  double e2_auc_all_designs = kNaN;
  double evaluable_fraction = kNaN;
  std::vector<std::pair<std::string, double>> e2_auc_by_stratum;
  // E3: fraction of null experiments labelled "Robust".
  double e3_null_robust_rate = kNaN;
  // E4: type I error of the joint test at alpha = 0.05.
  double e4_joint_type1 = kNaN;             // evaluable designs
  double e4_joint_type1_all_designs = kNaN;
  double default_type1 = kNaN;  // reference: the single default analysis
  // E5: most influential decision is background when a gradient is planted.
  double e5_background_hit_rate = kNaN;
  std::size_t failed = 0;

  bool g2_pass() const;
};

struct HarnessReport {
  HarnessConfig config;
  std::vector<ExperimentRecord> records;
  HarnessMetrics metrics;
  std::string spec_sha256;
  double seconds = 0.0;
};

using ProgressCallback = std::function<void(std::size_t done, std::size_t total)>;

HarnessReport run_harness(const MultiverseSpec& spec, const HarnessConfig& config,
                          const ProgressCallback& progress = {});

HarnessMetrics compute_metrics(const std::vector<ExperimentRecord>& records, double alpha);

// records.csv, metrics.yaml and summary.md
void write_harness_outputs(const std::filesystem::path& dir, const HarnessReport& report);

// Reloads a finished run (records.csv + metrics.yaml) so metrics can be
// recomputed without simulating again.
HarnessReport read_harness_outputs(const std::filesystem::path& dir);

}  // namespace blotverse
