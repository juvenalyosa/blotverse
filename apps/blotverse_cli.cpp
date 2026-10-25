// BlotVerse command line interface.
//
//   blotverse simulate  --out DIR [--level clean|moderate|severe] [--lfc X] ...
//   blotverse detect    EXPERIMENT.yaml --out ANNOTATED.yaml
//   blotverse run       EXPERIMENT.yaml --out DIR [--spec FILE] [--permutations N]
//   blotverse reproduce DIR/repro.yaml [--out DIR]
//   blotverse validate  --out DIR [--experiments N] [--permutations N] ...
//   blotverse spec      [--spec FILE]

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/experiment.hpp"
#include "blotverse/imaging/detect.hpp"
#include "blotverse/imaging/image_stats.hpp"
#include "blotverse/io/image_file.hpp"
#include "blotverse/io/sha256.hpp"
#include "blotverse/pipeline.hpp"
#include "blotverse/report/report.hpp"
#include "blotverse/sim/simulator.hpp"
#include "blotverse/validation/dilution.hpp"
#include "blotverse/validation/harness.hpp"
#include "blotverse/version.hpp"

namespace fs = std::filesystem;
using namespace blotverse;

namespace {

constexpr const char* kUsage = R"(BlotVerse - does a western blot conclusion survive every defensible quantification?

Usage:
  blotverse simulate  --out DIR [--level clean|moderate|severe] [--lfc X] [--gels N]
                      [--reps N] [--seed N] [--no-total-protein]
  blotverse detect    EXPERIMENT.yaml --out FILE    write detected lanes/bands for manual correction
  blotverse run       EXPERIMENT.yaml --out DIR [--spec FILE] [--permutations N] [--threads N]
  blotverse reproduce REPRO.yaml [--out DIR] [--threads N]
  blotverse validate  --out DIR [--spec FILE] [--experiments N] [--gradient-experiments N]
                      [--permutations N] [--seed N] [--threads N]
  blotverse validate  --recompute DIR               recompute metrics from a finished run
  blotverse spec      [--spec FILE]                 summarise the multiverse specification
  blotverse lanes     IMAGE --lanes N --target-y Y --reference-y Y [--lanes-x FIRST,LAST]
                                                    automatic lane/band detection on one image (TSV)
  blotverse dilution  CONFIG.yaml --out DIR [--spec FILE]
                                                    known-amount dilution series: linearity per branch
  blotverse imagestats FILE... | --simulate LEVEL [--count N]
                                                    image statistics for the realism check
  blotverse --version
)";

struct Args {
  std::vector<std::string> positional;
  std::map<std::string, std::string> options;
  std::set<std::string> flags;

  bool has(const std::string& k) const { return options.count(k) || flags.count(k); }
  std::string get(const std::string& k, const std::string& fallback = "") const {
    const auto it = options.find(k);
    return it == options.end() ? fallback : it->second;
  }
  std::string require(const std::string& k) const {
    if (!options.count(k)) throw std::runtime_error("missing required option --" + k);
    return options.at(k);
  }
  long long integer(const std::string& k, long long fallback) const {
    return options.count(k) ? std::stoll(options.at(k)) : fallback;
  }
  double number(const std::string& k, double fallback) const {
    return options.count(k) ? std::stod(options.at(k)) : fallback;
  }
};

Args parse_args(int argc, char** argv, int first, const std::set<std::string>& boolean_flags) {
  Args a;
  for (int i = first; i < argc; ++i) {
    std::string s = argv[i];
    if (s.rfind("--", 0) == 0) {
      const std::string key = s.substr(2);
      if (boolean_flags.count(key)) {
        a.flags.insert(key);
      } else {
        if (i + 1 >= argc) throw std::runtime_error("option --" + key + " needs a value");
        a.options[key] = argv[++i];
      }
    } else {
      a.positional.push_back(s);
    }
  }
  return a;
}

fs::path default_spec_path() {
  if (const char* env = std::getenv("BLOTVERSE_SPEC")) return env;
#ifdef BLOTVERSE_SOURCE_DIR
  const fs::path in_tree = fs::path(BLOTVERSE_SOURCE_DIR) / "spec" / "multiverse.yaml";
  if (fs::exists(in_tree)) return in_tree;
#endif
  return "spec/multiverse.yaml";
}

