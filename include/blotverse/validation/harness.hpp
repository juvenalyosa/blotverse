#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "blotverse/analysis/aggregate.hpp"
#include "blotverse/analysis/spec.hpp"
#include "blotverse/sim/simulator.hpp"
#include "blotverse/validation/metrics.hpp"
#include "blotverse/validation/records.hpp"

namespace blotverse {

// Level-1 validation (plan section 8): simulated experiments with known truth
// run through the complete tool, exactly as a laboratory would use it.
struct HarnessConfig {
  std::size_t experiments = 2000;          // E1-E4 (half null)
  std::size_t gradient_experiments = 200;  // E5 (planted background artefact, paired)
  std::vector<double> effect_sizes{0.3, 0.5, 1.0};
  std::vector<ArtifactLevel> levels{ArtifactLevel::Clean, ArtifactLevel::Moderate, ArtifactLevel::Severe};
  // E5 planted background artefact (pre-specified before the v1.1 E5 runs).
  double planted_gradient = 1.0;           // x peak signal
  int planted_blotches = 8;
  double planted_blotch_amplitude = 0.4;   // x peak signal
  std::size_t joint_permutations = 200;
  std::uint64_t seed = 1;
  unsigned threads = 0;
};

struct HarnessReport {
  HarnessConfig config;
  std::vector<ExperimentRecord> records;
  HarnessMetrics metrics;
  std::string spec_sha256;
  int simulator_presets = 0;
  double seconds = 0.0;
};

using ProgressCallback = std::function<void(std::size_t done, std::size_t total)>;

HarnessReport run_harness(const MultiverseSpec& spec, const HarnessConfig& config,
                          const ProgressCallback& progress = {});

// records.csv, metrics.yaml and summary.md
void write_harness_outputs(const std::filesystem::path& dir, const HarnessReport& report);

// Reloads a finished run (records.csv + metrics.yaml) so metrics can be
// recomputed without simulating again.
HarnessReport read_harness_outputs(const std::filesystem::path& dir);

}  // namespace blotverse
