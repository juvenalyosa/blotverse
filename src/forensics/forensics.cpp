#include "blotverse/forensics/forensics.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <iomanip>
#include <map>
#include <mutex>
#include <set>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_map>

#include "blotverse/core/numeric.hpp"
#include "blotverse/core/parallel.hpp"
#include "blotverse/core/rng.hpp"
#include "blotverse/io/tiff.hpp"

namespace blotverse {

const char* orientation_label(Orientation o) {
  switch (o) {
    case Orientation::identity: return "identity";
    case Orientation::flip_h: return "flip_horizontal";
    case Orientation::flip_v: return "flip_vertical";
    case Orientation::rotate_180: return "rotate_180";
  }
  return "?";
}

const char* duplicate_kind_label(DuplicateKind k) {
  return k == DuplicateKind::copy ? "copy" : "same_scene";
}

namespace {

bool flips_x(Orientation o) { return o == Orientation::flip_h || o == Orientation::rotate_180; }
bool flips_y(Orientation o) { return o == Orientation::flip_v || o == Orientation::rotate_180; }

int reflect(int i, int n) {
  if (n == 1) return 0;
  while (i < 0 || i >= n) i = i < 0 ? -i : 2 * n - 2 - i;
  return i;
}

// Value one quantisation step represents: 257 for 8-bit data stored in a 16-bit file.
double quantum_of(const ImageMeta& m) { return m.bits_per_sample == 16 && m.effective_bits == 8 ? 257.0 : 1.0; }

// Robust pixel noise from horizontal first differences (as in the exposure bench).
double pixel_noise(const ImageU16& img) {
  std::vector<double> d;
  d.reserve(img.size());
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x + 1 < img.width(); ++x) d.push_back(std::fabs(double(img(x + 1, y)) - img(x, y)));
  return d.empty() ? 0.0 : 1.4826 * median(std::move(d)) / std::sqrt(2.0);
}

// Summed-area table with one row and column of zero padding.
struct Integral {
  int w = 0;
  std::vector<double> s;
  template <typename F>
  void build(int width, int height, F value) {
    w = width + 1;
    s.assign(static_cast<std::size_t>(w) * (height + 1), 0.0);
    for (int y = 0; y < height; ++y) {
      double row = 0.0;
      for (int x = 0; x < width; ++x) {
        row += value(x, y);
        s[idx(x + 1, y + 1)] = s[idx(x + 1, y)] + row;
      }
    }
  }
  std::size_t idx(int x, int y) const { return static_cast<std::size_t>(y) * w + x; }
  double box(int x0, int y0, int x1, int y1) const {  // [x0, x1) x [y0, y1)
    return s[idx(x1, y1)] - s[idx(x0, y1)] - s[idx(x1, y0)] + s[idx(x0, y0)];
  }
};

// Per-image data shared by every search.
struct Prepared {
  const RawImage* raw = nullptr;
  ImageF res;
  int P = 16;
  int nx = 0, ny = 0;  // patch positions per row / column
  std::vector<std::uint8_t> valid;
  // Patches are compared after double centring: their own row and column
  // means are removed. That cancels CCD row/column read-out noise and lane or
  // band edges (constant along a row or column) using the patch alone, so a
  // pasted region and its source are centred identically whatever surrounds them.
  std::vector<double> row_prefix, col_prefix;  // running sums of the residual along rows / columns
  std::vector<float> mean, norm;  // residual patch mean and norm of the double-centred patch
  double row_sum(int x, int y) const {
    const std::size_t w1 = std::size_t(res.width()) + 1;
    return row_prefix[std::size_t(y) * w1 + std::size_t(x + P)] - row_prefix[std::size_t(y) * w1 + std::size_t(x)];
  }
  double col_sum(int x, int y) const {
    const std::size_t h1 = std::size_t(res.height()) + 1;
    return col_prefix[std::size_t(x) * h1 + std::size_t(y + P)] - col_prefix[std::size_t(x) * h1 + std::size_t(y)];
  }
  // Double-centred residual at (u, v) of the patch at (x, y).
  double centred(int x, int y, int u, int v) const {
    return res(x + u, y + v) - row_sum(x, y + v) / P - col_sum(x + u, y) / P + mean[pos(x, y)];
  }
  double noise = 1.0;    // camera noise (counts), first differences
  double quantum = 1.0;
  std::size_t pos(int x, int y) const { return static_cast<std::size_t>(y) * nx + x; }
};

// Clips the residual at +-3 local robust noise levels (block MAD, bilinearly
// interpolated between block centres), so that isolated specks and hot pixels
// cannot dominate a patch correlation.
void clip_residual(ImageF& r, int block) {
  const int w = r.width(), h = r.height();
  const int bx = std::max(1, (w + block - 1) / block), by = std::max(1, (h + block - 1) / block);
  std::vector<double> sd(static_cast<std::size_t>(bx) * by);
  std::vector<double> v;
  for (int j = 0; j < by; ++j)
    for (int i = 0; i < bx; ++i) {
      v.clear();
      for (int y = j * block; y < std::min(h, (j + 1) * block); ++y)
        for (int x = i * block; x < std::min(w, (i + 1) * block); ++x) v.push_back(std::fabs(r(x, y)));
      sd[std::size_t(j) * bx + i] = 1.4826 * median(v);
    }
  auto at = [&](int i, int j) { return sd[std::size_t(std::clamp(j, 0, by - 1)) * bx + std::clamp(i, 0, bx - 1)]; };
  for (int y = 0; y < h; ++y) {
    const double fy = (y + 0.5) / block - 0.5;
    const int j = int(std::floor(fy));
    const double ty = fy - j;
    for (int x = 0; x < w; ++x) {
      const double fx = (x + 0.5) / block - 0.5;
      const int i = int(std::floor(fx));
      const double tx = fx - i;
      const double s = (1 - ty) * ((1 - tx) * at(i, j) + tx * at(i + 1, j)) +
                       ty * ((1 - tx) * at(i, j + 1) + tx * at(i + 1, j + 1));
      const float lim = static_cast<float>(3.0 * s);
      r(x, y) = std::clamp(r(x, y), -lim, lim);
    }
  }
}

Prepared prepare(const RawImage& raw, int P) {
  Prepared p;
  p.raw = &raw;
  p.P = P;
  p.quantum = quantum_of(raw.meta);
  p.noise = std::max(pixel_noise(raw.pixels), 0.5 * p.quantum);
  p.res = noise_residual(raw.pixels);
  clip_residual(p.res, P);
  const int w = raw.pixels.width(), h = raw.pixels.height();
  p.nx = std::max(0, w - P + 1);
  p.ny = std::max(0, h - P + 1);
  if (p.nx == 0 || p.ny == 0) return p;
  p.row_prefix.assign(static_cast<std::size_t>(w + 1) * h, 0.0);
  p.col_prefix.assign(static_cast<std::size_t>(h + 1) * w, 0.0);
  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x) {
      p.row_prefix[std::size_t(y) * (w + 1) + x + 1] = p.row_prefix[std::size_t(y) * (w + 1) + x] + p.res(x, y);
      p.col_prefix[std::size_t(x) * (h + 1) + y + 1] = p.col_prefix[std::size_t(x) * (h + 1) + y] + p.res(x, y);
    }
  Integral r, r2, sat, zero;
  r.build(w, h, [&](int x, int y) { return double(p.res(x, y)); });
  r2.build(w, h, [&](int x, int y) { return double(p.res(x, y)) * p.res(x, y); });
  sat.build(w, h, [&](int x, int y) { return raw.pixels(x, y) >= raw.meta.detector_max ? 1.0 : 0.0; });
  zero.build(w, h, [&](int x, int y) { return raw.pixels(x, y) == 0 ? 1.0 : 0.0; });
  const std::size_t n = static_cast<std::size_t>(p.nx) * p.ny;
  p.valid.assign(n, 0);
  p.mean.assign(n, 0.f);
  p.norm.assign(n, 0.f);
  const double area = double(P) * P;
  const double min_sd = 0.5 * quantum_of(raw.meta);
  for (int y = 0; y < p.ny; ++y)
    for (int x = 0; x < p.nx; ++x) {
      const std::size_t i = p.pos(x, y);
      const double s = r.box(x, y, x + P, y + P), ss = r2.box(x, y, x + P, y + P);
      const double mu = s / area;
      double rows = 0.0, cols = 0.0;
      for (int k = 0; k < P; ++k) {
        rows += p.row_sum(x, y + k) * p.row_sum(x, y + k);
        cols += p.col_sum(x + k, y) * p.col_sum(x + k, y);
      }
      const double dev = std::max(0.0, ss - rows / P - cols / P + area * mu * mu);
      p.mean[i] = static_cast<float>(mu);
      p.norm[i] = static_cast<float>(std::sqrt(dev));
      // Saturated plateaus and zero-clipped background carry no noise and are trivially alike.
      p.valid[i] = sat.box(x, y, x + P, y + P) == 0 && zero.box(x, y, x + P, y + P) <= area / 4 &&
                   std::sqrt(dev / area) >= min_sd;
    }
  return p;
}