MultiverseSpec spec_from(const Args& a) {
  return load_spec(a.has("spec") ? fs::path(a.get("spec")) : default_spec_path());
}

void print_summary(const AnalysisResult& result) {
  std::cout << std::fixed << std::setprecision(3);
  for (const auto& s : result.summaries) {
    std::cout << "\n  " << s.contrast.label << ": " << verdict_label(s.verdict) << "\n"
              << "    R = " << s.robustness << "   joint p = " << s.joint.p << "\n"
              << "    default lfc = " << s.default_lfc << " (p = " << std::setprecision(4) << s.default_p
              << std::setprecision(3) << ")   median lfc = " << s.median_lfc << " [" << s.lfc_q05 << ", "
              << s.lfc_q95 << "]\n"
              << "    most influential decision: "
              << (s.most_influential ? decision_label(*s.most_influential) : "-") << "\n";
  }
  if (!result.warnings.empty()) {
    std::cout << "\n  Quality control:\n";
    for (const auto& w : result.warnings) std::cout << "    - " << w << "\n";
  }
}

void write_outputs(const fs::path& out, const MultiverseSpec& spec, const Experiment& ex,
                   const fs::path& experiment_dir, const AnalysisResult& result) {
  fs::create_directories(out);
  std::ofstream(out / "report.html") << render_html_report(spec, ex, result);
  write_universes_csv(out / "universes.csv", spec, result);
  write_summary_csv(out / "summary.csv", result);
  write_lanes_csv(out / "lanes.csv", spec, ex, result);
  write_repro_file(out / "repro.yaml", spec, ex, experiment_dir, result);
}

int cmd_simulate(const Args& a) {
  const fs::path out = a.require("out");
  SimParams p = sim_preset(parse_artifact_level(a.get("level", "moderate")));
  p.true_lfc = {0.0, a.number("lfc", 1.0)};
  p.n_gels = static_cast<int>(a.integer("gels", 3));
  p.reps_per_gel = static_cast<int>(a.integer("reps", 2));
  p.seed = static_cast<std::uint64_t>(a.integer("seed", 1));
  p.total_protein = !a.has("no-total-protein");
  write_simulation(out, simulate(p));
  std::cout << "Simulated " << p.n_gels << " blot(s), " << p.reps_per_gel * p.n_gels
            << " replicates per group, true lfc " << p.true_lfc[1] << " -> " << out / "experiment.yaml" << "\n";
  return 0;
}

int cmd_detect(const Args& a) {
  if (a.positional.size() != 1) throw std::runtime_error("detect needs EXPERIMENT.yaml");
  const fs::path in = a.positional[0];
  Experiment ex = load_experiment(in);
  for (auto& b : ex.blots) {
    if (!b.annotation)
      b.annotation = detect_annotation(to_float(b.image.pixels), b.lanes.size(), b.hints);
  }
  fs::path out = a.require("out");
  // Image paths stay valid when the annotated file is written next to the original.
  if (fs::absolute(out).parent_path() != fs::absolute(in).parent_path())
    std::cerr << "warning: image paths are relative; keep " << out << " next to the images\n";
  save_experiment(out, ex);
  std::cout << "Wrote detected lanes and bands to " << out
            << "\nEdit the 'annotation' blocks to correct them, then run: blotverse run " << out << "\n";
  return 0;
}

int cmd_run(const Args& a) {
  if (a.positional.size() != 1) throw std::runtime_error("run needs EXPERIMENT.yaml");
  const fs::path in = a.positional[0];
  const MultiverseSpec spec = spec_from(a);
  const Experiment ex = load_experiment(in);
  AnalysisOptions opts;
  opts.threads = static_cast<unsigned>(a.integer("threads", 0));
  if (a.has("permutations")) opts.joint_permutations = static_cast<std::size_t>(a.integer("permutations", 0));
  std::cout << "BlotVerse " << kVersion << ": " << ex.name << " (" << ex.blots.size() << " blots)\n";
  const AnalysisResult result = analyze(ex, spec, opts);
  const fs::path out = a.require("out");
  write_outputs(out, spec, ex, in.parent_path(), result);
  std::cout << "  " << result.multiverse.plan.universes.size() << " universes ("
            << result.multiverse.plan.excluded.size() << " excluded), " << result.joint_permutations
            << " joint permutations, " << std::setprecision(1) << std::fixed << result.seconds << " s";
  print_summary(result);
  std::cout << "\n  Report: " << out / "report.html" << "\n";
  return 0;
}

