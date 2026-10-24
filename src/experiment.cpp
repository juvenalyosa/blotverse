#include "blotverse/experiment.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>

namespace blotverse {
namespace {

class ExperimentError : public std::runtime_error {
 public:
  explicit ExperimentError(const std::string& msg) : std::runtime_error("experiment: " + msg) {}
};

template <typename T>
std::vector<T> as_vector(const YAML::Node& node, const char* what) {
  if (!node || !node.IsSequence()) throw ExperimentError(std::string("'") + what + "' must be a list");
  std::vector<T> out;
  for (const auto& v : node) out.push_back(v.as<T>());
  return out;
}

}  // namespace

std::vector<std::string> Experiment::groups() const {
  std::vector<std::string> out{control_group};
  for (const auto& b : blots)
    for (const auto& l : b.lanes)
      if (std::find(out.begin(), out.end(), l.group) == out.end()) out.push_back(l.group);
  return out;
}

bool Experiment::has_total_protein() const {
  return !blots.empty() &&
         std::all_of(blots.begin(), blots.end(), [](const Blot& b) { return b.total_protein.has_value(); });
}

YAML::Node annotation_to_yaml(const Annotation& ann) {
  YAML::Node n;
  for (const auto& l : ann.lanes) {
    YAML::Node lane;
    lane["x"] = l.x_center;
    lane["width"] = l.width;
    n["lanes"].push_back(lane);
  }
  for (double y : ann.target_y) n["target_y"].push_back(y);
  for (double y : ann.reference_y) n["reference_y"].push_back(y);
  n["band_half_height"] = ann.band_half_height;
  n["source"] = ann.source;
  return n;
}

Annotation annotation_from_yaml(const YAML::Node& node) {
  Annotation ann;
  if (!node["lanes"] || !node["lanes"].IsSequence()) throw ExperimentError("annotation needs 'lanes'");
  for (const auto& l : node["lanes"]) ann.lanes.push_back({l["x"].as<double>(), l["width"].as<double>()});
  ann.target_y = as_vector<double>(node["target_y"], "target_y");
  ann.reference_y = as_vector<double>(node["reference_y"], "reference_y");
  ann.band_half_height = node["band_half_height"].as<int>();
  ann.source = node["source"] ? node["source"].as<std::string>() : "manual";
  return ann;
}

Experiment parse_experiment(const std::string& yaml_text, const std::filesystem::path& base_dir) {
  YAML::Node root;
  try {
    root = YAML::Load(yaml_text);
  } catch (const YAML::Exception& e) {
    throw ExperimentError(std::string("invalid YAML: ") + e.what());
  }
  Experiment ex;
  ex.source_text = yaml_text;
  ex.name = root["name"] ? root["name"].as<std::string>() : "experiment";
  if (!root["control_group"]) throw ExperimentError("'control_group' is required");
  ex.control_group = root["control_group"].as<std::string>();
  if (!root["blots"] || !root["blots"].IsSequence() || root["blots"].size() == 0)
    throw ExperimentError("'blots' must list at least one blot");

  std::set<std::string> ids;
  for (const auto& node : root["blots"]) {
    Blot b;
    b.id = node["id"].as<std::string>();
    if (!ids.insert(b.id).second) throw ExperimentError("duplicate blot id '" + b.id + "'");
    b.image_file = node["image"].as<std::string>();
    b.image = read_tiff(base_dir / b.image_file);
    b.invert = node["invert"] && node["invert"].as<bool>();
    if (b.invert) invert_image(b.image);
    if (node["total_protein"]) {
      b.total_protein_file = node["total_protein"].as<std::string>();
      b.total_protein = read_tiff(base_dir / b.total_protein_file);
      if (b.invert) invert_image(*b.total_protein);
    }
    for (const auto& l : node["lanes"]) {
      LaneSpec lane;
      lane.group = l["group"].as<std::string>();
      lane.sample = l["sample"] ? l["sample"].as<std::string>() : "";
      if (l["load"]) lane.load = l["load"].as<double>();
      b.lanes.push_back(std::move(lane));
    }
    if (b.lanes.empty()) throw ExperimentError("blot '" + b.id + "' has no lanes");
    if (const auto h = node["bands"]) {
      if (h["target_y"]) b.hints.target_y = h["target_y"].as<double>();
      if (h["reference_y"]) b.hints.reference_y = h["reference_y"].as<double>();
      if (h["target_above_reference"]) b.hints.target_above_reference = h["target_above_reference"].as<bool>();
    }
    if (node["annotation"]) {
      b.annotation = annotation_from_yaml(node["annotation"]);
      if (b.annotation->lane_count() != b.lanes.size())
        throw ExperimentError("blot '" + b.id + "': annotation lane count differs from 'lanes'");
      b.annotation->validate(b.image.pixels.width(), b.image.pixels.height());
    }
    ex.blots.push_back(std::move(b));
  }
  const auto groups = ex.groups();
  if (groups.size() < 2) throw ExperimentError("at least two groups are required");
  for (const auto& b : ex.blots) {
    const bool has_control = std::any_of(b.lanes.begin(), b.lanes.end(),
                                         [&](const LaneSpec& l) { return l.group == ex.control_group; });
    if (!has_control) throw ExperimentError("blot '" + b.id + "' has no '" + ex.control_group + "' lane");
  }
  return ex;
}

Experiment load_experiment(const std::filesystem::path& path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("cannot open experiment file: " + path.string());
  std::stringstream ss;
  ss << in.rdbuf();
  return parse_experiment(ss.str(), path.parent_path());
}

void save_experiment(const std::filesystem::path& path, const Experiment& experiment) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "name" << YAML::Value << experiment.name;
  out << YAML::Key << "control_group" << YAML::Value << experiment.control_group;
  out << YAML::Key << "blots" << YAML::Value << YAML::BeginSeq;
  for (const auto& b : experiment.blots) {
    out << YAML::BeginMap;
    out << YAML::Key << "id" << YAML::Value << b.id;
    out << YAML::Key << "image" << YAML::Value << b.image_file;
    if (b.invert) out << YAML::Key << "invert" << YAML::Value << true;
    if (!b.total_protein_file.empty())
      out << YAML::Key << "total_protein" << YAML::Value << b.total_protein_file;
    if (b.hints.target_y || b.hints.reference_y) {
      out << YAML::Key << "bands" << YAML::Value << YAML::BeginMap;
      if (b.hints.target_y) out << YAML::Key << "target_y" << YAML::Value << *b.hints.target_y;
      if (b.hints.reference_y) out << YAML::Key << "reference_y" << YAML::Value << *b.hints.reference_y;
      out << YAML::Key << "target_above_reference" << YAML::Value << b.hints.target_above_reference;
      out << YAML::EndMap;
    }
    out << YAML::Key << "lanes" << YAML::Value << YAML::BeginSeq;
    for (const auto& l : b.lanes) {
      out << YAML::Flow << YAML::BeginMap;
      out << YAML::Key << "group" << YAML::Value << l.group;
      out << YAML::Key << "sample" << YAML::Value << l.sample;
      if (l.load) out << YAML::Key << "load" << YAML::Value << *l.load;
      out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    if (b.annotation) out << YAML::Key << "annotation" << YAML::Value << annotation_to_yaml(*b.annotation);
    out << YAML::EndMap;
  }
  out << YAML::EndSeq << YAML::EndMap;
  std::ofstream file(path);
  if (!file) throw std::runtime_error("cannot write " + path.string());
  file << out.c_str() << "\n";
}

}  // namespace blotverse
