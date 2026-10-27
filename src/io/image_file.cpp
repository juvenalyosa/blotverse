#include "blotverse/io/image_file.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>

namespace blotverse {
namespace {

std::string lower_extension(const std::filesystem::path& path) {
  std::string ext = path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return ext;
}

// Value of `attribute` in the first <element ...> tag (plain search: the file
// is mostly binary, so no regex over the whole buffer).
std::optional<std::string> xml_attribute(const std::string& text, const std::string& element,
                                         const std::string& attribute) {
  const std::size_t open = text.find("<" + element + " ");
  if (open == std::string::npos) return std::nullopt;
  const std::size_t close = text.find('>', open);
  const std::string tag = text.substr(open, close - open);
  const std::size_t at = tag.find(" " + attribute + "=\"");
  if (at == std::string::npos) return std::nullopt;
  const std::size_t start = at + attribute.size() + 3;
  return tag.substr(start, tag.find('"', start) - start);
}

}  // namespace

RawImage read_scn(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) throw std::runtime_error("image not found: " + path.string());
  const std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  if (bytes.rfind("MIME-Version", 0) != 0 || bytes.find("Image Lab") == std::string::npos)
    throw ImageRejected("not a Bio-Rad Image Lab .scn file: " + path.string());

  const auto width = xml_attribute(bytes, "size_pix", "width");
  const auto height = xml_attribute(bytes, "size_pix", "height");
  if (!width || !height) throw ImageRejected(".scn without image size: " + path.string());
  const int w = std::stoi(*width), h = std::stoi(*height);

  // The ImageData part: its MIME header ends with a blank line, then raw pixels.
  const std::size_t tag = bytes.find("Content-Description: ImageData");
  if (tag == std::string::npos) throw ImageRejected(".scn without ImageData part: " + path.string());
  const std::size_t length_at = bytes.rfind("Content-Length: ", tag);
  const std::size_t data = bytes.find("\r\n\r\n", tag);
  const std::size_t expected = static_cast<std::size_t>(w) * static_cast<std::size_t>(h) * 2;
  if (data == std::string::npos || length_at == std::string::npos ||
      std::stoull(bytes.substr(length_at + 16, 20)) != expected || data + 4 + expected > bytes.size())
    throw ImageRejected(".scn ImageData does not match a " + *width + "x" + *height +
                        " 16-bit image: " + path.string());

  RawImage raw;
  raw.meta.path = path.string();
  raw.meta.bits_per_sample = 16;
  raw.meta.detector_max = 65535;
  raw.meta.compression = "none (Image Lab .scn)";
  raw.pixels = ImageU16(w, h);
  const auto* p = reinterpret_cast<const unsigned char*>(bytes.data() + data + 4);
  for (std::size_t i = 0; i < static_cast<std::size_t>(w) * static_cast<std::size_t>(h); ++i)
    raw.pixels.data()[i] = static_cast<std::uint16_t>(p[2 * i] | (p[2 * i + 1] << 8));

  if (const auto exposure = xml_attribute(bytes, "exposure_time", "value"))
    raw.meta.exposure_s = std::stod(*exposure);
  analyse_pixel_depth(raw);
  raw.meta.sha256 = pixel_fingerprint(raw.pixels);
  return raw;
}

RawImage read_image(const std::filesystem::path& path) {
  const std::string ext = lower_extension(path);
  if (ext == ".scn") return read_scn(path);
  return read_tiff(path);  // .tif, .tiff, .gel and anything TIFF-shaped
}

std::string raw_acquisition_problem(const ImageMeta& meta) {
  if (meta.bits_per_sample == 8) return "8-bit file";
  if (meta.effective_bits == 8) return "upscaled 8-bit data in a 16-bit file";
  if (meta.colour_file) return "RGB export";
  if (meta.grey_levels < 128) return "only " + std::to_string(meta.grey_levels) + " grey levels";
  return {};
}

}  // namespace blotverse
