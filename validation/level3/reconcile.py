#!/usr/bin/env python3
"""Level 3: reconcile annotator A and B into experiment.yaml (protocol, *Saving and checking*).

    python3 -I validation/level3/reconcile.py validation/level3/<doi-slug>

* Lane-to-group mapping: must be identical (same blots and images, same group and replicate
  order). Any difference is listed and the script stops: the human auditor settles it.
* Geometry (lane centres and widths, band rows, half-heights): the mean of the two.
* Sample names are rewritten as <group>_<index>, since the annotators name samples
  differently.

Writes <dir>/experiment.yaml and prints the largest A-B difference of each quantity.
Needs PyYAML.
"""
import sys
from pathlib import Path

import yaml

GEOMETRY = ("band_half_height", "target_y", "reference_y")


def mapping(blot):
    return [l["group"] for l in blot["lanes"]]


def mean(a, b):
    m = (a + b) / 2
    return round(m, 1)


def merge_annotation(a, b, diffs, where):
    out = {"source": "manual"}
    if len(a["lanes"]) != len(b["lanes"]):
        raise SystemExit(f"{where}: {len(a['lanes'])} vs {len(b['lanes'])} lanes - auditor")
    # BlotVerse reads the half-height as an integer: round the mean half up
    out["band_half_height"] = int((a["band_half_height"] + b["band_half_height"]) / 2 + 0.5)
    diffs[f"{where} half-height"] = abs(a["band_half_height"] - b["band_half_height"])
    out["lanes"] = [{"x": mean(la["x"], lb["x"]), "width": mean(la["width"], lb["width"])}
                    for la, lb in zip(a["lanes"], b["lanes"])]
    diffs[f"{where} lane x"] = max(abs(la["x"] - lb["x"]) for la, lb in zip(a["lanes"], b["lanes"]))
    diffs[f"{where} lane width"] = max(abs(la["width"] - lb["width"])
                                       for la, lb in zip(a["lanes"], b["lanes"]))
    for key in ("target_y", "reference_y"):
        if key in a and key in b:
            out[key] = [mean(ya, yb) for ya, yb in zip(a[key], b[key])]
            diffs[f"{where} {key}"] = max(abs(ya - yb) for ya, yb in zip(a[key], b[key]))
    return out


def explicit_reference(blot, other):
    """A loading control on the same image can be written two ways: reference rows in the
    target annotation, or reference_image = the same file with its own reference_annotation.
    When the other annotator used the second form, rewrite the first into it, keeping this
    annotator's lanes, reference rows and half-height. Lane groups are not touched."""
    if blot.get("reference_image") or other.get("reference_image") != blot["image"]:
        return False
    ann = blot["annotation"]
    blot["reference_image"] = blot["image"]
    blot["reference_annotation"] = {"lanes": ann["lanes"], "target_y": ann["reference_y"],
                                    "reference_y": ann["reference_y"],
                                    "band_half_height": ann["band_half_height"]}
    ann["reference_y"] = ann["target_y"]
    return True


def main():
    d = Path(sys.argv[1])
    a = yaml.safe_load(open(d / "annotator_a.yaml"))
    b = yaml.safe_load(open(d / "annotator_b.yaml"))
    for i, (ba, bb) in enumerate(zip(a.get("blots", []), b.get("blots", []))):
        for name, x, y in (("A", ba, bb), ("B", bb, ba)):
            if explicit_reference(x, y):
                print(f"blot {i + 1}: annotator {name}'s same-image reference rewritten as "
                      "reference_image + reference_annotation (geometry kept)")
    problems = []
    if a.get("control_group") != b.get("control_group"):
        problems.append(f"control group: {a.get('control_group')} vs {b.get('control_group')}")
    if len(a["blots"]) != len(b["blots"]):
        problems.append(f"{len(a['blots'])} vs {len(b['blots'])} blots")
    for i, (ba, bb) in enumerate(zip(a["blots"], b["blots"])):
        for key in ("image", "reference_image", "invert"):
            if ba.get(key, False) != bb.get(key, False):
                problems.append(f"blot {i + 1} {key}: {ba.get(key)} vs {bb.get(key)}")
        if mapping(ba) != mapping(bb):
            problems.append(f"blot {i + 1} lane groups: {mapping(ba)} vs {mapping(bb)}")
    if problems:
        print("Mapping disagreements (settled by the human auditor):", *problems, sep="\n  ")
        raise SystemExit(1)

    diffs = {}
    blots = []
    for i, (ba, bb) in enumerate(zip(a["blots"], b["blots"])):
        count = {}
        lanes = []
        for g in mapping(ba):
            count[g] = count.get(g, 0) + 1
            lanes.append({"group": g, "sample": f"{g.replace(' ', '_')}_{count[g]}"})
        blot = {"id": f"blot{i + 1}", "image": ba["image"]}
        if ba.get("invert"):
            blot["invert"] = True
        if ba.get("reference_image"):
            blot["reference_image"] = ba["reference_image"]
        blot["lanes"] = lanes
        blot["annotation"] = merge_annotation(ba["annotation"], bb["annotation"], diffs, f"blot {i + 1}")
        if "reference_annotation" in ba:
            blot["reference_annotation"] = merge_annotation(ba["reference_annotation"],
                                                            bb["reference_annotation"], diffs,
                                                            f"blot {i + 1} reference")
        blots.append(blot)
    exp = {"name": d.name, "control_group": a["control_group"], "blots": blots}
    header = ("# Reconciled annotation (validation/level3/reconcile.py): lane mapping identical in\n"
              "# annotator_a.yaml and annotator_b.yaml; geometry is the mean of the two.\n")
    (d / "experiment.yaml").write_text(header + yaml.safe_dump(exp, sort_keys=False,
                                                                default_flow_style=None, width=100))
    print("max |A - B|:")
    for k, v in diffs.items():
        print(f"  {k}: {v:g}")


if __name__ == "__main__":
    main()
