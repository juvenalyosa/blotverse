#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

#include "blotverse/core/numeric.hpp"

namespace blotverse {

// Result of a two-group comparison. `estimate` is treatment minus control on
// the analysed scale; `p` is two-sided.
struct TestResult {
  double estimate = kNaN;
  double se = kNaN;
  double statistic = kNaN;
  double df = kNaN;
  double p = kNaN;
  bool ok = false;
  std::string error;
};

// Regularised incomplete beta I_x(a, b).
double regularized_incomplete_beta(double a, double b, double x);

// Two-sided p-value of Student's t with `df` degrees of freedom.
double student_t_two_sided_p(double t, double df);

// Welch's unequal-variance t test.
TestResult welch_test(std::span<const double> control, std::span<const double> treatment);

// Linear mixed model y = b0 + b1*treated + u_gel + e with a random gel
// intercept, fitted by REML (profile likelihood over the variance ratio).
// Inference on b1 uses a t distribution with nlme's containment degrees of
// freedom (N - n_gels - 1 when treatment varies within gels).
TestResult mixed_model_test(std::span<const double> y, std::span<const int> treated,
                            std::span<const int> gel);

// Two-sided permutation test on the difference in means. Exact over all
// relabelings when there are at most `max_permutations`, otherwise Monte Carlo
// with the given seed and p = (1 + hits) / (1 + permutations).
TestResult permutation_test(std::span<const double> control, std::span<const double> treatment,
                            std::size_t max_permutations, std::uint64_t seed);

// Smallest p-value permutation_test() can return for these group sizes: the
// observed split (and its mirror image when the groups are equal) is always
// counted as extreme.
double permutation_min_p(std::size_t n_control, std::size_t n_treatment, std::size_t max_permutations);

}  // namespace blotverse
