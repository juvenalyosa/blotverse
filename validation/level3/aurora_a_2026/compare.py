#!/usr/bin/env python3
"""Level-3 check on AURORA A Fig. 3G (EMBO Rep 2026, doi:10.1038/s44319-026-00850-0).

Compares BlotVerse lane signals (every background x band-definition branch)
with the authors' ImageQuant signals in 3G_WB_quantification.xlsx, for the
three HA-IP lanes (WT, mut1, mut2) of each replicate. HA of replicate 3 is
excluded: the deposited HA_R3 image holds four strips and the files do not
say which one is replicate 3.

Usage: compare.py MEASURE_DIR AUTHORS_XLSX
MEASURE_DIR holds m_<IMAGE>/dilution.csv from `blotverse dilution`.
Needs openpyxl.
"""
import csv
import math
import sys
from collections import defaultdict
from pathlib import Path

import openpyxl

LANES = ["WT", "mut1", "mut2"]
IMAGES = {"DICER_R1": ("DICER", "R1"), "HA_R1": ("HA", "R1"), "DICER_R2": ("DICER", "R2"),
          "HA_R2": ("HA", "R2"), "DICER_R3": ("DICER", "R3")}


def authors(xlsx):
    """{(protein, replicate, lane): signal} from the authors' workbook."""
    wb = openpyxl.load_workbook(xlsx, data_only=True)
    out = {}
    for rep in ("R1", "R2", "R3"):
        protein = None
        for row in wb[rep].iter_rows(values_only=True):
            if not row or row[0] is None:
                continue
            if row[0] == "Name":
                protein = "DICER" if "DICER" in str(row[1]) else "HA"
                continue
            if protein and str(row[0]).startswith("HA-IP") and isinstance(row[1], (int, float)):
                lane = {"WT": "WT", "mut1": "mut1", "mut2": "mut2"}[str(row[0]).split()[-1]]
                out.setdefault((protein, rep, lane), float(row[1]))
            if row[0] and str(row[0]).startswith("DICER Signal"):
                protein = None
    return out


def ours(measure_dir):
    """{(branch): {(protein, replicate, lane): signal}}."""
    out = defaultdict(dict)
    for image, (protein, rep) in IMAGES.items():
        rows = list(csv.DictReader(open(Path(measure_dir) / f"m_{image}" / "dilution.csv")))
        by_branch = defaultdict(list)
        for r in rows:
            by_branch[f"{r['background']}/{r['band_definition']}"].append(float(r["signal"]))
        for branch, values in by_branch.items():
            for lane, v in zip(LANES, values):
                out[branch][(protein, rep, lane)] = v
    return out


def pearson(x, y):
    mx, my = sum(x) / len(x), sum(y) / len(y)
    sxy = sum((a - mx) * (b - my) for a, b in zip(x, y))
    return sxy / math.sqrt(sum((a - mx) ** 2 for a in x) * sum((b - my) ** 2 for b in y))


def main(measure_dir, xlsx):
    ref = authors(xlsx)
    keys = [k for k in ref if (k[0], k[1]) != ("HA", "R3")]
    print(f"{len(keys)} lane measurements compared (HA R3 excluded as ambiguous)\n")
    print("| Branch | r (log signals) | median |rel. diff| | log2 FC mut1/WT, R1, R2 (authors) | log2 FC mut2/WT, R1, R2 (authors) |")
    print("|---|---|---|---|---|")

    def fc(values, rep, lane):
        ratio = lambda l: values[("DICER", rep, l)] / values[("HA", rep, l)]
        return math.log2(ratio(lane) / ratio("WT")) if ratio(lane) > 0 and ratio("WT") > 0 else float("nan")

    authors_fc = {(rep, lane): fc(ref, rep, lane) for rep in ("R1", "R2") for lane in ("mut1", "mut2")}
    for branch, values in sorted(ours(measure_dir).items()):
        xs = [math.log10(max(values[k], 1.0)) for k in keys]
        ys = [math.log10(ref[k]) for k in keys]
        rel = sorted(abs(values[k] - ref[k]) / ref[k] for k in keys)
        fcs = {k: fc(values, *k) for k in authors_fc}
        cell = lambda lane: ", ".join(f"{fcs[(rep, lane)]:+.2f} ({authors_fc[(rep, lane)]:+.2f})" for rep in ("R1", "R2"))
        print(f"| {branch} | {pearson(xs, ys):.3f} | {rel[len(rel) // 2]:.0%} | {cell('mut1')} | {cell('mut2')} |")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    main(*sys.argv[1:])
