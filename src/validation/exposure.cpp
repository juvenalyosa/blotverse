#include "blotverse/validation/exposure.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "blotverse/imaging/detect.hpp"
#include "blotverse/imaging/qc.hpp"
#include "blotverse/io/image_file.hpp"
#include "blotverse/validation/dilution.hpp"

namespace blotverse {
namespace {

// Image median and robust pixel noise. The noise comes from horizontal first
// differences, 1.4826 x MAD(I(x+1,y) - I(x,y)) / sqrt(2), so that membrane
// structure and smears (which dominate the spread of raw intensities when the
// membrane fills the frame) do not count as noise. It underestimates the noise
// when neighbouring pixels are correlated (smoothed exports).
void image_level(const ImageF& img, double& med, double& noise) {
  std::vector<double> v(img.data().begin(), img.data().end());
  med = median(v);
  std::vector<double> d;
  d.reserve(v.size());
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x + 1 < img.width(); ++x) d.push_back(std::fabs(static_cast<double>(img(x + 1, y)) - img(x, y)));
  noise = 1.4826 * median(std::move(d)) / std::sqrt(2.0);
}

ExposurePoint measure_point(const RawImage& raw, const ImageF& img, const Annotation& ann, std::size_t lane,
                            double med, double noise, double near_fraction) {
  ExposurePoint p;
  const Rect w = band_window(ann, lane, BandKind::Target).clipped(img.width(), img.height());
  p.saturated_px = count_saturated(raw.pixels, w, raw.meta.detector_max);
  const double top = std::min<double>(raw.meta.detector_max, 65535.0), near = near_fraction * top;
  double peak = 0;
  for (int y = w.y0; y < w.y1; ++y)
    for (int x = w.x0; x < w.x1; ++x) {
      if (raw.pixels(x, y) >= near) ++p.near_saturated_px;
      peak = std::max<double>(peak, raw.pixels(x, y));
    }
  if (!w.empty()) p.peak_fraction = peak / top;
  // Mean row profile over the lane window's columns.
  auto row_mean = [&](int y) {
    if (y < 0 || y >= img.height() || w.width() <= 0) return kNaN;
    double s = 0;
    for (int x = w.x0; x < w.x1; ++x) s += img(x, y);
    return s / w.width();
  };
  // Band prominence: central rows (+/- half of the band half-height) minus the
  // flanking rows just outside the window (one half-height above and below),
  // in units of the image's robust pixel noise. A lane without the band, or a
  // band lost in the lane smear, scores near zero or below.
  const int y0 = ann.target_y[lane], hh = ann.band_half_height;
  double core = 0, flank = 0;
  int nc = 0, nf = 0;
  for (int d = -hh / 2; d <= hh / 2; ++d) {
    const double v = row_mean(y0 + d);
    if (std::isfinite(v)) core += v, ++nc;
  }
  for (int d = hh + 1; d <= 2 * hh; ++d)
    for (int sign : {-1, 1}) {
      const double v = row_mean(y0 + sign * d);
      if (std::isfinite(v)) flank += v, ++nf;
    }
  if (nc && nf && noise > 0) p.snr = (core / nc - flank / nf) / noise;
  double sw = 0, swy = 0;
  for (int y = w.y0; y < w.y1; ++y)
    for (int x = w.x0; x < w.x1; ++x) {
      const double above = std::max(0.0, static_cast<double>(img(x, y)) - med);
      sw += above;
      swy += above * y;
    }
  p.drift_px = sw > 0 ? swy / sw : kNaN;  // absolute centroid; made relative once the reference is known
  return p;
}

}  // namespace

