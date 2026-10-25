#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"
#include "blotverse/imaging/detect.hpp"
#include "blotverse/io/tiff.hpp"

namespace blotverse {

struct LaneQc {
  bool excluded = false;  // excluded from every universe
  std::vector<std::string> flags;
};

struct BlotQc {
  std::string blot_id;
  std::vector<std::string> warnings;  // image-level (bit depth, compression, ...)
  std::vector<LaneQc> lanes;
  int saturated_bands = 0;
};

// Number of pixels at the detector maximum inside a rectangle.
std::size_t count_saturated(const ImageU16& image, const Rect& rect, std::uint32_t detector_max);

// Saturation and image-level checks for one blot. A saturated band excludes
// its lane from every universe; it is never quantified.
BlotQc check_blot(const std::string& blot_id, const RawImage& image, const RawImage* total_protein,
                  const Annotation& ann, std::size_t min_saturated_pixels);

// Dilution-series linearity. With loads sorted ascending, the proportional
// range is the longest run of consecutive points whose log-log slopes all lie
// in [min_slope, max_slope] (ties: smallest total |slope - 1|). Points outside
// it are flagged: saturation above (slope too low) and loss of signal below
// (slope too high). Returns one flag per input point (in input order); all
// points are flagged when no two consecutive points are proportional.
std::vector<bool> linear_range_flags(const std::vector<double>& load, const std::vector<double>& signal,
                                     double min_slope, double max_slope);

}  // namespace blotverse
