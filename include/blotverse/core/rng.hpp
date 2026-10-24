#pragma once

#include <cstdint>
#include <random>
#include <vector>

namespace blotverse {

// Deterministic random number generator.
//
// The engine (mt19937_64) is fully specified by the C++ standard, but the
// std::*_distribution classes are not, so results would differ between
// libc++ and libstdc++. All distributions are therefore implemented here, so
// the engine stream is identical everywhere; draws that pass through libm
// (exp, log, sqrt) are bit-identical on the same platform and compiler.
class Rng {
 public:
  explicit Rng(std::uint64_t seed) : engine_(seed) {}

  std::uint64_t next_u64() { return engine_(); }
  double uniform();                              // [0, 1)
  double uniform(double lo, double hi);          // [lo, hi)
  std::uint64_t uniform_int(std::uint64_t n);    // [0, n)
  double normal();                               // N(0, 1)
  double normal(double mean, double sd) { return mean + sd * normal(); }
  double lognormal_cv(double cv);                // mean 1, given coefficient of variation
  std::uint64_t poisson(double mean);

  template <typename T>
  void shuffle(std::vector<T>& v) {
    for (std::size_t i = v.size(); i > 1; --i) {
      const auto j = static_cast<std::size_t>(uniform_int(i));
      std::swap(v[i - 1], v[j]);
    }
  }

 private:
  std::mt19937_64 engine_;
  bool has_spare_ = false;
  double spare_ = 0.0;
};

// Derive an independent seed for a sub-stream (splitmix64 mixing).
std::uint64_t derive_seed(std::uint64_t base, std::uint64_t stream);

}  // namespace blotverse
