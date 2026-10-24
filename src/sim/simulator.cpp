#include "blotverse/sim/simulator.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

#include "blotverse/core/rng.hpp"
#include "blotverse/io/tiff.hpp"

namespace blotverse {
namespace {

constexpr int kTotalProteinBands = 24;

double detector(const SimParams& p, double linear_peak) {
  if (p.response == DetectorResponse::Linear) return linear_peak;
  return p.s_max * linear_peak / (p.s_max + linear_peak);
}

// Adds amplitude * G(x, y) for one band, evaluated within +/- 4 sigma.
void add_band(std::vector<double>& img, int width, int height, double xc, double yc, double sx,
              double sy, double curvature, double amplitude) {
  const int x0 = std::max(0, static_cast<int>(std::floor(xc - 4 * sx)));
  const int x1 = std::min(width - 1, static_cast<int>(std::ceil(xc + 4 * sx)));
  const double bend = curvature / 4.0;  // offset = bend * ((x - xc) / sx)^2, equals curvature at 2 sigma
  for (int x = x0; x <= x1; ++x) {
    const double u = (x - xc) / sx;
    const double centre = yc + bend * u * u;
    const int y0 = std::max(0, static_cast<int>(std::floor(centre - 4 * sy)));
    const int y1 = std::min(height - 1, static_cast<int>(std::ceil(centre + 4 * sy)));
    const double gx = std::exp(-0.5 * u * u);
    for (int y = y0; y <= y1; ++y) {
      const double v = (y - centre) / sy;
      img[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)] +=
          amplitude * gx * std::exp(-0.5 * v * v);
    }
  }
}

void add_background(std::vector<double>& img, int width, int height, const SimParams& p, Rng& rng,
                    const std::vector<double>& lane_x, const std::vector<double>& lane_load,
                    bool allow_nonspecific) {
  const double peak = p.peak_per_unit;
  // Gradient direction varies between gels.
  const double angle = rng.uniform(0.0, 2.0 * 3.141592653589793);
  const double gx = std::cos(angle), gy = std::sin(angle);
  const double hx = rng.uniform(0.2, 0.8) * width, hy = rng.uniform(0.2, 0.8) * height;
  const double haze_sigma = 0.35 * std::max(width, height);
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      const double t = 0.5 + 0.5 * (gx * (2.0 * x / width - 1.0) + gy * (2.0 * y / height - 1.0)) / std::sqrt(2.0);
      const double dx = x - hx, dy = y - hy;
      img[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)] +=
          p.offset + p.gradient * peak * t +
          p.haze * peak * std::exp(-(dx * dx + dy * dy) / (2 * haze_sigma * haze_sigma));
    }
  for (int s = 0; s < p.n_blotches; ++s) {
    const double sigma = rng.uniform(p.blotch_sigma_min, p.blotch_sigma_max);
    add_band(img, width, height, rng.uniform(0, width), rng.uniform(0, height), sigma,
             sigma * rng.uniform(0.6, 1.0), 0.0, rng.uniform(0.3, 1.0) * p.blotch_amplitude * peak);
  }
  for (int s = 0; s < p.n_spots; ++s) {
    const double sigma = rng.uniform(1.5, 5.0);
    add_band(img, width, height, rng.uniform(0, width), rng.uniform(0, height), sigma, sigma, 0.0,
             rng.uniform(0.1, 1.0) * p.spot_amplitude * peak);
  }
  if (!allow_nonspecific) return;
  for (int k = 0; k < p.n_nonspecific; ++k) {
    double y;
    if (p.nonspecific_near_target && k == 0)
      y = p.target_y + (rng.uniform() < 0.5 ? -1.0 : 1.0) * rng.uniform(3.5, 5.0) * p.band_sigma_y;
    else
      y = rng.uniform(0.1 * height, 0.9 * height);
    const double amp = rng.uniform(0.3, 1.0) * p.nonspecific_amplitude * peak;
    for (std::size_t j = 0; j < lane_x.size(); ++j)
      add_band(img, width, height, lane_x[j], y, p.band_sigma_x, p.band_sigma_y, 0.0, amp * lane_load[j]);
  }
}

ImageU16 finalize(const std::vector<double>& expected, int width, int height, const SimParams& p, Rng& rng) {
  ImageU16 out(width, height);
  for (std::size_t i = 0; i < expected.size(); ++i) {
    const double mean_e = std::max(0.0, expected[i]) * p.electrons_per_adu;
    const double adu = static_cast<double>(rng.poisson(mean_e)) / p.electrons_per_adu +
                       rng.normal(0.0, p.read_noise);
    out.data()[i] = static_cast<std::uint16_t>(std::clamp(std::round(adu), 0.0, 65535.0));
  }
  return out;
}

}  // namespace

