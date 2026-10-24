#include "blotverse/imaging/quantify.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "blotverse/core/numeric.hpp"

namespace blotverse {
namespace {

constexpr int kStripGap = 1;

int strip_height(const Annotation& ann) { return std::max(3, ann.band_half_height / 2); }

Rect strip_above(const Rect& window, int h) { return {window.x0, window.y0 - kStripGap - h, window.x1, window.y0 - kStripGap}; }
Rect strip_below(const Rect& window, int h) { return {window.x0, window.y1 + kStripGap, window.x1, window.y1 + kStripGap + h}; }

void collect(const ImageF& img, const Rect& r, std::vector<double>& out) {
  const Rect c = r.clipped(img.width(), img.height());
  for (int y = c.y0; y < c.y1; ++y)
    for (int x = c.x0; x < c.x1; ++x) out.push_back(img(x, y));
}

// Inter-lane gap columns to the left and right of a lane.
std::pair<Rect, Rect> lane_gaps(const Annotation& ann, std::size_t lane, int width, int height) {
  const Rect r = total_protein_rect(ann, lane, height);
  const int left_end = lane == 0 ? r.x0 - r.width() / 4 : lane_rect(ann, lane - 1).x1;
  const int right_end = lane + 1 == ann.lane_count() ? r.x1 + r.width() / 4 : lane_rect(ann, lane + 1).x0;
  Rect left{std::max(0, left_end), r.y0, r.x0, r.y1};
  Rect right{r.x1, r.y0, std::min(width, right_end), r.y1};
  return {left, right};
}

double profile_peak(const ImageF& img, const Rect& window) {
  const Rect w = window.clipped(img.width(), img.height());
  std::vector<double> prof;
  for (int y = w.y0; y < w.y1; ++y) {
    double s = 0.0;
    for (int x = w.x0; x < w.x1; ++x) s += img(x, y);
    prof.push_back(s);
  }
  if (prof.size() < 3) return 0.0;
  // Baseline: straight line between the mean of the two outermost rows at each end.
  const std::size_t n = prof.size();
  const double left = 0.5 * (prof[0] + prof[1]);
  const double right = 0.5 * (prof[n - 1] + prof[n - 2]);
  double area = 0.0;
  for (std::size_t i = 0; i < n; ++i) {
    const double t = static_cast<double>(i) / static_cast<double>(n - 1);
    area += std::max(0.0, prof[i] - (left + t * (right - left)));
  }
  return area;
}

double threshold_mask(const ImageF& img, const Rect& window) {
  const Rect w = window.clipped(img.width(), img.height());
  std::vector<float> px;
  for (int y = w.y0; y < w.y1; ++y)
    for (int x = w.x0; x < w.x1; ++x) px.push_back(img(x, y));
  if (px.empty()) return 0.0;
  const double t = otsu_threshold(px);
  double sum = 0.0;
  for (float v : px)
    if (v >= t) sum += v;
  return sum;
}

double quantify_rect(const ImageF& img, const Rect& window, BandMethod method) {
  switch (method) {
    case BandMethod::FixedRectangle: return sum_rect(img, window);
    case BandMethod::ProfilePeak: return profile_peak(img, window);
    case BandMethod::ThresholdMask: return threshold_mask(img, window);
  }
  return 0.0;
}

}  // namespace

LocalBackgroundCheck check_local_background(const Annotation& ann, int width, int height) {
  const int h = strip_height(ann);
  for (std::size_t i = 0; i < ann.lane_count(); ++i) {
    const Rect t = band_window(ann, i, BandKind::Target);
    const Rect r = band_window(ann, i, BandKind::Reference);
    if (t.intersects(r))
      return {false, "target and reference bands overlap in lane " + std::to_string(i + 1)};
    for (const auto& [window, other] : {std::pair{t, r}, std::pair{r, t}}) {
      for (const Rect& s : {strip_above(window, h), strip_below(window, h)}) {
        if (!s.inside(width, height))
          return {false, "local background strip leaves the image in lane " + std::to_string(i + 1)};
        if (s.intersects(other))
          return {false, "local background strip overlaps the neighbouring band in lane " +
                             std::to_string(i + 1)};
      }
    }
    if (i > 0 && lane_rect(ann, i - 1).x1 > lane_rect(ann, i).x0)
      return {false, "lanes " + std::to_string(i) + " and " + std::to_string(i + 1) + " overlap"};
  }
  return {};
}

LocalBackgroundCheck check_local_background_lanes(const Annotation& ann, int width, int height) {
  for (std::size_t i = 0; i < ann.lane_count(); ++i) {
    const auto [left, right] = lane_gaps(ann, i, width, height);
    if (left.width() < 2 && right.width() < 2)
      return {false, "no inter-lane gap for local background around lane " + std::to_string(i + 1)};
  }
  return {};
}

ImageF local_background_bands(const ImageF& image, const Annotation& ann) {
  ImageF out = image;
  const int h = strip_height(ann);
  for (std::size_t i = 0; i < ann.lane_count(); ++i) {
    for (BandKind kind : {BandKind::Target, BandKind::Reference}) {
      const Rect w = band_window(ann, i, kind).clipped(image.width(), image.height());
      std::vector<double> flank;
      collect(image, strip_above(w, h), flank);
      collect(image, strip_below(w, h), flank);
      const double bg = flank.empty() ? 0.0 : median(std::move(flank));
      for (int y = w.y0; y < w.y1; ++y)
        for (int x = w.x0; x < w.x1; ++x) out(x, y) = static_cast<float>(image(x, y) - bg);
    }
  }
  return out;
}

ImageF local_background_lanes(const ImageF& image, const Annotation& ann) {
  ImageF out = image;
  for (std::size_t i = 0; i < ann.lane_count(); ++i) {
    const auto [left, right] = lane_gaps(ann, i, image.width(), image.height());
    std::vector<double> gap;
    collect(image, left, gap);
    collect(image, right, gap);
    const double bg = gap.empty() ? 0.0 : median(std::move(gap));
    const Rect r = total_protein_rect(ann, i, image.height()).clipped(image.width(), image.height());
    for (int y = r.y0; y < r.y1; ++y)
      for (int x = r.x0; x < r.x1; ++x) out(x, y) = static_cast<float>(image(x, y) - bg);
  }
  return out;
}

double quantify_band(const ImageF& corrected, const Annotation& ann, std::size_t lane,
                     BandKind kind, BandMethod method) {
  return quantify_rect(corrected, band_window(ann, lane, kind), method);
}

Rect total_protein_rect(const Annotation& ann, std::size_t lane, int height) {
  Rect r = lane_rect(ann, lane);
  const int margin = std::max(1, height / 20);
  r.y0 = margin;
  r.y1 = height - margin;
  return r;
}

double quantify_lane_total(const ImageF& corrected, const Annotation& ann, std::size_t lane,
                           BandMethod method) {
  return quantify_rect(corrected, total_protein_rect(ann, lane, corrected.height()), method);
}

}  // namespace blotverse
