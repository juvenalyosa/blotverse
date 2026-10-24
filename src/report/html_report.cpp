#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <numeric>
#include <sstream>

#include "blotverse/report/report.hpp"
#include "blotverse/version.hpp"

namespace blotverse {
namespace {

std::string esc(const std::string& s) {
  std::string out;
  for (char c : s) {
    switch (c) {
      case '&': out += "&amp;"; break;
      case '<': out += "&lt;"; break;
      case '>': out += "&gt;"; break;
      case '"': out += "&quot;"; break;
      default: out += c;
    }
  }
  return out;
}

std::string num(double v, int precision = 3) {
  if (!std::isfinite(v)) return "&ndash;";
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << v;
  return ss.str();
}

std::string pval(double p) {
  if (!std::isfinite(p)) return "&ndash;";
  std::ostringstream ss;
  if (p < 1e-4) ss << std::scientific << std::setprecision(1) << p;
  else ss << std::fixed << std::setprecision(4) << p;
  return ss.str();
}

const char* verdict_class(Verdict v) {
  switch (v) {
    case Verdict::Robust: return "robust";
    case Verdict::Fragile: return "fragile";
    case Verdict::NotSupported: return "unsupported";
  }
  return "";
}

constexpr const char* kStyle = R"CSS(
:root { --bg:#fbfaf7; --fg:#1d1d1b; --muted:#6b6a65; --line:#dedbd2; --card:#ffffff;
        --pos:#1f6f8b; --neg:#b4472e; --ns:#a8a59b; --robust:#2e7d4f; --fragile:#b7791f; --unsupported:#9b2c2c; }
@media (prefers-color-scheme: dark) {
  :root { --bg:#161614; --fg:#ecebe6; --muted:#a3a197; --line:#34332f; --card:#1f1f1c;
          --pos:#5fb3d1; --neg:#e07b5f; --ns:#6d6b63; --robust:#5cbf86; --fragile:#e0a84a; --unsupported:#e06c6c; }
}
* { box-sizing:border-box; }
body { margin:0; background:var(--bg); color:var(--fg); font:15px/1.5 -apple-system,"Segoe UI",Helvetica,Arial,sans-serif; }
main { max-width:1080px; margin:0 auto; padding:24px 16px 64px; }
h1 { font-size:26px; margin:0 0 4px; } h2 { font-size:20px; margin:36px 0 8px; } h3 { font-size:16px; margin:20px 0 6px; }
.muted { color:var(--muted); } .card { background:var(--card); border:1px solid var(--line); border-radius:10px; padding:16px 18px; margin:12px 0; }
.verdict { display:inline-block; font-weight:700; font-size:20px; padding:4px 14px; border-radius:999px; color:#fff; }
.verdict.robust { background:var(--robust); } .verdict.fragile { background:var(--fragile); } .verdict.unsupported { background:var(--unsupported); }
.stats { display:grid; grid-template-columns:repeat(auto-fit,minmax(170px,1fr)); gap:10px; margin-top:12px; }
.stat b { display:block; font-size:20px; font-variant-numeric:tabular-nums; } .stat span { color:var(--muted); font-size:13px; }
svg { max-width:100%; height:auto; display:block; } svg text { fill:var(--fg); font-size:11px; }
table { border-collapse:collapse; width:100%; font-size:13px; font-variant-numeric:tabular-nums; }
th, td { border-bottom:1px solid var(--line); padding:4px 6px; text-align:left; } th { color:var(--muted); font-weight:600; }
.scroll { overflow-x:auto; } details summary { cursor:pointer; color:var(--muted); }
ul.warn li { margin:2px 0; } code { font-size:13px; }
)CSS";

// Specification curve: universes sorted by effect, with the branch panel below.
std::string spec_curve_svg(const MultiverseSpec& spec, const MultiverseResult& mv, std::size_t c,
                           const ContrastSummary& s) {
  const auto& outcomes = mv.outcomes[c];
  std::vector<std::size_t> order;
  for (std::size_t i = 0; i < outcomes.size(); ++i)
    if (outcomes[i].valid) order.push_back(i);
  if (order.empty()) return "<p class=\"muted\">No valid universes.</p>";
  std::stable_sort(order.begin(), order.end(),
                   [&](auto a, auto b) { return outcomes[a].lfc < outcomes[b].lfc; });

  std::size_t n_rows = 0;
  for (const auto& d : spec.decisions) n_rows += d.branches.size();
  const double W = 1000, left = 170, right = 16, top = 14, curve_h = 230, row_h = 11, gap = 30;
  const double panel_top = top + curve_h + gap;
  const double H = panel_top + static_cast<double>(n_rows + kNumDecisions) * row_h + 10;
  const double plot_w = W - left - right;
  const double dx = plot_w / static_cast<double>(order.size());

  double lo = 0, hi = 0;
  for (auto i : order) {
    lo = std::min(lo, outcomes[i].lfc);
    hi = std::max(hi, outcomes[i].lfc);
  }
  const double pad = 0.08 * std::max(1e-6, hi - lo);
  lo -= pad;
  hi += pad;
  auto ypos = [&](double v) { return top + curve_h * (hi - v) / (hi - lo); };
  const int default_sign = (s.default_lfc > 0) - (s.default_lfc < 0);

  std::ostringstream o;
  o << std::fixed << std::setprecision(1);
  o << "<svg viewBox=\"0 0 " << W << ' ' << H << "\" role=\"img\" aria-label=\"Specification curve\">";
  // Axes and zero line.
  o << "<line x1=\"" << left << "\" y1=\"" << top << "\" x2=\"" << left << "\" y2=\"" << top + curve_h
    << "\" stroke=\"var(--line)\"/>";
  o << "<line x1=\"" << left << "\" y1=\"" << ypos(0) << "\" x2=\"" << W - right << "\" y2=\"" << ypos(0)
    << "\" stroke=\"var(--muted)\" stroke-dasharray=\"4 3\"/>";
  for (int t = 0; t <= 4; ++t) {
    const double v = lo + (hi - lo) * t / 4.0;
    o << "<text x=\"" << left - 6 << "\" y=\"" << ypos(v) + 4 << "\" text-anchor=\"end\">"
      << num(v, 2) << "</text>";
  }
  o << "<text x=\"12\" y=\"" << top + curve_h / 2 << "\" transform=\"rotate(-90 12 " << top + curve_h / 2
    << ")\" text-anchor=\"middle\">log2 fold change</text>";

  for (std::size_t r = 0; r < order.size(); ++r) {
    const auto& u = outcomes[order[r]];
    const bool sig = u.p < spec.alpha;
    const int sgn = (u.lfc > 0) - (u.lfc < 0);
    const char* colour = !sig ? "var(--ns)" : (sgn == default_sign ? "var(--pos)" : "var(--neg)");
    const double x = left + (static_cast<double>(r) + 0.5) * dx;
    const bool is_default = mv.plan.default_index == order[r];
    o << "<circle cx=\"" << x << "\" cy=\"" << ypos(u.lfc) << "\" r=\"" << (is_default ? 5.0 : 2.2)
      << "\" fill=\"" << colour << "\"" << (is_default ? " stroke=\"var(--fg)\" stroke-width=\"1.5\"" : "")
      << "><title>" << esc(spec.describe(mv.plan.universes[order[r]])) << " | lfc " << num(u.lfc)
      << " | p " << pval(u.p) << "</title></circle>";
  }

  // Branch panel.
  double y = panel_top;
  for (std::size_t d = 0; d < kNumDecisions; ++d) {
    o << "<text x=\"4\" y=\"" << y + 8 << "\" font-weight=\"700\">"
      << decision_label(kDecisions[d]) << "</text>";
    y += row_h;
    for (std::size_t b = 0; b < spec.decisions[d].branches.size(); ++b) {
      o << "<text x=\"" << left - 6 << "\" y=\"" << y + 8 << "\" text-anchor=\"end\">"
        << esc(spec.decisions[d].branches[b].id) << "</text>";
      for (std::size_t r = 0; r < order.size(); ++r)
        if (mv.plan.universes[order[r]][d] == b)
          o << "<rect x=\"" << left + static_cast<double>(r) * dx << "\" y=\"" << y + 2 << "\" width=\""
            << std::max(1.0, dx * 0.9) << "\" height=\"7\" fill=\"var(--muted)\"/>";
      y += row_h;
    }
  }
  o << "</svg>";
  return o.str();
}

std::string influence_svg(const ContrastSummary& s) {
  const double W = 640, row = 26, left = 190, right = 60;
  const double H = row * kNumDecisions + 10;
  double mx = 0.0;
  for (double e : s.eta2) mx = std::max(mx, e);
  if (mx <= 0) mx = 1.0;
  std::ostringstream o;
  o << std::fixed << std::setprecision(1);
  o << "<svg viewBox=\"0 0 " << W << ' ' << H << "\" role=\"img\" aria-label=\"Decision influence\">";
  for (std::size_t d = 0; d < kNumDecisions; ++d) {
    const double y = 5 + d * row;
    const double w = (W - left - right) * s.eta2[d] / mx;
    const bool top = s.most_influential && static_cast<std::size_t>(*s.most_influential) == d;
    o << "<text x=\"" << left - 8 << "\" y=\"" << y + 15 << "\" text-anchor=\"end\">"
      << decision_label(kDecisions[d]) << "</text>";
    o << "<rect x=\"" << left << "\" y=\"" << y + 4 << "\" width=\"" << w << "\" height=\"14\" rx=\"3\" fill=\""
      << (top ? "var(--pos)" : "var(--ns)") << "\"/>";
    o << "<text x=\"" << left + w + 6 << "\" y=\"" << y + 15 << "\">" << num(s.eta2[d], 3) << "</text>";
  }
  o << "</svg>";
  return o.str();
}

}  // namespace

std::string render_html_report(const MultiverseSpec& spec, const Experiment& experiment,
                               const AnalysisResult& result) {
  const auto& mv = result.multiverse;
  std::ostringstream h;
  h << "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\">"
    << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    << "<title>BlotVerse report</title><style>" << kStyle << "</style></head><body><main>";
  h << "<h1>BlotVerse report: " << esc(experiment.name) << "</h1>";
  h << "<p class=\"muted\">BlotVerse " << kVersion << " &middot; spec <code>" << esc(spec.name) << " v"
    << esc(spec.version) << "</code> &middot; " << experiment.blots.size() << " blot(s), "
    << result.data.observations.size() << " quantified lanes &middot; " << mv.plan.universes.size()
    << " universes evaluated, " << mv.plan.excluded.size() << " excluded &middot; "
    << result.joint_permutations << " joint permutations &middot; " << num(result.seconds, 1) << " s</p>";

  for (std::size_t c = 0; c < result.summaries.size(); ++c) {
    const auto& s = result.summaries[c];
    h << "<h2>" << esc(s.contrast.label) << "</h2><div class=\"card\">";
    h << "<span class=\"verdict " << verdict_class(s.verdict) << "\">" << verdict_label(s.verdict) << "</span>";
    h << "<div class=\"stats\">";
    h << "<div class=\"stat\"><b>" << num(s.robustness) << "</b><span>Robustness R (same direction, p &lt; "
      << spec.alpha << ")</span></div>";
    h << "<div class=\"stat\"><b>" << pval(s.joint.p) << "</b><span>Joint permutation test ("
      << s.joint.observed << " significant universes)</span></div>";
    h << "<div class=\"stat\"><b>" << num(s.default_lfc) << "</b><span>Default analysis lfc (p "
      << pval(s.default_p) << ")</span></div>";
    h << "<div class=\"stat\"><b>" << num(s.median_lfc) << "</b><span>Median lfc, 5&ndash;95%: "
      << num(s.lfc_q05) << " to " << num(s.lfc_q95) << "</span></div>";
    h << "<div class=\"stat\"><b>" << num(s.opposite_fraction) << "</b><span>Fraction with opposite sign ("
      << num(s.opposite_significant_fraction) << " significant)</span></div>";
    h << "<div class=\"stat\"><b>" << (s.most_influential ? decision_label(*s.most_influential) : "&ndash;")
      << "</b><span>Decision that moves the result most</span></div>";
    h << "</div>";
    for (const auto& n : s.notes) h << "<p class=\"muted\">Note: " << esc(n) << "</p>";
    h << "</div>";
    h << "<h3>Specification curve</h3><p class=\"muted\">Every valid universe ordered by effect. "
         "Blue: significant in the default's direction; red: significant in the opposite direction; "
         "grey: not significant. The large outlined point is the pre-registered default.</p>";
    h << "<div class=\"card\">" << spec_curve_svg(spec, mv, c, s) << "</div>";
    h << "<h3>Influence of each decision (&eta;&sup2;)</h3><div class=\"card\">" << influence_svg(s) << "</div>";
  }

  h << "<h2>Quality control</h2><div class=\"card\">";
  if (result.warnings.empty()) h << "<p>No quality-control warnings.</p>";
  else {
    h << "<ul class=\"warn\">";
    for (const auto& w : result.warnings) h << "<li>" << esc(w) << "</li>";
    h << "</ul>";
  }
  h << "</div>";

  if (!mv.plan.excluded.empty()) {
    h << "<h2>Excluded combinations</h2><div class=\"card\"><p>" << mv.plan.excluded.size()
      << " universes were excluded before analysis by the specification's rules:</p><ul>";
    std::map<std::string, std::size_t> reasons;
    for (const auto& e : mv.plan.excluded) ++reasons[e.reason];
    for (const auto& [reason, count] : reasons) h << "<li>" << esc(reason) << " (" << count << ")</li>";
    h << "</ul></div>";
  }

  h << "<h2>All universes</h2><details><summary>Show the table (" << mv.plan.universes.size()
    << " rows per contrast)</summary><div class=\"scroll\"><table><tr><th>Contrast</th>";
  for (Decision d : kDecisions) h << "<th>" << decision_key(d) << "</th>";
  h << "<th>lfc</th><th>p</th><th>note</th></tr>";
  for (std::size_t c = 0; c < mv.contrasts.size(); ++c)
    for (std::size_t i = 0; i < mv.plan.universes.size(); ++i) {
      const auto& o = mv.outcomes[c][i];
      h << "<tr><td>" << esc(mv.contrasts[c].label) << (mv.plan.default_index == i ? " (default)" : "") << "</td>";
      for (std::size_t d = 0; d < kNumDecisions; ++d)
        h << "<td>" << esc(spec.decisions[d].branches[mv.plan.universes[i][d]].id) << "</td>";
      h << "<td>" << num(o.lfc) << "</td><td>" << pval(o.p) << "</td><td>" << esc(o.note) << "</td></tr>";
    }
  h << "</table></div></details>";

  h << "<h2>About this report</h2><div class=\"card\"><p>Every defensible combination of analysis choices "
       "listed in the frozen specification was run; all of them are shown. BlotVerse never selects or "
       "recommends a single universe. Verdict thresholds: Robust if R &ge; "
    << spec.verdict.robust_min_r << " and joint p &lt; " << spec.verdict.joint_alpha << "; Fragile if R &ge; "
    << spec.verdict.fragile_min_r << "; otherwise Not supported. The reproducibility file "
       "<code>repro.yaml</code> regenerates these numbers exactly.</p></div>";
  h << "</main></body></html>\n";
  return h.str();
}

}  // namespace blotverse
