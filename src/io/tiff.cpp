#include "blotverse/io/tiff.hpp"

#include <tiffio.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
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

// IEEE 754 half precision to double.
double half_to_float(std::uint16_t h) {
  const int sign = (h >> 15) & 1, exponent = (h >> 10) & 0x1F, mantissa = h & 0x3FF;
  double v;
  if (exponent == 0) v = std::ldexp(mantissa, -24);                       // subnormal
  else if (exponent == 31) v = mantissa ? std::nan("") : INFINITY;        // inf / NaN
  else v = std::ldexp(mantissa + 1024, exponent - 25);
  return sign ? -v : v;
}

// Whole image (first IFD) as row-major bytes, from strips or tiles.
std::vector<std::uint8_t> read_pixel_bytes(TIFF* tif, std::uint32_t width, std::uint32_t height,
                                           std::size_t bytes_per_pixel, const std::filesystem::path& path) {
  std::vector<std::uint8_t> out(static_cast<std::size_t>(width) * height * bytes_per_pixel);
  const std::size_t row_bytes = static_cast<std::size_t>(width) * bytes_per_pixel;
  if (!TIFFIsTiled(tif)) {
    std::vector<std::uint8_t> line(static_cast<std::size_t>(TIFFScanlineSize(tif)));
    for (std::uint32_t y = 0; y < height; ++y) {
      if (TIFFReadScanline(tif, line.data(), y, 0) < 0)
        throw std::runtime_error("failed to read TIFF row " + std::to_string(y) + ": " + path.string());
      std::memcpy(out.data() + y * row_bytes, line.data(), row_bytes);
    }
    return out;
  }
  std::uint32_t tw = 0, th = 0;
  TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
  TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
  std::vector<std::uint8_t> tile(static_cast<std::size_t>(TIFFTileSize(tif)));
  for (std::uint32_t ty = 0; ty < height; ty += th)
    for (std::uint32_t tx = 0; tx < width; tx += tw) {
      if (TIFFReadTile(tif, tile.data(), tx, ty, 0, 0) < 0)
        throw std::runtime_error("failed to read TIFF tile: " + path.string());
      const std::uint32_t cols = std::min(tw, width - tx), rows = std::min(th, height - ty);
      for (std::uint32_t r = 0; r < rows; ++r)
        std::memcpy(out.data() + (ty + r) * row_bytes + tx * bytes_per_pixel,
                    tile.data() + static_cast<std::size_t>(r) * tw * bytes_per_pixel,
                    static_cast<std::size_t>(cols) * bytes_per_pixel);
    }
  return out;
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
  // Molecular Dynamics / ImageQuant .gel: MD FileTag (33445) = 2 means the
  // pixels are square-root encoded, which densitometry must not read as linear.
  if (const TIFFField* md = TIFFFindField(tif.get(), 33445, TIFF_ANY)) {
    std::uint32_t count = 0;
    void* value = nullptr;
    if (TIFFFieldPassCount(md) ? TIFFGetField(tif.get(), 33445, &count, &value)
                               : TIFFGetField(tif.get(), 33445, &value)) {
      std::uint32_t code = 0;
      if (value && TIFFFieldDataType(md) == TIFF_LONG) std::memcpy(&code, value, sizeof code);
      else if (value && TIFFFieldDataType(md) == TIFF_SHORT) code = *static_cast<std::uint16_t*>(value);
      if (code == 2)
        throw ImageRejected("square-root encoded .gel file; export a linear image instead: " + path.string());
    }
  }
  if (samples != 1 && samples != 3 && samples != 4)
    throw ImageRejected("only greyscale images are supported in v1: " + path.string());
  if (samples > 1 && planar != PLANARCONFIG_CONTIG)
    throw ImageRejected("planar colour TIFFs are not supported: " + path.string());
  const bool is_float = sample_format == SAMPLEFORMAT_IEEEFP;
  if (sample_format != SAMPLEFORMAT_UINT && !is_float)
    throw ImageRejected("only unsigned integer or floating-point pixels are supported: " + path.string());
  if (is_float ? (bits != 16 && bits != 32) : (bits != 8 && bits != 16))
    throw ImageRejected("unsupported bit depth " + std::to_string(bits) + ": " + path.string());
  if (is_float && samples != 1)
    throw ImageRejected("floating-point colour images are not supported: " + path.string());
  if (width == 0 || height == 0) throw ImageRejected("empty image: " + path.string());

  raw.meta.bits_per_sample = bits;
  raw.meta.detector_max = bits == 8 ? 255u : 65535u;
  if (bits == 8)
    raw.meta.warnings.push_back(
        "8-bit image: limited dynamic range; saturation and quantisation error are likely");

  const std::size_t bytes_per_sample = bits / 8;
  const std::vector<std::uint8_t> buffer =
      read_pixel_bytes(tif.get(), width, height, bytes_per_sample * samples, path);
  auto sample = [&](std::size_t index) -> double {  // index in samples, libtiff returns host order
    const std::uint8_t* at = buffer.data() + index * bytes_per_sample;
    if (!is_float) {
      if (bits == 8) return *at;
      std::uint16_t w;
      std::memcpy(&w, at, sizeof w);
      return w;
    }
    if (bits == 16) {
      std::uint16_t h;
      std::memcpy(&h, at, sizeof h);
      return half_to_float(h);
    }
    float f;
    std::memcpy(&f, at, sizeof f);
    return f;
  };

  raw.pixels = ImageU16(static_cast<int>(width), static_cast<int>(height));
  const std::size_t n = static_cast<std::size_t>(width) * height;
  double scale = 1.0;
  if (is_float) {
    // Floating-point acquisitions (LI-COR Image Studio) are rescaled so their
    // maximum maps to 65535; ratios between bands are unchanged.
    double mx = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
      const double v = sample(i);
      if (std::isfinite(v)) mx = std::max(mx, v);
    }
    scale = mx > 0 ? 65535.0 / mx : 1.0;
    raw.meta.float_scale = scale;
    raw.meta.detector_max = 65536u;  // no pixel value can mark saturation
    raw.meta.warnings.push_back("floating-point image rescaled to 16-bit (x" + std::to_string(scale) +
                                "); detector saturation cannot be checked from pixel values");
  }
  bool grey_rgb = true;  // colour files whose R, G and B are identical
  for (std::size_t i = 0; i < n; ++i) {
    const double v = sample(i * samples);
    if (samples > 1 && (sample(i * samples + 1) != v || sample(i * samples + 2) != v)) grey_rgb = false;
    const double scaled = std::isfinite(v) ? std::clamp(std::round(v * scale), 0.0, 65535.0) : 0.0;
    raw.pixels.data()[i] = static_cast<std::uint16_t>(scaled);
  }
  if (samples > 1) {
    if (!grey_rgb)
      throw ImageRejected("colour (pseudo-coloured) image: export the greyscale acquisition file instead: " +
                          path.string());
    raw.meta.colour_file = true;
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
  const auto top = static_cast<std::uint16_t>(std::min<std::uint32_t>(raw.meta.detector_max, 65535u));
  for (auto& v : raw.pixels.data()) v = static_cast<std::uint16_t>(top - v);
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
