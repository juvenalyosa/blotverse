#!/usr/bin/env python3
"""Level-3 screening, criterion 2: does a candidate's deposit hold a raw acquisition?

For each candidate in candidates.csv (screening order) it lists the deposited
files, reads TIFF headers with HTTP range requests (8-bit and colour files are
settled from the header alone) and downloads the remaining image files, which
`blotverse inspect` judges. It stops at the first raw acquisition of a
candidate: whether the panel's own files qualify is checked when the panel is
chosen. Zip archives are opened and their image members checked the same way.

    python3 -I validation/level3/screen_files.py --blotverse build/apps/blotverse \
        --data DATA_DIR --out validation/level3/criterion2.tsv [--first N]

With --blot-names --only criterion2.tsv it rescreens the candidates that passed,
counting only files named as blots.

Downloads go to DATA_DIR/<doi-slug>/images/. Rows already in the output file
are kept, so the script can be resumed.
"""
import argparse
import concurrent.futures as cf
import csv
import io
import json
import re
import struct
import subprocess
import sys
import threading
import time
import urllib.error
import urllib.parse
import urllib.request
import zipfile
from pathlib import Path

IMAGE_EXT = (".scn", ".gel", ".tif", ".tiff")
MAX_ZIP = 4 * 1024 ** 3  # archives larger than this are not opened (logged)
RANGE_MIN = 50 * 1024 ** 2  # TIFFs above this size are first judged from their header
# --blot-names: file or member names that mark a blot (first-pass hits were often microscopy)
BLOT_NAME = re.compile(r"blot|western|(?<![a-z])wb(?![a-z])|uncrop|membrane|ponceau|stain.?free|"
                       r"actin|gapdh|tubulin|vinculin|\.scn$|\.gel$")
UA = {"User-Agent": "blotverse-level3-screening"}


def request(url, headers=None, data=None, tries=4):
    for attempt in range(tries):
        try:
            return urllib.request.urlopen(urllib.request.Request(url, data=data, headers={**UA, **(headers or {})}),
                                          timeout=120)
        except urllib.error.HTTPError as e:
            if e.code in (400, 401, 403, 404, 410):
                raise
            time.sleep(2 ** attempt)
        except (urllib.error.URLError, TimeoutError):
            time.sleep(2 ** attempt)
    raise RuntimeError(f"failed after {tries} tries: {url}")


def get_json(url):
    with request(url, {"Accept": "application/json"}) as r:
        return json.load(r)


def slug(doi):
    return re.sub(r"[^a-z0-9]+", "_", doi.lower()).strip("_")


# --- file listings: (name, size, url) per dataset -----------------------------

def files_embo(acc):
    d = get_json(f"https://www.ebi.ac.uk/biostudies/api/v1/studies/{acc}")
    info = get_json(f"https://www.ebi.ac.uk/biostudies/api/v1/studies/{acc}/info")
    base = info["httpLink"].rstrip("/") + "/Files/"

    def walk(s):
        for f in s.get("files", []):
            for x in f if isinstance(f, list) else [f]:
                yield x
        for sub in s.get("subsections", []):
            for y in sub if isinstance(sub, list) else [sub]:
                yield from walk(y)
    return [(x["path"], x.get("size", 0), base + urllib.parse.quote(x["path"])) for x in walk(d["section"])
            if x.get("type", "file") == "file"]


def files_zenodo(rid):
    d = get_json(f"https://zenodo.org/api/records/{rid}")
    return [(f["key"], f["size"], f["links"]["self"]) for f in d.get("files", [])]


def files_mendeley(mid):
    d = get_json(f"https://data.mendeley.com/public-api/datasets/{mid}")
    return [(f["filename"], f.get("size", 0), f["content_details"]["download_url"]) for f in d.get("files", [])]


def files_dryad(doi):
    d = get_json("https://datadryad.org/api/v2/datasets/" + urllib.parse.quote(doi, safe=""))
    v = get_json("https://datadryad.org" + d["_links"]["stash:version"]["href"] + "/files")
    out = []
    for f in v.get("_embedded", {}).get("stash:files", []):
        out.append((f["path"], f.get("size", 0), "https://datadryad.org" + f["_links"]["stash:download"]["href"]))
    return out


