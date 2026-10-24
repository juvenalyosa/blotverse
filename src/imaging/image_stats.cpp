#include "blotverse/imaging/image_stats.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "blotverse/core/numeric.hpp"
#include "blotverse/imaging/background.hpp"

namespace blotverse {

ImageStats compute_image_stats(const RawImage& image) {
  ImageStats s;
  s.width = image.pixels.width();
  s.height = image.pixels.height();
  s.grey_levels = image.meta.grey_levels;
  const auto& px = image.pixels.data();
  if (px.empty()) return s;

  std::vector<double> values(px.begin(), px.end());
  s.background = quantile(values, 0.5);
  std::vector<double> deviations(values.size());
  for (std::size_t i = 0; i < values.size(); ++i) deviations[i] = std::fabs(values[i] - s.background);
  s.noise = std::max(1.0, 1.4826 * quantile(deviations, 0.5));
  s.signal_range = quantile(values, 0.999) - s.background;
  s.peak_snr = s.signal_range / s.noise;

  std::size_t foreground = 0, saturated = 0;
  for (std::uint16_t v : px) {
    if (v > s.background + 5.0 * s.noise) ++foreground;
    if (v >= image.meta.detector_max) ++saturated;
  }
  s.foreground_fraction = static_cast<double>(foreground) / static_cast<double>(px.size());
  s.saturated_fraction = static_cast<double>(saturated) / static_cast<double>(px.size());

  // Smooth background: rolling ball with a radius of 1/8 of the short side,
  // far larger than any band, so it follows only slow trends and blotches.
  const double radius = std::max(10.0, std::min(s.width, s.height) / 8.0);
  const ImageF bg = rolling_ball_background(to_float(image.pixels), radius);
  std::vector<double> bgv(bg.data().begin(), bg.data().end());
  const double spread = quantile(bgv, 0.95) - quantile(bgv, 0.05);
  s.background_variation = s.signal_range > 0 ? spread / s.signal_range : 0.0;
  return s;
}

}  // namespace blotverse
