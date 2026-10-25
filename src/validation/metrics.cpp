#include "blotverse/validation/metrics.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>

#include "blotverse/core/rng.hpp"

namespace blotverse {
namespace {

constexpr std::uint64_t kBootstrapStream = 0xB0075;
constexpr double kE5Confidence = 1.0 - 0.05 / 4.0;  // Bonferroni over four comparisons

// Standard normal quantile (Acklam's rational approximation, |error| < 1.2e-9).
double normal_quantile(double p) {
  static constexpr double a[] = {-3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
                                 1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00};
  static constexpr double b[] = {-5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
                                 6.680131188771972e+01, -1.328068155288572e+01};
  static constexpr double c[] = {-7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
                                 -2.549732539343734e+00, 4.374664141464968e+00, 2.938163982698783e+00};
  static constexpr double d[] = {7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
                                 3.754408661907416e+00};
  const double lo = 0.02425;
  if (p < lo) {
    const double q = std::sqrt(-2 * std::log(p));
    return (((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
           ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1);
  }
  if (p > 1 - lo) return -normal_quantile(1 - p);
  const double q = p - 0.5, r = q * q;
  return (((((a[0] * r + a[1]) * r + a[2]) * r + a[3]) * r + a[4]) * r + a[5]) * q /
         (((((b[0] * r + b[1]) * r + b[2]) * r + b[3]) * r + b[4]) * r + 1);
}

Interval percentile_interval(double estimate, std::vector<double> draws, double confidence, std::size_t n) {
  Interval out;
  out.estimate = estimate;
  out.n = n;
  if (draws.empty()) return out;
  const double tail = 0.5 * (1.0 - confidence);
  out.lo = quantile(draws, tail);
  out.hi = quantile(std::move(draws), 1.0 - tail);
  return out;
}

// Paired bootstrap of the means of several per-experiment quantities at once.
std::vector<Interval> bootstrap_means(const std::vector<std::vector<double>>& columns, std::uint64_t seed,
                                      std::size_t resamples, double confidence) {
  const std::size_t n = columns.empty() ? 0 : columns.front().size();
  std::vector<std::vector<double>> draws(columns.size());
  Rng rng(seed);
  for (std::size_t b = 0; b < resamples && n > 0; ++b) {
    std::vector<double> sums(columns.size(), 0.0);
    for (std::size_t i = 0; i < n; ++i) {
      const auto k = static_cast<std::size_t>(rng.uniform_int(n));
      for (std::size_t c = 0; c < columns.size(); ++c) sums[c] += columns[c][k];
    }
    for (std::size_t c = 0; c < columns.size(); ++c) draws[c].push_back(sums[c] / static_cast<double>(n));
  }
  std::vector<Interval> out;
  for (std::size_t c = 0; c < columns.size(); ++c)
    out.push_back(percentile_interval(n ? mean(columns[c]) : kNaN, std::move(draws[c]), confidence, n));
  return out;
}

}  // namespace

Interval wilson_interval(std::size_t k, std::size_t n, double confidence) {
  Interval out;
  out.n = n;
  if (n == 0) return out;
  const double z = normal_quantile(1.0 - 0.5 * (1.0 - confidence));
  const double dn = static_cast<double>(n);
  const double p = static_cast<double>(k) / dn;
  const double denom = 1.0 + z * z / dn;
  const double centre = (p + z * z / (2.0 * dn)) / denom;
  const double half = z * std::sqrt(p * (1.0 - p) / dn + z * z / (4.0 * dn * dn)) / denom;
  out.estimate = p;
  // The bounds are exactly 0 when k = 0 and exactly 1 when k = n; computing
  // them leaves rounding residue (e.g. 4e-19) that differs across libms.
  out.lo = k == 0 ? 0.0 : std::max(0.0, centre - half);
  out.hi = k == n ? 1.0 : std::min(1.0, centre + half);
  return out;
}

Interval bootstrap_median(std::span<const double> values, std::uint64_t seed, std::size_t resamples,
                          double confidence) {
  if (values.empty()) return {};
  Rng rng(seed);
  std::vector<double> draws, sample(values.size());
  draws.reserve(resamples);
  for (std::size_t b = 0; b < resamples; ++b) {
    for (auto& s : sample) s = values[static_cast<std::size_t>(rng.uniform_int(values.size()))];
    draws.push_back(median(sample));
  }
  return percentile_interval(median(std::vector<double>(values.begin(), values.end())), std::move(draws),
                             confidence, values.size());
}

Interval bootstrap_auc(std::span<const double> positives, std::span<const double> negatives, std::uint64_t seed,
                       std::size_t resamples, double confidence) {
  if (positives.empty() || negatives.empty()) return {};
  Rng rng(seed);
  std::vector<double> draws, pos(positives.size()), neg(negatives.size());
  draws.reserve(resamples);
  for (std::size_t b = 0; b < resamples; ++b) {
    for (auto& v : pos) v = positives[static_cast<std::size_t>(rng.uniform_int(positives.size()))];
    for (auto& v : neg) v = negatives[static_cast<std::size_t>(rng.uniform_int(negatives.size()))];
    draws.push_back(roc_auc(pos, neg));
  }
  return percentile_interval(roc_auc(positives, negatives), std::move(draws), confidence,
                             positives.size() + negatives.size());
}

bool E5Result::pass() const {
  if (n == 0 || !(mean_handling.lo > 0.0)) return false;
  return std::all_of(mean_margin.begin(), mean_margin.end(), [](const Interval& c) { return c.lo > 0.0; });
}

HarnessMetrics compute_metrics(const std::vector<ExperimentRecord>& records, double alpha, std::uint64_t seed) {
  HarnessMetrics m;
  const std::uint64_t boot = derive_seed(seed, kBootstrapStream);
  std::vector<double> pos, neg, pos_eval, neg_eval;
  std::size_t main_eval = 0, nulls = 0, null_joint_sig = 0, null_default_sig = 0;
  std::size_t nulls_eval = 0, null_eval_robust = 0, null_eval_joint_sig = 0;
  std::map<std::string, std::pair<std::vector<double>, std::vector<double>>> strata;
  // E1 cells: (level, real) -> log10 ranges and disagreement counts.
  std::map<std::pair<std::string, bool>, std::vector<double>> ranges;
  std::map<std::pair<std::string, bool>, std::pair<std::size_t, std::size_t>> disagree;  // (k, n)
  // E5 paired columns.
  std::vector<std::vector<double>> e5_cols(2 + kE5Others.size());
  std::size_t e5_hits = 0, artefact_total = 0, artefact_bg_top = 0;

  for (const auto& r : records) {
    if (r.failed) {
      ++m.failed;
      continue;
    }
    if (r.set == "gradient" || r.set == "background") {
      ++artefact_total;
      if (r.most_influential == Decision::Background) ++artefact_bg_top;
      if (!r.paired) continue;
      auto delta = [&](Decision d) {
        const auto i = static_cast<std::size_t>(d);
        return r.eta2[i] - r.baseline_eta2[i];
      };
      const double h = delta(Decision::Background) + delta(Decision::Band);
      e5_cols[0].push_back(h);
      e5_cols[1].push_back(delta(Decision::Background));
      double max_other = -INFINITY;
      for (std::size_t o = 0; o < kE5Others.size(); ++o) {
        e5_cols[2 + o].push_back(h - delta(kE5Others[o]));
        max_other = std::max(max_other, delta(kE5Others[o]));
      }
      if (h > 0 && h > max_other) ++e5_hits;
      continue;
    }

    ++m.main_n;
    // R is undefined when the default universe fails; count it as 0 support.
    const double score = std::isfinite(r.robustness) ? r.robustness : 0.0;
    const bool is_null = r.true_lfc == 0.0;
    const bool evaluable = r.verdict != Verdict::NotEvaluable;
    (is_null ? neg : pos).push_back(score);
    if (evaluable) {
      ++main_eval;
      (is_null ? neg_eval : pos_eval).push_back(score);
    }
    auto& level_stratum = strata[std::string("level=") + artifact_label(r.level)];
    (is_null ? level_stratum.second : level_stratum.first).push_back(score);
    if (!is_null) {
      std::ostringstream key;
      key << "|lfc|=" << std::fabs(r.true_lfc);
      strata[key.str()].first.push_back(score);
    }
    if (is_null) {
      ++nulls;
      if (r.joint_p < alpha) ++null_joint_sig;
      if (r.default_p < alpha) ++null_default_sig;
      if (evaluable) {
        ++nulls_eval;
        if (r.verdict == Verdict::Robust) ++null_eval_robust;
        if (r.joint_p < alpha) ++null_eval_joint_sig;
      }
    }
    const std::pair<std::string, bool> cell{artifact_label(r.level), !is_null};
    if (r.p_min > 0 && std::isfinite(r.p_max)) ranges[cell].push_back(std::log10(r.p_max / r.p_min));
    if (std::isfinite(r.significant_fraction)) {
      auto& [k, n] = disagree[cell];
      ++n;
      if (r.significant_fraction > 0.0 && r.significant_fraction < 1.0) ++k;
    }
  }

  if (m.main_n) m.evaluable_fraction = static_cast<double>(main_eval) / static_cast<double>(m.main_n);
  m.e2_auc = bootstrap_auc(pos_eval, neg_eval, derive_seed(boot, 2));
  if (!pos.empty() && !neg.empty()) m.e2_auc_all_designs = roc_auc(pos, neg);
  for (auto& [name, sets] : strata) {
    auto& negatives = sets.second.empty() ? neg : sets.second;  // effect-size strata vs all nulls
    if (!sets.first.empty() && !negatives.empty())
      m.e2_auc_by_stratum.push_back({name, roc_auc(sets.first, negatives)});
  }
  m.e3_null_robust = wilson_interval(null_eval_robust, nulls_eval);
  m.e4_type1 = wilson_interval(null_eval_joint_sig, nulls_eval);
  if (nulls) m.e4_type1_all_designs = static_cast<double>(null_joint_sig) / static_cast<double>(nulls);
  m.default_type1 = wilson_interval(null_default_sig, nulls);

  std::uint64_t stream = 10;
  for (const char* level : {"clean", "moderate", "severe"})
    for (bool real : {true, false}) {
      DispersionRow row;
      row.level = level;
      row.real = real;
      const auto it = ranges.find({level, real});
      if (it != ranges.end()) row.orders = bootstrap_median(it->second, derive_seed(boot, stream));
      const auto d = disagree.find({level, real});
      if (d != disagree.end()) row.disagree = wilson_interval(d->second.first, d->second.second);
      ++stream;
      if (row.orders.n || row.disagree.n) m.e1.push_back(row);
    }

  m.e5.n = e5_cols[0].size();
  if (m.e5.n) {
    const auto ci = bootstrap_means(e5_cols, derive_seed(boot, 5), 2000, kE5Confidence);
    m.e5.mean_handling = ci[0];
    m.e5.mean_background_alone = ci[1];
    for (std::size_t o = 0; o < kE5Others.size(); ++o) m.e5.mean_margin[o] = ci[2 + o];
    m.e5.hit_rate = wilson_interval(e5_hits, m.e5.n);
  }
  if (artefact_total)
    m.e5_background_top_rate = static_cast<double>(artefact_bg_top) / static_cast<double>(artefact_total);
  return m;
}

}  // namespace blotverse