int cmd_reproduce(const Args& a) {
  if (a.positional.size() != 1) throw std::runtime_error("reproduce needs REPRO.yaml");
  const ReproRecord record = read_repro_file(a.positional[0]);
  const MultiverseSpec spec = parse_spec(record.spec_text);
  const Experiment ex = experiment_from_repro(record);
  AnalysisOptions opts;
  opts.threads = static_cast<unsigned>(a.integer("threads", 0));
  opts.joint_permutations = record.joint_permutations;
  const AnalysisResult result = analyze(ex, spec, opts);
  if (a.has("out")) write_outputs(a.get("out"), spec, ex, record.experiment_dir, result);
  const auto diffs = compare_with_repro(record, result);
  if (diffs.empty()) {
    std::cout << "Reproduced: image fingerprints and every headline number are identical.\n";
    return 0;
  }
  std::cout << "NOT reproduced:\n";
  for (const auto& d : diffs) std::cout << "  - " << d << "\n";
  return 2;
}

int cmd_validate(const Args& a) {
  if (a.has("recompute")) {
    // Recompute metrics and summaries of a finished run from its records.
    const fs::path dir = a.get("recompute");
    HarnessReport report = read_harness_outputs(dir);
    report.metrics = compute_metrics(report.records, spec_from(a).alpha, report.config.seed);
    write_harness_outputs(dir, report);
    std::ifstream summary(dir / "summary.md");
    std::cout << summary.rdbuf();
    return report.metrics.g2_pass() ? 0 : 3;
  }
  const MultiverseSpec spec = spec_from(a);
  HarnessConfig cfg;
  cfg.experiments = static_cast<std::size_t>(a.integer("experiments", 2000));
  cfg.gradient_experiments = static_cast<std::size_t>(a.integer("gradient-experiments", 200));
  cfg.joint_permutations = static_cast<std::size_t>(a.integer("permutations", 200));
  cfg.seed = static_cast<std::uint64_t>(a.integer("seed", 1));
  cfg.threads = static_cast<unsigned>(a.integer("threads", 0));
  const fs::path out = a.require("out");
  std::cout << "Level-1 validation: " << cfg.experiments << " + " << cfg.gradient_experiments
            << " simulated experiments, " << cfg.joint_permutations << " joint permutations each\n";
  const auto report = run_harness(spec, cfg, [](std::size_t done, std::size_t total) {
    if (done % 50 == 0 || done == total) std::cerr << "\r  " << done << "/" << total << std::flush;
  });
  std::cerr << "\n";
  write_harness_outputs(out, report);
  std::ifstream summary(out / "summary.md");
  std::cout << summary.rdbuf();
  return report.metrics.g2_pass() ? 0 : 3;
}

int cmd_imagestats(const Args& a) {
  std::cout << "image\twidth\theight\tgrey_levels\tbackground\tnoise\tpeak_snr\tforeground_frac"
               "\tbackground_variation\tsaturated_frac\n";
  auto print = [](const std::string& name, const RawImage& img) {
    const ImageStats s = compute_image_stats(img);
    std::cout << name << '\t' << s.width << '\t' << s.height << '\t' << s.grey_levels << '\t'
              << std::setprecision(6) << s.background << '\t' << s.noise << '\t' << s.peak_snr << '\t'
              << s.foreground_fraction << '\t' << s.background_variation << '\t' << s.saturated_fraction << '\n';
  };
  if (a.has("simulate")) {
    const ArtifactLevel level = parse_artifact_level(a.get("simulate"));
    const auto count = a.integer("count", 10);
    for (long long i = 0; i < count; ++i) {
      SimParams p = sim_preset(level);
      p.n_gels = 1;
      p.reps_per_gel = 3;
      p.seed = static_cast<std::uint64_t>(1000 + i);
      print(std::string("sim_") + artifact_label(level) + "_" + std::to_string(i),
            make_raw_image(simulate(p).blots[0].image, "sim"));
    }
    return 0;
  }
  for (const auto& f : a.positional) {
    try {
      print(fs::path(f).filename().string(), read_image(f));
    } catch (const std::exception& e) {
      std::cerr << f << ": " << e.what() << "\n";
    }
  }
  return 0;
}