const char* artifact_label(ArtifactLevel level) {
  switch (level) {
    case ArtifactLevel::Clean: return "clean";
    case ArtifactLevel::Moderate: return "moderate";
    case ArtifactLevel::Severe: return "severe";
  }
  return "?";
}

ArtifactLevel parse_artifact_level(const std::string& text) {
  if (text == "clean") return ArtifactLevel::Clean;
  if (text == "moderate") return ArtifactLevel::Moderate;
  if (text == "severe") return ArtifactLevel::Severe;
  throw std::invalid_argument("unknown artefact level '" + text + "' (clean|moderate|severe)");
}

SimParams sim_preset(ArtifactLevel level) {
  SimParams p;
  switch (level) {
    case ArtifactLevel::Clean:
      p.load_cv = 0.10;
      p.read_noise = 10.0;
      break;
    case ArtifactLevel::Moderate:
      p.response = DetectorResponse::Hyperbolic;
      p.s_max = 80000.0;
      p.gradient = 0.04;
      p.haze = 0.02;
      p.n_spots = 4;
      p.n_nonspecific = 2;
      p.band_curvature = 2.0;
      p.gel_smile = 3.0;
      p.load_cv = 0.15;
      break;
    case ArtifactLevel::Severe:
      p.response = DetectorResponse::Hyperbolic;
      p.s_max = 40000.0;
      p.peak_per_unit = 15000.0;
      p.gradient = 0.15;
      p.haze = 0.08;
      p.n_spots = 10;
      p.spot_amplitude = 0.5;
      p.n_nonspecific = 4;
      p.nonspecific_amplitude = 0.2;
      p.nonspecific_near_target = true;
      p.band_curvature = 4.0;
      p.gel_smile = 6.0;
      p.load_cv = 0.20;
      p.read_noise = 25.0;
      break;
  }
  return p;
}

SimExperiment simulate(const SimParams& p) {
  if (p.groups.size() < 2 || p.true_lfc.size() != p.groups.size())
    throw std::invalid_argument("simulate: need >= 2 groups and one true_lfc per group");
  if (p.n_gels < 1 || p.reps_per_gel < 1) throw std::invalid_argument("simulate: empty design");

  SimExperiment sim;
  sim.params = p;
  const int n_lanes = static_cast<int>(p.groups.size()) * p.reps_per_gel;
  const int width = 2 * p.margin + n_lanes * p.lane_pitch;
  const int height = p.height;
  const double centre_x = 0.5 * width;
  const double half_span = 0.5 * (n_lanes - 1) * p.lane_pitch;

  // Fixed total-protein band pattern (identical for every lane and gel).
  std::vector<double> tp_y, tp_abundance;
  {
    Rng pattern(0xB10770);
    for (int k = 0; k < kTotalProteinBands; ++k) {
      tp_y.push_back(0.08 * height + (0.84 * height) * (k + pattern.uniform(0.2, 0.8)) / kTotalProteinBands);
      tp_abundance.push_back(pattern.uniform(0.2, 1.0));
    }
  }

  for (int g = 0; g < p.n_gels; ++g) {
    Rng rng(derive_seed(p.seed, static_cast<std::uint64_t>(g)));
    SimBlot blot;
    blot.exposure = rng.lognormal_cv(p.exposure_cv);
    blot.gel_target_effect = rng.lognormal_cv(p.gel_target_cv);
    const double tp_exposure = rng.lognormal_cv(p.exposure_cv);

    std::vector<double> img(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), 0.0);
    std::vector<double> tp;
    if (p.total_protein) tp.assign(img.size(), 0.0);
    std::vector<double> lane_x, lane_load;

    for (int lane = 0; lane < n_lanes; ++lane) {
      const auto group = static_cast<std::size_t>(lane % static_cast<int>(p.groups.size()));
      const int rep = lane / static_cast<int>(p.groups.size());
      SimLaneTruth t;
      t.group = p.groups[group];
      t.sample = t.group + "_g" + std::to_string(g + 1) + "_r" + std::to_string(rep + 1);
      t.gel = g;
      t.load = rng.lognormal_cv(p.load_cv);
      t.target_amount = p.target_amount * std::exp2(p.true_lfc[group]) * rng.lognormal_cv(p.bio_cv) *
                        blot.gel_target_effect;
      t.reference_amount = p.reference_amount * rng.lognormal_cv(p.reference_bio_cv);
      t.x = p.margin + (lane + 0.5) * p.lane_pitch;
      const double u = half_span > 0 ? (t.x - centre_x) / half_span : 0.0;
      t.target_y = p.target_y - p.gel_smile * u * u;
      t.reference_y = p.reference_y - p.gel_smile * u * u;

      const double scale = p.peak_per_unit * blot.exposure;
      add_band(img, width, height, t.x, t.target_y, p.band_sigma_x, p.band_sigma_y, p.band_curvature,
               detector(p, scale * t.target_amount * t.load));
      add_band(img, width, height, t.x, t.reference_y, p.band_sigma_x, p.band_sigma_y, p.band_curvature,
               detector(p, scale * t.reference_amount * t.load));
      if (p.total_protein)
        for (int k = 0; k < kTotalProteinBands; ++k)
          add_band(tp, width, height, t.x, tp_y[static_cast<std::size_t>(k)] - p.gel_smile * u * u,
                   p.band_sigma_x, 2.5, p.band_curvature,
                   p.total_protein_peak * tp_exposure * tp_abundance[static_cast<std::size_t>(k)] * t.load);
      lane_x.push_back(t.x);
      lane_load.push_back(t.load);
      blot.lanes.push_back(std::move(t));
    }

    add_background(img, width, height, p, rng, lane_x, lane_load, true);
    blot.image = finalize(img, width, height, p, rng);
    if (p.total_protein) {
      SimParams tp_params = p;
      tp_params.peak_per_unit = p.total_protein_peak;
      add_background(tp, width, height, tp_params, rng, lane_x, lane_load, false);
      blot.total_protein = finalize(tp, width, height, p, rng);
    }
    sim.blots.push_back(std::move(blot));
  }
  return sim;
}