ExposureConfig load_exposure_config(const std::filesystem::path& path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("cannot open exposure config: " + path.string());
  std::stringstream ss;
  ss << in.rdbuf();
  const YAML::Node root = YAML::Load(ss.str());
  ExposureConfig c;
  c.source_text = ss.str();
  c.name = root["name"] ? root["name"].as<std::string>() : "exposure series";
  c.invert = root["invert"] && root["invert"].as<bool>();
  c.lane_width = root["lane_width"].as<double>();
  c.band_half_height = root["band_half_height"].as<int>();
  if (root["refine_rows"]) c.refine_rows = root["refine_rows"].as<int>();
  if (root["snr_floor"]) c.snr_floor = root["snr_floor"].as<double>();
  if (root["distortion_log2"]) c.distortion_log2 = root["distortion_log2"].as<double>();
  if (root["max_drift_px"]) c.max_drift_px = root["max_drift_px"].as<double>();
  if (root["near_saturation_fraction"]) c.near_saturation_fraction = root["near_saturation_fraction"].as<double>();
  if (root["bright_fraction"]) c.bright_fraction = root["bright_fraction"].as<double>();
  for (const auto& e : root["exposures"])
    c.exposures.push_back({path.parent_path() / e["image"].as<std::string>(),
                           e["seconds"] ? e["seconds"].as<double>() : -1.0});
  for (const auto& l : root["lanes"]) c.lane_x.push_back(l["x"].as<double>());
  for (const auto& b : root["bands"])
    c.bands.push_back({b["name"].as<std::string>(), b["y"].as<double>(),
                       b["half_height"] ? b["half_height"].as<int>() : 0});
  if (c.exposures.size() < 2) throw std::runtime_error("exposure config needs at least two exposures");
  if (c.lane_x.size() < 2 || c.bands.empty())
    throw std::runtime_error("exposure config needs at least two lanes and one band");
  return c;
}

