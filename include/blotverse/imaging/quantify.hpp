#pragma once

#include <cstddef>
#include <string>

#include "blotverse/core/image.hpp"
#include "blotverse/imaging/detect.hpp"

namespace blotverse {

enum class BandMethod { FixedRectangle, ProfilePeak, ThresholdMask };

// Local background (same lane) is only coherent when the flanking strips
// lie inside the image and do not overlap other bands or lanes.
struct LocalBackgroundCheck {
  bool valid = true;
  std::string reason;
};

LocalBackgroundCheck check_local_background(const Annotation& ann, int width, int height);
LocalBackgroundCheck check_local_background_lanes(const Annotation& ann, int width, int height);

// Target/reference image with the local background removed from every band
// window: each window minus the median of the strips just above and below.
ImageF local_background_bands(const ImageF& image, const Annotation& ann);

// Total-protein image with the local background removed from every lane:
// each lane minus the median of the inter-lane gaps on either side.
ImageF local_background_lanes(const ImageF& image, const Annotation& ann);

// Integrated band signal on a background-corrected image.
//  - FixedRectangle: sum over the band window (same size in every lane).
//  - ProfilePeak:    lane profile across the window, area above the straight
//                    baseline joining the window ends (Gel Analyzer style).
//  - ThresholdMask:  sum of pixels above the Otsu threshold of the window.
double quantify_band(const ImageF& corrected, const Annotation& ann, std::size_t lane,
                     BandKind kind, BandMethod method);

// Rows used for whole-lane total-protein quantification.
Rect total_protein_rect(const Annotation& ann, std::size_t lane, int height);

double quantify_lane_total(const ImageF& corrected, const Annotation& ann, std::size_t lane,
                           BandMethod method);

}  // namespace blotverse