// Residual correlation of target patch (tx, ty) with source patch (sx, sy) read in orientation o.
double patch_ncc(const Prepared& t, int tx, int ty, const Prepared& s, int sx, int sy, Orientation o) {
  const int P = t.P;
  const std::size_t i = t.pos(tx, ty), j = s.pos(sx, sy);
  const double denom = double(t.norm[i]) * s.norm[j];
  if (denom <= 0.0) return 0.0;
  const bool fx = flips_x(o), fy = flips_y(o);
  // <centred(a), b> equals <centred(a), centred(b)>: centring is an orthogonal projection.
  std::array<double, 64> row_mean{}, col_mean{};
  if (P > 64) throw std::invalid_argument("forensics: patch larger than 64");
  for (int k = 0; k < P; ++k) {
    row_mean[std::size_t(k)] = t.row_sum(tx, ty + k) / P;
    col_mean[std::size_t(k)] = t.col_sum(tx + k, ty) / P;
  }
  const double mu = t.mean[i];
  double dot = 0.0;
  for (int v = 0; v < P; ++v) {
    const float* a = &t.res(tx, ty + v);
    const float* b = &s.res(sx, sy + (fy ? P - 1 - v : v));
    const double shift = mu - row_mean[std::size_t(v)];
    double row = 0.0;
    if (fx)
      for (int u = 0; u < P; ++u) row += (a[u] + shift - col_mean[std::size_t(u)]) * b[P - 1 - u];
    else
      for (int u = 0; u < P; ++u) row += (a[u] + shift - col_mean[std::size_t(u)]) * b[u];
    dot += row;
  }
  return dot / denom;
}

// Raw-pixel test for a digital copy: least-squares fit target = g * source + c
// over the patch; returns the RMS fit error beyond rounding, over the noise (infinity
// when the fitted gain is not positive).
double copy_error(const Prepared& t, int tx, int ty, const Prepared& s, int sx, int sy, Orientation o) {
  const int P = t.P;
  const bool fx = flips_x(o), fy = flips_y(o);
  double sa = 0, sb = 0, saa = 0, sab = 0, sbb = 0;
  for (int v = 0; v < P; ++v)
    for (int u = 0; u < P; ++u) {
      const double a = t.raw->pixels(tx + u, ty + v);
      const double b = s.raw->pixels(sx + (fx ? P - 1 - u : u), sy + (fy ? P - 1 - v : v));
      sa += a;
      sb += b;
      saa += a * a;
      sab += a * b;
      sbb += b * b;
    }
  const double n = double(P) * P;
  const double vb = sbb - sb * sb / n, cab = sab - sa * sb / n, va = saa - sa * sa / n;
  if (vb <= 0 || cab <= 0) return std::numeric_limits<double>::infinity();
  const double g = cab / vb;
  const double sse = std::max(0.0, va - g * cab);
  // Noise of each patch from its own residual (the residual filter and double
  // centring keep about 0.82 of white noise), never above the image's camera noise: dark areas are
  // quieter than the image as a whole. Rounding after a gain change adds
  // q * sqrt((1 + g^2) / 12) of error, which is subtracted.
  auto local = [&](const Prepared& p, int x, int y) {
    return std::min(p.noise, double(p.norm[p.pos(x, y)]) / P / 0.82);
  };
  const double scale = std::max(std::min(local(t, tx, ty), g * local(s, sx, sy)), 0.5 * t.quantum);
  const double rounding = t.quantum * std::sqrt((1.0 + g * g) / 12.0);
  return std::max(0.0, std::sqrt(sse / n) - rounding) / scale;
}

