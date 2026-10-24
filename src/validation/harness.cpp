#include "blotverse/validation/harness.hpp"

#include <yaml-cpp/yaml.h>

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>

#include "blotverse/core/numeric.hpp"
#include "blotverse/core/parallel.hpp"
#include "blotverse/core/rng.hpp"
#include "blotverse/io/sha256.hpp"
#include "blotverse/pipeline.hpp"
#include "blotverse/version.hpp"

namespace blotverse {
namespace {

struct Design {
  int gels;
  int reps;
};
// 3 to 6 replicates per group spread over several gels (plan section 8).
constexpr Design kDesigns[] = {{3, 1}, {2, 2}, {3, 2}, {2, 3}};

ExperimentRecord run_one(const MultiverseSpec& spec, const HarnessConfig& cfg, std::size_t id,
                         bool gradient_set) {
  Rng rng(derive_seed(cfg.seed, id * 2 + (gradient_set ? 1 : 0)));
  ExperimentRecord rec;
  rec.id = id;
  rec.set = gradient_set ? "background" : "main";
  rec.level = gradient_set ? ArtifactLevel::Moderate
                           : cfg.levels[static_cast<std::size_t>(rng.uniform_int(cfg.levels.size()))];
  const Design design = kDesigns[rng.uniform_int(std::size(kDesigns))];
  rec.n_gels = design.gels;
  rec.reps_per_gel = design.reps;
  const bool null = id % 2 == 0;  // exactly half nulls
  if (!null) {
    const double size = cfg.effect_sizes[static_cast<std::size_t>(rng.uniform_int(cfg.effect_sizes.size()))];
    rec.true_lfc = rng.uniform() < 0.5 ? -size : size;
  }

  SimParams p = sim_preset(rec.level);
  p.n_gels = design.gels;
  p.reps_per_gel = design.reps;
  p.true_lfc = {0.0, rec.true_lfc};
  p.seed = rng.next_u64();
  const SimParams baseline = p;
  if (gradient_set) {
    p.gradient = cfg.planted_gradient;
    p.n_blotches = cfg.planted_blotches;
    p.blotch_amplitude = cfg.planted_blotch_amplitude;
  }

  try {
    const Experiment ex = to_experiment(simulate(p));
    AnalysisOptions opts;
    opts.threads = 1;
    opts.joint_permutations = cfg.joint_permutations;
    const AnalysisResult res = analyze(ex, spec, opts);
    if (gradient_set) {
      // Same experiment (same seed) without the planted artefact; only the
      // influence analysis is needed, so the joint test is skipped.
      AnalysisOptions base_opts = opts;
      base_opts.joint_permutations = 0;
      rec.baseline_eta2 = analyze(to_experiment(simulate(baseline)), spec, base_opts).summaries.at(0).eta2;
      rec.paired = true;
    }
    const auto& s = res.summaries.at(0);
    rec.robustness = s.robustness;
    rec.joint_p = s.joint.p;
    rec.verdict = s.verdict;
    rec.default_lfc = s.default_lfc;
    rec.default_p = s.default_p;
    rec.median_lfc = s.median_lfc;
    rec.p_min = s.p_min;
    rec.p_max = s.p_max;
    rec.n_valid = s.n_valid;
    rec.most_influential = s.most_influential;
    rec.eta2 = s.eta2;
  } catch (const std::exception& e) {
    rec.failed = true;
    rec.error = e.what();
  }
  return rec;
}

std::string pct(double v) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(1) << 100.0 * v << " %";
  return ss.str();
}

std::string fmt(double v, int precision = 3) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << v;
  return ss.str();
}

}  // namespace

bool HarnessMetrics::g2_pass() const {
  return e2_auc >= 0.85 && e3_null_robust_rate <= 0.05 && e4_joint_type1 <= 0.05;
}

