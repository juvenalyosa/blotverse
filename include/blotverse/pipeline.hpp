#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/analysis/aggregate.hpp"
#include "blotverse/analysis/multiverse.hpp"
#include "blotverse/analysis/spec.hpp"
#include "blotverse/experiment.hpp"
#include "blotverse/imaging/detect.hpp"
#include "blotverse/imaging/qc.hpp"

namespace blotverse {

struct AnalysisOptions {
  unsigned threads = 0;                            // 0 = all cores
  std::optional<std::size_t> joint_permutations;   // overrides the spec
};

struct PreparedBlot {
  Annotation annotation;
  BlotQc qc;
};

struct AnalysisResult {
  std::vector<PreparedBlot> blots;
  MultiverseData data;
  MultiverseResult multiverse;
  std::vector<ContrastSummary> summaries;
  std::vector<std::string> warnings;
  std::size_t joint_permutations = 0;
  double seconds = 0.0;
};

// Steps [2]-[5] of the pipeline: quality control, lanes and bands, the
// branch-cached quantification, every universe and the aggregation.
AnalysisResult analyze(const Experiment& experiment, const MultiverseSpec& spec,
                       const AnalysisOptions& options = {});

// Steps [2]-[3] only: quality control and quantification cache. Exposed so
// the validation harness and tests can inspect intermediate numbers.
struct PreparedExperiment {
  std::vector<PreparedBlot> blots;
  MultiverseData data;
  std::vector<std::string> warnings;
};
PreparedExperiment prepare(const Experiment& experiment, const MultiverseSpec& spec, unsigned threads);

}  // namespace blotverse