// Correlation at (sx, sy) minus the best correlation with the source moved by one pixel.
double peak_sharpness(const Prepared& t, int tx, int ty, const Prepared& s, int sx, int sy, Orientation o,
                      double at) {
  double best = -1.0;
  const std::array<std::pair<int, int>, 4> nb{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
  for (auto [dx, dy] : nb) {
    const int x = sx + dx, y = sy + dy;
    if (x < 0 || y < 0 || x >= s.nx || y >= s.ny || !s.valid[s.pos(x, y)]) continue;
    best = std::max(best, patch_ncc(t, tx, ty, s, x, y, o));
  }
  return at - best;
}

// Mapping from target to source coordinates: identity shifts by (a, b); a flip
// reflects through x' = a - x (or y' = b - y).
struct Mapping {
  std::size_t target = 0, source = 0;
  Orientation o = Orientation::identity;
  int a = 0, b = 0;
  auto key() const { return std::tuple(target, source, int(o), a, b); }
  bool operator<(const Mapping& m) const { return key() < m.key(); }
  int sx(int x) const { return flips_x(o) ? a - x : x + a; }
  int sy(int y) const { return flips_y(o) ? b - y : y + b; }
};

Mapping mapping_of(std::size_t ti, std::size_t si, Orientation o, int px, int py, int qx, int qy, int P) {
  Mapping m{ti, si, o, 0, 0};
  m.a = flips_x(o) ? qx + px + P - 1 : qx - px;
  m.b = flips_y(o) ? qy + py + P - 1 : qy - py;
  return m;
}

struct Seed {
  std::uint32_t target_pos, source_pos;
  Orientation o;
};

struct Pair {
  std::size_t target, source;
};

// Hash keys: signs of the residual at `bits` fixed random offsets in the patch,
// read in the given orientation (so a flipped copy gets the key of its source).
std::vector<std::uint64_t> hash_list(const Prepared& p, const std::vector<std::pair<int, int>>& offsets,
                                     Orientation o, unsigned threads) {
  std::vector<std::uint64_t> keys(p.valid.size(), ~std::uint64_t{0});
  const int P = p.P;
  parallel_for(
      static_cast<std::size_t>(p.ny),
      [&](std::size_t yy) {
        const int y = static_cast<int>(yy);
        for (int x = 0; x < p.nx; ++x) {
          const std::size_t i = p.pos(x, y);
          if (!p.valid[i]) continue;
          std::uint64_t k = 0;
          for (std::size_t b = 0; b < offsets.size(); ++b) {
            const int u = flips_x(o) ? P - 1 - offsets[b].first : offsets[b].first;
            const int v = flips_y(o) ? P - 1 - offsets[b].second : offsets[b].second;
            if (p.centred(x, y, u, v) > 0.0) k |= std::uint64_t{1} << b;
          }
          keys[i] = (k << 32) | i;
        }
      },
      threads);
  keys.erase(std::remove(keys.begin(), keys.end(), ~std::uint64_t{0}), keys.end());
  std::sort(keys.begin(), keys.end());
  return keys;
}

bool overlapping(int px, int py, int qx, int qy, int P) { return std::abs(px - qx) < P && std::abs(py - qy) < P; }

// Candidate pairs between two sorted key lists (same list for the within-image identity search).
void merge_candidates(const std::vector<std::uint64_t>& t, const std::vector<std::uint64_t>& s, bool same_list,
                      bool same_image, Orientation o, const Prepared& pt, std::size_t max_bucket,
                      std::vector<Seed>& out) {
  auto run_end = [](const std::vector<std::uint64_t>& v, std::size_t i) {
    const std::uint64_t k = v[i] >> 32;
    std::size_t j = i;
    while (j < v.size() && (v[j] >> 32) == k) ++j;
    return j;
  };
  const int P = pt.P;
  auto consider = [&](std::uint32_t a, std::uint32_t b) {
    if (same_image) {
      if (a == b) return;
      const int ax = int(a % pt.nx), ay = int(a / pt.nx), bx = int(b % pt.nx), by = int(b / pt.nx);
      if (overlapping(ax, ay, bx, by, P)) return;
    }
    out.push_back({a, b, o});
  };
  if (same_list) {
    for (std::size_t i = 0; i < t.size();) {
      const std::size_t j = run_end(t, i);
      if (j - i > 1 && j - i <= max_bucket)
        for (std::size_t a = i; a < j; ++a)
          for (std::size_t b = a + 1; b < j; ++b) consider(std::uint32_t(t[a]), std::uint32_t(t[b]));
      i = j;
    }
    return;
  }
  std::size_t i = 0, j = 0;
  while (i < t.size() && j < s.size()) {
    const std::uint64_t ki = t[i] >> 32, kj = s[j] >> 32;
    if (ki < kj) {
      i = run_end(t, i);
    } else if (kj < ki) {
      j = run_end(s, j);
    } else {
      const std::size_t ie = run_end(t, i), je = run_end(s, j);
      if (ie - i <= max_bucket && je - j <= max_bucket)
        for (std::size_t a = i; a < ie; ++a)
          for (std::size_t b = j; b < je; ++b) consider(std::uint32_t(t[a]), std::uint32_t(s[b]));
      i = ie;
      j = je;
    }
  }
}

// Grows a region from its seeds along one mapping (4-connected target patch positions).
DuplicateRegion grow_region(const Mapping& m, const std::vector<std::uint32_t>& seeds,
                            const std::vector<Prepared>& prep, const ForensicsOptions& opt, bool copy) {
  const Prepared& t = prep[m.target];
  const Prepared& s = prep[m.source];
  const int P = t.P;
  const bool same = m.target == m.source;
  std::unordered_map<std::uint32_t, bool> seen;
  std::vector<std::uint32_t> accepted, stack;
  double ncc_sum = 0.0, err_sum = 0.0;
  auto source_pos = [&](int x, int y, int& qx, int& qy) {
    qx = flips_x(m.o) ? m.a - x - (P - 1) : x + m.a;
    qy = flips_y(m.o) ? m.b - y - (P - 1) : y + m.b;
  };
  auto test = [&](std::uint32_t i) {
    auto [it, fresh] = seen.emplace(i, false);
    if (!fresh) return;
    const int x = int(i % t.nx), y = int(i / t.nx);
    int qx, qy;
    source_pos(x, y, qx, qy);
    if (qx < 0 || qy < 0 || qx >= s.nx || qy >= s.ny) return;
    const std::size_t j = s.pos(qx, qy);
    if (!t.valid[i] || !s.valid[j]) return;
    // Within one image each pair is kept once, from the earlier patch to the later one.
    if (same && (overlapping(x, y, qx, qy, P) || j <= i)) return;
    const double c = patch_ncc(t, x, y, s, qx, qy, m.o);
    if (c < opt.min_ncc) return;
    double err = 0.0;
    if (copy) {
      err = copy_error(t, x, y, s, qx, qy, m.o);
      if (err > opt.copy_noise_ratio) return;
    }
    it->second = true;
    accepted.push_back(i);
    ncc_sum += c;
    err_sum += err;
    stack.push_back(i);
  };
  for (std::uint32_t seed : seeds) {
    test(seed);
    while (!stack.empty()) {
      const std::uint32_t i = stack.back();
      stack.pop_back();
      const int x = int(i % t.nx), y = int(i / t.nx);
      if (x > 0) test(i - 1);
      if (x + 1 < t.nx) test(i + 1);
      if (y > 0) test(i - std::uint32_t(t.nx));
      if (y + 1 < t.ny) test(i + std::uint32_t(t.nx));
    }
  }
  DuplicateRegion r;
  r.target_image = m.target;
  r.source_image = m.source;
  r.orientation = m.o;
  r.patches = accepted.size();
  if (accepted.empty()) return r;
  r.mean_ncc = ncc_sum / double(accepted.size());
  r.error_noise = err_sum / double(accepted.size());
  {
    const std::size_t step = std::max<std::size_t>(1, accepted.size() / 64);
    double sum = 0.0;
    std::size_t n = 0;
    for (std::size_t k = 0; k < accepted.size(); k += step) {
      const int x = int(accepted[k] % t.nx), y = int(accepted[k] / t.nx);
      int qx, qy;
      source_pos(x, y, qx, qy);
      sum += peak_sharpness(t, x, y, s, qx, qy, m.o, patch_ncc(t, x, y, s, qx, qy, m.o));
      ++n;
    }
    r.sharpness = sum / double(n);
  }

  // Covered target pixels: union of the accepted patches.
  const int w = t.raw->pixels.width(), h = t.raw->pixels.height();
  int x0 = w, y0 = h, x1 = 0, y1 = 0;
  for (std::uint32_t i : accepted) {
    const int x = int(i % t.nx), y = int(i / t.nx);
    x0 = std::min(x0, x);
    y0 = std::min(y0, y);
    x1 = std::max(x1, x + P);
    y1 = std::max(y1, y + P);
  }
  r.target = {x0, y0, x1, y1};
  const int bw = x1 - x0, bh = y1 - y0;
  std::vector<int> diff(static_cast<std::size_t>(bw + 1) * (bh + 1), 0);
  auto d = [&](int x, int y) -> int& { return diff[static_cast<std::size_t>(y) * (bw + 1) + x]; };
  for (std::uint32_t i : accepted) {
    const int x = int(i % t.nx) - x0, y = int(i / t.nx) - y0;
    ++d(x, y);
    --d(x + P, y);
    --d(x, y + P);
    ++d(x + P, y + P);
  }
  for (int y = 0; y <= bh; ++y)
    for (int x = 0; x <= bw; ++x) {
      if (x > 0) d(x, y) += d(x - 1, y);
      if (y > 0) d(x, y) += d(x, y - 1);
      if (x > 0 && y > 0) d(x, y) -= d(x - 1, y - 1);
    }
  std::size_t covered = 0, identical = 0;
  double sx = 0, sy = 0, sxx = 0, sxy = 0;
  for (int y = 0; y < bh; ++y)
    for (int x = 0; x < bw; ++x) {
      if (d(x, y) <= 0) continue;
      const int tx = x0 + x, ty = y0 + y;
      const double a = t.raw->pixels(tx, ty), b = s.raw->pixels(m.sx(tx), m.sy(ty));
      ++covered;
      identical += a == b;
      sx += b;
      sy += a;
      sxx += b * b;
      sxy += a * b;
    }
  r.area_px = covered;
  r.identical_fraction = covered ? double(identical) / double(covered) : 0.0;
  const double n = double(covered), var = sxx - sx * sx / n;
  r.gain = var > 0 ? (sxy - sx * sy / n) / var : 1.0;
  r.offset = (sy - r.gain * sx) / n;
  const int ax = m.sx(x0), bx = m.sx(x1 - 1), ay = m.sy(y0), by = m.sy(y1 - 1);
  r.source = {std::min(ax, bx), std::min(ay, by), std::max(ax, bx) + 1, std::max(ay, by) + 1};
  return r;
}

std::vector<DuplicateRegion> find_duplicates(const std::vector<Prepared>& prep, const ForensicsOptions& opt) {
  const int P = opt.patch;
  std::vector<Orientation> orientations{Orientation::identity};
  if (opt.flips) orientations.insert(orientations.end(), {Orientation::flip_h, Orientation::flip_v, Orientation::rotate_180});
  std::vector<Pair> pairs;
  for (std::size_t i = 0; i < prep.size(); ++i)
    for (std::size_t j = i; j < prep.size(); ++j)
      if (i == j || opt.cross) pairs.push_back({i, j});

  std::vector<Seed> candidates;
  std::vector<Pair> candidate_pair;
  for (int table = 0; table < opt.hash_tables; ++table) {
    Rng rng(derive_seed(opt.seed, static_cast<std::uint64_t>(table)));
    std::vector<int> cells(static_cast<std::size_t>(P) * P);
    std::iota(cells.begin(), cells.end(), 0);
    rng.shuffle(cells);
    std::vector<std::pair<int, int>> offsets;
    for (int b = 0; b < std::min(opt.hash_bits, 32); ++b) offsets.emplace_back(cells[b] % P, cells[b] / P);
    std::vector<std::vector<std::vector<std::uint64_t>>> keys(prep.size());
    for (std::size_t i = 0; i < prep.size(); ++i)
      for (Orientation o : orientations) keys[i].push_back(hash_list(prep[i], offsets, o, opt.threads));
    for (const Pair& pr : pairs)
      for (std::size_t oi = 0; oi < orientations.size(); ++oi) {
        const std::size_t before = candidates.size();
        const bool same = pr.target == pr.source;
        merge_candidates(keys[pr.target][0], keys[pr.source][oi], same && oi == 0, same, orientations[oi],
                         prep[pr.target], opt.max_bucket, candidates);
        candidate_pair.insert(candidate_pair.end(), candidates.size() - before, pr);
      }
  }

  // Verify every candidate; keep the seeds above the correlation threshold.
  std::vector<std::uint8_t> keep(candidates.size(), 0);
  parallel_for(
      candidates.size(),
      [&](std::size_t k) {
        const Seed& c = candidates[k];
        const Prepared& t = prep[candidate_pair[k].target];
        const Prepared& s = prep[candidate_pair[k].source];
        const int tx = int(c.target_pos % t.nx), ty = int(c.target_pos / t.nx);
        const int sx = int(c.source_pos % s.nx), sy = int(c.source_pos / s.nx);
        const double ncc = patch_ncc(t, tx, ty, s, sx, sy, c.o);
        if (ncc < opt.min_ncc) return;
        if (copy_error(t, tx, ty, s, sx, sy, c.o) <= opt.copy_noise_ratio)
          keep[k] = 2;
        else if (candidate_pair[k].target != candidate_pair[k].source)
          keep[k] = 1;
      },
      opt.threads);
  if (std::getenv("BLOTVERSE_DEBUG"))
    std::cerr << "forensics: " << candidates.size() << " candidates, "
              << std::count(keep.begin(), keep.end(), 2) << " copy seeds, " << std::count(keep.begin(), keep.end(), 1)
              << " scene seeds\n";
  std::map<Mapping, std::pair<std::uint8_t, std::vector<std::uint32_t>>> by_mapping;
  for (std::size_t k = 0; k < candidates.size(); ++k) {
    if (!keep[k]) continue;
    const Seed& c = candidates[k];
    const Prepared& t = prep[candidate_pair[k].target];
    const Prepared& s = prep[candidate_pair[k].source];
    Mapping m = mapping_of(candidate_pair[k].target, candidate_pair[k].source, c.o, int(c.target_pos % t.nx),
                           int(c.target_pos / t.nx), int(c.source_pos % s.nx), int(c.source_pos / s.nx), P);
    std::uint32_t seed_pos = c.target_pos;
    if (m.target == m.source && c.source_pos < c.target_pos) {
      // Same pair seen from the other patch: store it from the earlier one.
      m = mapping_of(m.target, m.source, c.o, int(c.source_pos % t.nx), int(c.source_pos / t.nx),
                     int(c.target_pos % t.nx), int(c.target_pos / t.nx), P);
      seed_pos = c.source_pos;
    }
    auto& entry = by_mapping[m];
    entry.first = std::max(entry.first, keep[k]);
    entry.second.push_back(seed_pos);
  }
  // Every copy mapping is grown. Scene matches arrive as many seeds spread over
  // a few neighbouring mappings; only the most-seeded mappings of each image
  // pair and orientation are grown.
  std::vector<std::pair<Mapping, std::pair<std::uint8_t, std::vector<std::uint32_t>>>> work;
  std::map<std::tuple<std::size_t, std::size_t, int>, std::vector<std::pair<std::size_t, Mapping>>> scene;
  for (auto& [m, entry] : by_mapping) {
    if (entry.first == 2)
      work.emplace_back(m, entry);
    else
      scene[{m.target, m.source, int(m.o)}].emplace_back(entry.second.size(), m);
  }
  for (auto& [key, list] : scene) {
    std::stable_sort(list.begin(), list.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
    for (std::size_t k = 0; k < std::min<std::size_t>(3, list.size()); ++k)
      work.emplace_back(list[k].second, by_mapping[list[k].second]);
  }
  std::vector<DuplicateRegion> regions(work.size());
  parallel_for(
      work.size(),
      [&](std::size_t k) {
        const bool copy = work[k].second.first == 2;
        auto seeds = work[k].second.second;
        std::sort(seeds.begin(), seeds.end());
        seeds.erase(std::unique(seeds.begin(), seeds.end()), seeds.end());
        regions[k] = grow_region(work[k].first, seeds, prep, opt, copy);
        regions[k].kind = copy ? DuplicateKind::copy : DuplicateKind::same_scene;
      },
      opt.threads);
  auto image_area = [&](std::size_t i) { return double(prep[i].raw->pixels.size()); };
  regions.erase(std::remove_if(regions.begin(), regions.end(),
                               [&](const DuplicateRegion& r) {
                                 if (r.kind == DuplicateKind::copy) return r.patches < opt.min_patches;
                                 const double smaller = std::min(image_area(r.target_image), image_area(r.source_image));
                                 return double(r.area_px) < opt.scene_fraction * smaller;
                               }),
                regions.end());
  // One scene finding per image pair: the largest.
  std::stable_sort(regions.begin(), regions.end(),
                   [](const DuplicateRegion& a, const DuplicateRegion& b) { return a.area_px > b.area_px; });
  std::set<std::pair<std::size_t, std::size_t>> scene_pairs;
  regions.erase(std::remove_if(regions.begin(), regions.end(),
                               [&](const DuplicateRegion& r) {
                                 return r.kind == DuplicateKind::same_scene &&
                                        !scene_pairs.insert({r.target_image, r.source_image}).second;
                               }),
                regions.end());
  std::stable_sort(regions.begin(), regions.end(),
                   [](const DuplicateRegion& a, const DuplicateRegion& b) { return a.area_px > b.area_px; });
  return regions;
}

// Sharp straight steps in the background along columns (vertical) or rows.
std::vector<SeamLine> find_seams(std::size_t index, const RawImage& raw, const ForensicsOptions& opt) {
  std::vector<SeamLine> out;
  const ImageU16& img = raw.pixels;
  const double noise = std::max(pixel_noise(img), 0.5 * quantum_of(raw.meta));
  // Lane and band edges are blurred over several pixels and rarely stay
  // straight for seam_min_length; a digital paste is a one-pixel step.
  for (bool vertical : {true, false}) {
    const int across = vertical ? img.width() : img.height();
    const int along = vertical ? img.height() : img.width();
    if (across < 8 || along < opt.seam_segment / 2) continue;
    auto at = [&](int i, int j) -> double { return vertical ? img(i, j) : img(j, i); };
    auto excluded = [&](int i, int j) { return (vertical ? img(i, j) : img(j, i)) >= raw.meta.detector_max; };
    const int segments = std::max(1, along / opt.seam_segment);
    struct Hit {
      int segment;
      double step, z, share;
    };
    std::vector<std::vector<Hit>> hits(static_cast<std::size_t>(across));
    std::mutex lock;
    parallel_for(
        static_cast<std::size_t>(segments),
        [&](std::size_t sg) {
          const int j0 = int(sg) * along / segments, j1 = int(sg + 1) * along / segments;
          std::vector<double> m(static_cast<std::size_t>(across), kNaN), wide(m.size(), kNaN);
          std::vector<double> dn, dw;
          for (int i = 3; i + 2 < across; ++i) {
            dn.clear();
            dw.clear();
            for (int j = j0; j < j1; ++j) {
              bool clipped = false;
              for (int k = i - 3; k <= i + 2 && !clipped; ++k) clipped = excluded(k, j);
              if (clipped) continue;
              dn.push_back(at(i, j) - at(i - 1, j));
              dw.push_back((at(i, j) + at(i + 1, j) + at(i + 2, j) - at(i - 1, j) - at(i - 2, j) - at(i - 3, j)) / 3.0);
            }
            if (dn.size() < static_cast<std::size_t>(j1 - j0) / 2) continue;
            m[std::size_t(i)] = median(dn);
            wide[std::size_t(i)] = median(dw);
          }
          std::vector<double> finite;
          for (double v : m)
            if (std::isfinite(v)) finite.push_back(v);
          if (finite.size() < 8) return;
          const double centre = median(finite);
          for (double& v : finite) v = std::fabs(v - centre);
          const double floor = noise * std::sqrt(2.0) * 1.2533 / std::sqrt(double(j1 - j0));
          const double scale = std::max(1.4826 * median(finite), floor);
          for (int i = 3; i + 2 < across; ++i) {
            const double v = m[std::size_t(i)];
            if (!std::isfinite(v)) continue;
            const double z = (v - centre) / scale;
            if (std::fabs(z) < opt.seam_z) continue;
            const double total = wide[std::size_t(i)] - 3.0 * centre;
            const double share = total != 0.0 ? (v - centre) / total : kNaN;
            if (!(share >= opt.seam_share_lo && share <= opt.seam_share_hi)) continue;
            std::lock_guard<std::mutex> g(lock);
            hits[std::size_t(i)].push_back({int(sg), v - centre, z, share});
          }
        },
        opt.threads);
    for (int i = 0; i < across; ++i) {
      auto& h = hits[std::size_t(i)];
      std::sort(h.begin(), h.end(), [](const Hit& a, const Hit& b) { return a.segment < b.segment; });
      for (std::size_t k = 0; k < h.size();) {
        std::size_t e = k + 1;
        while (e < h.size() && h[e].segment == h[e - 1].segment + 1) ++e;
        SeamLine s;
        s.image = index;
        s.vertical = vertical;
        s.position = i;
        s.start = h[k].segment * along / segments;
        s.end = (h[e - 1].segment + 1) * along / segments;
        if (s.end - s.start < opt.seam_min_length) {
          k = e;
          continue;
        }
        double step = 0, share = 0;
        for (std::size_t q = k; q < e; ++q) {
          step += h[q].step;
          share += h[q].share;
          s.z = std::max(s.z, std::fabs(h[q].z));
        }
        s.step = step / double(e - k);
        s.share = share / double(e - k);
        s.step_noise = s.step / noise;
        out.push_back(s);
        k = e;
      }
    }
  }
  return out;
}

// Windows whose residual noise is far below that of the image's quietest windows.
std::vector<FlatRegion> find_flats(std::size_t index, const RawImage& raw, const Prepared& p,
                                   const ForensicsOptions& opt, ForensicsImage& info) {
  std::vector<FlatRegion> out;
  const ImageU16& img = raw.pixels;
  const int F = opt.flat_window, S = std::max(1, F / 2);
  const int w = img.width(), h = img.height();
  const int gx = w >= F ? (w - F) / S + 1 : 0, gy = h >= F ? (h - F) / S + 1 : 0;
  if (gx == 0 || gy == 0) {
    info.flat_tested = false;
    return out;
  }
  const std::uint16_t lowest = *std::min_element(img.data().begin(), img.data().end());
  Integral r, r2, clip;
  r.build(w, h, [&](int x, int y) { return double(p.res(x, y)); });
  r2.build(w, h, [&](int x, int y) { return double(p.res(x, y)) * p.res(x, y); });
  // Saturated pixels disqualify a window; so do many pixels at the image minimum (clipped background).
  clip.build(w, h, [&](int x, int y) {
    const std::uint16_t v = img(x, y);
    return v >= raw.meta.detector_max ? double(F) * F : v == lowest ? 1.0 : 0.0;
  });
  const double n = double(F) * F;
  std::vector<double> sd(static_cast<std::size_t>(gx) * gy, kNaN);
  for (int j = 0; j < gy; ++j)
    for (int i = 0; i < gx; ++i) {
      const int x = i * S, y = j * S;
      if (clip.box(x, y, x + F, y + F) > n / 4) continue;
      const double m = r.box(x, y, x + F, y + F) / n;
      sd[std::size_t(j) * gx + i] = std::sqrt(std::max(0.0, r2.box(x, y, x + F, y + F) / n - m * m));
    }
  std::vector<double> finite;
  for (double v : sd)
    if (std::isfinite(v)) finite.push_back(v);
  if (finite.empty()) {
    info.flat_tested = false;
    return out;
  }
  // Reference: the quietest decile of windows. Shot noise makes bright areas
  // noisier, so dark background is the right comparison for an erased area.
  const double typical = quantile(finite, 0.1);
  info.residual_noise = typical;
  if (typical < opt.min_flat_noise_quanta * quantum_of(raw.meta)) {
    info.flat_tested = false;
    info.notes.push_back("flat-region test skipped: typical residual noise below " +
                         std::to_string(opt.min_flat_noise_quanta) + " quantisation steps");
    return out;
  }
  std::vector<std::uint8_t> flag(sd.size(), 0), done(sd.size(), 0);
  for (std::size_t k = 0; k < sd.size(); ++k) flag[k] = std::isfinite(sd[k]) && sd[k] < opt.flat_ratio * typical;
  for (std::size_t k = 0; k < sd.size(); ++k) {
    if (!flag[k] || done[k]) continue;
    std::vector<std::size_t> stack{k};
    done[k] = 1;
    int x0 = gx, y0 = gy, x1 = 0, y1 = 0;
    double noise = 0;
    std::size_t count = 0;
    while (!stack.empty()) {
      const std::size_t c = stack.back();
      stack.pop_back();
      const int i = int(c % gx), j = int(c / gx);
      x0 = std::min(x0, i);
      y0 = std::min(y0, j);
      x1 = std::max(x1, i);
      y1 = std::max(y1, j);
      noise += sd[c];
      ++count;
      const std::array<std::pair<int, int>, 4> nb{{{i - 1, j}, {i + 1, j}, {i, j - 1}, {i, j + 1}}};
      for (auto [a, b] : nb) {
        if (a < 0 || b < 0 || a >= gx || b >= gy) continue;
        const std::size_t q = std::size_t(b) * gx + a;
        if (flag[q] && !done[q]) {
          done[q] = 1;
          stack.push_back(q);
        }
      }
    }
    out.push_back({index, Rect{x0 * S, y0 * S, x1 * S + F, y1 * S + F}, noise / double(count), typical});
  }
  return out;
}

std::string fmt(double v, int digits = 3) {
  std::ostringstream s;
  s << std::fixed << std::setprecision(digits) << v;
  return s.str();
}

std::string rect_text(const Rect& r) {
  return std::to_string(r.x0) + "," + std::to_string(r.y0) + "-" + std::to_string(r.x1) + "," + std::to_string(r.y1);
}

}  // namespace

ImageF noise_residual(const ImageU16& image) {
  static constexpr std::array<double, 5> k{-3.0 / 35, 12.0 / 35, 17.0 / 35, 12.0 / 35, -3.0 / 35};
  const int w = image.width(), h = image.height();
  ImageF tmp(w, h), out(w, h);
  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x) {
      double s = 0;
      for (int d = -2; d <= 2; ++d) s += k[std::size_t(d + 2)] * image(reflect(x + d, w), y);
      tmp(x, y) = static_cast<float>(s);
    }
  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x) {
      double s = 0;
      for (int d = -2; d <= 2; ++d) s += k[std::size_t(d + 2)] * tmp(x, reflect(y + d, h));
      out(x, y) = static_cast<float>(image(x, y) - s);
    }
  return out;
}

