#include "blotverse/analysis/spec.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

namespace blotverse {
namespace {

struct MethodInfo {
  Method method;
  const char* key;
  Decision decision;
};

constexpr MethodInfo kMethods[] = {
    {Method::RollingBall, "rolling_ball", Decision::Background},
    {Method::LocalBackground, "local", Decision::Background},
    {Method::FixedRectangle, "fixed_rectangle", Decision::Band},
    {Method::ProfilePeak, "profile_peak", Decision::Band},
    {Method::ThresholdMask, "threshold_mask", Decision::Band},
    {Method::Housekeeping, "housekeeping", Decision::Loading},
    {Method::TotalProtein, "total_protein", Decision::Loading},
    {Method::FixedPoint, "fixed_point", Decision::Normalization},
    {Method::BlotSum, "blot_sum", Decision::Normalization},
    {Method::OptimalAlignment, "optimal_alignment", Decision::Normalization},
    {Method::LinearRatio, "linear", Decision::Scale},
    {Method::LogRatio, "log", Decision::Scale},
    {Method::Welch, "welch", Decision::Test},
    {Method::MixedModel, "mixed_model", Decision::Test},
    {Method::Permutation, "permutation", Decision::Test},
};

class SpecError : public std::runtime_error {
 public:
  explicit SpecError(const std::string& msg) : std::runtime_error("multiverse spec: " + msg) {}
};

Method parse_method(const std::string& key, Decision expected) {
  for (const auto& m : kMethods)
    if (key == m.key) {
      if (m.decision != expected)
        throw SpecError(std::string("method '") + key + "' does not belong to decision '" +
                        decision_key(expected) + "'");
      return m.method;
    }
  throw SpecError("unknown method '" + key + "'");
}

ExclusionCondition parse_condition(const std::string& key) {
  if (key == "overlapping_neighbours" || key == "overlapping_neighbors")
    return ExclusionCondition::OverlappingNeighbours;
  if (key == "total_protein_missing") return ExclusionCondition::TotalProteinMissing;
  if (key == "permutation_underpowered") return ExclusionCondition::PermutationUnderpowered;
  if (key == "single_lane_per_group") return ExclusionCondition::SingleLanePerGroup;
  throw SpecError("unknown exclusion condition '" + key + "'");
}

template <typename T>
T get_or(const YAML::Node& node, const char* key, T fallback) {
  return node[key] ? node[key].as<T>() : fallback;
}

}  // namespace

const char* decision_key(Decision d) {
  switch (d) {
    case Decision::Background: return "background";
    case Decision::Band: return "band";
    case Decision::Loading: return "loading";
    case Decision::Normalization: return "normalization";
    case Decision::Scale: return "scale";
    case Decision::Test: return "test";
  }
  return "?";
}

const char* decision_label(Decision d) {
  switch (d) {
    case Decision::Background: return "Background subtraction";
    case Decision::Band: return "Band definition";
    case Decision::Loading: return "Loading control";
    case Decision::Normalization: return "Replicate normalisation";
    case Decision::Scale: return "Effect scale";
    case Decision::Test: return "Statistical test";
  }
  return "?";
}

const char* method_key(Method m) {
  for (const auto& info : kMethods)
    if (info.method == m) return info.key;
  return "?";
}

Decision method_decision(Method m) {
  for (const auto& info : kMethods)
    if (info.method == m) return info.decision;
  throw std::logic_error("method_decision: unknown method");
}

std::size_t MultiverseSpec::full_size() const {
  std::size_t n = 1;
  for (const auto& d : decisions) n *= d.branches.size();
  return n;
}

std::vector<Universe> MultiverseSpec::enumerate() const {
  std::vector<Universe> out;
  out.reserve(full_size());
  Universe u{};
  while (true) {
    out.push_back(u);
    // Odometer increment, last decision fastest.
    std::size_t d = kNumDecisions;
    while (d > 0) {
      --d;
      if (++u[d] < decisions[d].branches.size()) break;
      u[d] = 0;
      if (d == 0) return out;
    }
  }
}

std::string MultiverseSpec::describe(const Universe& u) const {
  std::string s;
  for (std::size_t d = 0; d < kNumDecisions; ++d) {
    if (d) s += '/';
    s += decisions[d].branches.at(u[d]).id;
  }
  return s;
}

MultiverseSpec parse_spec(const std::string& yaml_text) {
  YAML::Node root;
  try {
    root = YAML::Load(yaml_text);
  } catch (const YAML::Exception& e) {
    throw SpecError(std::string("invalid YAML: ") + e.what());
  }
  MultiverseSpec spec;
  spec.source_text = yaml_text;
  spec.name = get_or<std::string>(root, "name", "unnamed");
  spec.version = get_or<std::string>(root, "version", "0");
  spec.alpha = get_or<double>(root, "alpha", 0.05);
  if (!(spec.alpha > 0 && spec.alpha < 1)) throw SpecError("alpha must be in (0, 1)");

  const YAML::Node decisions = root["decisions"];
  if (!decisions || !decisions.IsMap()) throw SpecError("'decisions' map is required");
  for (Decision d : kDecisions) {
    const YAML::Node list = decisions[decision_key(d)];
    if (!list || !list.IsSequence() || list.size() == 0)
      throw SpecError(std::string("decision '") + decision_key(d) + "' needs at least one branch");
    auto& ds = spec.decisions[static_cast<std::size_t>(d)];
    ds.decision = d;
    std::set<std::string> ids;
    for (const auto& node : list) {
      Branch b;
      b.id = node["id"].as<std::string>();
      if (!ids.insert(b.id).second) throw SpecError("duplicate branch id '" + b.id + "'");
      b.method = parse_method(node["method"].as<std::string>(), d);
      b.radius_factor = get_or<double>(node, "radius_factor", 0.0);
      if (b.method == Method::RollingBall && b.radius_factor <= 0)
        throw SpecError("rolling_ball branch '" + b.id + "' needs radius_factor > 0");
      if (node["references"])
        for (const auto& r : node["references"]) b.references.push_back(r.as<std::string>());
      if (b.references.empty())
        throw SpecError("branch '" + b.id + "' has no literature reference");
      b.rationale = get_or<std::string>(node, "rationale", "");
      ds.branches.push_back(std::move(b));
    }
    if (ds.branches.size() > 255) throw SpecError("too many branches");
  }

  // Branch references must resolve when a bibliography is provided.
  if (const YAML::Node refs = root["references"]) {
    for (const auto& ds : spec.decisions)
      for (const auto& b : ds.branches)
        for (const auto& key : b.references)
          if (!refs[key]) throw SpecError("branch '" + b.id + "' cites undefined reference '" + key + "'");
  }

  const YAML::Node def = root["default_universe"];
  if (!def) throw SpecError("'default_universe' is required (fixed before seeing results)");
  for (Decision d : kDecisions) {
    const auto& ds = spec.decision(d);
    const std::string id = def[decision_key(d)].as<std::string>();
    bool found = false;
    for (std::size_t i = 0; i < ds.branches.size(); ++i)
      if (ds.branches[i].id == id) {
        spec.default_universe[static_cast<std::size_t>(d)] = static_cast<std::uint8_t>(i);
        found = true;
      }
    if (!found) throw SpecError("default branch '" + id + "' not found");
  }

  if (const YAML::Node ex = root["exclusions"]) {
    for (const auto& node : ex) {
      ExclusionRule r;
      r.id = node["id"].as<std::string>();
      r.condition = parse_condition(node["condition"].as<std::string>());
      r.reason = get_or<std::string>(node, "reason", r.id);
      const YAML::Node when = node["when"];
      if (!when || !when.IsMap() || when.size() != 1)
        throw SpecError("exclusion '" + r.id + "' needs exactly one 'when: {decision: method}'");
      const auto it = when.begin();
      const std::string dkey = it->first.as<std::string>();
      bool matched = false;
      for (Decision d : kDecisions)
        if (dkey == decision_key(d)) {
          r.decision = d;
          matched = true;
        }
      if (!matched) throw SpecError("exclusion '" + r.id + "': unknown decision '" + dkey + "'");
      r.branch_id = it->second.as<std::string>();
      spec.exclusions.push_back(std::move(r));
    }
  }

  if (const YAML::Node qc = root["qc"]) {
    spec.qc.saturation_min_pixels = get_or<std::size_t>(qc, "saturation_min_pixels", 1);
    spec.qc.linear_range_min_slope = get_or<double>(qc, "linear_range_min_slope", 0.8);
    spec.qc.linear_range_max_slope = get_or<double>(qc, "linear_range_max_slope", 1.25);
  }
  if (const YAML::Node v = root["verdict"]) {
    spec.verdict.robust_min_r = get_or<double>(v, "robust_min_r", 0.9);
    spec.verdict.fragile_min_r = get_or<double>(v, "fragile_min_r", 0.5);
    spec.verdict.joint_alpha = get_or<double>(v, "joint_alpha", 0.05);
  }
  if (const YAML::Node inf = root["inference"]) {
    spec.inference.joint_permutations = get_or<std::size_t>(inf, "joint_permutations", 1000);
    spec.inference.test_max_permutations = get_or<std::size_t>(inf, "test_max_permutations", 10000);
    spec.inference.seed = get_or<std::uint64_t>(inf, "seed", 20261024);
  }
  return spec;
}

MultiverseSpec load_spec(const std::filesystem::path& path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("cannot open spec: " + path.string());
  std::stringstream ss;
  ss << in.rdbuf();
  return parse_spec(ss.str());
}

std::optional<std::string> exclusion_reason(const MultiverseSpec& spec, const Universe& u,
                                            const ExperimentFacts& facts) {
  for (const auto& rule : spec.exclusions) {
    const Branch& b = spec.branch(rule.decision, u[static_cast<std::size_t>(rule.decision)]);
    if (b.id != rule.branch_id) continue;
    bool applies = false;
    switch (rule.condition) {
      case ExclusionCondition::OverlappingNeighbours: applies = !facts.local_background_valid; break;
      case ExclusionCondition::TotalProteinMissing: applies = !facts.has_total_protein; break;
      case ExclusionCondition::PermutationUnderpowered: applies = !facts.permutation_can_reach_alpha; break;
      case ExclusionCondition::SingleLanePerGroup: applies = !facts.every_blot_has_two_lanes_per_group; break;
    }
    if (applies) return rule.reason;
  }
  return std::nullopt;
}

}  // namespace blotverse
