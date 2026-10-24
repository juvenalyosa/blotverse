#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

namespace blotverse {

// Hex-encoded SHA-256 digest (FIPS 180-4). Self-contained so the image
// fingerprint in the reproducibility file is identical on every platform.
std::string sha256_hex(std::span<const std::uint8_t> bytes);
std::string sha256_hex(std::string_view text);

}  // namespace blotverse
