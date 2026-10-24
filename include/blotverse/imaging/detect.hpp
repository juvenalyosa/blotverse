#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"

namespace blotverse {

enum class BandKind { Target, Reference };

struct LaneGeometry {
  double x_center = 0.0;
  double width = 0.0;
};

// Lane and band positions for one blot. Produced by detect_annotation() or
// supplied by the user (manual correction); quantification only reads this.
struct Annotation {
  std::vector<LaneGeometry> lanes;
  std::vector<double> target_y;     // band centre row per lane
  std::vector<double> reference_y;  // band centre row per lane
  int band_half_height = 0;         // band window is centre +/- this many rows
  std::string source = "auto";      // "auto" or "manual"

  std::size_t lane_count() const noexcept { return lanes.size(); }
  // Band height used to scale rolling-ball radii.
  int band_height() const noexcept { return 2 * band_half_height + 1; }
  void validate(int image_width, int image_height) const;
};

// What the user roughly knows about the layout (all optional): band rows from
// molecular weight, and the centres of the first and last sample lanes (which
// keeps ladders and membrane edges out of the lane fit).
struct BandHints {
  std::optional<double> target_y;
  std::optional<double> reference_y;
  bool target_above_reference = true;
  std::optional<double> first_lane_x;
  std::optional<double> last_lane_x;
};

Rect lane_rect(const Annotation& ann, std::size_t lane);
Rect band_window(const Annotation& ann, std::size_t lane, BandKind kind);

// Automatic detection: band rows from the row profile; lanes by fitting an
// evenly spaced comb of n positions to the band-row column signal (robust to
// ladders, edges and spots, unlike picking the n strongest peaks); then
// per-lane refinement of each band centre (handles smiling).
// Throws std::runtime_error when the expected lanes cannot be found.
Annotation detect_annotation(const ImageF& image, std::size_t n_lanes, const BandHints& hints);

}  // namespace blotverse
