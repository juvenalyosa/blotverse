#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/core/numeric.hpp"
#include "blotverse/imaging/detect.hpp"

namespace blotverse {

// Known-amount dilution series on a real image (plan level 2): how linear is
// the signal in each band under every background x band-definition branch,
// and which loads does the linear-range QC rule flag?

struct DilutionLane {
  double x_center = 0.0;
  double load = 0.0;  // amount loaded (any unit)
};

struct DilutionBand {
  std::string name;
  double y = 0.0;  // approximate band row; refined per lane
};

struct DilutionConfig {
  std::string name;
  std::filesystem::path image;
  bool invert = false;
  double lane_width = 0.0;
  int band_half_height = 0;
  int refine_rows = 3;  // per-lane search for the band centre: y +/- refine_rows
  std::vector<DilutionLane> lanes;
  std::vector<DilutionBand> bands;
  std::string source_text;
};

DilutionConfig load_dilution_config(const std::filesystem::path& path);

// One band row across manually placed lanes (shared by the dilution and
// exposure benches): lane centres as given, the band centre refined per lane
// to the brightest row within y +/- refine_rows. The reference row is unused
// and set equal to the target row.
Annotation band_row_annotation(const ImageF& image, const std::vector<double>& lane_x, double lane_width,
                               int band_half_height, double y, int refine_rows);

// Every lane's target-band signal under one background x band-definition branch.
struct BranchSignals {
  std::string background;       // branch id
  std::string band_definition;  // branch id
  std::vector<double> signal;   // per lane
};

// All background x band-definition branches of the spec, in spec order.
std::vector<BranchSignals> measure_band_row(const ImageF& image, const Annotation& ann, const MultiverseSpec& spec);

struct DilutionFit {
  std::string band;
  std::string background;      // branch id
  std::string band_definition; // branch id
  std::vector<double> signal;  // per lane, in config order
  double slope = kNaN;         // log-log slope over all lanes with positive signal (1 = proportional)
  double r2 = kNaN;
  std::vector<bool> nonlinear; // linear-range QC flag per lane
  // Known fold changes: every lane pair, error of the estimated log2 ratio
  // against log2 of the true load ratio.
  double fc_mae_all = kNaN;       // mean |error| over all pairs
  double fc_mae_unflagged = kNaN; // ... over pairs where neither lane is flagged
  std::size_t pairs_unflagged = 0;
};

std::vector<DilutionFit> analyze_dilution(const DilutionConfig& config, const MultiverseSpec& spec);

// dilution.csv (every signal) and dilution.md (slopes, R^2, flags).
void write_dilution_outputs(const std::filesystem::path& dir, const DilutionConfig& config,
                            const std::vector<DilutionFit>& fits);

}  // namespace blotverse