int cmd_lanes(const Args& a) {
  if (a.positional.size() != 1) throw std::runtime_error("lanes needs one IMAGE");
  const RawImage raw = read_image(a.positional[0]);
  BandHints hints;
  hints.target_y = std::stod(a.require("target-y"));
  hints.reference_y = std::stod(a.require("reference-y"));
  if (a.has("lanes-x")) {
    const std::string v = a.get("lanes-x");
    const auto comma = v.find(',');
    if (comma == std::string::npos) throw std::runtime_error("--lanes-x expects FIRST,LAST");
    hints.first_lane_x = std::stod(v.substr(0, comma));
    hints.last_lane_x = std::stod(v.substr(comma + 1));
  }
  const Annotation ann =
      detect_annotation(to_float(raw.pixels), static_cast<std::size_t>(std::stoul(a.require("lanes"))), hints);
  std::cout << "lane\tx_center\twidth\ttarget_y\treference_y\n" << std::fixed << std::setprecision(1);
  for (std::size_t i = 0; i < ann.lane_count(); ++i)
    std::cout << i + 1 << '\t' << ann.lanes[i].x_center << '\t' << ann.lanes[i].width << '\t' << ann.target_y[i]
              << '\t' << ann.reference_y[i] << '\n';
  return 0;
}

int cmd_dilution(const Args& a) {
  if (a.positional.size() != 1) throw std::runtime_error("dilution needs CONFIG.yaml");
  const DilutionConfig config = load_dilution_config(a.positional[0]);
  const auto fits = analyze_dilution(config, spec_from(a));
  const fs::path out = a.require("out");
  write_dilution_outputs(out, config, fits);
  std::ifstream md(out / "dilution.md");
  std::cout << md.rdbuf();
  return 0;
}

int cmd_spec(const Args& a) {
  const MultiverseSpec spec = spec_from(a);
  std::cout << spec.name << " v" << spec.version << "  (sha256 " << sha256_hex(spec.source_text).substr(0, 16)
            << "...)\n";
  for (Decision d : kDecisions) {
    std::cout << "  " << std::left << std::setw(26) << decision_label(d);
    for (const auto& b : spec.decision(d).branches) std::cout << b.id << ' ';
    std::cout << "\n";
  }
  ExperimentFacts with_tp{true, true}, without_tp{false, true};
  std::cout << "  universes: " << plan_universes(spec, with_tp).universes.size()
            << " with total protein, " << plan_universes(spec, without_tp).universes.size() << " without\n"
            << "  default:   " << spec.describe(spec.default_universe) << "\n";
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << kUsage;
    return 1;
  }
  const std::string cmd = argv[1];
  try {
    if (cmd == "--version" || cmd == "version") {
      std::cout << "blotverse " << kVersion << "\n";
      return 0;
    }
    if (cmd == "-h" || cmd == "--help" || cmd == "help") {
      std::cout << kUsage;
      return 0;
    }
    const Args args = parse_args(argc, argv, 2, {"no-total-protein"});
    if (cmd == "simulate") return cmd_simulate(args);
    if (cmd == "detect") return cmd_detect(args);
    if (cmd == "run") return cmd_run(args);
    if (cmd == "reproduce") return cmd_reproduce(args);
    if (cmd == "validate") return cmd_validate(args);
    if (cmd == "spec") return cmd_spec(args);
    if (cmd == "imagestats") return cmd_imagestats(args);
    if (cmd == "lanes") return cmd_lanes(args);
    if (cmd == "dilution") return cmd_dilution(args);
    std::cerr << "unknown command '" << cmd << "'\n\n" << kUsage;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
}
