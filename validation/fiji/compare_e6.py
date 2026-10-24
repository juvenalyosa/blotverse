#!/usr/bin/env python3
"""E6: Pearson correlation between BlotVerse rolling-ball band values and Fiji.

Usage: compare_e6.py [--factor N] OUT_DIR [OUT_DIR ...]
Each OUT_DIR must contain lanes.csv (blotverse run) and fiji_rb<N>_rect.csv
(e6_fiji.ijm). N defaults to 2 (the default branch). Standard library only.
"""
import csv
import math
import sys
from pathlib import Path


def pearson(xs, ys):
    n = len(xs)
    mx, my = sum(xs) / n, sum(ys) / n
    sxy = sum((x - mx) * (y - my) for x, y in zip(xs, ys))
    sxx = sum((x - mx) ** 2 for x in xs)
    syy = sum((y - my) ** 2 for y in ys)
    return sxy / math.sqrt(sxx * syy)


def main(argv):
    factor = "2"
    if argv[:1] == ["--factor"]:
        factor, argv = argv[1], argv[2:]
    column = f"rb{factor}_rect"
    dirs = argv
    ours, fiji = [], []
    for d in map(Path, dirs):
        with open(d / "lanes.csv") as f:
            bv = {(r["blot"], r["lane"], r["band"]): float(r[column]) for r in csv.DictReader(f)}
        with open(d / f"fiji_{column}.csv") as f:
            for r in csv.DictReader(f):
                key = (r["blot"], r["lane"], r["band"])
                ours.append(bv[key])
                fiji.append(float(r[f"fiji_{column}"]))
    r = pearson(ours, fiji)
    rel = [abs(a - b) / abs(b) for a, b in zip(ours, fiji) if b]
    print(f"branch {column}: bands compared: {len(ours)}")
    print(f"Pearson r = {r:.5f}  (target >= 0.99: {'PASS' if r >= 0.99 else 'FAIL'})")
    print(f"median relative difference = {sorted(rel)[len(rel) // 2]:.4%}, max = {max(rel):.4%}")
    return 0 if r >= 0.99 else 1


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    sys.exit(main(sys.argv[1:]))
