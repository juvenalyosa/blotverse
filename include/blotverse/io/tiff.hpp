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
  std::string sha256;                  // fingerprint of the decoded pixel data
  std::vector<std::string> warnings;
};

struct RawImage {
  ImageU16 pixels;
  ImageMeta meta;
};

// Loads a single-channel TIFF. 16-bit is the supported input; 8-bit is
// accepted with a warning; JPEG (as file or as TIFF compression) is rejected.
RawImage read_tiff(const std::filesystem::path& path);

// Writes an uncompressed 16-bit greyscale TIFF.
void write_tiff16(const std::filesystem::path& path, const ImageU16& image);

// Wraps an in-memory image (simulator) with the same metadata a file would get.
RawImage make_raw_image(ImageU16 pixels, std::string label);

// SHA-256 of the pixel buffer (little-endian uint16) plus its dimensions.
std::string pixel_fingerprint(const ImageU16& image);

}  // namespace blotverse
