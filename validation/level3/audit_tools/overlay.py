"""Draw the reconciled annotation (experiment.yaml) on the raw images, for the human auditor.

    python -I overlay.py <experiment dir> <out dir>

For each blot writes <blot>_target_full.png, <blot>_target_zoom.png and the same for the
reference image. Display only: contrast is stretched and bands are shown dark on light.
Boxes are the measurement windows BlotVerse uses (lane_rect / band_window in detect.cpp).
"""
import sys
from pathlib import Path

import numpy as np
import tifffile
import yaml
from PIL import Image, ImageDraw, ImageFont

FONT = "/System/Library/Fonts/Supplemental/Arial Bold.ttf"
COLOURS = ["#d62728", "#1f77b4", "#2ca02c", "#9467bd", "#ff7f0e", "#8c564b"]


def raw(path):
    a = tifffile.imread(path).astype(float)
    if a.ndim == 3:  # RGB or multi-page: first plane / mean of channels
        a = a.mean(axis=2) if a.shape[2] in (3, 4) else a[0]
    return a


def load(a, invert, region=None):
    ref = a if region is None else a[region[1]:region[3], region[0]:region[2]]
    lo, hi = np.percentile(ref, [0.5, 99.8])
    a = np.clip((a - lo) / max(hi - lo, 1e-9), 0, 1)
    if not invert:  # bright bands: flip for display so bands look dark on white
        a = 1 - a
    return Image.fromarray((a * 255).astype(np.uint8)).convert("RGB")


def draw(img, ann, groups, scale_font):
    d = ImageDraw.Draw(img)
    font = ImageFont.truetype(FONT, scale_font)
    small = ImageFont.truetype(FONT, max(10, int(scale_font * 0.75)))
    order = list(dict.fromkeys(groups))
    h = ann["band_half_height"]
    ys = ann.get("reference_y") if ann.get("_use_ref") else ann["target_y"]
    for i, (lane, y) in enumerate(zip(ann["lanes"], ys)):
        c = COLOURS[order.index(groups[i]) % len(COLOURS)]
        x0 = round(lane["x"] - lane["width"] / 2)
        x1 = x0 + round(lane["width"])
        cy = round(y)
        d.rectangle([x0, cy - h, x1, cy + h + 1], outline=c, width=2)
        d.text(((x0 + x1) / 2, cy - h - 4), str(i + 1), fill=c, font=font, anchor="ms")
    # one bracket and label under each run of consecutive lanes of the same group
    i = 0
    bottom = max(round(y) for y in ys) + h + 8
    while i < len(groups):
        j = i
        while j + 1 < len(groups) and groups[j + 1] == groups[i]:
            j += 1
        c = COLOURS[order.index(groups[i]) % len(COLOURS)]
        xa = ann["lanes"][i]["x"] - ann["lanes"][i]["width"] / 2
        xb = ann["lanes"][j]["x"] + ann["lanes"][j]["width"] / 2
        d.line([xa, bottom, xb, bottom], fill=c, width=3)
        d.text(((xa + xb) / 2, bottom + 4), groups[i], fill=c, font=small, anchor="mt")
        i = j + 1
    return img


def zoom_box(size, ann, ys, pad_y=None):
    xs = [l["x"] for l in ann["lanes"]]
    w = max(l["width"] for l in ann["lanes"])
    h = ann["band_half_height"]
    pad_y = pad_y or max(60, 4 * h)
    return (max(0, int(min(xs) - 2 * w)), max(0, int(min(ys) - h - pad_y)),
            min(size[0], int(max(xs) + 2 * w)), min(size[1], int(max(ys) + h + pad_y)))


def main():
    d, out = Path(sys.argv[1]), Path(sys.argv[2])
    out.mkdir(parents=True, exist_ok=True)
    exp = yaml.safe_load(open(d / "experiment.yaml"))
    for b in exp["blots"]:
        groups = [l["group"] for l in b["lanes"]]
        inv = bool(b.get("invert"))
        jobs = [("target", b["image"], b["annotation"], "target_y")]
        if b.get("reference_image"):
            jobs.append(("reference", b["reference_image"], b["reference_annotation"], "reference_y"))
        else:  # reference band on the same image
            jobs.append(("reference", b["image"], b["annotation"], "reference_y"))
        for kind, f, ann, key in jobs:
            ann = dict(ann)
            ys = ann[key]
            ann["_use_ref"] = key == "reference_y"
            a = raw(d / f)
            base = load(a, inv)
            fs = max(14, base.width // 60)
            full = draw(base.copy(), ann, groups, fs)
            full.save(out / f"{b['id']}_{kind}_full.png")
            box = zoom_box(base.size, ann, ys, pad_y=None if ann["band_half_height"] < 60 else 40)
            z = draw(load(a, inv, box), ann, groups, fs).crop(box)
            if z.width < 1400:
                k = 1400 / z.width
                z = z.resize((1400, int(z.height * k)), Image.LANCZOS)
            z.save(out / f"{b['id']}_{kind}_zoom.png")
            print(out / f"{b['id']}_{kind}_zoom.png", base.size)


if __name__ == "__main__":
    main()
