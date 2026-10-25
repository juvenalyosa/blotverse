#!/usr/bin/env python3
"""Refine approximate lane centres and a band row on a raw blot image.

Usage: refine_lanes.py IMAGE.tif NAME Y X1 X2 ... [--search 25] [--band-search 20]

Prints a BlotVerse measurement config (YAML) with lane centres refined to the
centroid of the background-subtracted column signal around each approximate
x, per-lane band rows (row maximum near Y) and a band half-height from the
full width at half maximum. The approximate positions are read off the image
by eye; this script only removes the eyeballing error. Needs numpy + tifffile.
"""
import argparse
import numpy as np
import tifffile

ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
ap.add_argument("image")
ap.add_argument("name")
ap.add_argument("y", type=int, help="approximate band row")
ap.add_argument("x", type=int, nargs="+", help="approximate lane centres, left to right")
ap.add_argument("--search", type=int, default=25, help="+/- columns searched around each x")
ap.add_argument("--band-search", type=int, default=20, help="+/- rows searched around y")
args = ap.parse_args()
image, name, y0, xs = args.image, args.name, args.y, args.x
search, band_search = args.search, args.band_search

a = tifffile.imread(image).astype(float)
if a.ndim == 3:
    a = a[0]
# Local background: the membrane around the band (rows 2-3 band-searches
# away), not the whole image, which includes the empty area off the membrane.
far = np.r_[max(0, y0 - 3 * band_search):max(0, y0 - 2 * band_search),
            y0 + 2 * band_search:min(a.shape[0], y0 + 3 * band_search)]
lane_span = slice(max(0, min(xs) - search), min(a.shape[1], max(xs) + search))
bg = float(np.median(a[far][:, lane_span]))

# Lane centres: centroid of the column signal in the band rows.
rows = slice(max(0, y0 - band_search), y0 + band_search)
col = (a[rows] - bg).clip(0).sum(0)
centres = []
for x in xs:
    lo, hi = max(0, x - search), min(a.shape[1], x + search)
    seg = col[lo:hi]
    centres.append(float((seg * np.arange(lo, hi)).sum() / seg.sum()) if seg.sum() > 0 else float(x))
pitch = np.median(np.diff(centres)) if len(centres) > 1 else 2 * search
width = round(0.75 * pitch)

# Band rows per lane, and the band height from the mean profile.
ys, profile = [], np.zeros(2 * band_search)
for c in centres:
    p = (a[y0 - band_search:y0 + band_search, int(c) - width // 4:int(c) + width // 4] - bg).clip(0).mean(1)
    ys.append(int(y0 - band_search + np.argmax(p)))
    profile += p
peak = int(np.argmax(profile))
half = profile[peak] / 2
lo = peak
while lo > 0 and profile[lo] > half:
    lo -= 1
hi = peak
while hi < len(profile) - 1 and profile[hi] > half:
    hi += 1
half_height = max(3, int(np.ceil(1.1 * (hi - lo))))

print(f"name: {name}")
print(f"image: {image.split('/')[-1]}")
print(f"lane_width: {width}")
print(f"band_half_height: {half_height}")
print("refine_rows: 3")
print("lanes:")
for c in centres:
    print(f"  - {{x: {c:.1f}}}")
print("bands:")
print(f"  - {{name: band, y: {int(np.median(ys))}}}")
