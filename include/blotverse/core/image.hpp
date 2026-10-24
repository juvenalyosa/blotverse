#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace blotverse {

// Dense row-major single-channel image.
template <typename T>
class Image {
 public:
  using value_type = T;

  Image() = default;
  Image(int width, int height, T fill = T{}) : width_(width), height_(height) {
    if (width < 0 || height < 0) throw std::invalid_argument("Image: negative size");
    data_.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), fill);
  }

  int width() const noexcept { return width_; }
  int height() const noexcept { return height_; }
  std::size_t size() const noexcept { return data_.size(); }
  bool empty() const noexcept { return data_.empty(); }

  bool contains(int x, int y) const noexcept {
    return x >= 0 && y >= 0 && x < width_ && y < height_;
  }

  T& operator()(int x, int y) noexcept { return data_[index(x, y)]; }
  const T& operator()(int x, int y) const noexcept { return data_[index(x, y)]; }

  std::vector<T>& data() noexcept { return data_; }
  const std::vector<T>& data() const noexcept { return data_; }

 private:
  std::size_t index(int x, int y) const noexcept {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
           static_cast<std::size_t>(x);
  }

  int width_ = 0;
  int height_ = 0;
  std::vector<T> data_;
};

using ImageU16 = Image<std::uint16_t>;
using ImageF = Image<float>;

ImageF to_float(const ImageU16& image);

// Half-open pixel rectangle [x0, x1) x [y0, y1).
struct Rect {
  int x0 = 0;
  int y0 = 0;
  int x1 = 0;
  int y1 = 0;

  int width() const noexcept { return x1 - x0; }
  int height() const noexcept { return y1 - y0; }
  bool empty() const noexcept { return x1 <= x0 || y1 <= y0; }
  bool intersects(const Rect& o) const noexcept {
    return x0 < o.x1 && o.x0 < x1 && y0 < o.y1 && o.y0 < y1;
  }
  bool inside(int width, int height) const noexcept {
    return x0 >= 0 && y0 >= 0 && x1 <= width && y1 <= height;
  }
  Rect clipped(int width, int height) const noexcept;
};

// Sum of pixel values inside a rectangle (clipped to the image).
double sum_rect(const ImageF& image, const Rect& rect);

}  // namespace blotverse
