#include "blotverse/imaging/detect.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

#include "blotverse/core/numeric.hpp"

namespace blotverse {
namespace {

std::vector<double> box_smooth(const std::vector<double>& v, int half) {
  std::vector<double> out(v.size());
  const int n = static_cast<int>(v.size());
  for (int i = 0; i < n; ++i) {
    double s = 0.0;
    int c = 0;
    for (int j = std::max(0, i - half); j <= std::min(n - 1, i + half); ++j, ++c)
      s += v[static_cast<std::size_t>(j)];
    out[static_cast<std::size_t>(i)] = s / c;
  }
  return out;
}

// Profile minus its running minimum: removes slow background trends.
std::vector<double> remove_baseline(const std::vector<double>& v, int half) {
  std::vector<double> out(v.size());
  const int n = static_cast<int>(v.size());
  std::vector<double> mins(v.size());
  for (int i = 0; i < n; ++i) {
    double m = v[static_cast<std::size_t>(i)];
    for (int j = std::max(0, i - half); j <= std::min(n - 1, i + half); ++j)
      m = std::min(m, v[static_cast<std::size_t>(j)]);
    mins[static_cast<std::size_t>(i)] = m;
  }
  const auto base = box_smooth(mins, half / 2);
  for (int i = 0; i < n; ++i)
    out[static_cast<std::size_t>(i)] = v[static_cast<std::size_t>(i)] - base[static_cast<std::size_t>(i)];
  return out;
}

// Greedy selection of the `count` highest local maxima at least `min_sep` apart.
std::vector<int> pick_peaks(const std::vector<double>& v, std::size_t count, int min_sep) {
  std::vector<int> maxima;
  for (int i = 1; i + 1 < static_cast<int>(v.size()); ++i) {
    const auto u = static_cast<std::size_t>(i);
    if (v[u] >= v[u - 1] && v[u] > v[u + 1]) maxima.push_back(i);
  }
  std::sort(maxima.begin(), maxima.end(), [&](int a, int b) {
    return v[static_cast<std::size_t>(a)] > v[static_cast<std::size_t>(b)];
  });
  std::vector<int> chosen;
  for (int m : maxima) {
    if (chosen.size() == count) break;
    const bool far = std::all_of(chosen.begin(), chosen.end(),
                                 [&](int c) { return std::abs(c - m) >= min_sep; });
    if (far) chosen.push_back(m);
  }
  std::sort(chosen.begin(), chosen.end());
  return chosen;
}

int argmax_in(const std::vector<double>& v, int lo, int hi) {
  lo = std::max(lo, 0);
  hi = std::min(hi, static_cast<int>(v.size()) - 1);
  int best = lo;
  for (int i = lo; i <= hi; ++i)
    if (v[static_cast<std::size_t>(i)] > v[static_cast<std::size_t>(best)]) best = i;
  return best;
}

double fwhm_at(const std::vector<double>& v, int peak) {
  const double half = 0.5 * v[static_cast<std::size_t>(peak)];
  int lo = peak, hi = peak;
  while (lo > 0 && v[static_cast<std::size_t>(lo)] > half) --lo;
  while (hi + 1 < static_cast<int>(v.size()) && v[static_cast<std::size_t>(hi)] > half) ++hi;
  return static_cast<double>(hi - lo);
}

std::vector<double> lane_row_profile(const ImageF& image, const Rect& lane) {
  // Central 60 % of the lane avoids edge effects from neighbouring lanes.
  const int margin = static_cast<int>(0.2 * lane.width());
  const Rect core = Rect{lane.x0 + margin, 0, lane.x1 - margin, image.height()}.clipped(
      image.width(), image.height());
  std::vector<double> profile(static_cast<std::size_t>(image.height()), 0.0);
  for (int y = 0; y < image.height(); ++y) {
    double s = 0.0;
    for (int x = core.x0; x < core.x1; ++x) s += image(x, y);
    profile[static_cast<std::size_t>(y)] = core.width() > 0 ? s / core.width() : 0.0;
  }
  return profile;
}


// Target and reference rows: near the hints when given, else the two strongest peaks.
std::pair<int, int> find_band_rows(const std::vector<double>& rows, const BandHints& hints, int H) {
  const int search = std::max(8, H / 30);
  if (hints.target_y && hints.reference_y) {
    const int t = static_cast<int>(*hints.target_y);
    const int r = static_cast<int>(*hints.reference_y);
    return {argmax_in(rows, t - search, t + search), argmax_in(rows, r - search, r + search)};
  }
  const auto peaks = pick_peaks(box_smooth(rows, 1), 2, std::max(10, H / 15));
  if (peaks.size() != 2) throw std::runtime_error("band detection failed; supply band hints");
  return hints.target_above_reference ? std::pair{peaks[0], peaks[1]} : std::pair{peaks[1], peaks[0]};
}

// Half-height of the band window from the FWHM, capped so a broad hump (haze,
// neighbouring band) cannot push the window off the image or into the other band.
int band_half_height_from(const std::vector<double>& rows, int target_row, int reference_row, int H) {
  const double fwhm = std::max(fwhm_at(rows, target_row), fwhm_at(rows, reference_row));
  const int edge_room = std::min({target_row, reference_row, H - 1 - target_row, H - 1 - reference_row});
  const int max_half = std::max(3, std::min({H / 12, std::abs(reference_row - target_row) / 3, edge_room - 2}));
  return std::clamp(static_cast<int>(std::ceil(1.1 * fwhm)), 3, max_half);
}

}  // namespace

void Annotation::validate(int image_width, int image_height) const {
  if (lanes.empty()) throw std::runtime_error("annotation has no lanes");
  if (target_y.size() != lanes.size() || reference_y.size() != lanes.size())
    throw std::runtime_error("annotation: band rows must be given for every lane");
  if (band_half_height < 1) throw std::runtime_error("annotation: band_half_height must be >= 1");
  for (std::size_t i = 0; i < lanes.size(); ++i) {
    if (lanes[i].width < 2) throw std::runtime_error("annotation: lane width too small");
    for (BandKind k : {BandKind::Target, BandKind::Reference})
      if (!band_window(*this, i, k).inside(image_width, image_height))
        throw std::runtime_error("annotation: band window outside image in lane " +
                                 std::to_string(i + 1));
  }
}

Rect lane_rect(const Annotation& ann, std::size_t lane) {
  const auto& g = ann.lanes.at(lane);
  const int x0 = static_cast<int>(std::lround(g.x_center - 0.5 * g.width));
  return Rect{x0, 0, x0 + static_cast<int>(std::lround(g.width)), 0};
}

Rect band_window(const Annotation& ann, std::size_t lane, BandKind kind) {
  Rect r = lane_rect(ann, lane);
  const double centre = kind == BandKind::Target ? ann.target_y.at(lane) : ann.reference_y.at(lane);
  const int cy = static_cast<int>(std::lround(centre));
  r.y0 = cy - ann.band_half_height;
  r.y1 = cy + ann.band_half_height + 1;
  return r;
}

Annotation detect_annotation(const ImageF& image, std::size_t n_lanes, const BandHints& hints) {
  if (n_lanes == 0) throw std::invalid_argument("detect_annotation: no lanes expected");
  const int W = image.width();
  const int H = image.height();

  // --- 1. Band rows from the full-width row profile -------------------------
  std::vector<double> full_rows(static_cast<std::size_t>(H), 0.0);
  for (int y = 0; y < H; ++y) {
    double s = 0.0;
    for (int x = 0; x < W; ++x) s += image(x, y);
    full_rows[static_cast<std::size_t>(y)] = s / W;
  }
  const auto rough_rows = remove_baseline(full_rows, 30);
  const auto [rough_target, rough_reference] = find_band_rows(rough_rows, hints, H);
  const int rough_half = band_half_height_from(rough_rows, rough_target, rough_reference, H);

  // --- 2. Lanes: band windows minus flanking strips, column by column ---------
  // The local difference cancels smooth background gradients, which would
  // otherwise swamp the lane peaks of a plain column projection.
  std::vector<double> cols(static_cast<std::size_t>(W), 0.0);
  for (int centre : {rough_target, rough_reference}) {
    for (int x = 0; x < W; ++x) {
      double in = 0.0, out = 0.0;
      int n_in = 0, n_out = 0;
      for (int dy = -2 * rough_half - 2; dy <= 2 * rough_half + 2; ++dy) {
        const int y = centre + dy;
        if (y < 0 || y >= H) continue;
        if (std::abs(dy) <= rough_half) {
          in += image(x, y);
          ++n_in;
        } else if (std::abs(dy) >= rough_half + 2) {
          out += image(x, y);
          ++n_out;
        }
      }
      if (n_in && n_out) cols[static_cast<std::size_t>(x)] += in / n_in - out / n_out;
    }
  }
  const double nominal_pitch = static_cast<double>(W) / static_cast<double>(n_lanes);
  const auto col_signal = box_smooth(cols, std::max(1, static_cast<int>(nominal_pitch / 12)));
  // W / n overestimates the pitch because of the side margins (badly so for
  // few lanes), so the minimum separation is kept well below it.
  const auto lane_peaks = pick_peaks(col_signal, n_lanes, std::max(3, static_cast<int>(0.4 * nominal_pitch)));
  if (lane_peaks.size() != n_lanes)
    throw std::runtime_error("lane detection found " + std::to_string(lane_peaks.size()) + " of " +
                             std::to_string(n_lanes) + " lanes; supply a manual annotation");

  double pitch = nominal_pitch;
  if (n_lanes > 1) {
    std::vector<double> gaps;
    for (std::size_t i = 1; i < lane_peaks.size(); ++i)
      gaps.push_back(lane_peaks[i] - lane_peaks[i - 1]);
    pitch = median(gaps);
  }

  Annotation ann;
  ann.source = "auto";
  const double lane_width = std::max(4.0, std::round(0.75 * pitch));
  for (int p : lane_peaks) {
    // Refine to the signal centroid around the peak.
    double sw = 0.0, sxw = 0.0;
    const int half = static_cast<int>(0.4 * pitch);
    for (int x = std::max(0, p - half); x <= std::min(W - 1, p + half); ++x) {
      const double w = std::max(0.0, col_signal[static_cast<std::size_t>(x)]);
      sw += w;
      sxw += w * x;
    }
    // Keep the whole lane inside the image (edge lanes next to a bright margin).
    const double centre = sw > 0 ? sxw / sw : static_cast<double>(p);
    ann.lanes.push_back({std::clamp(centre, 0.5 * lane_width + 1.0, W - 0.5 * lane_width - 1.0), lane_width});
  }

  // --- 3. Band rows and height refined on the lanes only ----------------------
  std::vector<std::vector<double>> lane_profiles;
  std::vector<double> rows(static_cast<std::size_t>(H), 0.0);
  for (std::size_t i = 0; i < n_lanes; ++i) {
    lane_profiles.push_back(remove_baseline(lane_row_profile(image, lane_rect(ann, i)), 30));
    for (int y = 0; y < H; ++y) rows[static_cast<std::size_t>(y)] += lane_profiles.back()[static_cast<std::size_t>(y)];
  }
  const int target_row = argmax_in(rows, rough_target - rough_half, rough_target + rough_half);
  const int reference_row = argmax_in(rows, rough_reference - rough_half, rough_reference + rough_half);
  ann.band_half_height = band_half_height_from(rows, target_row, reference_row, H);

  // --- Per-lane refinement (smile) -----------------------------------------
  const int refine = std::max(4, ann.band_half_height);
  for (std::size_t i = 0; i < n_lanes; ++i) {
    const auto& prof = lane_profiles[i];
    // Keep every band window inside the image.
    const int lo = ann.band_half_height, hi = H - 1 - ann.band_half_height;
    ann.target_y.push_back(std::clamp(argmax_in(prof, target_row - refine, target_row + refine), lo, hi));
    ann.reference_y.push_back(std::clamp(argmax_in(prof, reference_row - refine, reference_row + refine), lo, hi));
  }
  return ann;
}

}  // namespace blotverse