def files_dataverse(pid, host="dataverse.harvard.edu"):
    d = get_json(f"https://{host}/api/datasets/:persistentId/?persistentId={pid}")
    return [(f["dataFile"]["filename"], f["dataFile"].get("filesize", 0),
             f"https://{host}/api/access/datafile/{f['dataFile']['id']}?format=original")
            for f in d["data"]["latestVersion"]["files"]]


def list_files(dataset):
    if dataset.startswith("S-SCDT-"):
        return files_embo(dataset)
    kind, _, ident = dataset.partition(":")
    if kind == "zenodo":
        return files_zenodo(ident)
    if kind == "mendeley":
        return files_mendeley(ident)
    if dataset.startswith("doi:10.5061/"):
        return files_dryad(dataset)
    if dataset.startswith("doi:10.7910/"):
        return files_dataverse(dataset)
    raise ValueError(f"unknown dataset kind: {dataset}")


# --- TIFF header from a range request -----------------------------------------

def read_range(url, start, length):
    """Bytes [start, start+length) or None when the server ignores ranges."""
    with request(url, {"Range": f"bytes={start}-{start + length - 1}"}) as r:
        if r.status != 206:
            return None
        return r.read()


def tiff_header(url):
    """(bits per sample, samples per pixel) from the first IFD, or None if unknown."""
    head = read_range(url, 0, 16)
    if not head or head[:2] not in (b"II", b"MM"):
        return None
    e = "<" if head[:2] == b"II" else ">"
    magic = struct.unpack(e + "H", head[2:4])[0]
    if magic == 43:  # BigTIFF
        off = struct.unpack(e + "Q", head[8:16])[0]
        block = read_range(url, off, 8 + 20 * 64)
        n = struct.unpack(e + "Q", block[:8])[0]
        entries = [block[8 + 20 * i: 28 + 20 * i] for i in range(min(n, 64))]
        parse = lambda b: (struct.unpack(e + "HHQ", b[:12]), b[12:20])
    elif magic == 42:
        off = struct.unpack(e + "I", head[4:8])[0]
        block = read_range(url, off, 2 + 12 * 64)
        n = struct.unpack(e + "H", block[:2])[0]
        entries = [block[2 + 12 * i: 14 + 12 * i] for i in range(min(n, 64))]
        parse = lambda b: (struct.unpack(e + "HHI", b[:8]), b[8:12])
    else:
        return None
    bits = samples = None
    for b in entries:
        (tag, typ, count), value = parse(b)
        if tag == 258 and typ == 3:  # BitsPerSample, SHORT
            bits = struct.unpack(e + "H", value[:2])[0] if count * 2 <= len(value) else None
            if bits is None:  # values stored elsewhere: all samples share one depth in practice
                ptr = struct.unpack(e + ("Q" if magic == 43 else "I"), value)[0]
                bits = struct.unpack(e + "H", read_range(url, ptr, 2))[0]
        elif tag == 277:  # SamplesPerPixel
            samples = struct.unpack(e + "H", value[:2])[0]
    return (bits or 1, samples or 1)


# --- screening ----------------------------------------------------------------

def inspect(blotverse, path):
    out = subprocess.run([blotverse, "inspect", str(path)], capture_output=True, text=True).stdout.splitlines()
    cols = out[1].split("\t") if len(out) > 1 else []
    return (cols[8] == "yes", cols[9]) if len(cols) > 9 else (False, "inspect failed")


def download(url, dest):
    dest.parent.mkdir(parents=True, exist_ok=True)
    if not dest.exists():
        with request(url) as r, open(str(dest) + ".part", "wb") as f:
            while chunk := r.read(1 << 20):
                f.write(chunk)
        Path(str(dest) + ".part").rename(dest)
    return dest