ExposureResult analyze_exposure(const ExposureConfig& config, const MultiverseSpec& spec) {
  const std::size_t ne = config.exposures.size(), nl = config.lane_x.size();
  std::vector<RawImage> raws;
  std::vector<ImageF> imgs;
  std::vector<double> med(ne), noise(ne);
  ExposureResult result;
  result.excluded.assign(ne, false);
  result.notes.assign(ne, "");
  for (std::size_t e = 0; e < ne; ++e) {
    RawImage raw = read_image(config.exposures[e].image);
    if (config.invert) invert_image(raw);
    result.seconds.push_back(config.exposures[e].seconds >= 0 ? config.exposures[e].seconds : raw.meta.exposure_s);
    imgs.push_back(to_float(raw.pixels));
    image_level(imgs.back(), med[e], noise[e]);
    // Geometry is fixed across the series, so every image must share the
    // first one's pixel grid (a different binning cannot be overlaid).
    if (!raws.empty() && (raw.pixels.width() != raws.front().pixels.width() ||
                          raw.pixels.height() != raws.front().pixels.height())) {
      result.excluded[e] = true;
      result.notes[e] = "excluded: " + std::to_string(raw.pixels.width()) + "x" +
                        std::to_string(raw.pixels.height()) + " differs from the first exposure";
    }
    raws.push_back(std::move(raw));
  }

  result.default_branch =
      spec.decision(Decision::Background).branches[spec.default_universe[static_cast<std::size_t>(Decision::Background)]].id +
      "/" + spec.decision(Decision::Band).branches[spec.default_universe[static_cast<std::size_t>(Decision::Band)]].id;

  for (const auto& band : config.bands) {
    ExposureBand out;
    out.name = band.name;
    const int half_height = band.half_height > 0 ? band.half_height : config.band_half_height;
    const Annotation ann =
        band_row_annotation(imgs.front(), config.lane_x, config.lane_width, half_height, band.y, config.refine_rows);
    out.points.assign(ne, std::vector<ExposurePoint>(nl));
    for (std::size_t e = 0; e < ne; ++e)
      if (!result.excluded[e])
        for (std::size_t i = 0; i < nl; ++i) out.points[e][i] =
            measure_point(raws[e], imgs[e], ann, i, med[e], noise[e], config.near_saturation_fraction);

    // Reference: the longest exposure (by time when known, else the last
    // listed) whose band windows hold no pixel at the detector maximum.
    for (std::size_t e = 0; e < ne; ++e) {
      if (result.excluded[e]) continue;
      bool saturated = false;
      for (const auto& p : out.points[e]) saturated |= p.saturated_px >= spec.qc.saturation_min_pixels;
      if (saturated) continue;
      if (out.reference < 0) {
        out.reference = static_cast<long>(e);
        continue;
      }
      const double t = result.seconds[e], tr = result.seconds[static_cast<std::size_t>(out.reference)];
      if ((t >= 0 && tr >= 0) ? t > tr : true) out.reference = static_cast<long>(e);
    }
    out.misaligned.assign(ne, false);
    if (out.reference >= 0) {
      const auto ref = static_cast<std::size_t>(out.reference);
      std::vector<double> ref_row(nl);
      for (std::size_t i = 0; i < nl; ++i) ref_row[i] = out.points[ref][i].drift_px;
      for (std::size_t e = 0; e < ne; ++e) {
        if (result.excluded[e]) continue;
        for (std::size_t i = 0; i < nl; ++i) {
          auto& p = out.points[e][i];
          p.drift_px -= ref_row[i];
          // Only a band that is there at both exposures has a centroid to compare.
          const bool present = p.snr >= config.snr_floor && out.points[ref][i].snr >= config.snr_floor;
          if (present && std::fabs(p.drift_px) > config.max_drift_px) out.misaligned[e] = true;
        }
      }
    } else {
      for (auto& row : out.points)
        for (auto& p : row) p.drift_px = kNaN;
    }

    // Signals: every branch, every usable exposure.
    std::vector<std::vector<BranchSignals>> per_exposure(ne);
    for (std::size_t e = 0; e < ne; ++e)
      if (!result.excluded[e]) per_exposure[e] = measure_band_row(imgs[e], ann, spec);
    const std::size_t nb = per_exposure.front().size();
    for (std::size_t b = 0; b < nb; ++b) {
      ExposureBranch br;
      br.background = per_exposure.front()[b].background;
      br.band_definition = per_exposure.front()[b].band_definition;
      br.signal.assign(ne, std::vector<double>(nl, kNaN));
      for (std::size_t e = 0; e < ne; ++e)
        if (!result.excluded[e]) br.signal[e] = per_exposure[e][b].signal;
      if (out.reference >= 0) {
        const auto ref = static_cast<std::size_t>(out.reference);
        const auto& sr = br.signal[ref];
        std::vector<double> clean, flagged;
        for (std::size_t e = 0; e < ne; ++e) {
          if (e == ref || result.excluded[e] || out.misaligned[e]) continue;
          const auto& s = br.signal[e];
          for (std::size_t i = 0; i < nl; ++i)
            for (std::size_t j = i + 1; j < nl; ++j) {
              if (!(s[i] > 0 && s[j] > 0 && sr[i] > 0 && sr[j] > 0)) {
                ++br.nonpositive_pairs;
                continue;
              }
              ExposurePair p;
              p.exposure = e;
              p.lane_i = i;
              p.lane_j = j;
              p.delta = std::log2(s[i] / s[j]) - std::log2(sr[i] / sr[j]);
              const auto& pe = out.points[e];
              const auto& pr = out.points[ref];
              p.flagged = pe[i].saturated_px >= spec.qc.saturation_min_pixels ||
                          pe[j].saturated_px >= spec.qc.saturation_min_pixels;
              // A NaN SNR (empty window) counts as dim.
              p.dim = !(pe[i].snr >= config.snr_floor && pe[j].snr >= config.snr_floor &&
                        pr[i].snr >= config.snr_floor && pr[j].snr >= config.snr_floor);
              if (!p.dim) {
                const double a = std::fabs(p.delta);
                (p.flagged ? flagged : clean).push_back(a);
                const bool distorted = a > config.distortion_log2;
                // Which of the two exposures is longer (list order when a time is unknown)?
                const double te = result.seconds[e], tr = result.seconds[ref];
                const bool e_longer = (te >= 0 && tr >= 0) ? te > tr : e > ref;
                p.blamed = e_longer ? e : ref;
                // Change in log ratio from the shorter to the longer exposure,
                // against the log ratio at the shorter one.
                const double change = e_longer ? p.delta : -p.delta;
                const double base = e_longer ? std::log2(sr[i] / sr[j]) : std::log2(s[i] / s[j]);
                p.compressive = distorted && change * base < 0;
                if (p.flagged) (distorted ? br.tp : br.fp)++;
                else (distorted ? br.fn : br.tn)++;
              }
              br.pairs.push_back(p);
            }
        }
        br.clean_n = clean.size();
        br.flagged_n = flagged.size();
        if (!clean.empty()) {
          br.clean_median_abs = median(clean);
          br.clean_q95_abs = quantile(clean, 0.95);
        }
        if (!flagged.empty()) br.flagged_median_abs = median(flagged);
      }
      out.branches.push_back(std::move(br));
    }

    out.any_flagged.assign(ne, false);
    out.compressive_default.assign(ne, false);
    for (std::size_t e = 0; e < ne; ++e)
      for (const auto& p : out.points[e]) out.any_flagged[e] = out.any_flagged[e] || p.saturated_px >= spec.qc.saturation_min_pixels;
    for (const auto& br : out.branches)
      if (br.background + "/" + br.band_definition == result.default_branch)
        for (const auto& p : br.pairs)
          if (!p.dim && p.compressive) out.compressive_default[p.blamed] = true;
    for (std::size_t e = 0; e < ne; ++e)
      if (out.compressive_default[e] && !out.any_flagged[e]) ++out.qc_misses;

    // Protocol v1.2: lane-level attribution in the default branch.
    out.any_near_saturated.assign(ne, false);
    for (std::size_t e = 0; e < ne; ++e)
      for (const auto& p : out.points[e]) out.any_near_saturated[e] = out.any_near_saturated[e] || p.near_saturated_px > 0;
    out.lane_scale.assign(ne, std::vector<double>(nl, kNaN));
    out.attributable.assign(ne, false);
    out.sublinear.assign(ne, false);
    out.superlinear.assign(ne, false);
    if (out.reference >= 0) {
      const auto ref = static_cast<std::size_t>(out.reference);
      const ExposureBranch* def = nullptr;
      for (const auto& br : out.branches)
        if (br.background + "/" + br.band_definition == result.default_branch) def = &br;
      for (std::size_t e = 0; e < ne && def; ++e) {
        if (e == ref || result.excluded[e] || out.misaligned[e]) continue;
        const double te = result.seconds[e], tr = result.seconds[ref];
        const bool e_longer = (te >= 0 && tr >= 0) ? te > tr : e > ref;
        const std::size_t longer = e_longer ? e : ref, shorter = e_longer ? ref : e;
        std::vector<double> scale(nl, kNaN), measured;
        for (std::size_t i = 0; i < nl; ++i) {
          const double sl = def->signal[longer][i], ss = def->signal[shorter][i];
          if (out.points[e][i].snr >= config.snr_floor && out.points[ref][i].snr >= config.snr_floor && sl > 0 &&
              ss > 0) {
            scale[i] = std::log2(sl / ss);
            measured.push_back(scale[i]);
          }
        }
        if (measured.size() < 3) continue;
        out.attributable[e] = true;
        const double m = median(measured);
        for (std::size_t i = 0; i < nl; ++i) {
          if (!std::isfinite(scale[i])) continue;
          const double d = scale[i] - m;
          out.lane_scale[e][i] = d;
          if (std::fabs(d) <= config.distortion_log2) continue;
          if (d < 0 && out.points[longer][i].peak_fraction >= config.bright_fraction) out.sublinear[longer] = true;
          // With saturated lanes at the longer exposure the median lane is
          // itself compressed, so unsaturated lanes only look super-linear.
          else if (!out.any_flagged[longer]) out.superlinear[shorter] = true;
        }
      }
    }
    for (std::size_t e = 0; e < ne; ++e) {
      if (!out.sublinear[e] || out.any_flagged[e]) continue;
      ++out.misses_spec;
      if (!out.any_near_saturated[e]) ++out.misses_candidate;
    }
    result.bands.push_back(std::move(out));
  }
  return result;
}

