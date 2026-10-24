#include "blotverse/core/numeric.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace blotverse {

double mean(std::span<const double> v) {
  if (v.empty()) return kNaN;
  return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

double variance(std::span<const double> v) {
  if (v.size() < 2) return kNaN;
  const double m = mean(v);
  double ss = 0.0;
  for (double x : v) ss += (x - m) * (x - m);
  return ss / static_cast<double>(v.size() - 1);
}

double median(std::vector<double> v) { return quantile(std::move(v), 0.5); }

double quantile(std::vector<double> v, double q) {
  if (v.empty()) return kNaN;
  std::sort(v.begin(), v.end());
  const double h = (static_cast<double>(v.size()) - 1.0) * std::clamp(q, 0.0, 1.0);
  const auto lo = static_cast<std::size_t>(std::floor(h));
  const auto hi = std::min(lo + 1, v.size() - 1);
  return v[lo] + (h - static_cast<double>(lo)) * (v[hi] - v[lo]);
}

double otsu_threshold(std::span<const float> values) {
  if (values.empty()) return 0.0;
  const auto [mn_it, mx_it] = std::minmax_element(values.begin(), values.end());
  const double mn = *mn_it;
  const double mx = *mx_it;
  if (mx <= mn) return mn;

  constexpr int kBins = 256;
  std::vector<double> hist(kBins, 0.0);
  const double scale = (kBins - 1) / (mx - mn);
  for (float v : values) hist[static_cast<std::size_t>((v - mn) * scale)] += 1.0;

  const double total = static_cast<double>(values.size());
  double sum_all = 0.0;
  for (int i = 0; i < kBins; ++i) sum_all += i * hist[static_cast<std::size_t>(i)];

  double weight_bg = 0.0, sum_bg = 0.0, best = -1.0;
  int best_bin = 0;
  for (int i = 0; i < kBins; ++i) {
    weight_bg += hist[static_cast<std::size_t>(i)];
    if (weight_bg == 0.0) continue;
    const double weight_fg = total - weight_bg;
    if (weight_fg == 0.0) break;
    sum_bg += i * hist[static_cast<std::size_t>(i)];
    const double mean_bg = sum_bg / weight_bg;
    const double mean_fg = (sum_all - sum_bg) / weight_fg;
    const double between = weight_bg * weight_fg * (mean_bg - mean_fg) * (mean_bg - mean_fg);
    if (between > best) {
      best = between;
      best_bin = i;
    }
  }
  // Pixels strictly above the upper edge of the best background bin are foreground.
  return mn + (best_bin + 1) / scale;
}

std::size_t binomial_capped(std::size_t n, std::size_t k, std::size_t cap) {
  if (k > n) return 0;
  k = std::min(k, n - k);
  double result = 1.0;
  for (std::size_t i = 1; i <= k; ++i) {
    result = result * static_cast<double>(n - k + i) / static_cast<double>(i);
    if (result > static_cast<double>(cap)) return cap;
  }
  return static_cast<std::size_t>(std::llround(result));
}

double roc_auc(std::span<const double> positives, std::span<const double> negatives) {
  if (positives.empty() || negatives.empty()) throw std::invalid_argument("roc_auc: empty class");
  // Mann-Whitney U via midranks.
  struct Item {
    double score;
    bool positive;
  };
  std::vector<Item> items;
  items.reserve(positives.size() + negatives.size());
  for (double s : positives) items.push_back({s, true});
  for (double s : negatives) items.push_back({s, false});
  std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) { return a.score < b.score; });

  double rank_sum_pos = 0.0;
  std::size_t i = 0;
  while (i < items.size()) {
    std::size_t j = i;
    while (j < items.size() && items[j].score == items[i].score) ++j;
    const double midrank = 0.5 * static_cast<double>(i + 1 + j);
    for (std::size_t k = i; k < j; ++k)
      if (items[k].positive) rank_sum_pos += midrank;
    i = j;
  }
  const double np = static_cast<double>(positives.size());
  const double nn = static_cast<double>(negatives.size());
  return (rank_sum_pos - np * (np + 1.0) / 2.0) / (np * nn);
}

}  // namespace blotverse
