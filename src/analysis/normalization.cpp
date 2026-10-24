#include "blotverse/analysis/normalization.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace blotverse {
namespace {

std::optional<std::vector<double>> fixed_point(std::span<const double> y, std::span<const int> blot,
                                               std::span<const int> group, int n_blots) {
  std::vector<double> sum(static_cast<std::size_t>(n_blots), 0.0), count(sum.size(), 0.0);
  for (std::size_t i = 0; i < y.size(); ++i)
    if (group[i] == 0) {
      sum[static_cast<std::size_t>(blot[i])] += y[i];
      count[static_cast<std::size_t>(blot[i])] += 1.0;
    }
  std::vector<double> out(y.size());
  for (std::size_t i = 0; i < y.size(); ++i) {
    const auto b = static_cast<std::size_t>(blot[i]);
    if (count[b] == 0 || !(sum[b] > 0)) return std::nullopt;
    out[i] = y[i] / (sum[b] / count[b]);
  }
  return out;
}

std::optional<std::vector<double>> blot_sum(std::span<const double> y, std::span<const int> blot,
                                            int n_blots) {
  std::vector<double> sum(static_cast<std::size_t>(n_blots), 0.0);
  for (std::size_t i = 0; i < y.size(); ++i) sum[static_cast<std::size_t>(blot[i])] += y[i];
  std::vector<double> out(y.size());
  for (std::size_t i = 0; i < y.size(); ++i) {
    const double s = sum[static_cast<std::size_t>(blot[i])];
    if (!(s > 0)) return std::nullopt;
    out[i] = y[i] / s;
  }
  return out;
}

std::optional<std::vector<double>> optimal_alignment(std::span<const double> y, std::span<const int> blot,
                                                     std::span<const int> group, int n_blots,
                                                     int n_groups) {
  const auto nb = static_cast<std::size_t>(n_blots);
  const auto ng = static_cast<std::size_t>(n_groups);
  std::vector<double> k(nb, 0.0), m(ng, 0.0);
  {
    // Start from fixed-sum scaling.
    std::vector<double> s(nb, 0.0), c(nb, 0.0);
    for (std::size_t i = 0; i < y.size(); ++i) {
      s[static_cast<std::size_t>(blot[i])] += y[i];
      c[static_cast<std::size_t>(blot[i])] += 1.0;
    }
    for (std::size_t b = 0; b < nb; ++b) {
      if (c[b] > 0 && !(s[b] > 0)) return std::nullopt;
      k[b] = c[b] > 0 ? c[b] / s[b] : 1.0;
    }
  }
  constexpr int kMaxIter = 500;
  for (int iter = 0; iter < kMaxIter; ++iter) {
    // Condition means given scale factors, normalised to mean 1.
    std::vector<double> ms(ng, 0.0), mc(ng, 0.0);
    for (std::size_t i = 0; i < y.size(); ++i) {
      ms[static_cast<std::size_t>(group[i])] += k[static_cast<std::size_t>(blot[i])] * y[i];
      mc[static_cast<std::size_t>(group[i])] += 1.0;
    }
    double avg = 0.0, used = 0.0;
    for (std::size_t g = 0; g < ng; ++g)
      if (mc[g] > 0) {
        m[g] = ms[g] / mc[g];
        avg += m[g];
        used += 1.0;
      }
    avg /= used;
    if (!(avg > 0)) return std::nullopt;
    for (auto& v : m) v /= avg;
    // Scale factors given condition means: k_b = sum(y m) / sum(y^2).
    std::vector<double> num(nb, 0.0), den(nb, 0.0);
    for (std::size_t i = 0; i < y.size(); ++i) {
      const auto b = static_cast<std::size_t>(blot[i]);
      num[b] += y[i] * m[static_cast<std::size_t>(group[i])];
      den[b] += y[i] * y[i];
    }
    double change = 0.0;
    for (std::size_t b = 0; b < nb; ++b) {
      if (den[b] == 0) continue;
      const double next = num[b] / den[b];
      if (!(next > 0)) return std::nullopt;
      change = std::max(change, std::fabs(next - k[b]) / k[b]);
      k[b] = next;
    }
    if (change < 1e-12) break;
  }
  std::vector<double> out(y.size());
  for (std::size_t i = 0; i < y.size(); ++i) out[i] = k[static_cast<std::size_t>(blot[i])] * y[i];
  return out;
}

}  // namespace

std::optional<std::vector<double>> normalize_replicates(Method method, std::span<const double> values,
                                                        std::span<const int> blot,
                                                        std::span<const int> group, int n_blots,
                                                        int n_groups) {
  if (values.size() != blot.size() || values.size() != group.size())
    throw std::invalid_argument("normalize_replicates: size mismatch");
  switch (method) {
    case Method::FixedPoint: return fixed_point(values, blot, group, n_blots);
    case Method::BlotSum: return blot_sum(values, blot, n_blots);
    case Method::OptimalAlignment: return optimal_alignment(values, blot, group, n_blots, n_groups);
    default: throw std::invalid_argument("normalize_replicates: not a normalisation method");
  }
}

}  // namespace blotverse
