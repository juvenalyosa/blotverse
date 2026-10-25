#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/core/numeric.hpp"

namespace blotverse {

// Exposure-invariance bench: the same membrane imaged at several exposures.
// Within the linear range the ratio between two lanes of one band does not
// depend on the exposure, so every exposure is a known-truth replicate of the
// others. Deviations measure how each background x band-definition branch
// handles noise, background drift and saturation, and whether the saturation
// QC rule flags the lanes whose ratios are distorted.

struct ExposureImage {
  std::filesystem::path image;
  double seconds = -1.0;  // from the config, else the acquisition metadata, else unknown
};

struct ExposureConfig {
  std::string name;
  bool invert = false;
  double lane_width = 0.0;
  int band_half_height = 0;
  int refine_rows = 0;            // band rows refined on the first exposure only; geometry is then fixed
  double snr_floor = 10.0;        // band prominence below it at either exposure: "dim", left out
  double distortion_log2 = 0.2;   // |change in log2 lane ratio| counted as distorted
  double max_drift_px = 2.0;      // band centroid shift that marks an exposure as misaligned
  std::vector<ExposureImage> exposures;
  std::vector<double> lane_x;
  struct Band {
    std::string name;
    double y = 0.0;
  };
  std::vector<Band> bands;
  std::string source_text;
};

ExposureConfig load_exposure_config(const std::filesystem::path& path);

// One lane of one band at one exposure.
struct ExposurePoint {
  std::size_t saturated_px = 0;  // pixels at the detector maximum in the band window
  double snr = kNaN;             // band prominence over the flanking rows / robust image noise
  double drift_px = kNaN;        // band centroid row minus the reference exposure's
};

// One lane pair at one non-reference exposure under one branch.
struct ExposurePair {
  std::size_t exposure = 0;
  std::size_t lane_i = 0, lane_j = 0;
  double delta = kNaN;     // log2(S_i/S_j) at this exposure minus at the reference exposure
  bool flagged = false;    // the saturation QC rule excludes lane i or j at this exposure
  bool dim = false;        // either lane below the SNR floor at this or the reference exposure
  // Distorted, and the ratio is closer to equality at the longer of the two
  // exposures (the brighter lane lost ground there): the signature of
  // saturation or detector non-linearity rather than noise. It is blamed on
  // the longer exposure, which may be the reference itself.
  bool compressive = false;
  std::size_t blamed = 0;
};

struct ExposureBranch {
  std::string background;
  std::string band_definition;
  std::vector<std::vector<double>> signal;  // [exposure][lane]
  std::vector<ExposurePair> pairs;          // only pairs with positive signals at both exposures
  std::size_t nonpositive_pairs = 0;        // pairs dropped because a signal was <= 0
  // Unflagged, non-dim pairs: does the ratio stay put inside the linear range?
  std::size_t clean_n = 0;
  double clean_median_abs = kNaN, clean_q95_abs = kNaN;
  // Flagged, non-dim pairs: how far does saturation move the ratio?
  std::size_t flagged_n = 0;
  double flagged_median_abs = kNaN;
  // Saturation QC rule vs distortion (|delta| > distortion_log2), non-dim pairs.
  std::size_t tp = 0, fp = 0, fn = 0, tn = 0;
};

struct ExposureBand {
  std::string name;
  std::vector<std::vector<ExposurePoint>> points;  // [exposure][lane]
  long reference = -1;   // longest usable exposure with no saturated lane; -1: none (band not evaluable)
  std::vector<bool> misaligned;  // per exposure
  // G3 per exposure: does any lane carry the saturation flag, and is a
  // compressive distortion (non-dim pair, spec's default branch) blamed on
  // it? An exposure with compressive distortion and no flag is a QC miss.
  std::vector<bool> any_flagged;
  std::vector<bool> compressive_default;
  std::size_t qc_misses = 0;
  std::vector<ExposureBranch> branches;
};

struct ExposureResult {
  std::string default_branch;              // "background/band definition" used for the G3 check
  std::vector<bool> excluded;              // per exposure: dimensions differ from the first
  std::vector<std::string> notes;          // per exposure
  std::vector<double> seconds;             // per exposure
  std::vector<ExposureBand> bands;
};

ExposureResult analyze_exposure(const ExposureConfig& config, const MultiverseSpec& spec);

// exposure_lanes.csv (every lane, exposure and branch), exposure_pairs.csv
// (every lane-pair deviation), exposure_summary.csv (the G3 check per band
// and exposure) and exposure.md (summary per band and branch).
void write_exposure_outputs(const std::filesystem::path& dir, const ExposureConfig& config,
                            const MultiverseSpec& spec, const ExposureResult& result);

}  // namespace blotverse
