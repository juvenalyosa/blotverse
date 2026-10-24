#include "blotverse/analysis/aggregate.hpp"

#include <Eigen/Dense>

#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include "blotverse/core/parallel.hpp"
#include "blotverse/core/rng.hpp"

namespace blotverse {
namespace {

constexpr std::uint64_t kJointSeedStream = 0x101E7;

int sign_of(double v) { return (v > 0) - (v < 0); }

// Residual sum of squares of y on [1, dummies of the given decisions].
double ols_rss(const std::vector<std::array<std::uint8_t, kNumDecisions>>& rows,
               const Eigen::VectorXd& y, const std::vector<std::size_t>& decisions,
               const std::array<std::vector<std::uint8_t>, kNumDecisions>& levels) {
  std::size_t cols = 1;
  for (auto d : decisions) cols += levels[d].size() - 1;
  Eigen::MatrixXd X = Eigen::MatrixXd::Zero(static_cast<Eigen::Index>(rows.size()),
                                            static_cast<Eigen::Index>(cols));
  for (std::size_t r = 0; r < rows.size(); ++r) {
    const auto ri = static_cast<Eigen::Index>(r);
    X(ri, 0) = 1.0;
    Eigen::Index c = 1;
    for (auto d : decisions) {
      // First level is the reference category.
      for (std::size_t l = 1; l < levels[d].size(); ++l, ++c)
        if (rows[r][d] == levels[d][l]) X(ri, c) = 1.0;
    }
  }
  const Eigen::VectorXd beta = X.colPivHouseholderQr().solve(y);
  return (y - X * beta).squaredNorm();
}

}  // namespace

const char* verdict_label(Verdict v) {
  switch (v) {
    case Verdict::Robust: return "Robust";
    case Verdict::Fragile: return "Fragile";
    case Verdict::NotSupported: return "Not supported";
  }
  return "?";
}

std::array<double, kNumDecisions> decision_influence(const std::vector<Universe>& universes,
                                                     const std::vector<double>& lfc,
                                                     const std::vector<bool>& valid) {
  std::array<double, kNumDecisions> eta2{};
  std::vector<Universe> rows;
  std::vector<double> ys;
  for (std::size_t i = 0; i < universes.size(); ++i)
    if (valid[i]) {
      rows.push_back(universes[i]);
      ys.push_back(lfc[i]);
    }
  if (rows.size() < 3) return eta2;

  std::array<std::vector<std::uint8_t>, kNumDecisions> levels;
  std::vector<std::size_t> active;
  for (std::size_t d = 0; d < kNumDecisions; ++d) {
    std::set<std::uint8_t> seen;
    for (const auto& r : rows) seen.insert(r[d]);
    levels[d].assign(seen.begin(), seen.end());
    if (levels[d].size() > 1) active.push_back(d);
  }
  const Eigen::VectorXd y = Eigen::Map<const Eigen::VectorXd>(ys.data(), static_cast<Eigen::Index>(ys.size()));
  const double tss = (y.array() - y.mean()).square().sum();
  if (!(tss > 0)) return eta2;
  const double rss_full = ols_rss(rows, y, active, levels);
  for (std::size_t d : active) {
    std::vector<std::size_t> others;
    for (std::size_t o : active)
      if (o != d) others.push_back(o);
    eta2[d] = std::max(0.0, (ols_rss(rows, y, others, levels) - rss_full) / tss);
  }
  return eta2;
}

std::size_t dominant_significant_count(const std::vector<UniverseOutcome>& outcomes, double alpha) {
  std::size_t pos = 0, neg = 0;
  for (const auto& o : outcomes) {
    if (!o.valid || !(o.p < alpha)) continue;
    if (o.lfc > 0) ++pos;
    else if (o.lfc < 0) ++neg;
  }
  return std::max(pos, neg);
}

std::vector<JointTest> joint_test(const MultiverseSpec& spec, const MultiverseData& data,
                                  const MultiverseResult& result, std::size_t permutations,
                                  unsigned threads) {
  const std::size_t nc = result.contrasts.size();
  std::vector<JointTest> tests(nc);
  for (std::size_t c = 0; c < nc; ++c) {
    tests[c].observed = dominant_significant_count(result.outcomes[c], spec.alpha);
    tests[c].null_counts.assign(permutations, 0);
  }
  if (permutations == 0) return tests;

  // Lanes of each blot, so labels are shuffled within blots (keeps the design
  // every normalisation strategy relies on, e.g. controls on every blot).
  std::map<int, std::vector<std::size_t>> by_blot;
  for (std::size_t i = 0; i < data.observations.size(); ++i) by_blot[data.observations[i].blot].push_back(i);
  const std::vector<int> observed_labels = data.labels();

  // Product over blots of the multinomial coefficient of the group counts.
  double log_relabelings = 0.0;
  for (const auto& [blot, members] : by_blot) {
    std::map<int, std::size_t> counts;
    for (auto i : members) ++counts[observed_labels[i]];
    log_relabelings += std::lgamma(static_cast<double>(members.size()) + 1.0);
    for (const auto& [g, n] : counts) log_relabelings -= std::lgamma(static_cast<double>(n) + 1.0);
  }
  for (auto& t : tests) t.distinct_relabelings = std::round(std::exp(log_relabelings));

  parallel_for(
      permutations,
      [&](std::size_t k) {
        Rng rng(derive_seed(spec.inference.seed ^ kJointSeedStream, k));
        std::vector<int> labels = observed_labels;
        for (const auto& [blot, members] : by_blot) {
          std::vector<int> l;
          for (auto i : members) l.push_back(observed_labels[i]);
          rng.shuffle(l);
          for (std::size_t j = 0; j < members.size(); ++j) labels[members[j]] = l[j];
        }
        const auto outcomes =
            evaluate_universes(spec, data, result.plan.universes, result.contrasts, labels, 1);
        for (std::size_t c = 0; c < nc; ++c)
          tests[c].null_counts[k] = dominant_significant_count(outcomes[c], spec.alpha);
      },
      threads);

  for (auto& t : tests) {
    std::size_t hits = 0;
    for (auto n : t.null_counts)
      if (n >= t.observed) ++hits;
    t.p = (1.0 + static_cast<double>(hits)) / (1.0 + static_cast<double>(permutations));
  }
  return tests;
}

Verdict decide_verdict(const VerdictSpec& v, double robustness, double joint_p) {
  if (!std::isfinite(robustness) || !std::isfinite(joint_p) || !(joint_p < v.joint_alpha))
    return Verdict::NotSupported;
  if (robustness >= v.robust_min_r) return Verdict::Robust;
  if (robustness >= v.fragile_min_r) return Verdict::Fragile;
  return Verdict::NotSupported;
}

ContrastSummary summarize_contrast(const MultiverseSpec& spec, const MultiverseResult& result,
                                   std::size_t contrast_index, JointTest joint) {
  const auto& outcomes = result.outcomes.at(contrast_index);
  ContrastSummary s;
  s.contrast = result.contrasts.at(contrast_index);
  s.n_universes = outcomes.size();
  s.n_excluded = result.plan.excluded.size();
  s.joint = std::move(joint);

  std::vector<double> lfcs, ps, all_lfc(outcomes.size(), 0.0);
  std::vector<bool> valid(outcomes.size(), false);
  for (std::size_t i = 0; i < outcomes.size(); ++i) {
    const auto& o = outcomes[i];
    if (!o.valid) continue;
    valid[i] = true;
    all_lfc[i] = o.lfc;
    lfcs.push_back(o.lfc);
    ps.push_back(o.p);
  }
  s.n_valid = lfcs.size();

  if (result.plan.default_index) {
    const auto& d = outcomes[*result.plan.default_index];
    s.default_valid = d.valid;
    s.default_lfc = d.lfc;
    s.default_p = d.p;
    if (!d.valid) s.notes.push_back("default universe could not be evaluated: " + d.note);
  } else {
    s.notes.push_back("default universe is excluded for this experiment");
  }

  if (s.n_valid > 0) {
    const double n = static_cast<double>(s.n_valid);
    std::size_t sig = 0, robust = 0, opposite = 0, opposite_sig = 0, agree = 0;
    const int default_sign = s.default_valid ? sign_of(s.default_lfc) : 0;
    const bool default_sig = s.default_valid && s.default_p < spec.alpha;
    for (const auto& o : outcomes) {
      if (!o.valid) continue;
      const bool is_sig = o.p < spec.alpha;
      const int sgn = sign_of(o.lfc);
      if (is_sig) ++sig;
      if (s.default_valid) {
        if (is_sig && sgn == default_sign) ++robust;
        if (sgn == -default_sign && sgn != 0) {
          ++opposite;
          if (is_sig) ++opposite_sig;
        }
        if (is_sig == default_sig && (!is_sig || sgn == default_sign)) ++agree;
      }
    }
    s.significant_fraction = static_cast<double>(sig) / n;
    if (s.default_valid) {
      s.robustness = static_cast<double>(robust) / n;
      s.opposite_fraction = static_cast<double>(opposite) / n;
      s.opposite_significant_fraction = static_cast<double>(opposite_sig) / n;
      s.agreement = static_cast<double>(agree) / n;
    }
    s.median_lfc = median(lfcs);
    s.lfc_q05 = quantile(lfcs, 0.05);
    s.lfc_q95 = quantile(lfcs, 0.95);
    s.p_min = *std::min_element(ps.begin(), ps.end());
    s.p_max = *std::max_element(ps.begin(), ps.end());
  }
  if (s.n_valid < s.n_universes)
    s.notes.push_back(std::to_string(s.n_universes - s.n_valid) + " universe(s) could not be evaluated");

  s.eta2 = decision_influence(result.plan.universes, all_lfc, valid);
  double best = 0.0;
  for (Decision d : kDecisions)
    if (s.eta2[static_cast<std::size_t>(d)] > best) {
      best = s.eta2[static_cast<std::size_t>(d)];
      s.most_influential = d;
    }

  // With few lanes per blot the joint test has a floor that no data can beat
  // (the observed labelling and its mirror image are always as extreme).
  if (s.joint.distinct_relabelings > 0 && 2.0 / s.joint.distinct_relabelings >= spec.verdict.joint_alpha)
    s.notes.push_back("design too small for the joint test: only " +
                      std::to_string(static_cast<long long>(s.joint.distinct_relabelings)) +
                      " within-blot relabellings exist, so its p-value cannot fall below about " +
                      std::to_string(2.0 / s.joint.distinct_relabelings).substr(0, 5) +
                      "; a 'Robust' verdict is unreachable with this layout");

  s.verdict = decide_verdict(spec.verdict, s.robustness, s.joint.p);
  return s;
}

}  // namespace blotverse
