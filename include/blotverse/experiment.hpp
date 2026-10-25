#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/imaging/detect.hpp"
#include "blotverse/io/tiff.hpp"

namespace YAML {
class Node;
}

namespace blotverse {

struct LaneSpec {
  std::string sample;
  std::string group;
  std::optional<double> load;  // relative amount loaded (dilution series only)
};

// One membrane: the target/reference image, an optional registered
// total-protein image and the lane layout (left to right).
struct Blot {
  std::string id;
  RawImage image;
  std::optional<RawImage> total_protein;
  // Loading control imaged separately (stripped and re-probed membrane, cut
  // strip, other exposure): its own image and lane/band annotation, whose
  // target rows are the reference band. Without it, the reference band is
  // read from `image`.
  std::optional<RawImage> reference_image;
  std::optional<Annotation> reference_annotation;
  std::vector<LaneSpec> lanes;
  BandHints hints;
  std::optional<Annotation> annotation;  // manual correction overrides detection
  bool invert = false;                   // dark bands on a light background
  std::string image_file;                // as written in the experiment file
  std::string total_protein_file;
  std::string reference_image_file;
};

struct Experiment {
  std::string name;
  std::string control_group;
  std::vector<Blot> blots;
  std::string source_text;  // verbatim experiment YAML (reproducibility file)

  // Control group first, then the others in order of first appearance.
  std::vector<std::string> groups() const;
  bool has_total_protein() const;
};

// Reads an experiment description; image paths are relative to the file.
Experiment load_experiment(const std::filesystem::path& path);
Experiment parse_experiment(const std::string& yaml_text, const std::filesystem::path& base_dir);

// Writes an experiment file referencing the blots' image_file paths.
void save_experiment(const std::filesystem::path& path, const Experiment& experiment);

YAML::Node annotation_to_yaml(const Annotation& ann);
Annotation annotation_from_yaml(const YAML::Node& node);

}  // namespace blotverse
