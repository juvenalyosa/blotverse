#include "blotverse/analysis/multiverse.hpp"

#include <cmath>
#include <stdexcept>

#include "blotverse/analysis/normalization.hpp"
#include "blotverse/analysis/stats.hpp"
#include "blotverse/core/parallel.hpp"
#include "blotverse/core/rng.hpp"

namespace blotverse {
namespace {

constexpr std::uint64_t kTestSeedStream = 0x7E57;

std::size_t idx(Decision d) { return static_cast<std::size_t>(d); }

UniverseOutcome invalid(std::string why) {
  UniverseOutcome o;
  o.note = std::move(why);
  return o;
}

// Evaluates one universe for all contrasts; normalisation is shared.
void evaluate_one(const MultiverseSpec& spec, const MultiverseData& data, const Universe& u,
                  std::size_t universe_index, const std::vector<Contrast>& contrasts,
                  const std::vector<int>& labels,
                  std::vector<std::vector<UniverseOutcome>>& out) {
  const auto& q = data.quant.at(u[idx(Decision::Background)]).at(u[idx(Decision::Band)]);
  const Method loading = spec.branch(Decision::Loading, u[idx(Decision::Loading)]).method;
  const Method norm = spec.branch(Decision::Normalization, u[idx(Decision::Normalization)]).method;
  const Method scale = spec.branch(Decision::Scale, u[idx(Decision::Scale)]).method;
  const Method test = spec.branch(Decision::Test, u[idx(Decision::Test)]).method;

  auto fail_all = [&](const std::string& why) {
    for (std::size_t c = 0; c < contrasts.size(); ++c) out[c][universe_index] = invalid(why);
  };

  // 1. Loading control.
  const auto& denominator = loading == Method::Housekeeping ? q.reference : q.total;
  if (denominator.size() != q.target.size()) {
    fail_all("loading control unavailable");
    return;
  }
  std::vector<double> y;
  std::vector<int> blot, group;
  std::size_t dropped = 0;
  for (std::size_t i = 0; i < q.target.size(); ++i) {
    const double num = q.target[i], den = denominator[i];
    if (!(num > 0) || !(den > 0) || !std::isfinite(num) || !std::isfinite(den)) {
      ++dropped;  // non-positive after background subtraction: not quantifiable here
      continue;
    }
    y.push_back(num / den);
    blot.push_back(data.observations[i].blot);
    group.push_back(labels[i]);
  }

  // 2. Replicate normalisation.
  const auto normalised = normalize_replicates(norm, y, blot, group, data.n_blots,
                                               static_cast<int>(data.groups.size()));
  if (!normalised) {
    fail_all(std::string("normalisation undefined (") + method_key(norm) + ")");
    return;
  }

  for (std::size_t c = 0; c < contrasts.size(); ++c) {
    const int treat_group = contrasts[c].treatment;
    // 3. Effect scale.
    std::vector<double> ctrl, trt, values;
    std::vector<int> treated, gel;
    for (std::size_t i = 0; i < normalised->size(); ++i) {
      if (group[i] != 0 && group[i] != treat_group) continue;
      const double v = scale == Method::LogRatio ? std::log2((*normalised)[i]) : (*normalised)[i];
      (group[i] == 0 ? ctrl : trt).push_back(v);
      values.push_back(v);
      treated.push_back(group[i] == 0 ? 0 : 1);
      gel.push_back(blot[i]);
    }

    // 4. Statistical test.
    TestResult r;
    switch (test) {
      case Method::Welch: r = welch_test(ctrl, trt); break;
      case Method::MixedModel: r = mixed_model_test(values, treated, gel); break;
      case Method::Permutation:
        r = permutation_test(ctrl, trt, spec.inference.test_max_permutations,
                             derive_seed(spec.inference.seed ^ kTestSeedStream,
                                         universe_index * 64 + c));
        break;
      default: throw std::logic_error("unknown test method");
    }

    UniverseOutcome o;
    o.n_control = static_cast<std::uint16_t>(ctrl.size());
    o.n_treatment = static_cast<std::uint16_t>(trt.size());
    if (!r.ok) {
      o.note = r.error;
    } else {
      o.effect = r.estimate;
      o.p = r.p;
      if (scale == Method::LogRatio) {
        o.lfc = r.estimate;
      } else {
        const double mc = mean(ctrl);
        const double ratio = (mc + r.estimate) / mc;
        o.lfc = ratio > 0 ? std::log2(ratio) : kNaN;
      }
      o.valid = std::isfinite(o.lfc) && std::isfinite(o.p);
      if (!o.valid) o.note = "effect not defined on the log scale";
      else if (dropped > 0) o.note = std::to_string(dropped) + " lane(s) non-positive after background";
    }
    out[c][universe_index] = std::move(o);
  }
}

}  // namespace

std::vector<int> MultiverseData::labels() const {
  std::vector<int> l;
  l.reserve(observations.size());
  for (const auto& o : observations) l.push_back(o.group);
  return l;
}

UniversePlan plan_universes(const MultiverseSpec& spec, const ExperimentFacts& facts) {
  UniversePlan plan;
  for (const Universe& u : spec.enumerate()) {
    if (auto reason = exclusion_reason(spec, u, facts)) {
      plan.excluded.push_back({u, *reason});
      continue;
    }
    if (u == spec.default_universe) plan.default_index = plan.universes.size();
    plan.universes.push_back(u);
  }
  return plan;
}

std::vector<Contrast> make_contrasts(const std::vector<std::string>& groups) {
  if (groups.size() < 2) throw std::runtime_error("at least two groups are required");
  std::vector<Contrast> out;
  for (std::size_t g = 1; g < groups.size(); ++g)
    out.push_back({static_cast<int>(g), groups[g] + " vs " + groups[0]});
  return out;
}

std::vector<std::vector<UniverseOutcome>> evaluate_universes(
    const MultiverseSpec& spec, const MultiverseData& data, const std::vector<Universe>& universes,
    const std::vector<Contrast>& contrasts, const std::vector<int>& labels, unsigned threads) {
  if (labels.size() != data.observations.size())
    throw std::invalid_argument("evaluate_universes: one label per observation required");
  std::vector<std::vector<UniverseOutcome>> out(contrasts.size(),
                                                std::vector<UniverseOutcome>(universes.size()));
  parallel_for(
      universes.size(),
      [&](std::size_t i) { evaluate_one(spec, data, universes[i], i, contrasts, labels, out); },
      threads);
  return out;
}

MultiverseResult run_multiverse(const MultiverseSpec& spec, const MultiverseData& data,
                                unsigned threads) {
  MultiverseResult result;
  result.plan = plan_universes(spec, data.facts);
  result.contrasts = make_contrasts(data.groups);
  result.outcomes = evaluate_universes(spec, data, result.plan.universes, result.contrasts,
                                       data.labels(), threads);
  return result;
}

}  // namespace blotverse
