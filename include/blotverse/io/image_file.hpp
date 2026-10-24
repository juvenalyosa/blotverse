#pragma once

#include <filesystem>

#include "blotverse/io/tiff.hpp"

namespace blotverse {

// Bio-Rad Image Lab .scn (ChemiDoc and other Bio-Rad imagers): a MIME
// multipart container whose "ImageData" part holds uncompressed little-endian
// uint16 pixels; the XML parts carry size, exposure and binning.
RawImage read_scn(const std::filesystem::path& path);

// Reads any supported acquisition file, chosen by extension:
// .tif/.tiff, .gel (ImageQuant/Molecular Dynamics TIFF) and .scn.
RawImage read_image(const std::filesystem::path& path);

}  // namespace blotverse
