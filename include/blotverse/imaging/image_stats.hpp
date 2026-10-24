#pragma once

#include <cstddef>

#include "blotverse/io/tiff.hpp"

namespace blotverse {

// Annotation-free image statistics used by the realism check (plan section 8):
// the same numbers are computed on real acquisitions and on simulated blots.
struct ImageStats {
  int width = 0;
  int height = 0;
  std::size_t grey_levels = 0;
  double background = 0.0;        // median pixel (background dominates a blot)
  double noise = 0.0;             // 1.4826 * MAD of pixels: background noise
  double signal_range = 0.0;      // p99.9 - median
  double peak_snr = 0.0;          // signal_range / noise
  double foreground_fraction = 0.0;    // pixels above median + 5 noise
  double background_variation = 0.0;   // (p95 - p5) of a smooth background / signal_range
  double saturated_fraction = 0.0;     // pixels at the detector maximum
};

ImageStats compute_image_stats(const RawImage& image);

}  // namespace blotverse