def screen(row, blotverse, data_dir, blot_names=False):
    images_dir = data_dir / slug(row["doi"]) / "images"
    checked, notes = 0, []
    for dataset in row["datasets"].split(";"):
        try:
            files = list_files(dataset)
        except Exception as e:  # noqa: BLE001 - logged, screening continues
            notes.append(f"{dataset}: file list failed ({e})")
            continue
        for name, size, url in files:
            low = name.lower()
            if low.endswith(IMAGE_EXT):
                if blot_names and not BLOT_NAME.search(low):
                    continue
                checked += 1
                # Header-only check for large files; some servers (EBI) answer range
                # requests far more slowly than a whole small file.
                if low.endswith((".tif", ".tiff", ".gel")) and size > RANGE_MIN:
                    try:
                        h = tiff_header(url)
                    except Exception:  # noqa: BLE001
                        h = None
                    if h and h[0] == 8:
                        notes.append(f"{name}: 8-bit (header)")
                        continue
                    if h and h[1] > 1:
                        notes.append(f"{name}: {h[1]} samples per pixel (header)")
                        continue
                try:
                    path = download(url, images_dir / Path(name).name)
                except Exception as e:  # noqa: BLE001
                    notes.append(f"{name}: download failed ({e})")
                    continue
                ok, reason = inspect(blotverse, path)
                if ok:
                    return "yes", checked, name, notes
                notes.append(f"{name}: {reason}")
            elif low.endswith(".zip"):
                if size and size > MAX_ZIP:
                    notes.append(f"{name}: archive of {size / 1024 ** 3:.1f} GB not opened")
                    continue
                try:
                    zpath = download(url, images_dir.parent / "archives" / Path(name).name)
                    with zipfile.ZipFile(zpath) as z:
                        members = [m for m in z.namelist()
                                   if m.lower().endswith(IMAGE_EXT) and not m.startswith("__MACOSX")
                                   and (not blot_names or BLOT_NAME.search((name + "/" + m).lower()))]
                        for m in members:
                            checked += 1
                            target = images_dir / (Path(name).stem + "__" + Path(m).name)
                            if not target.exists():
                                target.parent.mkdir(parents=True, exist_ok=True)
                                target.write_bytes(z.read(m))
                            ok, reason = inspect(blotverse, target)
                            if ok:
                                return "yes", checked, f"{name}/{m}", notes
                            notes.append(f"{name}/{m}: {reason}")
                except Exception as e:  # noqa: BLE001
                    notes.append(f"{name}: archive failed ({e})")
            elif low.endswith((".rar", ".7z", ".tar", ".gz")):
                notes.append(f"{name}: archive format not opened")
    return ("no" if checked else "no image files"), checked, "", notes


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--blotverse", required=True)
    ap.add_argument("--data", required=True, type=Path)
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("--candidates", type=Path, default=Path(__file__).with_name("candidates.csv"))
    ap.add_argument("--first", type=int, default=0, help="screen only the first N candidates")
    ap.add_argument("--only", type=Path, help="screen only the DOIs marked yes in this criterion-2 table")
    ap.add_argument("--blot-names", action="store_true", help="count only files whose name marks a blot")
    ap.add_argument("--jobs", type=int, default=6, help="candidates screened in parallel")
    args = ap.parse_args()
    rows = list(csv.DictReader(open(args.candidates)))
    if args.first:
        rows = rows[: args.first]
    if args.only:
        keep = {r["doi"] for r in csv.DictReader(open(args.only), delimiter="\t") if r["raw_acquisition"] == "yes"}
        rows = [r for r in rows if r["doi"] in keep]
    done = set()
    if args.out.exists():
        done = {r["doi"] for r in csv.DictReader(open(args.out), delimiter="\t")}
    new = not args.out.exists()
    with open(args.out, "a", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        if new:
            w.writerow(["rank", "doi", "raw_acquisition", "image_files_checked", "first_raw_file", "notes"])
        lock = threading.Lock()

        def one(row):
            result, checked, first, notes = screen(row, args.blotverse, args.data, args.blot_names)
            with lock:
                w.writerow([row["rank"], row["doi"], result, checked, first, " | ".join(notes)[:2000]])
                f.flush()
                print(row["rank"], row["doi"], result, checked, file=sys.stderr)

        with cf.ThreadPoolExecutor(args.jobs) as pool:
            list(pool.map(one, [r for r in rows if r["doi"] not in done]))


if __name__ == "__main__":
    main()
