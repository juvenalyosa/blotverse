#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"
#include "blotverse/forensics/forensics.hpp"

namespace blotverse {

// Known-truth bench for the image-integrity screening: manipulations are
// injected into real raw acquisitions, and the screening must find them
// (sensitivity) without flagging anything else (false findings). Untouched
// images measure the false-alarm rate directly.

enum class Tamper { none, copy, copy_gain, copy_flip, copy_noise, cross_copy, splice, erase };
const char* tamper_label(Tamper t);
inline constexpr Tamper kTampers[] = {Tamper::none,       Tamper::copy,       Tamper::copy_gain, Tamper::copy_flip,
                                      Tamper::copy_noise, Tamper::cross_copy, Tamper::splice,    Tamper::erase};

struct ForensicsBenchConfig {
  std::vector<std::filesystem::path> images;
  std::size_t repeats = 5;  // trials per image and manipulation type
  std::uint64_t seed = 1;
  ForensicsOptions options;
};

struct ForensicsTrial {
  std::string image;
  std::string donor;          // cross_copy and splice: the other image
  Tamper tamper = Tamper::none;
  std::size_t repeat = 0;
  Rect truth;                 // pasted, erased or spliced region in the tampered image
  Rect source;                // copies: where the pasted pixels came from
  std::string detail;         // gain, orientation, noise level
  bool detected = false;
  std::size_t false_findings = 0;  // findings unrelated to the injected manipulation
  std::string false_kinds;         // e.g. "seam;flat"
};

struct ForensicsBenchResult {
  ForensicsOptions options;
  std::vector<ForensicsTrial> trials;
  double seconds = 0.0;
};

ForensicsBenchResult run_forensics_bench(const ForensicsBenchConfig& config,
                                         const std::function<void(std::size_t, std::size_t)>& progress = {});

// forensics_bench_trials.csv and forensics_bench.md (detection rate per
// manipulation with Wilson 95 % intervals; false findings per trial).
void write_forensics_bench(const std::filesystem::path& dir, const ForensicsBenchResult& result);

}  // namespace blotverse
