#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/experiment.hpp"
#include "blotverse/pipeline.hpp"

namespace blotverse {

// --- HTML report (step [6]) --------------------------------------------------
// Self-contained page: verdict, R, effect distribution, specification curve,
// decision influence, QC warnings, exclusions and every universe. By design it
// never ranks universes or points to a "best" one.
std::string render_html_report(const MultiverseSpec& spec, const Experiment& experiment,
                               const AnalysisResult& result);

// --- Tables -------------------------------------------------------------------
void write_universes_csv(const std::filesystem::path& path, const MultiverseSpec& spec,
                         const AnalysisResult& result);
void write_summary_csv(const std::filesystem::path& path, const AnalysisResult& result);
// Per-lane band windows and raw signals for every (background, band) branch
// pair; also the input of the Fiji comparison (E6).
void write_lanes_csv(const std::filesystem::path& path, const MultiverseSpec& spec,
                     const Experiment& experiment, const AnalysisResult& result);

// --- Reproducibility file ----------------------------------------------------
// YAML with the tool version, the verbatim spec and experiment, every image
// fingerprint, the lane/band annotations actually used, seeds and the headline
// numbers. `blotverse reproduce` reruns it and checks the numbers match.
void write_repro_file(const std::filesystem::path& path, const MultiverseSpec& spec,
                      const Experiment& experiment, const std::filesystem::path& experiment_dir,
                      const AnalysisResult& result);

struct ReproRecord {
  std::string version;
  std::string spec_text;
  std::string experiment_text;
  std::filesystem::path experiment_dir;
  std::map<std::string, std::string> image_sha256;  // file -> fingerprint
  std::map<std::string, Annotation> annotations;    // blot id -> annotation used
  std::size_t joint_permutations = 0;
  struct Headline {
    std::string contrast;
    double robustness;
    double joint_p;
    double default_lfc;
    double median_lfc;
  };
  std::vector<Headline> headlines;
};

ReproRecord read_repro_file(const std::filesystem::path& path);

// Rebuilds the experiment exactly as analysed (annotations pinned) and checks
// the image fingerprints. Throws if an image changed.
Experiment experiment_from_repro(const ReproRecord& record);

// Compares headline numbers; returns human-readable mismatches (empty = identical).
std::vector<std::string> compare_with_repro(const ReproRecord& record, const AnalysisResult& result);

}  // namespace blotverse
