#include "blotverse/core/image.hpp"

#include <algorithm>

namespace blotverse {

ImageF to_float(const ImageU16& image) {
  ImageF out(image.width(), image.height());
  std::transform(image.data().begin(), image.data().end(), out.data().begin(),
                 [](std::uint16_t v) { return static_cast<float>(v); });
  return out;
}

Rect Rect::clipped(int width, int height) const noexcept {
  return Rect{std::clamp(x0, 0, width), std::clamp(y0, 0, height), std::clamp(x1, 0, width),
              std::clamp(y1, 0, height)};
}

double sum_rect(const ImageF& image, const Rect& rect) {
  const Rect r = rect.clipped(image.width(), image.height());
  double total = 0.0;
  for (int y = r.y0; y < r.y1; ++y)
    for (int x = r.x0; x < r.x1; ++x) total += image(x, y);
  return total;
}

}  // namespace blotverse
