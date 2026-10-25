#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"
#include "blotverse/io/tiff.hpp"

namespace blotverse {

// Image-integrity screening of raw blot acquisitions. Every finding is a
// region to review by a person, never evidence of misconduct on its own:
// legitimate operations (a declared reused loading control, membrane edges,
// background-clipped exports) can produce findings too.
//
// Duplicated regions are found on the noise residual, not on the signal: two
// genuine bands of the same shape correlate strongly in intensity but carry
// independent noise, while a copied region carries the noise of its source.
// The residual is the image minus a separable 5-point quadratic
// Savitzky-Golay smoothing, which removes band shapes up to third order in each
// direction and commutes with gain and offset changes.

struct ForensicsOptions {
  int patch = 12;              // side of the square patches compared (pixels)
  // Candidate pairs need this residual correlation (the search statistic).
  double min_ncc = 0.5;
  // Digital copy, the decisive test: after a least-squares brightness/contrast
  // fit, the raw pixels of the two patches differ (beyond rounding) by less
  // than this fraction of the camera noise. Separate regions and separate
  // captures each carry their own noise and differ by about sqrt(2) x noise;
  // frames of one cumulative acquisition share most photons and differ by
  // 0.2-0.5 x noise (seen in development data); a digital copy differs only by rounding.
  double copy_noise_ratio = 0.1;
  // Same scene (cross-image only): another capture of the same membrane shares
  // its fine texture and fixed-pattern noise but not its photon noise. Reported
  // when correlated patches cover this fraction of the smaller image.
  double scene_fraction = 0.1;  // of the smaller image's area
  std::size_t min_patches = 16;  // accepted patch positions before a region is reported
  int hash_tables = 8;         // locality-sensitive hash tables used to find candidate pairs
  int hash_bits = 24;          // residual signs per hash key
  std::size_t max_bucket = 64;   // equal-key runs longer than this are skipped (degenerate)
  bool flips = true;           // also search horizontally / vertically flipped and 180-degree copies
  bool cross = true;           // compare every pair of images, not only each image with itself
  // Seams: a straight, sharp step in the background along a column or row.
  int seam_segment = 64;       // rows (or columns) per tested segment
  double seam_z = 10.0;        // robust z of the step against the other columns
  int seam_min_length = 128;   // consecutive flagged pixels along the line
  double seam_share_lo = 0.8;  // step across the boundary / step across 6 pixels: 1 for a seam,
  double seam_share_hi = 1.25; // about 0.2-0.6 for a blurred physical edge, about 3 for a bad column
  // Flat regions: windows (half-overlapping) whose residual noise is far below
  // that of the image's quietest decile of windows.
  int flat_window = 8;
  double flat_ratio = 0.25;
  double min_flat_noise_quanta = 2.0;  // flat test skipped when typical noise is below this
  std::uint64_t seed = 1;
  unsigned threads = 0;
};

enum class Orientation { identity, flip_h, flip_v, rotate_180 };
const char* orientation_label(Orientation o);

enum class DuplicateKind { copy, same_scene };
const char* duplicate_kind_label(DuplicateKind k);

struct DuplicateRegion {
  DuplicateKind kind = DuplicateKind::copy;
  std::size_t target_image = 0;
  std::size_t source_image = 0;  // equal to target_image: copy within one image
  Orientation orientation = Orientation::identity;
  Rect target;                   // bounding box of the accepted patches
  Rect source;
  std::size_t patches = 0;       // accepted patch positions
  std::size_t area_px = 0;       // target pixels covered by accepted patches
  double mean_ncc = 0.0;
  double error_noise = 0.0;      // copies: mean raw fit error / camera noise
  double sharpness = 0.0;        // correlation at the match minus the best one pixel away (sampled mean)
  double identical_fraction = 0.0;  // covered pixels whose raw value equals the mapped source pixel
  double gain = 1.0;             // least squares: target = gain * source + offset (raw values)
  double offset = 0.0;
};

struct SeamLine {
  std::size_t image = 0;
  bool vertical = true;  // a column boundary (between position - 1 and position); else a row boundary
  int position = 0;
  int start = 0;         // extent along the line [start, end)
  int end = 0;
  double step = 0.0;     // median step across the boundary (counts)
  double step_noise = 0.0;  // step / pixel noise
  double z = 0.0;        // largest robust z among the segments
  double share = 0.0;    // step across the boundary / step across 6 pixels
};

struct FlatRegion {
  std::size_t image = 0;
  Rect rect;
  double noise = 0.0;           // residual noise inside (counts)
  double typical_noise = 0.0;   // residual noise of the image's quietest decile of windows
};

struct ForensicsImage {
  std::string name;
  std::string sha256;
  int width = 0;
  int height = 0;
  int effective_bits = 16;
  std::size_t grey_levels = 0;
  double residual_noise = 0.0;  // quietest-decile window residual noise
  bool flat_tested = true;
  std::size_t valid_patches = 0;
  std::vector<std::string> notes;
};

struct ForensicsResult {
  ForensicsOptions options;
  std::vector<ForensicsImage> images;
  std::vector<std::pair<std::size_t, std::size_t>> identical_files;  // same pixel fingerprint
  std::vector<DuplicateRegion> duplicates;
  std::vector<SeamLine> seams;
  std::vector<FlatRegion> flats;
  double seconds = 0.0;
};

// Noise residual: image minus separable 5-point quadratic Savitzky-Golay smoothing.
ImageF noise_residual(const ImageU16& image);

ForensicsResult analyze_forensics(const std::vector<RawImage>& images, const std::vector<std::string>& names,
                                  const ForensicsOptions& options);

// forensics.md, forensics_duplicates.csv, forensics_seams.csv, forensics_flat.csv and one
// 16-bit mask TIFF per image with findings (region number at each flagged pixel).
void write_forensics_outputs(const std::filesystem::path& dir, const ForensicsResult& result,
                             const std::vector<RawImage>& images);

}  // namespace blotverse
