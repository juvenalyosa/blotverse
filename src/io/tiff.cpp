#include "blotverse/io/tiff.hpp"

#include <tiffio.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <fstream>
#include <memory>

#include "blotverse/io/sha256.hpp"

namespace blotverse {
namespace {

struct TiffCloser {
  void operator()(TIFF* t) const { TIFFClose(t); }
};
using TiffHandle = std::unique_ptr<TIFF, TiffCloser>;

void silence_libtiff() {
  static const bool once = [] {
    TIFFSetWarningHandler(nullptr);
    TIFFSetErrorHandler(nullptr);
    return true;
  }();
  (void)once;
}

bool has_jpeg_magic(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  std::array<unsigned char, 3> magic{};
  in.read(reinterpret_cast<char*>(magic.data()), magic.size());
  return in.gcount() == 3 && magic[0] == 0xFF && magic[1] == 0xD8 && magic[2] == 0xFF;
}

std::string lower_extension(const std::filesystem::path& path) {
  std::string ext = path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return ext;
}

std::string compression_name(std::uint16_t c, bool& lossless) {
  lossless = true;
  switch (c) {
    case COMPRESSION_NONE: return "none";
    case COMPRESSION_LZW: return "lzw";
    case COMPRESSION_ADOBE_DEFLATE:
    case COMPRESSION_DEFLATE: return "deflate";
    case COMPRESSION_PACKBITS: return "packbits";
    case COMPRESSION_JPEG:
    case COMPRESSION_OJPEG: lossless = false; return "jpeg";
    default: lossless = false; return "unknown(" + std::to_string(c) + ")";
  }
}

}  // namespace

std::string pixel_fingerprint(const ImageU16& image) {
  std::vector<std::uint8_t> bytes;
  bytes.reserve(8 + image.size() * 2);
  for (int v : {image.width(), image.height()})
    for (int i = 0; i < 4; ++i) bytes.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
  for (std::uint16_t p : image.data()) {
    bytes.push_back(static_cast<std::uint8_t>(p & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(p >> 8));
  }
  return sha256_hex(bytes);
}

RawImage make_raw_image(ImageU16 pixels, std::string label) {
  RawImage raw{std::move(pixels), {}};
  raw.meta.path = std::move(label);
  analyse_pixel_depth(raw);
  raw.meta.sha256 = pixel_fingerprint(raw.pixels);
  return raw;
}

RawImage read_tiff(const std::filesystem::path& path) {
  silence_libtiff();
  if (!std::filesystem::exists(path)) throw std::runtime_error("image not found: " + path.string());

  const std::string ext = lower_extension(path);
  if (ext == ".jpg" || ext == ".jpeg" || has_jpeg_magic(path))
    throw ImageRejected("JPEG images are rejected (lossy compression destroys densitometry): " +
                        path.string());

  TiffHandle tif(TIFFOpen(path.string().c_str(), "r"));
  if (!tif) throw ImageRejected("not a readable TIFF file: " + path.string());

  std::uint32_t width = 0, height = 0;
  std::uint16_t bits = 0, samples = 1, compression = COMPRESSION_NONE;
  std::uint16_t sample_format = SAMPLEFORMAT_UINT, planar = PLANARCONFIG_CONTIG;
  TIFFGetField(tif.get(), TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif.get(), TIFFTAG_IMAGELENGTH, &height);
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_BITSPERSAMPLE, &bits);
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_SAMPLESPERPIXEL, &samples);
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_COMPRESSION, &compression);
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_SAMPLEFORMAT, &sample_format);
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_PLANARCONFIG, &planar);

  RawImage raw;
  raw.meta.path = path.string();
  raw.meta.compression = compression_name(compression, raw.meta.lossless);
  if (!raw.meta.lossless)
    throw ImageRejected("TIFF uses lossy or unsupported compression (" + raw.meta.compression +
                        "): " + path.string());
  std::uint16_t photometric = PHOTOMETRIC_MINISBLACK;
  TIFFGetFieldDefaulted(tif.get(), TIFFTAG_PHOTOMETRIC, &photometric);
  if (photometric == PHOTOMETRIC_SEPARATED)
    throw ImageRejected("CMYK file (a figure prepared for print, not an acquisition): " + path.string());
  if (samples != 1 && samples != 3 && samples != 4)
    throw ImageRejected("only greyscale images are supported in v1: " + path.string());
  if (samples > 1 && planar != PLANARCONFIG_CONTIG)
    throw ImageRejected("planar colour TIFFs are not supported: " + path.string());
  if (sample_format != SAMPLEFORMAT_UINT)
    throw ImageRejected("only unsigned integer pixels are supported: " + path.string());
  if (bits != 8 && bits != 16)
    throw ImageRejected("unsupported bit depth " + std::to_string(bits) + ": " + path.string());
  if (width == 0 || height == 0) throw ImageRejected("empty image: " + path.string());

  raw.meta.bits_per_sample = bits;
  raw.meta.detector_max = bits == 16 ? 65535u : 255u;
  if (bits == 8)
    raw.meta.warnings.push_back(
        "8-bit image: limited dynamic range; saturation and quantisation error are likely");

  raw.pixels = ImageU16(static_cast<int>(width), static_cast<int>(height));
  std::vector<std::uint8_t> line(static_cast<std::size_t>(TIFFScanlineSize(tif.get())));
  auto sample = [&](std::uint32_t index) -> std::uint16_t {
    if (bits == 8) return line[index];
    std::uint16_t word;
    std::memcpy(&word, line.data() + 2 * index, sizeof word);  // libtiff returns host order
    return word;
  };
  bool grey_rgb = true;  // colour files whose R, G and B are identical
  for (std::uint32_t y = 0; y < height; ++y) {
    if (TIFFReadScanline(tif.get(), line.data(), y, 0) < 0)
      throw std::runtime_error("failed to read TIFF row " + std::to_string(y) + ": " +
                               path.string());
    for (std::uint32_t x = 0; x < width; ++x) {
      const std::uint16_t v = sample(x * samples);
      if (samples > 1 && (sample(x * samples + 1) != v || sample(x * samples + 2) != v)) grey_rgb = false;
      raw.pixels(static_cast<int>(x), static_cast<int>(y)) = v;
    }
  }
  if (samples > 1) {
    if (!grey_rgb)
      throw ImageRejected("colour (pseudo-coloured) image: export the greyscale acquisition file instead: " +
                          path.string());
    raw.meta.warnings.push_back("RGB file with identical channels: read as greyscale");
  }
  analyse_pixel_depth(raw);
  raw.meta.sha256 = pixel_fingerprint(raw.pixels);
  return raw;
}

