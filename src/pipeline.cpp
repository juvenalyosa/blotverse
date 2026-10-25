#include "blotverse/pipeline.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <sstream>
#include <stdexcept>

#include "blotverse/analysis/stats.hpp"
#include "blotverse/core/parallel.hpp"
#include "blotverse/imaging/background.hpp"
#include "blotverse/imaging/quantify.hpp"

namespace blotverse {
namespace {

// Background-corrected images for one blot under one background branch.
struct Corrected {
  ImageF target;
  std::optional<ImageF> total;
};

Corrected correct_background(const Blot& blot, const ImageF& target, const std::optional<ImageF>& total,
                             const Annotation& ann, const Branch& branch) {
  Corrected c;
  if (branch.method == Method::RollingBall) {
    const double radius = branch.radius_factor * ann.band_height();
    c.target = subtract(target, rolling_ball_background(target, radius));
    if (total) c.total = subtract(*total, rolling_ball_background(*total, radius));
  } else if (branch.method == Method::LocalBackground) {
    c.target = local_background_bands(target, ann);
    if (total) c.total = local_background_lanes(*total, ann);
  } else {
    throw std::logic_error("unknown background method for blot " + blot.id);
  }
  return c;
}

BandMethod band_method(Method m) {
  switch (m) {
    case Method::FixedRectangle: return BandMethod::FixedRectangle;
    case Method::ProfilePeak: return BandMethod::ProfilePeak;
    case Method::ThresholdMask: return BandMethod::ThresholdMask;
    default: throw std::logic_error("not a band definition method");
  }
}

}  // namespace

PreparedExperiment prepare(const Experiment& experiment, const MultiverseSpec& spec, unsigned threads) {
  PreparedExperiment prep;
  auto& data = prep.data;
  data.groups = experiment.groups();
  data.n_blots = static_cast<int>(experiment.blots.size());
  std::map<std::string, int> group_index;
  for (std::size_t g = 0; g < data.groups.size(); ++g) group_index[data.groups[g]] = static_cast<int>(g);

  // --- [2]-[3] Images, lanes/bands and quality control, per blot ---------------
  const std::size_t nb = experiment.blots.size();
  std::vector<ImageF> target_f(nb);
  std::vector<std::optional<ImageF>> total_f(nb);
  prep.blots.resize(nb);
  parallel_for(
      nb,
      [&](std::size_t b) {
        const Blot& blot = experiment.blots[b];
        target_f[b] = to_float(blot.image.pixels);
        if (blot.total_protein) total_f[b] = to_float(blot.total_protein->pixels);
        auto& pb = prep.blots[b];
        try {
          pb.annotation = blot.annotation ? *blot.annotation
                                           : detect_annotation(target_f[b], blot.lanes.size(), blot.hints);
          pb.annotation.validate(target_f[b].width(), target_f[b].height());
        } catch (const std::exception& e) {
          throw std::runtime_error("blot '" + blot.id + "': " + e.what());
        }
        pb.qc = check_blot(blot.id, blot.image, blot.total_protein ? &*blot.total_protein : nullptr,
                           pb.annotation, spec.qc.saturation_min_pixels);
      },
      threads);

  data.facts.has_total_protein = experiment.has_total_protein();
  for (std::size_t b = 0; b < nb; ++b) {
    const auto& ann = prep.blots[b].annotation;
    const int w = target_f[b].width(), h = target_f[b].height();
    auto check = check_local_background(ann, w, h);
    if (check.valid && total_f[b]) check = check_local_background_lanes(ann, w, h);
    if (!check.valid) {
      data.facts.local_background_valid = false;
      prep.warnings.push_back("blot '" + experiment.blots[b].id +
                              "': local background excluded (" + check.reason + ")");
    }
    for (const auto& warning : prep.blots[b].qc.warnings)
      prep.warnings.push_back("blot '" + experiment.blots[b].id + "': " + warning);
    for (std::size_t l = 0; l < prep.blots[b].qc.lanes.size(); ++l) {
      const auto& lq = prep.blots[b].qc.lanes[l];
      for (const auto& f : lq.flags)
        prep.warnings.push_back("blot '" + experiment.blots[b].id + "' lane " + std::to_string(l + 1) +
                                ": " + f + (lq.excluded ? " -> excluded from every universe" : ""));
      if (!lq.excluded)
        data.observations.push_back(
            {static_cast<int>(b), static_cast<int>(l), group_index.at(experiment.blots[b].lanes[l].group)});
    }
  }
  // Design facts used by the exclusion rules (computed on the QC-passed lanes).
  {
    std::map<std::pair<int, int>, std::size_t> per_blot_group;
    std::vector<std::size_t> per_group(data.groups.size(), 0);
    for (const auto& o : data.observations) {
      ++per_blot_group[{o.blot, o.group}];
      ++per_group[static_cast<std::size_t>(o.group)];
    }
    for (int b = 0; b < data.n_blots; ++b)
      for (std::size_t g = 0; g < data.groups.size(); ++g) {
        const auto it = per_blot_group.find({b, static_cast<int>(g)});
        if (it != per_blot_group.end() && it->second < 2) data.facts.every_blot_has_two_lanes_per_group = false;
      }
    for (std::size_t g = 1; g < data.groups.size(); ++g)
      if (permutation_min_p(per_group[0], per_group[g], spec.inference.test_max_permutations) >= spec.alpha)
        data.facts.permutation_can_reach_alpha = false;
  }
  if (!experiment.has_total_protein() &&
      std::any_of(experiment.blots.begin(), experiment.blots.end(),
                  [](const Blot& b) { return b.total_protein.has_value(); }))
    prep.warnings.push_back("total-protein images missing for some blots: total-protein branch excluded");

  // --- [4a] Quantification cache: one pass per (blot, background branch) -----
  const auto& bg_branches = spec.decision(Decision::Background).branches;
  const auto& band_branches = spec.decision(Decision::Band).branches;
  const std::size_t n_obs = data.observations.size();
  data.quant.assign(bg_branches.size(), std::vector<QuantValues>(band_branches.size()));
  for (auto& row : data.quant)
    for (auto& q : row) {
      q.target.assign(n_obs, kNaN);
      q.reference.assign(n_obs, kNaN);
      if (data.facts.has_total_protein) q.total.assign(n_obs, kNaN);
    }
  std::vector<std::vector<std::size_t>> obs_of_blot(nb);
  for (std::size_t i = 0; i < n_obs; ++i)
    obs_of_blot[static_cast<std::size_t>(data.observations[i].blot)].push_back(i);

  parallel_for(
      nb * bg_branches.size(),
      [&](std::size_t task) {
        const std::size_t b = task / bg_branches.size();
        const std::size_t k = task % bg_branches.size();
        const auto& ann = prep.blots[b].annotation;
        const Corrected c = correct_background(experiment.blots[b], target_f[b],
                                               data.facts.has_total_protein ? total_f[b] : std::nullopt,
                                               ann, bg_branches[k]);
        for (std::size_t m = 0; m < band_branches.size(); ++m) {
          const BandMethod method = band_method(band_branches[m].method);
          auto& q = data.quant[k][m];
          for (std::size_t i : obs_of_blot[b]) {
            const auto lane = static_cast<std::size_t>(data.observations[i].lane);
            q.target[i] = quantify_band(c.target, ann, lane, BandKind::Target, method);
            q.reference[i] = quantify_band(c.target, ann, lane, BandKind::Reference, method);
            if (c.total) q.total[i] = quantify_lane_total(*c.total, ann, lane, method);
          }
        }
      },
      threads);

  // Dilution series on a blot (lanes that declare a load): flag the lanes
  // outside the proportional range, measured with the default branches.
  const auto& def = spec.default_universe;
  const auto& q_default = data.quant[def[static_cast<std::size_t>(Decision::Background)]]
                                    [def[static_cast<std::size_t>(Decision::Band)]];
  for (std::size_t b = 0; b < nb; ++b) {
    std::vector<double> loads, signals;
    std::vector<std::size_t> lanes;
    for (std::size_t i : obs_of_blot[b]) {
      const auto lane = static_cast<std::size_t>(data.observations[i].lane);
      if (const auto& load = experiment.blots[b].lanes[lane].load) {
        loads.push_back(*load);
        signals.push_back(q_default.target[i]);
        lanes.push_back(lane);
      }
    }
    if (loads.size() < 3) continue;
    const auto flags = linear_range_flags(loads, signals, spec.qc.linear_range_min_slope,
                                          spec.qc.linear_range_max_slope);
    for (std::size_t k = 0; k < flags.size(); ++k)
      if (flags[k]) {
        std::ostringstream msg;
        msg << "blot '" << experiment.blots[b].id << "' lane " << lanes[k] + 1 << " (load " << loads[k]
            << "): outside the proportional range of the dilution series";
        prep.warnings.push_back(msg.str());
      }
  }
  return prep;
}

AnalysisResult analyze(const Experiment& experiment, const MultiverseSpec& spec,
                       const AnalysisOptions& options) {
  const auto start = std::chrono::steady_clock::now();
  PreparedExperiment prep = prepare(experiment, spec, options.threads);

  AnalysisResult result;
  result.blots = std::move(prep.blots);
  result.data = std::move(prep.data);
  result.warnings = std::move(prep.warnings);

  // --- [4b] Every universe; [5] aggregation and joint inference --------------
  result.multiverse = run_multiverse(spec, result.data, options.threads);
  result.joint_permutations = options.joint_permutations.value_or(spec.inference.joint_permutations);
  auto joint = joint_test(spec, result.data, result.multiverse, result.joint_permutations, options.threads);
  for (std::size_t c = 0; c < result.multiverse.contrasts.size(); ++c)
    result.summaries.push_back(summarize_contrast(spec, result.multiverse, c, std::move(joint[c])));

  result.seconds =
      std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  return result;
}

}  // namespace blotverse
