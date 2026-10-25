#include "blotverse/imaging/qc.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

#include "blotverse/imaging/quantify.hpp"

namespace blotverse {

std::size_t count_saturated(const ImageU16& image, const Rect& rect, std::uint32_t detector_max) {
  const Rect r = rect.clipped(image.width(), image.height());
  std::size_t n = 0;
  for (int y = r.y0; y < r.y1; ++y)
    for (int x = r.x0; x < r.x1; ++x)
      if (image(x, y) >= detector_max) ++n;
  return n;
}

BlotQc check_blot(const std::string& blot_id, const RawImage& image, const RawImage* total_protein,
                  const Annotation& ann, std::size_t min_saturated_pixels) {
  BlotQc qc;
  qc.blot_id = blot_id;
  qc.warnings = image.meta.warnings;
  if (total_protein) {
    for (const auto& w : total_protein->meta.warnings) qc.warnings.push_back("total protein: " + w);
    if (total_protein->pixels.width() != image.pixels.width() ||
        total_protein->pixels.height() != image.pixels.height())
      throw std::runtime_error("blot " + blot_id +
                               ": total-protein image must be registered to the target image");
  }
  qc.lanes.resize(ann.lane_count());

  for (std::size_t i = 0; i < ann.lane_count(); ++i) {
    auto& lane = qc.lanes[i];
    for (BandKind kind : {BandKind::Target, BandKind::Reference}) {
      const std::size_t n = count_saturated(image.pixels, band_window(ann, i, kind), image.meta.detector_max);
      if (n >= min_saturated_pixels) {
        lane.excluded = true;
        ++qc.saturated_bands;
        lane.flags.push_back(std::string(kind == BandKind::Target ? "target" : "reference") +
                             " band saturated (" + std::to_string(n) + " px at detector maximum)");
      }
    }
    if (total_protein) {
      const std::size_t n =
          count_saturated(total_protein->pixels, total_protein_rect(ann, i, total_protein->pixels.height()),
                          total_protein->meta.detector_max);
      if (n >= min_saturated_pixels) {
        lane.excluded = true;
        lane.flags.push_back("total-protein lane saturated (" + std::to_string(n) + " px)");
      }
    }
  }
  return qc;
}

std::vector<bool> linear_range_flags(const std::vector<double>& load, const std::vector<double>& signal,
                                     double min_slope, double max_slope) {
  if (load.size() != signal.size()) throw std::invalid_argument("linear_range_flags: size mismatch");
  std::vector<std::size_t> order(load.size());
  std::iota(order.begin(), order.end(), 0);
  std::sort(order.begin(), order.end(), [&](auto a, auto b) { return load[a] < load[b]; });

  // slope[k]: log-log slope between sorted points k and k+1 (NaN if undefined).
  std::vector<double> slope;
  for (std::size_t k = 0; k + 1 < order.size(); ++k) {
    const double l0 = load[order[k]], l1 = load[order[k + 1]];
    const double s0 = signal[order[k]], s1 = signal[order[k + 1]];
    slope.push_back(l0 > 0 && l1 > l0 && s0 > 0 && s1 > 0 ? std::log(s1 / s0) / std::log(l1 / l0)
                                                           : std::nan(""));
  }
  auto proportional = [&](double v) { return v >= min_slope && v <= max_slope; };

  std::size_t best_start = 0, best_len = 0;  // in segments
  double best_dev = INFINITY;
  for (std::size_t i = 0; i < slope.size(); ++i) {
    double dev = 0.0;
    for (std::size_t j = i; j < slope.size() && proportional(slope[j]); ++j) {
      dev += std::fabs(slope[j] - 1.0);
      const std::size_t len = j - i + 1;
      if (len > best_len || (len == best_len && dev < best_dev)) {
        best_start = i;
        best_len = len;
        best_dev = dev;
      }
    }
  }
  std::vector<bool> flags(load.size(), true);
  if (best_len > 0)
    for (std::size_t k = best_start; k <= best_start + best_len; ++k) flags[order[k]] = false;
  return flags;
}

}  // namespace blotverse
