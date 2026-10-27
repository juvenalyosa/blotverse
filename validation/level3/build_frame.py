#!/usr/bin/env python3
"""Builds the level-3 sampling frame (docs/level3_protocol.md, "Sampling frame").

Queries every repository of the protocol through its public API, keeps the hits
that link to an article, merges them by article DOI, drops the development
datasets and writes candidates.csv in screening order.

    python3 -I validation/level3/build_frame.py --out validation/level3

Only repository metadata is read: no image is downloaded.
"""
import argparse
import concurrent.futures as cf
import csv
import datetime as dt
import hashlib
import json
import re
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path

START = "2020-01-01"
FREEZE = "2026-10-27"
TODAY = dt.date.today().isoformat()

IMAGE_EXT = (".scn", ".gel", ".tif", ".tiff")
BLOT_WORD = re.compile(r"blot|(?<![a-z])wb(?![a-z])", re.I)
WESTERN = re.compile(r"western[\s_-]*blot", re.I)
DOI = re.compile(r"\b(10\.\d{4,9}/[^\s\"'<>,;)\]\u201c\u201d\u2018\u2019]+)", re.I)
# Data repositories: a DOI under these prefixes is a dataset, not an article.
DATA_PREFIXES = ("10.5281/", "10.17632/", "10.6084/", "10.5061/", "10.7910/", "10.15139/", "10.6019/")

# Development datasets (protocol, "Excluded from the frame").
EXCLUDED_EMBO = {
    "S44319-026-00850-0", "S44318-026-00753-9", "S44319-026-00914-1", "S44318-025-00572-4",
    "S44321-025-00317-0", "S44319-025-00521-6",
}
# ... and every study screened for the exposure bench.
EXCLUDED_EMBO |= set(re.findall(r"S443\d\d-\d{3}-\d{5}-[0-9A-Z]",
                                (Path(__file__).resolve().parents[1] / "exposure" / "screening.md").read_text()))
EXCLUDED_DATASETS = {
    "doi:10.15139/s3/4esrc1", "doi:10.7910/dvn/ynd7fa", "zenodo:10050244", "zenodo:13752216",
    "zenodo:18299536", "zenodo:4598136", "figshare:4516847", "mendeley:czm3p6t2xk",
    "mendeley:vwcy56z5jh",
}


def get_json(url, data=None, tries=4):
    for attempt in range(tries):
        try:
            req = urllib.request.Request(url, data=data, headers={
                "Accept": "application/json", "User-Agent": "blotverse-level3-frame",
                **({"Content-Type": "application/json"} if data else {})})
            with urllib.request.urlopen(req, timeout=60) as r:
                return json.load(r)
        except (urllib.error.URLError, TimeoutError, json.JSONDecodeError) as e:
            if isinstance(e, urllib.error.HTTPError) and e.code in (400, 401, 403, 404):
                raise
            time.sleep(2 ** attempt)
    raise RuntimeError(f"failed after {tries} tries: {url}")


def clean_doi(s):
    s = s.strip().lower()
    s = re.sub(r"^(https?://(dx\.)?doi\.org/|doi:)", "", s)
    return s.rstrip(".")


def article_doi(candidates):
    """First DOI in `candidates` that is not a data-repository DOI."""
    for c in candidates:
        for m in DOI.findall(c or ""):
            d = clean_doi(m)
            if not d.startswith(DATA_PREFIXES):
                return d
    return None


def in_window(date):
    return bool(date) and START <= date[:10] <= FREEZE


class Frame:
    def __init__(self):
        self.rows = {}        # article DOI -> row
        self.log = []         # (repository, query, hits, kept, linked)
        self.dropped = set()  # article DOIs behind excluded datasets

    def add(self, doi, repo, dataset, query):
        if dataset.lower() in EXCLUDED_DATASETS:
            self.dropped.add(doi)
            return
        row = self.rows.setdefault(doi, {"doi": doi, "repositories": set(), "datasets": set(), "queries": set()})
        row["repositories"].add(repo)
        row["datasets"].add(dataset)
        row["queries"].add(query)


# --- EMBO SourceData ---------------------------------------------------------

def embo_files(section, context=""):
    """(file path, text of the enclosing sections) for every file in a study."""
    text = context + " " + " ".join(str(a.get("value", "")) for a in section.get("attributes", []))
    for f in section.get("files", []):
        for x in f if isinstance(f, list) else [f]:
            desc = " ".join(str(a.get("value", "")) for a in x.get("attributes", []))
            yield x.get("path", ""), text + " " + desc
    for sub in section.get("subsections", []):
        for y in sub if isinstance(sub, list) else [sub]:
            yield from embo_files(y, text)