Experiment to_experiment(const SimExperiment& sim, const std::string& name) {
  Experiment ex;
  ex.name = name;
  ex.control_group = sim.params.groups.front();
  for (std::size_t g = 0; g < sim.blots.size(); ++g) {
    const auto& sb = sim.blots[g];
    Blot b;
    b.id = "gel" + std::to_string(g + 1);
    b.image_file = "blot_" + std::to_string(g + 1) + ".tif";
    b.image = make_raw_image(sb.image, b.image_file);
    if (sb.total_protein) {
      b.total_protein_file = "blot_" + std::to_string(g + 1) + "_total.tif";
      b.total_protein = make_raw_image(*sb.total_protein, b.total_protein_file);
    }
    for (const auto& lane : sb.lanes) b.lanes.push_back({lane.sample, lane.group, std::nullopt});
    // Approximate band rows, as a user would mark them from molecular weight.
    b.hints.target_y = std::round(sim.params.target_y);
    b.hints.reference_y = std::round(sim.params.reference_y);
    ex.blots.push_back(std::move(b));
  }
  return ex;
}

void write_simulation(const std::filesystem::path& dir, const SimExperiment& sim) {
  std::filesystem::create_directories(dir);
  const Experiment ex = to_experiment(sim);
  for (std::size_t g = 0; g < sim.blots.size(); ++g) {
    write_tiff16(dir / ex.blots[g].image_file, sim.blots[g].image);
    if (sim.blots[g].total_protein)
      write_tiff16(dir / ex.blots[g].total_protein_file, *sim.blots[g].total_protein);
  }
  save_experiment(dir / "experiment.yaml", ex);

  const auto& p = sim.params;
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "seed" << YAML::Value << p.seed;
  out << YAML::Key << "groups" << YAML::Value << YAML::Flow << p.groups;
  out << YAML::Key << "true_lfc" << YAML::Value << YAML::Flow << p.true_lfc;
  out << YAML::Key << "detector" << YAML::Value
      << (p.response == DetectorResponse::Linear ? "linear" : "hyperbolic");
  out << YAML::Key << "gradient" << YAML::Value << p.gradient;
  out << YAML::Key << "blots" << YAML::Value << YAML::BeginSeq;
  for (const auto& b : sim.blots) {
    out << YAML::BeginMap;
    out << YAML::Key << "exposure" << YAML::Value << b.exposure;
    out << YAML::Key << "gel_target_effect" << YAML::Value << b.gel_target_effect;
    out << YAML::Key << "lanes" << YAML::Value << YAML::BeginSeq;
    for (const auto& l : b.lanes) {
      out << YAML::Flow << YAML::BeginMap;
      out << YAML::Key << "sample" << YAML::Value << l.sample;
      out << YAML::Key << "group" << YAML::Value << l.group;
      out << YAML::Key << "load" << YAML::Value << l.load;
      out << YAML::Key << "target_amount" << YAML::Value << l.target_amount;
      out << YAML::Key << "reference_amount" << YAML::Value << l.reference_amount;
      out << YAML::Key << "x" << YAML::Value << l.x;
      out << YAML::Key << "target_y" << YAML::Value << l.target_y;
      out << YAML::Key << "reference_y" << YAML::Value << l.reference_y;
      out << YAML::EndMap;
    }
    out << YAML::EndSeq << YAML::EndMap;
  }
  out << YAML::EndSeq << YAML::EndMap;
  std::ofstream file(dir / "truth.yaml");
  file << out.c_str() << "\n";
}

}  // namespace blotverse
