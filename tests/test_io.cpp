#include <doctest/doctest.h>
#include <tiffio.h>

#include <filesystem>
#include <fstream>

#include "blotverse/io/sha256.hpp"
#include "blotverse/io/tiff.hpp"

using namespace blotverse;
namespace fs = std::filesystem;

namespace {

fs::path temp_dir() {
  const fs::path d = fs::temp_directory_path() / "blotverse_tests";
  fs::create_directories(d);
  return d;
}

void write_tiff8(const fs::path& path, int w, int h, std::uint16_t compression = COMPRESSION_NONE) {
  TIFF* t = TIFFOpen(path.string().c_str(), "w");
  REQUIRE(t != nullptr);
  TIFFSetField(t, TIFFTAG_IMAGEWIDTH, w);
  TIFFSetField(t, TIFFTAG_IMAGELENGTH, h);
  TIFFSetField(t, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(t, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(t, TIFFTAG_COMPRESSION, compression);
  TIFFSetField(t, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(t, TIFFTAG_ROWSPERSTRIP, h);
  std::vector<std::uint8_t> row(static_cast<std::size_t>(w));
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) row[static_cast<std::size_t>(x)] = static_cast<std::uint8_t>(x + y);
    TIFFWriteScanline(t, row.data(), static_cast<std::uint32_t>(y), 0);
  }
  TIFFClose(t);
}

}  // namespace

TEST_CASE("sha256 matches FIPS test vectors") {
  CHECK(sha256_hex("") == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
  CHECK(sha256_hex("abc") == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
  CHECK(sha256_hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") ==
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
  CHECK(sha256_hex(std::string(1000, 'a')) ==
        "41edece42d63e8d9bf515a9ba6932e1c20cbc9f5a5d134645adb5db1b9737ea3");
}

TEST_CASE("16-bit TIFF round trip keeps pixels and fingerprint") {
  ImageU16 img(40, 30);
  for (int y = 0; y < 30; ++y)
    for (int x = 0; x < 40; ++x) img(x, y) = static_cast<std::uint16_t>(100 + x * 37 + y * 11);
  const fs::path p = temp_dir() / "roundtrip.tif";
  write_tiff16(p, img);
  const RawImage raw = read_tiff(p);
  CHECK(raw.meta.bits_per_sample == 16);
  CHECK(raw.meta.detector_max == 65535u);
  CHECK(raw.meta.warnings.empty());
  CHECK(raw.pixels.data() == img.data());
  CHECK(raw.meta.sha256 == pixel_fingerprint(img));
  CHECK(raw.meta.sha256 == make_raw_image(img, "mem").meta.sha256);
}

TEST_CASE("8-bit TIFF is accepted with a warning; lossless compression is fine") {
  const fs::path p = temp_dir() / "eight.tif";
  write_tiff8(p, 10, 4, COMPRESSION_LZW);
  const RawImage raw = read_tiff(p);
  CHECK(raw.meta.bits_per_sample == 8);
  CHECK(raw.meta.detector_max == 255u);
  CHECK(raw.meta.compression == "lzw");
  CHECK(raw.meta.warnings.front().find("8-bit") != std::string::npos);
  CHECK(raw.pixels(3, 2) == 5);
}

TEST_CASE("JPEG files are rejected") {
  const fs::path p = temp_dir() / "photo.tif";
  {
    std::ofstream out(p, std::ios::binary);
    const unsigned char magic[] = {0xFF, 0xD8, 0xFF, 0xE0, 0, 0};
    out.write(reinterpret_cast<const char*>(magic), sizeof magic);
  }
  CHECK_THROWS_AS(read_tiff(p), ImageRejected);
  std::ofstream(temp_dir() / "blot.jpg") << "x";
  CHECK_THROWS_AS(read_tiff(temp_dir() / "blot.jpg"), ImageRejected);
  CHECK_THROWS(read_tiff(temp_dir() / "missing.tif"));
}

namespace {
void write_rgb16(const fs::path& path, int w, int h, bool grey) {
  TIFF* t = TIFFOpen(path.string().c_str(), "w");
  REQUIRE(t != nullptr);
  TIFFSetField(t, TIFFTAG_IMAGEWIDTH, w);
  TIFFSetField(t, TIFFTAG_IMAGELENGTH, h);
  TIFFSetField(t, TIFFTAG_BITSPERSAMPLE, 16);
  TIFFSetField(t, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(t, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(t, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(t, TIFFTAG_ROWSPERSTRIP, h);
  std::vector<std::uint16_t> row(static_cast<std::size_t>(3 * w));
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const auto v = static_cast<std::uint16_t>(1000 + 13 * x + 7 * y);
      row[static_cast<std::size_t>(3 * x)] = v;
      row[static_cast<std::size_t>(3 * x + 1)] = grey ? v : static_cast<std::uint16_t>(v / 2);
      row[static_cast<std::size_t>(3 * x + 2)] = v;
    }
    TIFFWriteScanline(t, row.data(), static_cast<std::uint32_t>(y), 0);
  }
  TIFFClose(t);
}
}  // namespace

TEST_CASE("RGB files: identical channels read as greyscale, colour rejected") {
  write_rgb16(temp_dir() / "grey_rgb.tif", 30, 20, true);
  const RawImage raw = read_tiff(temp_dir() / "grey_rgb.tif");
  CHECK(raw.pixels(2, 1) == 1000 + 26 + 7);
  CHECK(raw.meta.warnings.front().find("identical channels") != std::string::npos);
  write_rgb16(temp_dir() / "colour.tif", 30, 20, false);
  CHECK_THROWS_WITH_AS(read_tiff(temp_dir() / "colour.tif"), doctest::Contains("pseudo-coloured"), ImageRejected);
}

TEST_CASE("upscaled 8-bit data and inverted images are flagged") {
  // Light background (level 249) with dark band pixels (levels 200-248), x257.
  ImageU16 img(64, 64, static_cast<std::uint16_t>(257 * 249));
  for (int y = 20; y < 30; ++y)
    for (int x = 0; x < 64; ++x) img(x, y) = static_cast<std::uint16_t>(257 * (200 + (x + y) % 49));
  const fs::path p = temp_dir() / "upscaled.tif";
  write_tiff16(p, img);
  RawImage raw = read_tiff(p);
  CHECK(raw.meta.effective_bits == 8);
  CHECK(raw.meta.grey_levels == 50);
  CHECK(raw.meta.appears_inverted);  // bright background
  invert_image(raw);
  CHECK(raw.pixels(0, 0) == 65535 - 257 * 249);
  CHECK_FALSE(raw.meta.appears_inverted);
}
