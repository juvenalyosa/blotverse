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
  // Candidate QC rule under test (protocol v1.2), not part of the spec: a lane
  // is "near saturation" when its band window holds a pixel at or above this
  // fraction of the detector maximum.
  double near_saturation_fraction = 0.75;
  // Protocol v1.2: detector compression is only possible in bright lanes, so a
  // sub-linear lane counts towards P1 only when its band window reaches this
  // fraction of the detector maximum at the longer exposure; dimmer sub-linear
  // lanes are low-signal distortion.
  double bright_fraction = 0.5;
  std::vector<ExposureImage> exposures;
  std::vector<double> lane_x;
  struct Band {
    std::string name;
    double y = 0.0;
    int half_height = 0;  // 0: band_half_height
  };
  std::vector<Band> bands;
  std::string source_text;
};

ExposureConfig load_exposure_config(const std::filesystem::path& path);

// One lane of one band at one exposure.
struct ExposurePoint {
  std::size_t saturated_px = 0;  // pixels at the detector maximum in the band window
  std::size_t near_saturated_px = 0;  // pixels >= near_saturation_fraction x detector maximum
  double peak_fraction = kNaN;        // brightest pixel in the band window / detector maximum
  double snr = kNaN;             // band prominence over the flanking rows / robust pixel noise
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
  // G3 per exposure: does any lane carry the saturation flag (spec rule) or
  // the candidate near-saturation flag?
  std::vector<bool> any_flagged;
  std::vector<bool> any_near_saturated;
  // Protocol v1.1, pair level (default branch): is a compressive pair
  // distortion blamed on the exposure? Kept for comparison.
  std::vector<bool> compressive_default;
  std::size_t qc_misses = 0;  // v1.1: compressive and not flagged
  // Protocol v1.2, lane level (default branch). Each non-reference exposure is
  // compared with the reference; over the lanes measured above the SNR floor
  // at both, a lane's scale is d_i = log2(S_i,longer / S_i,shorter) minus the
  // median of the same over those lanes (the median absorbs exposure time and
  // substrate decay). d_i < -distortion_log2: the lane is sub-linear at the
  // longer exposure (detector-like compression, blamed on that exposure);
  // d_i > +distortion_log2: super-linear, i.e. under-measured at the shorter
  // exposure (low-signal distortion, blamed on the shorter one). It needs >= 3
  // such lanes; otherwise the comparison is not attributable.
  std::vector<std::vector<double>> lane_scale;  // [exposure][lane], vs the reference; NaN if not measured
  std::vector<bool> attributable;               // per non-reference exposure
  std::vector<bool> sublinear;                  // per exposure blamed
  std::vector<bool> superlinear;                // low-signal distortion, per exposure blamed (the shorter one)
  std::size_t misses_spec = 0;                  // sub-linear and no saturation flag
  std::size_t misses_candidate = 0;             // sub-linear and neither flag
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
