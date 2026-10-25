#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "blotverse/report/report.hpp"

namespace blotverse {
namespace {

std::ofstream open_out(const std::filesystem::path& path) {
  std::ofstream out(path);
  if (!out) throw std::runtime_error("cannot write " + path.string());
  out << std::setprecision(10);
  return out;
}

std::string csv_escape(const std::string& s) {
  if (s.find_first_of(",\"\n") == std::string::npos) return s;
  std::string out = "\"";
  for (char c : s) {
    if (c == '"') out += '"';
    out += c;
  }
  return out + "\"";
}

}  // namespace

void write_universes_csv(const std::filesystem::path& path, const MultiverseSpec& spec,
                         const AnalysisResult& result) {
  auto out = open_out(path);
  out << "contrast,universe_id,is_default";
  for (Decision d : kDecisions) out << ',' << decision_key(d);
  out << ",valid,lfc,effect,p,n_control,n_treatment,note\n";
  const auto& mv = result.multiverse;
  for (std::size_t c = 0; c < mv.contrasts.size(); ++c) {
    for (std::size_t i = 0; i < mv.plan.universes.size(); ++i) {
      const auto& u = mv.plan.universes[i];
      const auto& o = mv.outcomes[c][i];
      out << csv_escape(mv.contrasts[c].label) << ',' << i << ','
          << (mv.plan.default_index == i ? 1 : 0);
      for (std::size_t d = 0; d < kNumDecisions; ++d) out << ',' << spec.decisions[d].branches[u[d]].id;
      out << ',' << (o.valid ? 1 : 0) << ',' << o.lfc << ',' << o.effect << ',' << o.p << ','
          << o.n_control << ',' << o.n_treatment << ',' << csv_escape(o.note) << '\n';
    }
  }
}

void write_summary_csv(const std::filesystem::path& path, const AnalysisResult& result) {
  auto out = open_out(path);
  out << "contrast,verdict,robustness_R,joint_p,default_lfc,default_p,median_lfc,lfc_q05,lfc_q95,"
         "opposite_fraction,significant_fraction,agreement,n_valid,n_universes,n_excluded,"
         "most_influential";
  for (Decision d : kDecisions) out << ",eta2_" << decision_key(d);
  out << '\n';
  for (const auto& s : result.summaries) {
    out << csv_escape(s.contrast.label) << ',' << verdict_label(s.verdict) << ',' << s.robustness << ','
        << s.joint.p << ',' << s.default_lfc << ',' << s.default_p << ',' << s.median_lfc << ','
        << s.lfc_q05 << ',' << s.lfc_q95 << ',' << s.opposite_fraction << ',' << s.significant_fraction
        << ',' << s.agreement << ',' << s.n_valid << ',' << s.n_universes << ',' << s.n_excluded << ','
        << (s.most_influential ? decision_key(*s.most_influential) : "");
    for (double e : s.eta2) out << ',' << e;
    out << '\n';
  }
}

void write_lanes_csv(const std::filesystem::path& path, const MultiverseSpec& spec,
                     const Experiment& experiment, const AnalysisResult& result) {
  auto out = open_out(path);
  const auto& bgs = spec.decision(Decision::Background).branches;
  const auto& bands = spec.decision(Decision::Band).branches;
  out << "blot,image,lane,group,band,x0,y0,x1,y1,band_height";
  for (const auto& bg : bgs)
    for (const auto& band : bands) out << ',' << bg.id << '_' << band.id;
  out << '\n';
  const auto& data = result.data;
  for (std::size_t i = 0; i < data.observations.size(); ++i) {
    const auto& o = data.observations[i];
    const auto& blot = experiment.blots[static_cast<std::size_t>(o.blot)];
    const auto& ann = result.blots[static_cast<std::size_t>(o.blot)].annotation;
    const auto& ref_ann = result.blots[static_cast<std::size_t>(o.blot)].reference_annotation;
    for (BandKind kind : {BandKind::Target, BandKind::Reference}) {
      // A loading control imaged separately has its own windows and file.
      const bool separate = kind == BandKind::Reference && ref_ann.has_value();
      const Rect w = separate ? band_window(*ref_ann, static_cast<std::size_t>(o.lane), BandKind::Target)
                              : band_window(ann, static_cast<std::size_t>(o.lane), kind);
      out << csv_escape(blot.id) << ',' << csv_escape(separate ? blot.reference_image_file : blot.image_file)
          << ',' << o.lane + 1 << ','
          << csv_escape(data.groups[static_cast<std::size_t>(o.group)]) << ','
          << (kind == BandKind::Target ? "target" : "reference") << ',' << w.x0 << ',' << w.y0 << ',' << w.x1
          << ',' << w.y1 << ',' << ann.band_height();
      for (std::size_t k = 0; k < bgs.size(); ++k)
        for (std::size_t m = 0; m < bands.size(); ++m) {
          const auto& q = data.quant[k][m];
          out << ',' << (kind == BandKind::Target ? q.target[i] : q.reference[i]);
        }
      out << '\n';
    }
  }
}

}  // namespace blotverse