HarnessMetrics compute_metrics(const std::vector<ExperimentRecord>& records, double alpha) {
  HarnessMetrics m;
  std::vector<double> pos, neg, pos_eval, neg_eval, p_ranges, null_ranges;
  std::map<std::string, std::vector<double>> ranges_by_level;
  std::size_t nulls = 0, null_robust = 0, null_joint_sig = 0, null_default_sig = 0;
  std::size_t nulls_eval = 0, null_joint_sig_eval = 0, main_total = 0, main_eval = 0;
  std::size_t gradient_total = 0, gradient_hits = 0, paired_total = 0, paired_hits = 0;
  std::map<std::string, std::pair<std::vector<double>, std::vector<double>>> strata;

  for (const auto& r : records) {
    if (r.failed) {
      ++m.failed;
      continue;
    }
    // R is undefined when the default universe fails; count it as 0 support.
    const double score = std::isfinite(r.robustness) ? r.robustness : 0.0;
    if (r.set == "gradient" || r.set == "background") {
      ++gradient_total;
      if (r.most_influential == Decision::Background) ++gradient_hits;
      if (r.paired) {
        std::array<double, kNumDecisions> delta{};
        for (std::size_t d = 0; d < kNumDecisions; ++d) delta[d] = r.eta2[d] - r.baseline_eta2[d];
        const double handling = delta[static_cast<std::size_t>(Decision::Background)] +
                                delta[static_cast<std::size_t>(Decision::Band)];
        double other = -INFINITY;
        for (Decision d : {Decision::Loading, Decision::Normalization, Decision::Scale, Decision::Test})
          other = std::max(other, delta[static_cast<std::size_t>(d)]);
        ++paired_total;
        if (handling > 0 && handling > other) ++paired_hits;
      }
      continue;
    }
    const bool is_null = r.true_lfc == 0.0;
    (is_null ? neg : pos).push_back(score);
    const bool evaluable = r.verdict != Verdict::NotEvaluable;
    ++main_total;
    if (evaluable) {
      ++main_eval;
      (is_null ? neg_eval : pos_eval).push_back(score);
      if (is_null) {
        ++nulls_eval;
        if (r.joint_p < alpha) ++null_joint_sig_eval;
      }
    }
    auto& level_stratum = strata[std::string("level=") + artifact_label(r.level)];
    (is_null ? level_stratum.second : level_stratum.first).push_back(score);
    if (!is_null) strata["|lfc|=" + fmt(std::fabs(r.true_lfc), 1)].first.push_back(score);
    if (is_null) {
      ++nulls;
      if (r.verdict == Verdict::Robust) ++null_robust;
      if (r.joint_p < alpha) ++null_joint_sig;
      if (r.default_p < alpha) ++null_default_sig;
    }
    if (r.p_min > 0 && std::isfinite(r.p_max)) {
      const double range = std::log10(r.p_max / r.p_min);
      if (!is_null) ranges_by_level[artifact_label(r.level)].push_back(range);
      if (r.level == ArtifactLevel::Moderate) (is_null ? null_ranges : p_ranges).push_back(range);
    }
  }
  if (!pos.empty() && !neg.empty()) m.e2_auc_all_designs = roc_auc(pos, neg);
  if (!pos_eval.empty() && !neg_eval.empty()) m.e2_auc = roc_auc(pos_eval, neg_eval);
  if (main_total) m.evaluable_fraction = static_cast<double>(main_eval) / static_cast<double>(main_total);
  if (nulls_eval) m.e4_joint_type1 = static_cast<double>(null_joint_sig_eval) / static_cast<double>(nulls_eval);
  for (auto& [name, sets] : strata) {
    // Effect-size strata are compared against all nulls.
    auto& negatives = sets.second.empty() ? neg : sets.second;
    if (!sets.first.empty() && !negatives.empty())
      m.e2_auc_by_stratum.push_back({name, roc_auc(sets.first, negatives)});
  }
  if (nulls) {
    m.e3_null_robust_rate = static_cast<double>(null_robust) / static_cast<double>(nulls);
    m.e4_joint_type1_all_designs = static_cast<double>(null_joint_sig) / static_cast<double>(nulls);
    m.default_type1 = static_cast<double>(null_default_sig) / static_cast<double>(nulls);
  }
  if (!null_ranges.empty()) m.e1_null_median = median(null_ranges);
  for (const char* level : {"clean", "moderate", "severe"})
    if (ranges_by_level.count(level)) m.e1_median_by_level.push_back({level, median(ranges_by_level[level])});
  if (!p_ranges.empty()) {
    m.e1_median_log10_p_range = median(p_ranges);
    std::size_t over = 0;
    for (double r : p_ranges)
      if (r >= 2.0) ++over;
    m.e1_fraction_over_2_orders = static_cast<double>(over) / static_cast<double>(p_ranges.size());
  }
  if (paired_total)
    m.e5_paired_hit_rate = static_cast<double>(paired_hits) / static_cast<double>(paired_total);
  if (gradient_total)
    m.e5_background_hit_rate = static_cast<double>(gradient_hits) / static_cast<double>(gradient_total);
  return m;
}