ForensicsResult analyze_forensics(const std::vector<RawImage>& images, const std::vector<std::string>& names,
                                  const ForensicsOptions& options) {
  if (options.patch < 4 || options.hash_bits < 1 || options.hash_bits > 32)
    throw std::invalid_argument("forensics: patch must be >= 4 and hash bits in [1, 32]");
  const auto start = std::chrono::steady_clock::now();
  ForensicsResult result;
  result.options = options;
  std::vector<Prepared> prep(images.size());
  parallel_for(images.size(), [&](std::size_t i) { prep[i] = prepare(images[i], options.patch); }, options.threads);
  for (std::size_t i = 0; i < images.size(); ++i) {
    const RawImage& raw = images[i];
    ForensicsImage info;
    info.name = i < names.size() ? names[i] : raw.meta.path;
    info.sha256 = raw.meta.sha256.empty() ? pixel_fingerprint(raw.pixels) : raw.meta.sha256;
    info.width = raw.pixels.width();
    info.height = raw.pixels.height();
    info.effective_bits = raw.meta.effective_bits;
    info.grey_levels = raw.meta.grey_levels;
    info.valid_patches = static_cast<std::size_t>(std::count(prep[i].valid.begin(), prep[i].valid.end(), 1));
    info.notes = raw.meta.warnings;
    const auto flats = find_flats(i, raw, prep[i], options, info);
    result.flats.insert(result.flats.end(), flats.begin(), flats.end());
    const auto seams = find_seams(i, raw, options);
    result.seams.insert(result.seams.end(), seams.begin(), seams.end());
    result.images.push_back(std::move(info));
  }
  for (std::size_t i = 0; i < images.size(); ++i)
    for (std::size_t j = i + 1; j < images.size(); ++j)
      if (result.images[i].sha256 == result.images[j].sha256) result.identical_files.emplace_back(i, j);
  result.duplicates = find_duplicates(prep, options);
  result.seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  return result;
}