def embo(frame):
    accessions = {}
    for query in ("blot", "WB"):
        page, hits = 1, 0
        while True:
            d = get_json("https://www.ebi.ac.uk/biostudies/api/v1/SourceData/search?" +
                         urllib.parse.urlencode({"query": query, "pageSize": 100, "page": page}))
            for h in d["hits"]:
                hits += 1
                if in_window(h.get("release_date")):
                    accessions.setdefault(h["accession"], query)
            if page * 100 >= d["totalHits"] or not d["hits"]:
                break
            page += 1
        frame.log.append(("EMBO SourceData", f"collection search '{query}'", hits, None, None))

    def check(acc):
        d = get_json(f"https://www.ebi.ac.uk/biostudies/api/v1/studies/{acc}")
        for path, text in embo_files(d["section"]):
            name = path.rsplit("/", 1)[-1]
            if name.lower().endswith(IMAGE_EXT) and (BLOT_WORD.search(name) or BLOT_WORD.search(text)):
                return True
        return False

    with cf.ThreadPoolExecutor(8) as pool:
        keep = dict(zip(accessions, pool.map(check, accessions)))
    kept = 0
    for acc, ok in keep.items():
        m = re.match(r"S-SCDT-10_(\d{4})-(.+)$", acc)
        if not ok or not m:
            continue
        short = m.group(2)
        doi = f"10.{m.group(1)}/{short}".lower()
        if short.upper() in EXCLUDED_EMBO:
            frame.dropped.add(doi)
            continue
        kept += 1
        frame.add(doi, "EMBO SourceData", acc, accessions[acc])
    frame.log.append(("EMBO SourceData", "studies in window / with a blot image file",
                      len(accessions), sum(keep.values()), kept))


# --- Zenodo ------------------------------------------------------------------

def zenodo(frame):
    q = f'"western blot" AND (raw OR uncropped OR original) AND publication_date:[{START} TO {FREEZE}]'
    page, hits, kept, linked = 1, 0, 0, 0
    while True:
        d = get_json("https://zenodo.org/api/records?" + urllib.parse.urlencode(
            {"q": q, "type": "dataset", "size": 25, "page": page}))
        for h in d["hits"]["hits"]:
            hits += 1
            md = h["metadata"]
            if not WESTERN.search(md.get("title", "") + " " + md.get("description", "")):
                continue
            kept += 1
            rel = [r.get("identifier", "") for r in md.get("related_identifiers", [])
                   if r.get("relation") in ("isSupplementTo", "isReferencedBy", "isCitedBy", "isPartOf",
                                            "isDocumentedBy", "isDescribedBy")]
            doi = article_doi(rel + [md.get("description", "")])
            if doi:
                linked += 1
                frame.add(doi, "Zenodo", f"zenodo:{h['id']}", "zenodo")
        if page * 25 >= d["hits"]["total"] or not d["hits"]["hits"]:
            break
        page += 1
    frame.log.append(("Zenodo", q, hits, kept, linked))


# --- Mendeley Data -----------------------------------------------------------

def mendeley(frame):
    records, page = [], 1
    while True:
        d = get_json("https://data.mendeley.com/api/research-data/search?" +
                     urllib.parse.urlencode({"query": '"western blot"', "size": 100, "page": page}))
        records += d["records"]
        if not d["records"] or len(records) >= d["total_records"]:
            break
        page += 1
    cand = [r for r in records if r.get("source", {}).get("name") == "Mendeley Data"
            and in_window(r.get("publication_date"))
            and WESTERN.search(" ".join([r.get("title", ""), r.get("description", ""), *r.get("categories", [])]))]

    def detail(r):
        try:
            d = get_json(f"https://data.mendeley.com/public-api/datasets/{r['id']}")
        except urllib.error.HTTPError:
            return r, None
        links = [a.get("doi") or a.get("url") or "" for a in d.get("articles", [])]
        links += [l.get("href") or l.get("url") or "" for l in d.get("related_links", [])]
        return r, article_doi(links + [d.get("description", "")])

    linked = 0
    with cf.ThreadPoolExecutor(8) as pool:
        for r, doi in pool.map(detail, cand):
            if doi:
                linked += 1
                frame.add(doi, "Mendeley Data", f"mendeley:{r['id']}", "mendeley")
    frame.log.append(("Mendeley Data", '"western blot"', len(records), len(cand), linked))


# --- Figshare ----------------------------------------------------------------

def figshare(frame):
    items, page = [], 1
    while True:
        # Only the title qualifier honours the phrase; ":description:" and plain
        # queries return the whole catalogue (Amendment 1).
        body = json.dumps({"search_for": ':title: "western blot"', "item_type": 3, "page_size": 1000,
                           "page": page, "published_since": START}).encode()
        d = get_json("https://api.figshare.com/v2/articles/search", data=body)
        items += d
        if len(d) < 1000:
            break
        page += 1

    def detail(it):
        d = get_json(f"https://api.figshare.com/v2/articles/{it['id']}")
        if not in_window(d.get("published_date")) or not WESTERN.search(d.get("title", "") + " " + d.get("description", "")):
            return it, False, None
        return it, True, article_doi([d.get("resource_doi") or "", *d.get("references", []), d.get("description", "")])

    kept = linked = 0
    with cf.ThreadPoolExecutor(8) as pool:
        for it, ok, doi in pool.map(detail, items):
            kept += ok
            if ok and doi:
                linked += 1
                frame.add(doi, "Figshare", f"figshare:{it['id']}", "figshare")
    frame.log.append(("Figshare", ':title: "western blot" (datasets)', len(items), kept, linked))