void write_exposure_outputs(const std::filesystem::path& dir, const ExposureConfig& config,
                            const MultiverseSpec& spec, const ExposureResult& result) {
  std::filesystem::create_directories(dir);
  const std::size_t ne = config.exposures.size(), nl = config.lane_x.size();
  {
    std::ofstream csv(dir / "exposure_lanes.csv");
    csv << std::setprecision(10)
        << "band,exposure,image,seconds,lane,saturated_px,near_saturated_px,peak_fraction,snr,drift_px,"
           "lane_scale_default";
    if (!result.bands.empty())
      for (const auto& br : result.bands.front().branches) csv << ',' << br.background << '_' << br.band_definition;
    csv << '\n';
    for (const auto& band : result.bands)
      for (std::size_t e = 0; e < ne; ++e) {
        if (result.excluded[e]) continue;
        for (std::size_t i = 0; i < nl; ++i) {
          const auto& p = band.points[e][i];
          csv << band.name << ',' << e + 1 << ',' << config.exposures[e].image.filename().string() << ','
              << result.seconds[e] << ',' << i + 1 << ',' << p.saturated_px << ',' << p.near_saturated_px << ','
              << p.peak_fraction << ',' << p.snr << ',' << p.drift_px << ',' << band.lane_scale[e][i];
          for (const auto& br : band.branches) csv << ',' << br.signal[e][i];
          csv << '\n';
        }
      }
  }
  {
    std::ofstream csv(dir / "exposure_pairs.csv");
    csv << std::setprecision(10) << "band,background,band_definition,exposure,seconds,lane_i,lane_j,delta_log2,"
                                    "qc_flagged,dim,compressive,blamed_exposure\n";
    for (const auto& band : result.bands)
      for (const auto& br : band.branches)
        for (const auto& p : br.pairs)
          csv << band.name << ',' << br.background << ',' << br.band_definition << ',' << p.exposure + 1 << ','
              << result.seconds[p.exposure] << ',' << p.lane_i + 1 << ',' << p.lane_j + 1 << ',' << p.delta << ','
              << (p.flagged ? 1 : 0) << ',' << (p.dim ? 1 : 0) << ',' << (p.compressive ? 1 : 0) << ',' << p.blamed + 1 << '\n';
  }

  {
    std::ofstream csv(dir / "exposure_summary.csv");
    csv << std::setprecision(10)
        << "band,exposure,image,seconds,excluded,reference,misaligned,qc_flagged,near_saturation_flagged,"
           "compressive_default,qc_miss_v11,attributable,sublinear,superlinear,miss_spec,miss_candidate\n";
    for (const auto& band : result.bands)
      for (std::size_t e = 0; e < ne; ++e) {
        const bool excluded = result.excluded[e];
        csv << band.name << ',' << e + 1 << ',' << config.exposures[e].image.filename().string() << ','
            << result.seconds[e] << ',' << (excluded ? 1 : 0) << ','
            << (static_cast<long>(e) == band.reference ? 1 : 0) << ',' << (!excluded && band.misaligned[e] ? 1 : 0)
            << ',' << (!excluded && band.any_flagged[e] ? 1 : 0) << ','
            << (!excluded && band.any_near_saturated[e] ? 1 : 0) << ','
            << (!excluded && band.compressive_default[e] ? 1 : 0) << ','
            << (!excluded && band.compressive_default[e] && !band.any_flagged[e] ? 1 : 0) << ','
            << (band.attributable[e] ? 1 : 0) << ',' << (band.sublinear[e] ? 1 : 0) << ','
            << (band.superlinear[e] ? 1 : 0) << ',' << (band.sublinear[e] && !band.any_flagged[e] ? 1 : 0) << ','
            << (band.sublinear[e] && !band.any_flagged[e] && !band.any_near_saturated[e] ? 1 : 0) << '\n';
      }
  }

  std::ofstream md(dir / "exposure.md");
  md << "# Exposure series: " << config.name << "\n\n"
     << "Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not "
        "depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the "
        "longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR "
     << config.snr_floor << " are left out; a pair is *distorted* when its ratio moves by more than "
     << config.distortion_log2 << " log2 and *flagged* when the saturation QC rule (>= "
     << spec.qc.saturation_min_pixels << " px at the detector maximum) excludes either lane.\n\n## Exposures\n\n"
     << "| # | Image | Seconds | Note |\n|---|---|---|---|\n";
  for (std::size_t e = 0; e < ne; ++e)
    md << "| " << e + 1 << " | `" << config.exposures[e].image.filename().string() << "` | "
       << (result.seconds[e] >= 0 ? std::to_string(result.seconds[e]).substr(0, 7) : "?") << " | "
       << (result.notes[e].empty() ? "-" : result.notes[e]) << " |\n";
  md << std::fixed;
  for (const auto& band : result.bands) {
    md << "\n## Band `" << band.name << "`\n\n";
    md << "Saturated pixels per lane window, by exposure:\n\n| # |";
    for (std::size_t i = 0; i < nl; ++i) md << " lane " << i + 1 << " |";
    md << " min SNR | max drift (px) | G3 (" << result.default_branch << ") |\n|---|";
    for (std::size_t i = 0; i < nl + 3; ++i) md << "---|";
    md << '\n';
    for (std::size_t e = 0; e < ne; ++e) {
      if (result.excluded[e]) continue;
      md << "| " << e + 1 << (static_cast<long>(e) == band.reference ? " (ref)" : "")
         << (band.misaligned[e] ? " (misaligned)" : "") << " |";
      double min_snr = INFINITY, max_drift = 0;
      for (const auto& p : band.points[e]) {
        md << ' ' << p.saturated_px << " |";
        min_snr = std::min(min_snr, p.snr);
        if (std::isfinite(p.drift_px) && p.snr >= config.snr_floor) max_drift = std::max(max_drift, std::fabs(p.drift_px));
      }
      // Protocol v1.2: lane-level attribution (sub-linear lane blamed on this exposure).
      std::string g3 = band.any_flagged[e]           ? "flagged"
                       : band.any_near_saturated[e]  ? "near-saturation only"
                                                     : "no flag";
      if (band.sublinear[e]) g3 += band.any_flagged[e] ? ", sub-linear lane" : ", **sub-linear lane: MISS**";
      if (band.superlinear[e]) g3 += ", low-signal distortion";
      md << std::setprecision(1) << ' ' << min_snr << " | " << max_drift << " | " << g3 << " |\n";
    }
    if (band.reference < 0) {
      md << "\nNot evaluable: every exposure has a saturated lane, so no exposure can serve as reference.\n";
      continue;
    }
    md << "\n| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | "
          "median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |\n"
          "|---|---|---|---|---|---|---|---|---|---|---|---|\n"
       << std::setprecision(3);
    for (const auto& br : band.branches)
      md << "| " << br.background << " | " << br.band_definition << " | " << br.clean_n << " | "
         << br.clean_median_abs << " | " << br.clean_q95_abs << " | " << br.flagged_n << " | "
         << br.flagged_median_abs << " | " << br.tp << " | " << br.fp << " | " << br.fn << " | " << br.tn << " | "
         << br.nonpositive_pairs << " |\n";
  }
}

}  // namespace blotverse
