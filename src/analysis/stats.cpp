#include "blotverse/analysis/stats.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

#include "blotverse/core/rng.hpp"

namespace blotverse {
namespace {

// Continued fraction for the incomplete beta (modified Lentz method).
double beta_continued_fraction(double a, double b, double x) {
  constexpr int kMaxIter = 300;
  constexpr double kEps = 1e-15;
  constexpr double kTiny = 1e-300;
  const double qab = a + b, qap = a + 1.0, qam = a - 1.0;
  double c = 1.0;
  double d = 1.0 - qab * x / qap;
  if (std::fabs(d) < kTiny) d = kTiny;
  d = 1.0 / d;
  double h = d;
  for (int m = 1; m <= kMaxIter; ++m) {
    const int m2 = 2 * m;
    double aa = m * (b - m) * x / ((qam + m2) * (a + m2));
    d = 1.0 + aa * d;
    if (std::fabs(d) < kTiny) d = kTiny;
    c = 1.0 + aa / c;
    if (std::fabs(c) < kTiny) c = kTiny;
    d = 1.0 / d;
    h *= d * c;
    aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
    d = 1.0 + aa * d;
    if (std::fabs(d) < kTiny) d = kTiny;
    c = 1.0 + aa / c;
    if (std::fabs(c) < kTiny) c = kTiny;
    d = 1.0 / d;
    const double del = d * c;
    h *= del;
    if (std::fabs(del - 1.0) < kEps) break;
  }
  return h;
}

TestResult failure(std::string why) {
  TestResult r;
  r.error = std::move(why);
  return r;
}

// Pieces of the REML criterion for a given variance ratio lambda.
struct GlsFit {
  double reml = -INFINITY;
  double b0 = kNaN, b1 = kNaN;
  double rss = kNaN;                // r' H^-1 r
  double xtx[2][2] = {{0, 0}, {0, 0}};  // X' H^-1 X
};

struct GelBlock {
  double n = 0, sum_t = 0, sum_y = 0;
};

GlsFit gls(double lambda, const std::vector<GelBlock>& blocks, double sy2, double syt, double st,
           double st2, double n) {
  // H_g^{-1} = I - c_g J with c_g = lambda / (1 + lambda n_g); X = [1, t].
  double a00 = n, a01 = st, a11 = st2, v0 = 0, v1 = syt, yy = sy2, logdet = 0;
  for (const auto& g : blocks) v0 += g.sum_y;
  for (const auto& g : blocks) {
    const double c = lambda / (1.0 + lambda * g.n);
    a00 -= c * g.n * g.n;
    a01 -= c * g.n * g.sum_t;
    a11 -= c * g.sum_t * g.sum_t;
    v0 -= c * g.n * g.sum_y;
    v1 -= c * g.sum_t * g.sum_y;
    yy -= c * g.sum_y * g.sum_y;
    logdet += std::log1p(lambda * g.n);
  }
  GlsFit fit;
  const double det = a00 * a11 - a01 * a01;
  if (!(det > 0)) return fit;
  fit.b0 = (a11 * v0 - a01 * v1) / det;
  fit.b1 = (a00 * v1 - a01 * v0) / det;
  fit.rss = yy - (fit.b0 * v0 + fit.b1 * v1);
  if (!(fit.rss > 0)) return fit;
  fit.xtx[0][0] = a00;
  fit.xtx[0][1] = fit.xtx[1][0] = a01;
  fit.xtx[1][1] = a11;
  fit.reml = -0.5 * (logdet + std::log(det) + (n - 2.0) * std::log(fit.rss));
  return fit;
}

}  // namespace

double regularized_incomplete_beta(double a, double b, double x) {
  if (x <= 0.0) return 0.0;
  if (x >= 1.0) return 1.0;
  const double ln_front =
      std::lgamma(a + b) - std::lgamma(a) - std::lgamma(b) + a * std::log(x) + b * std::log1p(-x);
  const double front = std::exp(ln_front);
  if (x < (a + 1.0) / (a + b + 2.0)) return front * beta_continued_fraction(a, b, x) / a;
  return 1.0 - front * beta_continued_fraction(b, a, 1.0 - x) / b;
}

double student_t_two_sided_p(double t, double df) {
  if (std::isnan(t) || !(df > 0)) return kNaN;
  if (std::isinf(t)) return 0.0;
  return regularized_incomplete_beta(0.5 * df, 0.5, df / (df + t * t));
}

TestResult welch_test(std::span<const double> control, std::span<const double> treatment) {
  if (control.size() < 2 || treatment.size() < 2) return failure("Welch test needs >= 2 per group");
  const double n1 = static_cast<double>(control.size());
  const double n2 = static_cast<double>(treatment.size());
  const double q1 = variance(control) / n1;
  const double q2 = variance(treatment) / n2;
  const double se2 = q1 + q2;
  if (!(se2 > 0)) return failure("zero variance in both groups");
  TestResult r;
  r.estimate = mean(treatment) - mean(control);
  r.se = std::sqrt(se2);
  r.statistic = r.estimate / r.se;
  r.df = se2 * se2 / (q1 * q1 / (n1 - 1.0) + q2 * q2 / (n2 - 1.0));
  r.p = student_t_two_sided_p(r.statistic, r.df);
  r.ok = true;
  return r;
}

TestResult mixed_model_test(std::span<const double> y, std::span<const int> treated,
                            std::span<const int> gel) {
  const std::size_t n = y.size();
  if (treated.size() != n || gel.size() != n) return failure("mixed model: size mismatch");
  if (n < 4) return failure("mixed model needs >= 4 observations");

  std::map<int, GelBlock> by_gel;
  std::map<int, std::pair<bool, bool>> groups_in_gel;
  double sy2 = 0, syt = 0, st = 0, st2 = 0;
  for (std::size_t i = 0; i < n; ++i) {
    const double t = treated[i] ? 1.0 : 0.0;
    auto& g = by_gel[gel[i]];
    g.n += 1;
    g.sum_t += t;
    g.sum_y += y[i];
    sy2 += y[i] * y[i];
    syt += y[i] * t;
    st += t;
    st2 += t * t;
    auto& seen = groups_in_gel[gel[i]];
    (treated[i] ? seen.second : seen.first) = true;
  }
  if (st < 1 || st > static_cast<double>(n) - 1) return failure("mixed model: both groups required");
  std::vector<GelBlock> blocks;
  for (const auto& [id, b] : by_gel) blocks.push_back(b);
  const double dn = static_cast<double>(n);
  const auto n_gels = static_cast<double>(blocks.size());

  GlsFit best = gls(0.0, blocks, sy2, syt, st, st2, dn);
  if (blocks.size() >= 2) {
    // Profile the REML criterion over log(lambda): coarse grid, then golden section.
    constexpr double kLo = -12.0, kHi = 8.0;
    constexpr int kGrid = 41;
    double best_theta = kLo;
    double best_val = -INFINITY;
    for (int i = 0; i < kGrid; ++i) {
      const double theta = kLo + (kHi - kLo) * i / (kGrid - 1);
      const double v = gls(std::exp(theta), blocks, sy2, syt, st, st2, dn).reml;
      if (v > best_val) {
        best_val = v;
        best_theta = theta;
      }
    }
    const double step = (kHi - kLo) / (kGrid - 1);
    double a = best_theta - step, b = best_theta + step;
    const double phi = 0.5 * (std::sqrt(5.0) - 1.0);
    double c = b - phi * (b - a), d = a + phi * (b - a);
    auto f = [&](double th) { return gls(std::exp(th), blocks, sy2, syt, st, st2, dn).reml; };
    double fc = f(c), fd = f(d);
    for (int it = 0; it < 80 && (b - a) > 1e-10; ++it) {
      if (fc > fd) {
        b = d; d = c; fd = fc; c = b - phi * (b - a); fc = f(c);
      } else {
        a = c; c = d; fc = fd; d = a + phi * (b - a); fd = f(d);
      }
    }
    const GlsFit interior = gls(std::exp(0.5 * (a + b)), blocks, sy2, syt, st, st2, dn);
    if (interior.reml > best.reml) best = interior;
  }
  if (!std::isfinite(best.reml)) return failure("mixed model: degenerate design or zero residual");

  const double sigma2 = best.rss / (dn - 2.0);
  const double det = best.xtx[0][0] * best.xtx[1][1] - best.xtx[0][1] * best.xtx[1][0];
  const double var_b1 = sigma2 * best.xtx[0][0] / det;

  bool within = true;
  for (const auto& [id, seen] : groups_in_gel) within = within && seen.first && seen.second;
  double df = within ? dn - n_gels - 1.0 : n_gels - 2.0;
  if (blocks.size() < 2) df = dn - 2.0;
  if (df < 1.0) return failure("mixed model: not enough degrees of freedom");

  TestResult r;
  r.estimate = best.b1;
  r.se = std::sqrt(var_b1);
  r.statistic = r.estimate / r.se;
  r.df = df;
  r.p = student_t_two_sided_p(r.statistic, r.df);
  r.ok = std::isfinite(r.p);
  if (!r.ok) r.error = "mixed model: non-finite p-value";
  return r;
}

TestResult permutation_test(std::span<const double> control, std::span<const double> treatment,
                            std::size_t max_permutations, std::uint64_t seed) {
  const std::size_t nc = control.size(), nt = treatment.size();
  if (nc < 1 || nt < 1 || nc + nt < 3) return failure("permutation test needs data in both groups");
  std::vector<double> pooled(control.begin(), control.end());
  pooled.insert(pooled.end(), treatment.begin(), treatment.end());
  const std::size_t n = pooled.size();
  double total = 0.0, scale = 0.0;
  for (double v : pooled) {
    total += v;
    scale = std::max(scale, std::fabs(v));
  }
  const double dnt = static_cast<double>(nt), dnc = static_cast<double>(nc);
  auto diff = [&](double sum_t) { return sum_t / dnt - (total - sum_t) / dnc; };
  double obs_sum = 0.0;
  for (double v : treatment) obs_sum += v;
  const double observed = diff(obs_sum);
  const double tol = 1e-12 * std::max(1.0, scale);
  const double threshold = std::fabs(observed) - tol;

  TestResult r;
  r.estimate = observed;
  r.statistic = observed;
  const std::size_t combos = binomial_capped(n, nt, max_permutations + 1);
  if (combos <= max_permutations) {
    // Exact: enumerate every subset of size nt (lexicographic order).
    std::vector<char> mask(n, 0);
    std::fill(mask.end() - static_cast<std::ptrdiff_t>(nt), mask.end(), 1);
    std::size_t hits = 0, count = 0;
    do {
      double s = 0.0;
      for (std::size_t i = 0; i < n; ++i)
        if (mask[i]) s += pooled[i];
      if (std::fabs(diff(s)) >= threshold) ++hits;
      ++count;
    } while (std::next_permutation(mask.begin(), mask.end()));
    r.p = static_cast<double>(hits) / static_cast<double>(count);
    r.df = static_cast<double>(count);
  } else {
    Rng rng(seed);
    std::vector<std::size_t> idx(n);
    for (std::size_t i = 0; i < n; ++i) idx[i] = i;
    std::size_t hits = 0;
    for (std::size_t k = 0; k < max_permutations; ++k) {
      // Partial Fisher-Yates: the first nt indices form the relabelled treatment group.
      for (std::size_t i = 0; i < nt; ++i) {
        const auto j = i + static_cast<std::size_t>(rng.uniform_int(n - i));
        std::swap(idx[i], idx[j]);
      }
      double s = 0.0;
      for (std::size_t i = 0; i < nt; ++i) s += pooled[idx[i]];
      if (std::fabs(diff(s)) >= threshold) ++hits;
    }
    r.p = (1.0 + static_cast<double>(hits)) / (1.0 + static_cast<double>(max_permutations));
    r.df = static_cast<double>(max_permutations);
  }
  r.ok = true;
  return r;
}

}  // namespace blotverse
