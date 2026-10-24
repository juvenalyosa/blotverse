#pragma once

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include "blotverse/core/image.hpp"

namespace blotverse {

// Raised for files BlotVerse refuses to analyse (JPEG, colour, float, ...).
class ImageRejected : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

struct ImageMeta {
  std::string path;
  int bits_per_sample = 16;
  std::uint32_t detector_max = 65535;  // value that marks a saturated pixel
  std::string compression = "none";
  bool lossless = true;
  int effective_bits = 16;             // 8 when a 16-bit file holds upscaled 8-bit data
  std::size_t grey_levels = 0;         // distinct pixel values in the image
  bool appears_inverted = false;       // bright background (dark bands)
  double exposure_s = -1.0;            // from the acquisition metadata, if known
  std::string sha256;                  // fingerprint of the decoded pixel data
  std::vector<std::string> warnings;
};

struct RawImage {
  ImageU16 pixels;
  ImageMeta meta;
};

// Loads a greyscale TIFF. 16-bit is the supported input; 8-bit is accepted
// with a warning; RGB files are accepted only when R = G = B; JPEG (as file or
// as TIFF compression) and pseudo-coloured images are rejected. Upscaled 8-bit
// data, strong quantisation and inverted images are flagged.
RawImage read_tiff(const std::filesystem::path& path);

// Grey-level census and inversion check (called by read_tiff).
void analyse_pixel_depth(RawImage& raw);

// value -> detector_max - value, for images with dark bands on a light background.
void invert_image(RawImage& raw);

// Writes an uncompressed 16-bit greyscale TIFF.
void write_tiff16(const std::filesystem::path& path, const ImageU16& image);

// Wraps an in-memory image (simulator) with the same metadata a file would get.
RawImage make_raw_image(ImageU16 pixels, std::string label);

// SHA-256 of the pixel buffer (little-endian uint16) plus its dimensions.
std::string pixel_fingerprint(const ImageU16& image);

}  // namespace blotverse
