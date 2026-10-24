#include "blotverse/core/rng.hpp"

#include <cmath>
#include <numbers>

namespace blotverse {

double Rng::uniform() {
  return static_cast<double>(engine_() >> 11) * 0x1.0p-53;
}

double Rng::uniform(double lo, double hi) { return lo + (hi - lo) * uniform(); }

std::uint64_t Rng::uniform_int(std::uint64_t n) {
  if (n == 0) return 0;
  // Rejection sampling removes modulo bias.
  const std::uint64_t limit = UINT64_MAX - UINT64_MAX % n;
  std::uint64_t x;
  do {
    x = engine_();
  } while (x >= limit);
  return x % n;
}

double Rng::normal() {
  if (has_spare_) {
    has_spare_ = false;
    return spare_;
  }
  // Marsaglia polar method.
  double u, v, s;
  do {
    u = 2.0 * uniform() - 1.0;
    v = 2.0 * uniform() - 1.0;
    s = u * u + v * v;
  } while (s >= 1.0 || s == 0.0);
  const double m = std::sqrt(-2.0 * std::log(s) / s);
  spare_ = v * m;
  has_spare_ = true;
  return u * m;
}

double Rng::lognormal_cv(double cv) {
  if (cv <= 0.0) return 1.0;
  const double sigma2 = std::log1p(cv * cv);
  return std::exp(normal(-0.5 * sigma2, std::sqrt(sigma2)));
}

std::uint64_t Rng::poisson(double mean) {
  if (mean <= 0.0) return 0;
  if (mean < 10.0) {
    // Knuth multiplication method.
    const double limit = std::exp(-mean);
    std::uint64_t k = 0;
    double prod = uniform();
    while (prod > limit) {
      ++k;
      prod *= uniform();
    }
    return k;
  }
  // Hormann (1993) PTRS transformed rejection, as used by NumPy.
  const double slam = std::sqrt(mean);
  const double loglam = std::log(mean);
  const double b = 0.931 + 2.53 * slam;
  const double a = -0.059 + 0.02483 * b;
  const double invalpha = 1.1239 + 1.1328 / (b - 3.4);
  const double vr = 0.9277 - 3.6224 / (b - 2.0);
  while (true) {
    const double u = uniform() - 0.5;
    const double v = uniform();
    const double us = 0.5 - std::fabs(u);
    const double k = std::floor((2.0 * a / us + b) * u + mean + 0.43);
    if (us >= 0.07 && v <= vr) return static_cast<std::uint64_t>(k);
    if (k < 0.0 || (us < 0.013 && v > us)) continue;
    if (std::log(v) + std::log(invalpha) - std::log(a / (us * us) + b) <=
        -mean + k * loglam - std::lgamma(k + 1.0))
      return static_cast<std::uint64_t>(k);
  }
}

std::uint64_t derive_seed(std::uint64_t base, std::uint64_t stream) {
  std::uint64_t z = base + 0x9E3779B97F4A7C15ULL * (stream + 1);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}

}  // namespace blotverse
