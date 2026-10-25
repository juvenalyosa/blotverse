#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "blotverse/core/numeric.hpp"
#include "blotverse/validation/records.hpp"

namespace blotverse {

// Level-1 validation metrics, protocol version 2 (docs/validation_protocol.md):
// every estimand comes with a confidence interval and an explicit rule.

struct Interval {
  double estimate = kNaN;
  double lo = kNaN;
  double hi = kNaN;
  std::size_t n = 0;  // observations behind the estimate
};

// Wilson score interval for k successes out of n (two-sided, level 1 - alpha).
Interval wilson_interval(std::size_t k, std::size_t n, double confidence = 0.95);

// Percentile bootstrap interval of the median.
Interval bootstrap_median(std::span<const double> values, std::uint64_t seed, std::size_t resamples = 2000,
                          double confidence = 0.95);

// Percentile bootstrap interval of the ROC AUC, resampling each class separately.
Interval bootstrap_auc(std::span<const double> positives, std::span<const double> negatives, std::uint64_t seed,
                       std::size_t resamples = 2000, double confidence = 0.95);

// E1 (descriptive): spread of evidence across universes.
struct DispersionRow {
  std::string level;  // artefact level
  bool real = true;   // real effects (true) or nulls (false)
  Interval orders;    // D1: median log10(p_max / p_min), bootstrap CI
  Interval disagree;  // D2: universes disagree on significance, Wilson CI
};

// E5 (paired): change in influence when a background artefact is planted.
inline constexpr std::array<Decision, 4> kE5Others = {Decision::Loading, Decision::Normalization,
                                                      Decision::Scale, Decision::Test};
struct E5Result {
  std::size_t n = 0;
  Interval mean_handling;              // mean H = mean(delta background + delta band), 98.75 % CI
  std::array<Interval, 4> mean_margin; // mean C_o = mean(H - delta o), o in kE5Others, 98.75 % CI
  Interval mean_background_alone;      // reference, 98.75 % CI
  Interval hit_rate;                   // reference: per-experiment rule, Wilson 95 % CI
  bool pass() const;
};

struct HarnessMetrics {
  std::size_t failed = 0;
  std::size_t main_n = 0;
  double evaluable_fraction = kNaN;

  std::vector<DispersionRow> e1;                // descriptive
  Interval e2_auc;                              // evaluable designs
  double e2_auc_all_designs = kNaN;             // reference
  std::vector<std::pair<std::string, double>> e2_auc_by_stratum;
  Interval e3_null_robust;                      // evaluable nulls labelled Robust
  Interval e4_type1;                            // evaluable nulls with joint p < alpha
  double e4_type1_all_designs = kNaN;           // reference
  Interval default_type1;                       // reference: the single default analysis
  E5Result e5;
  double e5_background_top_rate = kNaN;         // reference: protocol v1 rule

  bool e2_pass() const { return e2_auc.lo >= 0.85; }
  bool e3_pass() const { return e3_null_robust.hi <= 0.05; }
  bool e4_pass() const { return e4_type1.estimate <= 0.05 && e4_type1.lo <= 0.05; }
  bool e5_pass() const { return e5.pass(); }
  bool g2_pass() const { return e2_pass() && e3_pass() && e4_pass(); }
};

// `seed` drives the bootstrap so intervals are reproducible.
HarnessMetrics compute_metrics(const std::vector<ExperimentRecord>& records, double alpha, std::uint64_t seed);

}  // namespace blotverse
