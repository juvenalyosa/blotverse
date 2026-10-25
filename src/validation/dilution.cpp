#include "blotverse/validation/dilution.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <stdexcept>

#include "blotverse/imaging/background.hpp"
#include "blotverse/imaging/detect.hpp"
#include "blotverse/imaging/qc.hpp"
#include "blotverse/imaging/quantify.hpp"
#include "blotverse/io/image_file.hpp"

namespace blotverse {
namespace {

BandMethod band_method(Method m) {
  switch (m) {
    case Method::FixedRectangle: return BandMethod::FixedRectangle;
    case Method::ProfilePeak: return BandMethod::ProfilePeak;
    case Method::ThresholdMask: return BandMethod::ThresholdMask;
    default: throw std::logic_error("not a band definition method");
  }
}

// Ordinary least squares of log(signal) on log(load).
void loglog_fit(const std::vector<double>& load, const std::vector<double>& signal, double& slope, double& r2) {
  std::vector<double> x, y;
  for (std::size_t i = 0; i < load.size(); ++i)
    if (load[i] > 0 && signal[i] > 0) {
      x.push_back(std::log(load[i]));
      y.push_back(std::log(signal[i]));
    }
  if (x.size() < 3) return;
  const double mx = mean(x), my = mean(y);
  double sxy = 0, sxx = 0, syy = 0;
  for (std::size_t i = 0; i < x.size(); ++i) {
    sxy += (x[i] - mx) * (y[i] - my);
    sxx += (x[i] - mx) * (x[i] - mx);
    syy += (y[i] - my) * (y[i] - my);
  }
  slope = sxy / sxx;
  r2 = syy > 0 ? sxy * sxy / (sxx * syy) : kNaN;
}

}  // namespace

DilutionConfig load_dilution_config(const std::filesystem::path& path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("cannot open dilution config: " + path.string());
  std::stringstream ss;
  ss << in.rdbuf();
  const YAML::Node root = YAML::Load(ss.str());
  DilutionConfig c;
  c.source_text = ss.str();
  c.name = root["name"] ? root["name"].as<std::string>() : "dilution";
  c.image = path.parent_path() / root["image"].as<std::string>();
  c.invert = root["invert"] && root["invert"].as<bool>();
  c.lane_width = root["lane_width"].as<double>();
  c.band_half_height = root["band_half_height"].as<int>();
  if (root["refine_rows"]) c.refine_rows = root["refine_rows"].as<int>();
  for (const auto& l : root["lanes"]) c.lanes.push_back({l["x"].as<double>(), l["load"].as<double>()});
  for (const auto& b : root["bands"]) c.bands.push_back({b["name"].as<std::string>(), b["y"].as<double>()});
  if (c.lanes.size() < 3 || c.bands.empty())
    throw std::runtime_error("dilution config needs >= 3 lanes and >= 1 band");
  return c;
}

std::vector<DilutionFit> analyze_dilution(const DilutionConfig& config, const MultiverseSpec& spec) {
  RawImage raw = read_image(config.image);
  if (config.invert) invert_image(raw);
  const ImageF img = to_float(raw.pixels);
  std::vector<double> loads;
  for (const auto& l : config.lanes) loads.push_back(l.load);

  std::vector<DilutionFit> fits;
  for (const auto& band : config.bands) {
    // One annotation per band: every lane, the band centre refined per lane
    // (the reference row is unused and set equal to the target row).
    Annotation ann;
    ann.source = "manual";
    ann.band_half_height = config.band_half_height;
    for (const auto& l : config.lanes) ann.lanes.push_back({l.x_center, config.lane_width});
    for (std::size_t i = 0; i < config.lanes.size(); ++i) {
      // Brightest row within +/- refine_rows of the approximate band row.
      const Rect lane = lane_rect(ann, i).clipped(img.width(), img.height());
      const int guess = static_cast<int>(std::lround(band.y));
      int best_y = guess;
      double best = -INFINITY;
      for (int y = std::max(0, guess - config.refine_rows);
           y <= std::min(img.height() - 1, guess + config.refine_rows); ++y) {
        double s = 0;
        for (int x = lane.x0; x < lane.x1; ++x) s += img(x, y);
        if (s > best) {
          best = s;
          best_y = y;
        }
      }
      ann.target_y.push_back(best_y);
    }
    ann.reference_y = ann.target_y;

    for (const auto& bg : spec.decision(Decision::Background).branches) {
      ImageF corrected;
      if (bg.method == Method::RollingBall) {
        corrected = subtract(img, rolling_ball_background(img, bg.radius_factor * ann.band_height()));
      } else {
        corrected = local_background_bands(img, ann);
      }
      for (const auto& bd : spec.decision(Decision::Band).branches) {
        DilutionFit fit;
        fit.band = band.name;
        fit.background = bg.id;
        fit.band_definition = bd.id;
        for (std::size_t i = 0; i < config.lanes.size(); ++i)
          fit.signal.push_back(quantify_band(corrected, ann, i, BandKind::Target, band_method(bd.method)));
        loglog_fit(loads, fit.signal, fit.slope, fit.r2);
        fit.nonlinear = linear_range_flags(loads, fit.signal, spec.qc.linear_range_min_slope,
                                           spec.qc.linear_range_max_slope);
        double err_all = 0, err_ok = 0;
        std::size_t n_all = 0;
        for (std::size_t i = 0; i < loads.size(); ++i)
          for (std::size_t j = i + 1; j < loads.size(); ++j) {
            if (!(fit.signal[i] > 0 && fit.signal[j] > 0 && loads[i] > 0 && loads[j] > 0)) continue;
            const double err = std::fabs(std::log2(fit.signal[i] / fit.signal[j]) - std::log2(loads[i] / loads[j]));
            err_all += err;
            ++n_all;
            if (!fit.nonlinear[i] && !fit.nonlinear[j]) {
              err_ok += err;
              ++fit.pairs_unflagged;
            }
          }
        if (n_all) fit.fc_mae_all = err_all / static_cast<double>(n_all);
        if (fit.pairs_unflagged) fit.fc_mae_unflagged = err_ok / static_cast<double>(fit.pairs_unflagged);
        fits.push_back(std::move(fit));
      }
    }
  }
  return fits;
}

void write_dilution_outputs(const std::filesystem::path& dir, const DilutionConfig& config,
                            const std::vector<DilutionFit>& fits) {
  std::filesystem::create_directories(dir);
  {
    std::ofstream csv(dir / "dilution.csv");
    csv << std::setprecision(10) << "band,background,band_definition,load,signal,nonlinear_flag\n";
    for (const auto& f : fits)
      for (std::size_t i = 0; i < config.lanes.size(); ++i)
        csv << f.band << ',' << f.background << ',' << f.band_definition << ',' << config.lanes[i].load << ','
            << f.signal[i] << ',' << (f.nonlinear[i] ? 1 : 0) << '\n';
  }
  std::ofstream md(dir / "dilution.md");
  md << "# Dilution series: " << config.name << "\n\nImage `" << config.image.filename().string() << "`; loads:";
  for (const auto& l : config.lanes) md << ' ' << l.load;
  md << ".\n\nLog-log slope of signal on load over all lanes (1 = proportional), R^2, the loads the "
        "linear-range QC rule flags, and the mean absolute error of the estimated log2 fold change between "
        "lanes (known from the loads) over all lane pairs and over pairs of unflagged lanes.\n\n"
        "| Band | Background | Band definition | Slope | R^2 | Flagged loads | FC error, all pairs | "
        "FC error, unflagged pairs |\n|---|---|---|---|---|---|---|---|\n";
  md << std::fixed << std::setprecision(3);
  for (const auto& f : fits) {
    std::string flagged;
    for (std::size_t i = 0; i < f.nonlinear.size(); ++i)
      if (f.nonlinear[i]) {
        std::ostringstream v;
        v << config.lanes[i].load;
        flagged += (flagged.empty() ? "" : ", ") + v.str();
      }
    md << "| " << f.band << " | " << f.background << " | " << f.band_definition << " | " << f.slope << " | " << f.r2
       << " | " << (flagged.empty() ? "-" : flagged) << " | " << f.fc_mae_all << " | " << f.fc_mae_unflagged
       << " (" << f.pairs_unflagged << ") |\n";
  }
}

}  // namespace blotverse
