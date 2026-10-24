#pragma once

#include <cstddef>
#include <limits>
#include <span>
#include <vector>

namespace blotverse {

inline constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();

double mean(std::span<const double> v);
// Unbiased sample variance (n - 1 denominator).
double variance(std::span<const double> v);
double median(std::vector<double> v);
// Quantile with linear interpolation (R type 7, NumPy default).
double quantile(std::vector<double> v, double q);

// Otsu threshold of a set of values using a 256-bin histogram.
double otsu_threshold(std::span<const float> values);

// Number of k-subsets of n items, saturating at `cap`.
std::size_t binomial_capped(std::size_t n, std::size_t k, std::size_t cap);

// Area under the ROC curve: P(score_pos > score_neg) + 0.5 P(tie).
double roc_auc(std::span<const double> positives, std::span<const double> negatives);

}  // namespace blotverse
