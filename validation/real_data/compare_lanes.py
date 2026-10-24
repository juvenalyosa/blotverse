#!/usr/bin/env python3
"""Compare BlotVerse lane centres with an author's Image Lab lanes.

Usage: compare_lanes.py IMAGELAB_TSV BLOTVERSE_TSV
"""
import sys


def centres(path):
    rows = [line.split("\t") for line in open(path).read().splitlines()[1:] if line]
    return [float(r[1]) for r in rows]


il, bv = centres(sys.argv[1]), centres(sys.argv[2])
if len(il) != len(bv):
    sys.exit(f"lane counts differ: {len(il)} vs {len(bv)}")
d = [abs(a - b) for a, b in zip(il, bv)]
pitch = (il[-1] - il[0]) / (len(il) - 1)
print(f"lanes {len(il)}, pitch {pitch:.0f} px: mean |dx| = {sum(d) / len(d):.1f} px, max = {max(d):.1f} px")