# --- Dryad -------------------------------------------------------------------

def dryad(frame):
    page, hits, kept, linked = 1, 0, 0, 0
    while True:
        d = get_json("https://datadryad.org/api/v2/search?" + urllib.parse.urlencode(
            {"q": '"western blot"', "per_page": 100, "page": page}))
        batch = d.get("_embedded", {}).get("stash:datasets", [])
        for x in batch:
            hits += 1
            if not in_window(x.get("publicationDate")) or not WESTERN.search(
                    x.get("title", "") + " " + x.get("abstract", "") + " " + x.get("methods", "")):
                continue
            kept += 1
            rel = [w["identifier"] for w in x.get("relatedWorks", [])
                   if w.get("relationship") in ("primary_article", "article")]
            doi = article_doi(rel)
            if doi:
                linked += 1
                frame.add(doi, "Dryad", x["identifier"].lower(), "dryad")
        if not batch or hits >= d.get("total", 0):
            break
        page += 1
    frame.log.append(("Dryad", '"western blot"', hits, kept, linked))


# --- Dataverse ---------------------------------------------------------------

def dataverse(frame, name, host):
    start, items = 0, []
    try:
        while True:
            d = get_json(f"https://{host}/api/search?" + urllib.parse.urlencode(
                {"q": '"western blot"', "type": "dataset", "per_page": 100, "start": start}))["data"]
            items += d["items"]
            start += 100
            if start >= d["total_count"]:
                break
    except urllib.error.HTTPError as e:
        frame.log.append((name, f'"western blot": search API refused ({e.code})', 0, 0, 0))
        return
    cand = [i for i in items if in_window(i.get("published_at"))
            and WESTERN.search(i.get("name", "") + " " + i.get("description", ""))]

    def detail(i):
        try:
            d = get_json(f"https://{host}/api/datasets/:persistentId/?persistentId={i['global_id']}")
        except urllib.error.HTTPError:  # restricted or deaccessioned
            return i, None
        fields = d["data"]["latestVersion"]["metadataBlocks"]["citation"]["fields"]
        links = []
        for f in fields:
            if f["typeName"] == "publication":
                for p in f["value"]:
                    links += [str(v.get("value", "")) for v in p.values() if isinstance(v, dict)]
        return i, article_doi(links)

    linked = 0
    with cf.ThreadPoolExecutor(8) as pool:
        for i, doi in pool.map(detail, cand):
            if doi:
                linked += 1
                frame.add(doi, name, i["global_id"].lower(), "dataverse")
    frame.log.append((name, '"western blot" (datasets)', len(items), len(cand), linked))


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--out", required=True, type=Path)
    args = ap.parse_args()
    frame = Frame()
    for step in (embo, zenodo, mendeley, figshare, dryad,
                 lambda f: dataverse(f, "Harvard Dataverse", "dataverse.harvard.edu"),
                 lambda f: dataverse(f, "UNC Dataverse", "dataverse.unc.edu")):
        step(frame)
        print(frame.log[-1], file=sys.stderr)

    rows = [r for d, r in frame.rows.items() if d not in frame.dropped]
    for r in rows:
        r["order"] = hashlib.sha256(("blotverse-level3-v1:" + r["doi"]).encode()).hexdigest()
    rows.sort(key=lambda r: r["order"])
    args.out.mkdir(parents=True, exist_ok=True)
    with open(args.out / "candidates.csv", "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["rank", "doi", "repositories", "datasets", "queries", "date_searched", "order_sha256"])
        for k, r in enumerate(rows, 1):
            w.writerow([k, r["doi"], ";".join(sorted(r["repositories"])), ";".join(sorted(r["datasets"])),
                        ";".join(sorted(r["queries"])), TODAY, r["order"]])
    with open(args.out / "frame_log.md", "w") as f:
        f.write(f"# Level-3 sampling frame: search log ({TODAY})\n\n"
                "Written by `build_frame.py`. *Hits*: records returned. *Kept*: in the date window and\n"
                "matching the text filter (EMBO: with a blot image file). *Linked*: linked to an article DOI.\n\n"
                "| Repository | Query | Hits | Kept | Linked |\n|---|---|---|---|---|\n")
        for repo, q, hits, kept, linked in frame.log:
            f.write(f"| {repo} | {q} | {hits} | {'' if kept is None else kept} | {'' if linked is None else linked} |\n")
        f.write(f"\nArticles (merged by DOI): {len(frame.rows)}; dropped as development datasets: "
                f"{len(frame.dropped & frame.rows.keys())}; **candidates: {len(rows)}**.\n")
    print(f"{len(rows)} candidates", file=sys.stderr)


if __name__ == "__main__":
    main()
