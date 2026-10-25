#!/usr/bin/env python3
"""Pool the exposure-invariance bench over every series (docs/exposure_protocol.md).

Usage: summarize.py RESULTS_DIR [SERIES_PREFIX ...]

RESULTS_DIR holds one sub-directory per series, each written by
`blotverse exposure`; with prefixes, only the series whose directory name
starts with one of them are pooled. Prints the pre-specified endpoints as Markdown:

  P1  QC misses: exposures with a compressive distortion in the spec's
      default branch and no lane flagged by the saturation rule (criterion: 0).
  P2  pair level, default branch: fraction of compressive-distorted pairs
      with a flagged lane (Wilson 95 % interval).
  S1  per branch: |delta log2 ratio| of clean pairs (no flag, above the SNR
      floor), pooled: n, median, 95th percentile.
  S2  per branch: median |delta| of flagged pairs (damage done by saturation).
  S3  per branch: clean pairs distorted beyond the threshold (not saturation).
Standard library only.
"""
import csv
import math
import re
import statistics
import sys
from collections import defaultdict
from pathlib import Path


def wilson(k, n, z=1.959964):
    if n == 0:
        return float("nan"), float("nan")
    p = k / n
    d = 1 + z * z / n
    c = (p + z * z / (2 * n)) / d
    h = z * math.sqrt(p * (1 - p) / n + z * z / (4 * n * n)) / d
    return c - h, c + h


def q95(v):
    v = sorted(v)
    if not v:
        return float("nan")
    pos = 0.95 * (len(v) - 1)
    lo = math.floor(pos)
    hi = min(lo + 1, len(v) - 1)
    return v[lo] + (pos - lo) * (v[hi] - v[lo])


def fmt(x, d=3):
    return "-" if x != x else f"{x:.{d}f}"


def main():
    root = Path(sys.argv[1])
    prefixes = tuple(sys.argv[2:])
    series = sorted(p for p in root.iterdir() if (p / "exposure_pairs.csv").exists()
                    and (not prefixes or p.name.startswith(prefixes)))
    default = None
    misses, not_evaluable, exposures_checked, flagged_exposures = [], [], 0, 0
    clean = defaultdict(list)
    flagged = defaultdict(list)
    threshold = None
    p2_k = p2_n = 0
    for s in series:
        md = (s / "exposure.md").read_text()
        m = re.search(r"G3 \(([^)]+)\)", md)
        if m:
            default = default or m.group(1)
        t = re.search(r"moves by more than ([0-9.]+) log2", md)
        if t:
            threshold = float(t.group(1))
        rows = list(csv.DictReader(open(s / "exposure_summary.csv")))
        for band in dict.fromkeys(r["band"] for r in rows):
            band_rows = [r for r in rows if r["band"] == band]
            # A band saturated at every exposure has no reference: not evaluable.
            if not any(r["reference"] == "1" for r in band_rows):
                not_evaluable.append(f"{s.name} band {band}")
                continue
            for r in band_rows:
                if r["excluded"] == "1" or r["misaligned"] == "1":
                    continue
                exposures_checked += 1
                flagged_exposures += r["qc_flagged"] == "1"
                if r["qc_miss"] == "1":
                    misses.append(f"{s.name} band {r['band']} exposure {r['exposure']} ({r['image']})")
        for r in csv.DictReader(open(s / "exposure_pairs.csv")):
            if r["dim"] == "1":
                continue
            branch = f"{r['background']}/{r['band_definition']}"
            a = abs(float(r["delta_log2"]))
            (flagged if r["qc_flagged"] == "1" else clean)[branch].append(a)
            if branch == default and r["compressive"] == "1":
                p2_n += 1
                p2_k += r["qc_flagged"] == "1"

    print(f"# Exposure-invariance bench: pooled results\n\n{len(series)} series "
          f"({', '.join(p.name for p in series)}); default branch `{default}`; "
          f"distortion threshold {threshold} log2.\n")
    print(f"## P1. QC misses\n\n{len(misses)} of {exposures_checked} usable exposure x band combinations "
          f"({flagged_exposures} carry a saturation flag).\n")
    for m in misses:
        print(f"* {m}")
    print(f"\nNot evaluable (a lane saturated at every exposure, so no reference): {len(not_evaluable)} bands"
          + (": " + "; ".join(not_evaluable) if not_evaluable else "") + ".")
    lo, hi = wilson(p2_k, p2_n)
    print(f"\n## P2. Compressive-distorted pairs with a flagged lane (default branch)\n\n"
          f"{p2_k} / {p2_n} = {fmt(p2_k / p2_n if p2_n else float('nan'))} (Wilson 95 % {fmt(lo)}-{fmt(hi)}).\n")
    print("## S1-S3. Per branch\n\n| Branch | Clean pairs | median abs delta | q95 abs delta | "
          f"Clean pairs > {threshold} | Flagged pairs | median abs delta (flagged) |\n|---|---|---|---|---|---|---|")
    for b in sorted(set(clean) | set(flagged)):
        c, f = clean[b], flagged[b]
        over = sum(x > threshold for x in c) if threshold is not None else 0
        print(f"| {b} | {len(c)} | {fmt(statistics.median(c) if c else float('nan'))} | {fmt(q95(c))} | "
              f"{over} | {len(f)} | {fmt(statistics.median(f) if f else float('nan'))} |")


if __name__ == "__main__":
    main()