void analyse_pixel_depth(RawImage& raw) {
  std::vector<bool> seen(65536, false);
  bool multiples_of_257 = true;
  std::size_t levels = 0;
  for (std::uint16_t v : raw.pixels.data()) {
    if (!seen[v]) {
      seen[v] = true;
      ++levels;
    }
    if (v % 257 != 0) multiples_of_257 = false;
  }
  raw.meta.grey_levels = levels;
  if (raw.meta.bits_per_sample == 16 && (levels <= 256 || multiples_of_257)) {
    raw.meta.effective_bits = 8;
    raw.meta.warnings.push_back("16-bit file holds only " + std::to_string(levels) +
                                " grey levels: it is an upscaled 8-bit image, not a raw acquisition");
  } else {
    raw.meta.effective_bits = raw.meta.bits_per_sample;
  }
  if (levels < 128)
    raw.meta.warnings.push_back("only " + std::to_string(levels) +
                                " distinct grey levels: strong quantisation (processed or contrast-adjusted image?)");

  // Chemiluminescence/fluorescence acquisitions have a dark background and
  // bright bands; a bright median suggests an inverted (display) image.
  std::vector<std::uint16_t> sorted = raw.pixels.data();
  if (!sorted.empty()) {
    std::nth_element(sorted.begin(), sorted.begin() + static_cast<std::ptrdiff_t>(sorted.size() / 2), sorted.end());
    const double med = sorted[sorted.size() / 2];
    const auto [mn, mx] = std::minmax_element(raw.pixels.data().begin(), raw.pixels.data().end());
    // The median pixel is background: near the minimum for a normal blot,
    // near the maximum for an inverted one.
    raw.meta.appears_inverted = *mx > *mn && med - *mn > 0.75 * (*mx - *mn);
    if (raw.meta.appears_inverted)
      raw.meta.warnings.push_back(
          "bright background: the image looks inverted (dark bands); set 'invert: true' for this blot");
  }
}

void invert_image(RawImage& raw) {
  for (auto& v : raw.pixels.data()) v = static_cast<std::uint16_t>(raw.meta.detector_max - v);
  raw.meta.appears_inverted = !raw.meta.appears_inverted;
  raw.meta.warnings.push_back("image inverted on load (invert: true)");
}

void write_tiff16(const std::filesystem::path& path, const ImageU16& image) {
  silence_libtiff();
  TiffHandle tif(TIFFOpen(path.string().c_str(), "w"));
  if (!tif) throw std::runtime_error("cannot write TIFF: " + path.string());
  TIFFSetField(tif.get(), TIFFTAG_IMAGEWIDTH, static_cast<std::uint32_t>(image.width()));
  TIFFSetField(tif.get(), TIFFTAG_IMAGELENGTH, static_cast<std::uint32_t>(image.height()));
  TIFFSetField(tif.get(), TIFFTAG_BITSPERSAMPLE, 16);
  TIFFSetField(tif.get(), TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif.get(), TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif.get(), TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(tif.get(), TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif.get(), TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
  TIFFSetField(tif.get(), TIFFTAG_ROWSPERSTRIP, 1);
  TIFFSetField(tif.get(), TIFFTAG_SOFTWARE, "BlotVerse simulator");
  std::vector<std::uint16_t> row(static_cast<std::size_t>(image.width()));
  for (int y = 0; y < image.height(); ++y) {
    for (int x = 0; x < image.width(); ++x) row[static_cast<std::size_t>(x)] = image(x, y);
    if (TIFFWriteScanline(tif.get(), row.data(), static_cast<std::uint32_t>(y), 0) < 0)
      throw std::runtime_error("failed to write TIFF: " + path.string());
  }
}

}  // namespace blotverse
