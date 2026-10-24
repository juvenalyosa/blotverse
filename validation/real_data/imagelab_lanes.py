#!/usr/bin/env python3
"""Print the lane centres an author saved with Bio-Rad Image Lab in a .scn file.

Usage: imagelab_lanes.py FILE.scn
Output (TSV): lane, x_center, width, band rows... (image pixels). The lane
centre is taken at the first band's row (lanes may be slanted). Standard
library only; the file is read as data.
"""
import sys
import xml.etree.ElementTree as ET


def lanes(path):
    data = open(path, "rb").read()
    start = data.find(b"Content-Description: ItemLaneAndBandDataTag")
    if start < 0:
        sys.exit(f"{path}: no Image Lab lane analysis saved in this file")
    body = data[data.find(b"\r\n\r\n", start) + 4:]
    text = body[: body.find(b"</root>") + 7].decode("utf8").replace("<!DOCTYPE XML>", "")
    frame = ET.fromstring(text).find(".//lane_frame")
    x0, x1 = float(frame.find("top_left").get("x")), float(frame.find("top_right").get("x"))
    y0, y1 = float(frame.find("top_left").get("y")), float(frame.find("bottom_left").get("y"))
    for lane in frame.find("lanes"):
        top, bottom = float(lane.find("top_anchor").text), float(lane.find("bottom_anchor").text)
        bands = lane.find("bands")
        rows = [y0 + float(b.get("rel_peak")) * (y1 - y0) for b in (bands if bands is not None else [])]
        t = ((rows[0] if rows else (y0 + y1) / 2) - y0) / (y1 - y0)
        yield x0 + (top + (bottom - top) * t) * (x1 - x0), float(lane.find("lane_width").text), rows


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    print("lane\tx_center\twidth\tbands")
    for i, (x, w, rows) in enumerate(lanes(sys.argv[1]), 1):
        print(f"{i}\t{x:.1f}\t{w:.0f}\t" + ",".join(f"{r:.1f}" for r in rows))
