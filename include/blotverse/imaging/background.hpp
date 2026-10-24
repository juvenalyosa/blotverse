#pragma once

#include "blotverse/core/image.hpp"

namespace blotverse {

// Port of ImageJ's "Subtract Background" rolling-ball algorithm
// (ij.plugin.filter.BackgroundSubtracter, bright objects on a dark
// background, no paraboloid): optional 3x3 mean pre-smoothing, shrinking by
// block minimum for large radii, rolling a trimmed ball under the surface and
// bilinear re-enlargement. Returns the estimated background, not the
// corrected image, so it can be cached per branch.
ImageF rolling_ball_background(const ImageF& image, double radius, bool presmooth = true);

// image - background, pixel by pixel (no clamping, as ImageJ does for 32-bit).
ImageF subtract(const ImageF& image, const ImageF& background);

}  // namespace blotverse
