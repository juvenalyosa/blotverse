#include <yaml-cpp/yaml.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "blotverse/io/sha256.hpp"
#include "blotverse/report/report.hpp"
#include "blotverse/version.hpp"

namespace blotverse {
namespace {

// Doubles are written with round-trip precision so reruns compare exactly.
std::string exact(double v) {
  if (std::isnan(v)) return ".nan";
  std::ostringstream ss;
  ss.precision(17);
  ss << v;
  return ss.str();
}

double parse_exact(const YAML::Node& n) {
  const auto s = n.as<std::string>();
  return s == ".nan" ? std::nan("") : std::stod(s);
}

bool same(double a, double b) { return (std::isnan(a) && std::isnan(b)) || a == b; }

}  // namespace

void write_repro_file(const std::filesystem::path& path, const MultiverseSpec& spec,
                      const Experiment& experiment, const std::filesystem::path& experiment_dir,
                      const AnalysisResult& result) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "blotverse_version" << YAML::Value << kVersion;
  out << YAML::Key << "spec_sha256" << YAML::Value << sha256_hex(spec.source_text);
  out << YAML::Key << "seeds" << YAML::Value << YAML::BeginMap;
  out << YAML::Key << "base" << YAML::Value << spec.inference.seed;
  out << YAML::Key << "joint_permutations" << YAML::Value << result.joint_permutations;
  out << YAML::Key << "test_max_permutations" << YAML::Value << spec.inference.test_max_permutations;
  out << YAML::EndMap;
  out << YAML::Key << "experiment_dir" << YAML::Value
      << std::filesystem::absolute(experiment_dir).lexically_normal().string();

  out << YAML::Key << "images" << YAML::Value << YAML::BeginMap;
  for (const auto& b : experiment.blots) {
    out << YAML::Key << b.image_file << YAML::Value << b.image.meta.sha256;
    if (b.total_protein) out << YAML::Key << b.total_protein_file << YAML::Value << b.total_protein->meta.sha256;
  }
  out << YAML::EndMap;

  out << YAML::Key << "annotations" << YAML::Value << YAML::BeginMap;
  for (std::size_t b = 0; b < experiment.blots.size(); ++b)
    out << YAML::Key << experiment.blots[b].id << YAML::Value << annotation_to_yaml(result.blots[b].annotation);
  out << YAML::EndMap;

  out << YAML::Key << "results" << YAML::Value << YAML::BeginSeq;
  for (const auto& s : result.summaries) {
    out << YAML::BeginMap;
    out << YAML::Key << "contrast" << YAML::Value << s.contrast.label;
    out << YAML::Key << "verdict" << YAML::Value << verdict_label(s.verdict);
    out << YAML::Key << "robustness" << YAML::Value << exact(s.robustness);
    out << YAML::Key << "joint_p" << YAML::Value << exact(s.joint.p);
    out << YAML::Key << "default_lfc" << YAML::Value << exact(s.default_lfc);
    out << YAML::Key << "median_lfc" << YAML::Value << exact(s.median_lfc);
    out << YAML::EndMap;
  }
  out << YAML::EndSeq;

  out << YAML::Key << "spec" << YAML::Value << YAML::Literal << spec.source_text;
  out << YAML::Key << "experiment" << YAML::Value << YAML::Literal << experiment.source_text;
  out << YAML::EndMap;

  std::ofstream file(path);
  if (!file) throw std::runtime_error("cannot write " + path.string());
  file << out.c_str() << "\n";
}

ReproRecord read_repro_file(const std::filesystem::path& path) {
  const YAML::Node root = YAML::LoadFile(path.string());
  ReproRecord r;
  r.version = root["blotverse_version"].as<std::string>();
  r.spec_text = root["spec"].as<std::string>();
  r.experiment_text = root["experiment"].as<std::string>();
  r.experiment_dir = root["experiment_dir"].as<std::string>();
  r.joint_permutations = root["seeds"]["joint_permutations"].as<std::size_t>();
  for (const auto& kv : root["images"]) r.image_sha256[kv.first.as<std::string>()] = kv.second.as<std::string>();
  for (const auto& kv : root["annotations"])
    r.annotations[kv.first.as<std::string>()] = annotation_from_yaml(kv.second);
  for (const auto& h : root["results"])
    r.headlines.push_back({h["contrast"].as<std::string>(), parse_exact(h["robustness"]),
                           parse_exact(h["joint_p"]), parse_exact(h["default_lfc"]),
                           parse_exact(h["median_lfc"])});
  return r;
}

Experiment experiment_from_repro(const ReproRecord& record) {
  if (record.experiment_text.empty())
    throw std::runtime_error("reproducibility file has no embedded experiment (simulated in memory?)");
  Experiment ex = parse_experiment(record.experiment_text, record.experiment_dir);
  for (auto& b : ex.blots) {
    auto check = [&](const std::string& file, const RawImage& img) {
      const auto it = record.image_sha256.find(file);
      if (it == record.image_sha256.end()) throw std::runtime_error("image not in repro file: " + file);
      if (it->second != img.meta.sha256)
        throw std::runtime_error("image changed since the analysis (fingerprint mismatch): " + file);
    };
    check(b.image_file, b.image);
    if (b.total_protein) check(b.total_protein_file, *b.total_protein);
    if (const auto it = record.annotations.find(b.id); it != record.annotations.end()) b.annotation = it->second;
  }
  return ex;
}

std::vector<std::string> compare_with_repro(const ReproRecord& record, const AnalysisResult& result) {
  std::vector<std::string> diffs;
  if (record.version != kVersion)
    diffs.push_back("tool version differs: " + record.version + " vs " + kVersion);
  if (record.headlines.size() != result.summaries.size()) {
    diffs.push_back("number of contrasts differs");
    return diffs;
  }
  for (std::size_t i = 0; i < record.headlines.size(); ++i) {
    const auto& h = record.headlines[i];
    const auto& s = result.summaries[i];
    auto cmp = [&](const char* what, double a, double b) {
      if (!same(a, b)) diffs.push_back(h.contrast + ": " + what + " " + exact(a) + " vs " + exact(b));
    };
    cmp("R", h.robustness, s.robustness);
    cmp("joint p", h.joint_p, s.joint.p);
    cmp("default lfc", h.default_lfc, s.default_lfc);
    cmp("median lfc", h.median_lfc, s.median_lfc);
  }
  return diffs;
}

}  // namespace blotverse