HarnessReport run_harness(const MultiverseSpec& spec, const HarnessConfig& config,
                          const ProgressCallback& progress) {
  const auto start = std::chrono::steady_clock::now();
  HarnessReport report;
  report.config = config;
  report.spec_sha256 = sha256_hex(spec.source_text);
  report.simulator_presets = kSimulatorPresetVersion;
  const std::size_t total = config.experiments + config.gradient_experiments;
  report.records.resize(total);
  std::atomic<std::size_t> done{0};
  std::mutex progress_mutex;
  parallel_for(
      total,
      [&](std::size_t i) {
        const bool gradient = i >= config.experiments;
        const std::size_t id = gradient ? i - config.experiments : i;
        report.records[i] = run_one(spec, config, id, gradient);
        const std::size_t d = ++done;
        if (progress) {
          std::lock_guard lock(progress_mutex);
          progress(d, total);
        }
      },
      config.threads);
  report.metrics = compute_metrics(report.records, spec.alpha);
  report.seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  return report;
}

void write_harness_outputs(const std::filesystem::path& dir, const HarnessReport& report) {
  std::filesystem::create_directories(dir);
  {
    std::ofstream out(dir / "records.csv");
    out << std::setprecision(10);
    out << "set,id,level,true_lfc,n_gels,reps_per_gel,failed,R,joint_p,verdict,default_lfc,default_p,"
           "median_lfc,p_min,p_max,n_valid,most_influential";
    for (Decision d : kDecisions) out << ",eta2_" << decision_key(d);
    for (Decision d : kDecisions) out << ",base_eta2_" << decision_key(d);
    out << ",error\n";
    for (const auto& r : report.records) {
      out << r.set << ',' << r.id << ',' << artifact_label(r.level) << ',' << r.true_lfc << ',' << r.n_gels
          << ',' << r.reps_per_gel << ',' << r.failed << ',' << r.robustness << ',' << r.joint_p << ','
          << verdict_label(r.verdict) << ',' << r.default_lfc << ',' << r.default_p << ',' << r.median_lfc
          << ',' << r.p_min << ',' << r.p_max << ',' << r.n_valid << ','
          << (r.most_influential ? decision_key(*r.most_influential) : "");
      for (double e : r.eta2) out << ',' << e;
      for (double e : r.baseline_eta2) out << ',' << (r.paired ? e : kNaN);
      out << ",\"" << r.error << "\"\n";
    }
  }
  const auto& m = report.metrics;
  {
    YAML::Emitter y;
    y << YAML::BeginMap;
    y << YAML::Key << "blotverse_version" << YAML::Value << kVersion;
    y << YAML::Key << "spec_sha256" << YAML::Value << report.spec_sha256;
    y << YAML::Key << "simulator_presets" << YAML::Value << report.simulator_presets;
    y << YAML::Key << "experiments" << YAML::Value << report.config.experiments;
    y << YAML::Key << "gradient_experiments" << YAML::Value << report.config.gradient_experiments;
    y << YAML::Key << "joint_permutations" << YAML::Value << report.config.joint_permutations;
    y << YAML::Key << "seed" << YAML::Value << report.config.seed;
    y << YAML::Key << "failed" << YAML::Value << m.failed;
    y << YAML::Key << "e1_median_log10_p_range" << YAML::Value << m.e1_median_log10_p_range;
    y << YAML::Key << "e1_fraction_over_2_orders" << YAML::Value << m.e1_fraction_over_2_orders;
    y << YAML::Key << "e1_null_median" << YAML::Value << m.e1_null_median;
    y << YAML::Key << "evaluable_fraction" << YAML::Value << m.evaluable_fraction;
    y << YAML::Key << "e2_auc" << YAML::Value << m.e2_auc;
    y << YAML::Key << "e2_auc_all_designs" << YAML::Value << m.e2_auc_all_designs;
    y << YAML::Key << "e2_auc_by_stratum" << YAML::Value << YAML::BeginMap;
    for (const auto& [k, v] : m.e2_auc_by_stratum) y << YAML::Key << k << YAML::Value << v;
    y << YAML::EndMap;
    y << YAML::Key << "e3_null_robust_rate" << YAML::Value << m.e3_null_robust_rate;
    y << YAML::Key << "e4_joint_type1" << YAML::Value << m.e4_joint_type1;
    y << YAML::Key << "e4_joint_type1_all_designs" << YAML::Value << m.e4_joint_type1_all_designs;
    y << YAML::Key << "default_analysis_type1" << YAML::Value << m.default_type1;
    y << YAML::Key << "e5_paired_hit_rate" << YAML::Value << m.e5_paired_hit_rate;
    y << YAML::Key << "e5_background_hit_rate" << YAML::Value << m.e5_background_hit_rate;
    y << YAML::Key << "planted_artefact" << YAML::Value << YAML::Flow << YAML::BeginMap
      << YAML::Key << "gradient" << YAML::Value << report.config.planted_gradient
      << YAML::Key << "blotches" << YAML::Value << report.config.planted_blotches
      << YAML::Key << "blotch_amplitude" << YAML::Value << report.config.planted_blotch_amplitude << YAML::EndMap;
    y << YAML::Key << "g2_pass" << YAML::Value << m.g2_pass();
    y << YAML::Key << "seconds" << YAML::Value << report.seconds;
    y << YAML::EndMap;
    std::ofstream(dir / "metrics.yaml") << y.c_str() << "\n";
  }
  {
    auto mark = [](bool ok) { return ok ? "PASS" : "FAIL"; };
    std::ofstream md(dir / "summary.md");
    md << "# BlotVerse level-1 validation\n\n";
    md << report.config.experiments << " simulated experiments (half null) + " << report.config.gradient_experiments
       << " with a planted background artefact (paired with and without it); " << report.config.joint_permutations
       << " joint permutations each; seed " << report.config.seed << "; " << fmt(report.seconds, 0) << " s; "
       << m.failed << " failed.\n\nBlotVerse " << kVersion << ", spec sha256 `" << report.spec_sha256 << "`, simulator presets v"
       << report.simulator_presets << ".\n\n";
    md << "| Experiment | Metric | Value | Target | Result |\n|---|---|---|---|---|\n";
    md << "| E1 | median log10(p_max/p_min), real effects, moderate artefacts | "
       << fmt(m.e1_median_log10_p_range, 2) << " (" << pct(m.e1_fraction_over_2_orders)
       << " span >= 2 orders) | several orders (>= 2) | " << mark(m.e1_median_log10_p_range >= 2.0) << " |\n";
    md << "| E2 | AUC of R (real vs null), evaluable designs | " << fmt(m.e2_auc) << " | >= 0.85 | "
       << mark(m.e2_auc >= 0.85) << " |\n";
    md << "| E2' | AUC of R, all designs (reference) | " << fmt(m.e2_auc_all_designs) << " | - | - |\n";
    md << "| E3 | nulls labelled Robust | " << pct(m.e3_null_robust_rate) << " | <= 5 % | "
       << mark(m.e3_null_robust_rate <= 0.05) << " |\n";
    md << "| E4 | joint-test type I error, evaluable designs | " << fmt(m.e4_joint_type1) << " | <= 0.05 | "
       << mark(m.e4_joint_type1 <= 0.05) << " |\n";
    md << "| E4' | joint-test type I error, all designs (reference) | " << fmt(m.e4_joint_type1_all_designs)
       << " | - | - |\n";
    md << "| E5 | planted background artefact raises background + band influence most (paired) | "
       << pct(m.e5_rate()) << " | >= 80 % | " << mark(m.e5_rate() >= 0.8) << " |\n";
    md << "| E5' | background is the single most influential decision (reference) | "
       << pct(m.e5_background_hit_rate) << " | - | - |\n";
    md << "| E6 | rolling-ball branches vs Fiji | separate run | r >= 0.99 | see validation/results/e6_fiji |\n\n";
    md << "Reference: type I error of the single default analysis = " << fmt(m.default_type1) << ". "
       << pct(m.evaluable_fraction) << " of experiments had a design the joint test can evaluate.\n\n";
    md << "**Decision point G2 (E2, E3, E4): " << (m.g2_pass() ? "PASS" : "FAIL") << "**\n\n";
    md << "E1 by artefact level (real effects, median orders of magnitude):";
    for (const auto& [k, v] : m.e1_median_by_level) md << " " << k << " " << fmt(v, 2) << ";";
    md << " moderate nulls (reference) " << fmt(m.e1_null_median, 2) << ".\n\n";
    md << "AUC by stratum:\n\n| Stratum | AUC |\n|---|---|\n";
    for (const auto& [k, v] : m.e2_auc_by_stratum) md << "| " << k << " | " << fmt(v) << " |\n";

    // Verdicts by design, separately for real effects and nulls.
    md << "\nVerdicts by design (gels x replicates per group per gel):\n\n"
          "| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |\n|---|---|---|---|---|---|---|\n";
    std::map<std::pair<std::string, std::string>, std::array<std::size_t, 5>> tally;
    for (const auto& r : report.records) {
      if (r.failed || r.set != "main") continue;
      auto& t = tally[{std::to_string(r.n_gels) + "x" + std::to_string(r.reps_per_gel),
                       r.true_lfc == 0.0 ? "null" : "real"}];
      ++t[0];
      ++t[1 + static_cast<std::size_t>(r.verdict)];
    }
    for (const auto& [key, t] : tally) {
      md << "| " << key.first << " | " << key.second << " | " << t[0];
      for (std::size_t v = 1; v < 5; ++v)
        md << " | " << pct(static_cast<double>(t[v]) / static_cast<double>(t[0]));
      md << " |\n";
    }
  }
}

