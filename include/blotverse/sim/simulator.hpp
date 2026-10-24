#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"
#include "blotverse/experiment.hpp"

namespace blotverse {

// Synthetic western blots with known ground truth (plan section 8):
//
//   I(x,y) = min( B(x,y) + sum_j f(a_j * L_j) * G_j(x,y) + eps(x,y), 2^16 - 1 )
//
//  G_j  anisotropic 2D Gaussian band with optional curvature ("smile")
//  a_j  true protein amount; L_j lane loading factor (lognormal), shared by
//       target, reference and total protein of the lane
//  f    detector response: linear or hyperbolic S_max*q/(S_max + q)
//  B    offset + gradient + haze + blotches + spots + non-specific bands
//  eps  Poisson (shot) noise plus Gaussian read noise
// Preset version. 2: moderate/severe background amplitudes calibrated to 18
// real 16-bit acquisitions (docs/validation.md, realism check); 1: original.
inline constexpr int kSimulatorPresetVersion = 2;

enum class ArtifactLevel { Clean, Moderate, Severe };
enum class DetectorResponse { Linear, Hyperbolic };

const char* artifact_label(ArtifactLevel level);
ArtifactLevel parse_artifact_level(const std::string& text);

struct SimParams {
  // Design: lanes are interleaved by group on every gel.
  std::vector<std::string> groups{"ctrl", "treat"};
  std::vector<double> true_lfc{0.0, 1.0};  // log2 target change per group vs groups[0]
  int n_gels = 2;
  int reps_per_gel = 2;                    // lanes per group per gel
  bool total_protein = true;

  // Geometry (pixels).
  int lane_pitch = 48;
  int margin = 32;
  int height = 300;
  double target_y = 95.0;
  double reference_y = 205.0;
  double band_sigma_x = 7.0;
  double band_sigma_y = 3.2;
  double band_curvature = 0.0;  // row offset at +/- 2 sigma_x from the lane centre
  double gel_smile = 0.0;       // row offset of the outermost lanes

  // Biology and loading.
  double target_amount = 1.0;
  double reference_amount = 1.2;
  double bio_cv = 0.15;           // biological variation of the target
  double reference_bio_cv = 0.05;
  double load_cv = 0.15;          // lane loading factor L_j
  double gel_target_cv = 0.10;    // gel random effect on target relative to reference
  double exposure_cv = 0.20;      // gel-level exposure / transfer factor

  // Detector.
  DetectorResponse response = DetectorResponse::Linear;
  double peak_per_unit = 12000.0;  // peak ADU above background for amount*load = 1
  double s_max = 60000.0;          // hyperbolic saturation level
  double offset = 600.0;
  double electrons_per_adu = 1.0;
  double read_noise = 15.0;
  double total_protein_peak = 2500.0;

  // Background artefacts (amplitudes relative to peak_per_unit).
  double gradient = 0.0;
  double haze = 0.0;
  int n_spots = 0;
  double spot_amplitude = 0.3;
  int n_nonspecific = 0;
  double nonspecific_amplitude = 0.1;
  bool nonspecific_near_target = false;
  // Mid-scale background blotches (uneven substrate, membrane drying): broad
  // Gaussians whose size lies between the rolling-ball radii, so background
  // branches genuinely disagree about them.
  int n_blotches = 0;
  double blotch_amplitude = 0.3;
  double blotch_sigma_min = 8.0;
  double blotch_sigma_max = 30.0;

  std::uint64_t seed = 1;
};

SimParams sim_preset(ArtifactLevel level);

struct SimLaneTruth {
  std::string group;
  std::string sample;
  int gel = 0;
  double load = 1.0;
  double target_amount = 0.0;     // a_j for the target band
  double reference_amount = 0.0;
  double x = 0.0;
  double target_y = 0.0;
  double reference_y = 0.0;
};

struct SimBlot {
  ImageU16 image;
  std::optional<ImageU16> total_protein;
  std::vector<SimLaneTruth> lanes;
  double exposure = 1.0;
  double gel_target_effect = 1.0;
};

struct SimExperiment {
  SimParams params;
  std::vector<SimBlot> blots;
};

SimExperiment simulate(const SimParams& params);

// In-memory experiment that follows exactly the same analysis path as files.
Experiment to_experiment(const SimExperiment& sim, const std::string& name = "simulated");

// Writes blot_<n>.tif (+ blot_<n>_total.tif), experiment.yaml and truth.yaml.
void write_simulation(const std::filesystem::path& dir, const SimExperiment& sim);

}  // namespace blotverse
