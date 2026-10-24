#include "blotverse/imaging/background.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace blotverse {
namespace {

struct RollingBall {
  std::vector<float> z;
  int width = 0;
  int shrink_factor = 1;
};

RollingBall make_ball(double radius) {
  RollingBall ball;
  int arc_trim_per;
  if (radius <= 10) {
    ball.shrink_factor = 1;
    arc_trim_per = 24;
  } else if (radius <= 30) {
    ball.shrink_factor = 2;
    arc_trim_per = 24;
  } else if (radius <= 100) {
    ball.shrink_factor = 4;
    arc_trim_per = 32;
  } else {
    ball.shrink_factor = 8;
    arc_trim_per = 40;
  }
  double small_radius = radius / ball.shrink_factor;
  if (small_radius < 1) small_radius = 1;
  const double rsquare = small_radius * small_radius;
  const int xtrim = static_cast<int>(arc_trim_per * small_radius) / 100;
  // Java Math.round: floor(x + 0.5).
  const int half_width = static_cast<int>(std::floor(small_radius - xtrim + 0.5));
  ball.width = 2 * half_width + 1;
  ball.z.resize(static_cast<std::size_t>(ball.width) * static_cast<std::size_t>(ball.width));
  for (int y = 0, p = 0; y < ball.width; ++y)
    for (int x = 0; x < ball.width; ++x, ++p) {
      const int xv = x - half_width;
      const int yv = y - half_width;
      const double temp = rsquare - xv * xv - yv * yv;
      ball.z[static_cast<std::size_t>(p)] = temp > 0.0 ? static_cast<float>(std::sqrt(temp)) : 0.0f;
    }
  return ball;
}

// 3-point running mean along one line with edge replication (ImageJ filter3).
void filter3_mean(std::vector<float>& px, int length, std::size_t pixel0, std::size_t inc) {
  float v3 = px[pixel0];
  float v2 = v3;
  std::size_t p = pixel0;
  for (int i = 0; i < length; ++i, p += inc) {
    const float v1 = v2;
    v2 = v3;
    if (i < length - 1) v3 = px[p + inc];
    px[p] = (v1 + v2 + v3) * 0.33333333f;
  }
}

void filter3x3_mean(ImageF& img) {
  auto& px = img.data();
  const auto w = static_cast<std::size_t>(img.width());
  for (int y = 0; y < img.height(); ++y) filter3_mean(px, img.width(), static_cast<std::size_t>(y) * w, 1);
  for (int x = 0; x < img.width(); ++x) filter3_mean(px, img.height(), static_cast<std::size_t>(x), w);
}

ImageF shrink_image(const ImageF& img, int factor) {
  const int sw = (img.width() + factor - 1) / factor;
  const int sh = (img.height() + factor - 1) / factor;
  ImageF small(sw, sh);
  for (int ys = 0; ys < sh; ++ys)
    for (int xs = 0; xs < sw; ++xs) {
      float mn = std::numeric_limits<float>::max();
      for (int j = 0, y = factor * ys; j < factor && y < img.height(); ++j, ++y)
        for (int k = 0, x = factor * xs; k < factor && x < img.width(); ++k, ++x)
          mn = std::min(mn, img(x, y));
      small(xs, ys) = mn;
    }
  return small;
}

// The ball is rolled under the surface; every pixel is raised to the highest
// ball surface that touches it. Equivalent to a grey-scale opening with the
// ball as structuring element, written as in ImageJ for bit-level parity.
void roll_ball(const RollingBall& ball, ImageF& img) {
  const int width = img.width();
  const int height = img.height();
  auto& pixels = img.data();
  const int ball_width = ball.width;
  const int radius = ball_width / 2;
  std::vector<float> cache(static_cast<std::size_t>(width) * static_cast<std::size_t>(ball_width));

  for (int y = -radius; y < height + radius; ++y) {
    const int next_line_in_cache = (y + radius) % ball_width;
    const int next_line_to_read = y + radius;
    if (next_line_to_read < height && next_line_to_read >= 0) {
      std::copy_n(pixels.begin() + static_cast<std::ptrdiff_t>(next_line_to_read) * width, width,
                  cache.begin() + static_cast<std::ptrdiff_t>(next_line_in_cache) * width);
      std::fill_n(pixels.begin() + static_cast<std::ptrdiff_t>(next_line_to_read) * width, width,
                  -std::numeric_limits<float>::max());
    }
    const int y0 = std::max(y - radius, 0);
    const int y_ball0 = y0 - y + radius;
    const int yend = std::min(y + radius, height - 1);
    for (int x = -radius; x < width + radius; ++x) {
      float z = std::numeric_limits<float>::max();
      const int x0 = std::max(x - radius, 0);
      const int x_ball0 = x0 - x + radius;
      const int xend = std::min(x + radius, width - 1);
      for (int yp = y0, yb = y_ball0; yp <= yend; ++yp, ++yb) {
        std::size_t cp = static_cast<std::size_t>(yp % ball_width) * static_cast<std::size_t>(width) +
                         static_cast<std::size_t>(x0);
        std::size_t bp = static_cast<std::size_t>(x_ball0 + yb * ball_width);
        for (int xp = x0; xp <= xend; ++xp, ++cp, ++bp) z = std::min(z, cache[cp] - ball.z[bp]);
      }
      for (int yp = y0, yb = y_ball0; yp <= yend; ++yp, ++yb) {
        std::size_t p = static_cast<std::size_t>(yp) * static_cast<std::size_t>(width) +
                        static_cast<std::size_t>(x0);
        std::size_t bp = static_cast<std::size_t>(x_ball0 + yb * ball_width);
        for (int xp = x0; xp <= xend; ++xp, ++p, ++bp) pixels[p] = std::max(pixels[p], z + ball.z[bp]);
      }
    }
  }
}

void make_interpolation_arrays(std::vector<int>& indices, std::vector<float>& weights, int length,
                               int small_length, int factor) {
  for (int i = 0; i < length; ++i) {
    int small_index = (i - factor / 2) / factor;  // truncation toward zero, as in Java
    if (small_index >= small_length - 1) small_index = small_length - 2;
    indices[static_cast<std::size_t>(i)] = small_index;
    const float distance = (static_cast<float>(i) + 0.5f) / static_cast<float>(factor) -
                           (static_cast<float>(small_index) + 0.5f);
    weights[static_cast<std::size_t>(i)] = 1.0f - distance;
  }
}

void enlarge_image(const ImageF& small, ImageF& out, int factor) {
  const int width = out.width();
  const int height = out.height();
  const int sw = small.width();
  const auto& sp = small.data();
  std::vector<int> xi(static_cast<std::size_t>(width)), yi(static_cast<std::size_t>(height));
  std::vector<float> xw(static_cast<std::size_t>(width)), yw(static_cast<std::size_t>(height));
  make_interpolation_arrays(xi, xw, width, sw, factor);
  make_interpolation_arrays(yi, yw, height, small.height(), factor);

  auto interpolate_line = [&](std::vector<float>& line, int small_row) {
    const std::size_t base = static_cast<std::size_t>(small_row) * static_cast<std::size_t>(sw);
    for (int x = 0; x < width; ++x) {
      const auto ux = static_cast<std::size_t>(x);
      const auto ix = static_cast<std::size_t>(xi[ux]);
      line[ux] = sp[base + ix] * xw[ux] + sp[base + ix + 1] * (1.0f - xw[ux]);
    }
  };

  std::vector<float> line0(static_cast<std::size_t>(width)), line1(static_cast<std::size_t>(width));
  interpolate_line(line1, 0);
  int y_small_line0 = -1;
  for (int y = 0; y < height; ++y) {
    const auto uy = static_cast<std::size_t>(y);
    if (y_small_line0 < yi[uy]) {
      std::swap(line0, line1);
      ++y_small_line0;
      interpolate_line(line1, yi[uy] + 1);
    }
    const float weight = yw[uy];
    for (int x = 0; x < width; ++x) {
      const auto ux = static_cast<std::size_t>(x);
      out(x, y) = line0[ux] * weight + line1[ux] * (1.0f - weight);
    }
  }
}

}  // namespace

ImageF rolling_ball_background(const ImageF& image, double radius, bool presmooth) {
  if (radius <= 0) throw std::invalid_argument("rolling ball radius must be positive");
  if (image.empty()) return image;
  const RollingBall ball = make_ball(radius);
  ImageF background = image;
  if (presmooth) filter3x3_mean(background);
  // Enlarging needs at least two rows/columns in the shrunken image.
  const bool shrink = ball.shrink_factor > 1 && image.width() > ball.shrink_factor &&
                      image.height() > ball.shrink_factor;
  if (shrink) {
    ImageF small = shrink_image(background, ball.shrink_factor);
    roll_ball(ball, small);
    enlarge_image(small, background, ball.shrink_factor);
  } else {
    roll_ball(ball, background);
  }
  return background;
}

ImageF subtract(const ImageF& image, const ImageF& background) {
  if (image.width() != background.width() || image.height() != background.height())
    throw std::invalid_argument("subtract: size mismatch");
  ImageF out(image.width(), image.height());
  for (std::size_t i = 0; i < image.size(); ++i) out.data()[i] = image.data()[i] - background.data()[i];
  return out;
}

}  // namespace blotverse