HarnessReport read_harness_outputs(const std::filesystem::path& dir) {
  HarnessReport report;
  const YAML::Node meta = YAML::LoadFile((dir / "metrics.yaml").string());
  report.config.experiments = meta["experiments"].as<std::size_t>();
  report.config.gradient_experiments = meta["gradient_experiments"].as<std::size_t>();
  report.config.joint_permutations = meta["joint_permutations"].as<std::size_t>();
  report.config.seed = meta["seed"].as<std::uint64_t>();
  report.seconds = meta["seconds"].as<double>();
  if (meta["spec_sha256"]) report.spec_sha256 = meta["spec_sha256"].as<std::string>();
  report.simulator_presets = meta["simulator_presets"] ? meta["simulator_presets"].as<int>() : 1;

  std::ifstream in(dir / "records.csv");
  if (!in) throw std::runtime_error("cannot read " + (dir / "records.csv").string());
  std::string line;
  std::getline(in, line);  // header: columns are located by name
  std::map<std::string, std::size_t> col;
  {
    std::size_t start = 0, k = 0;
    while (true) {
      const std::size_t comma = line.find(',', start);
      col[line.substr(start, comma - start)] = k++;
      if (comma == std::string::npos) break;
      start = comma + 1;
    }
  }
  const std::size_t n_cols = col.size();
  auto num = [](const std::string& v) { return v == "nan" ? kNaN : std::stod(v); };
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    // Every column but the last (quoted error message) is comma-free.
    std::vector<std::string> f;
    std::size_t start = 0;
    for (std::size_t k = 0; k + 1 < n_cols; ++k) {
      const std::size_t comma = line.find(',', start);
      f.push_back(line.substr(start, comma - start));
      start = comma + 1;
    }
    f.push_back(line.substr(start));
    auto at = [&](const std::string& name) -> const std::string& { return f.at(col.at(name)); };
    ExperimentRecord r;
    r.set = at("set");
    r.id = std::stoul(at("id"));
    r.level = parse_artifact_level(at("level"));
    r.true_lfc = num(at("true_lfc"));
    r.n_gels = std::stoi(at("n_gels"));
    r.reps_per_gel = std::stoi(at("reps_per_gel"));
    r.failed = at("failed") == "1";
    r.robustness = num(at("R"));
    r.joint_p = num(at("joint_p"));
    for (Verdict v : {Verdict::Robust, Verdict::Fragile, Verdict::NotSupported, Verdict::NotEvaluable})
      if (at("verdict") == verdict_label(v)) r.verdict = v;
    r.default_lfc = num(at("default_lfc"));
    r.default_p = num(at("default_p"));
    r.median_lfc = num(at("median_lfc"));
    r.p_min = num(at("p_min"));
    r.p_max = num(at("p_max"));
    r.n_valid = std::stoul(at("n_valid"));
    for (Decision d : kDecisions)
      if (at("most_influential") == decision_key(d)) r.most_influential = d;
    for (std::size_t d = 0; d < kNumDecisions; ++d) {
      r.eta2[d] = num(at(std::string("eta2_") + decision_key(kDecisions[d])));
      const auto base = col.find(std::string("base_eta2_") + decision_key(kDecisions[d]));
      if (base != col.end()) r.baseline_eta2[d] = num(f.at(base->second));
    }
    r.paired = col.count("base_eta2_background") && !std::isnan(r.baseline_eta2[0]);
    r.error = at("error");
    if (r.error.size() >= 2 && r.error.front() == '"' && r.error.back() == '"')
      r.error = r.error.substr(1, r.error.size() - 2);
    report.records.push_back(std::move(r));
  }
  return report;
}

}  // namespace blotverse