void write_forensics_outputs(const std::filesystem::path& dir, const ForensicsResult& result,
                             const std::vector<RawImage>& images) {
  std::filesystem::create_directories(dir);
  const auto& opt = result.options;
  {
    std::ofstream f(dir / "forensics_duplicates.csv");
    f << "region,kind,target_image,source_image,orientation,target_x0,target_y0,target_x1,target_y1,source_x0,source_y0,"
         "source_x1,source_y1,patches,area_px,mean_ncc,error_over_noise,sharpness,identical_fraction,gain,offset\n";
    for (std::size_t k = 0; k < result.duplicates.size(); ++k) {
      const auto& r = result.duplicates[k];
      f << k + 1 << ',' << duplicate_kind_label(r.kind) << ',' << result.images[r.target_image].name << ',' << result.images[r.source_image].name << ','
        << orientation_label(r.orientation) << ',' << r.target.x0 << ',' << r.target.y0 << ',' << r.target.x1 << ','
        << r.target.y1 << ',' << r.source.x0 << ',' << r.source.y0 << ',' << r.source.x1 << ',' << r.source.y1 << ','
        << r.patches << ',' << r.area_px << ',' << fmt(r.mean_ncc, 4) << ',' << fmt(r.error_noise, 4) << ',' << fmt(r.sharpness, 4) << ','
        << fmt(r.identical_fraction, 4) << ','
        << fmt(r.gain, 4) << ',' << fmt(r.offset, 2) << '\n';
    }
  }
  {
    std::ofstream f(dir / "forensics_seams.csv");
    f << "image,direction,position,start,end,step,step_over_noise,z,share\n";
    for (const auto& s : result.seams)
      f << result.images[s.image].name << ',' << (s.vertical ? "column" : "row") << ',' << s.position << ','
        << s.start << ',' << s.end << ',' << fmt(s.step, 2) << ',' << fmt(s.step_noise, 2) << ',' << fmt(s.z, 1)
        << ',' << fmt(s.share, 2) << '\n';
  }
  {
    std::ofstream f(dir / "forensics_flat.csv");
    f << "image,x0,y0,x1,y1,residual_noise,typical_residual_noise\n";
    for (const auto& r : result.flats)
      f << result.images[r.image].name << ',' << r.rect.x0 << ',' << r.rect.y0 << ',' << r.rect.x1 << ','
        << r.rect.y1 << ',' << fmt(r.noise, 3) << ',' << fmt(r.typical_noise, 3) << '\n';
  }
  {
    std::ofstream f(dir / "forensics_images.csv");
    f << "image,sha256,width,height,effective_bits,grey_levels,valid_patches,residual_noise,flat_tested\n";
    for (const auto& im : result.images)
      f << im.name << ',' << im.sha256 << ',' << im.width << ',' << im.height << ',' << im.effective_bits << ','
        << im.grey_levels << ',' << im.valid_patches << ',' << fmt(im.residual_noise, 3) << ','
        << (im.flat_tested ? 1 : 0) << '\n';
  }

  // Masks: the finding number at each flagged pixel (duplicates: target and source).
  std::vector<ImageU16> masks;
  std::vector<bool> used(images.size(), false);
  for (const auto& im : images) masks.emplace_back(im.pixels.width(), im.pixels.height(), 0);
  auto paint = [&](std::size_t i, const Rect& r, std::size_t label) {
    const Rect c = r.clipped(masks[i].width(), masks[i].height());
    for (int y = c.y0; y < c.y1; ++y)
      for (int x = c.x0; x < c.x1; ++x) masks[i](x, y) = static_cast<std::uint16_t>(std::min<std::size_t>(label, 65535));
    used[i] = true;
  };
  std::size_t label = 0;
  for (const auto& r : result.duplicates) {
    ++label;
    paint(r.target_image, r.target, label);
    paint(r.source_image, r.source, label);
  }
  for (const auto& s : result.seams) {
    ++label;
    paint(s.image, s.vertical ? Rect{s.position - 1, s.start, s.position + 1, s.end}
                              : Rect{s.start, s.position - 1, s.end, s.position + 1},
          label);
  }
  for (const auto& r : result.flats) paint(r.image, r.rect, ++label);
  for (std::size_t i = 0; i < images.size(); ++i)
    if (used[i]) write_tiff16(dir / ("mask_" + std::to_string(i + 1) + ".tif"), masks[i]);

  std::ofstream md(dir / "forensics.md");
  md << "# Image-integrity screening\n\n"
     << "Findings are **regions to review**, not evidence of manipulation. Legitimate operations can produce "
        "them: a reused loading control that the authors declared, membrane or strip edges, exports with "
        "clipped or processed background. Review each one against the original acquisition and the methods.\n\n"
     << "## Images\n\n| # | Image | Size | Bits | Grey levels | Usable patches | Pixel SHA-256 |\n"
        "|---|---|---|---|---|---|---|\n";
  for (std::size_t i = 0; i < result.images.size(); ++i) {
    const auto& im = result.images[i];
    md << "| " << i + 1 << " | " << im.name << " | " << im.width << "x" << im.height << " | " << im.effective_bits
       << " | " << im.grey_levels << " | " << im.valid_patches << " | `" << im.sha256.substr(0, 16) << "...` |\n";
  }
  for (std::size_t i = 0; i < result.images.size(); ++i)
    for (const auto& n : result.images[i].notes) md << "\n* Image " << i + 1 << ": " << n;
  md << "\n\n## Identical pixel data\n\n";
  if (result.identical_files.empty()) md << "No two images have the same pixel fingerprint.\n";
  for (auto [i, j] : result.identical_files)
    md << "* Images " << i + 1 << " and " << j + 1 << " have identical pixels.\n";

  md << "\n## Duplicated regions (shared noise)\n\n"
     << "Residual correlation >= " << opt.min_ncc << " over " << opt.patch << "x" << opt.patch
     << " patches, at least " << opt.min_patches << " connected patch positions"
     << (opt.flips ? "; flipped and 180-degree copies included" : "")
     << (opt.cross ? "; every pair of images compared" : "; each image compared with itself only")
     << ". Genuine bands of the same shape do not share noise; independent noise gives a correlation near 0 "
        "(standard deviation about 1/" << opt.patch << ").\n\n";
  if (result.duplicates.empty()) {
    md << "None found.\n";
  } else {
    md << "| # | Region (x0,y0-x1,y1) | Image | Matches | Image | Orientation | Area px | Mean corr. | Identical px | "
          "Gain | Offset |\n|---|---|---|---|---|---|---|---|---|---|---|\n";
    for (std::size_t k = 0; k < result.duplicates.size(); ++k) {
      const auto& r = result.duplicates[k];
      md << "| " << k + 1 << " | " << rect_text(r.target) << " | " << r.target_image + 1 << " | "
         << rect_text(r.source) << " | " << r.source_image + 1 << " | " << orientation_label(r.orientation) << " | "
         << r.area_px << " | " << fmt(r.mean_ncc) << " | " << fmt(100 * r.identical_fraction, 1) << " % | "
         << fmt(r.gain) << " | " << fmt(r.offset, 1) << " |\n";
    }
  }
  md << "\n## Straight seams (lower confidence)\n\nA sharp step in the background along a single column or row, "
        "sustained over at least "
     << opt.seam_segment << " pixels and at least " << opt.seam_min_length << " in total (robust z >= " << opt.seam_z << ", step share " << opt.seam_share_lo << "-"
     << opt.seam_share_hi
     << "). Pasted strips from another acquisition produce them; so can a straight membrane edge or a "
        "stitched acquisition.\n\n";
  if (result.seams.empty()) {
    md << "None found.\n";
  } else {
    md << "| Image | Direction | Position | Extent | Step (counts) | Step / noise | z | Share |\n"
          "|---|---|---|---|---|---|---|---|\n";
    for (const auto& s : result.seams)
      md << "| " << s.image + 1 << " | " << (s.vertical ? "column" : "row") << " | " << s.position << " | "
         << s.start << "-" << s.end << " | " << fmt(s.step, 1) << " | " << fmt(s.step_noise, 1) << " | "
         << fmt(s.z, 1) << " | " << fmt(s.share, 2) << " |\n";
  }
  md << "\n## Flat regions (lower confidence)\n\n" << opt.flat_window << "x" << opt.flat_window
     << " windows whose residual noise is below " << opt.flat_ratio
     << " x the noise of the image's quietest decile of windows, excluding saturated and minimum-clipped windows. Erased or "
        "painted areas produce them; so can processed exports.\n\n";
  if (result.flats.empty()) {
    md << "None found.\n";
  } else {
    md << "| Image | Region (x0,y0-x1,y1) | Residual noise | Typical |\n|---|---|---|---|\n";
    for (const auto& r : result.flats)
      md << "| " << r.image + 1 << " | " << rect_text(r.rect) << " | " << fmt(r.noise) << " | "
         << fmt(r.typical_noise) << " |\n";
  }
  md << "\n## Not covered\n\n* Copies that were rescaled, rotated by other than 180 degrees, blurred, or "
        "re-noised beyond about the image's own noise level.\n* Figures that went through JPEG compression or "
        "resampling: the noise this screening relies on is gone.\n* Masks: `mask_N.tif` holds the finding number "
        "at each flagged pixel of image N.\n\nRuntime " << fmt(result.seconds, 1) << " s.\n";
}

}  // namespace blotverse
