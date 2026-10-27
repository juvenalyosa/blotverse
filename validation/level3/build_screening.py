"""Writes screening.md (stdout) from the criterion-2 tables and the screening-agent outcomes."""
import csv, re, sys
from pathlib import Path
D = str(Path(__file__).parent) + "/"
slug = lambda d: re.sub(r"[^a-z0-9]+", "_", d.lower()).strip("_")
c2 = {r["doi"]: r for r in csv.DictReader(open(D + "criterion2.tsv"), delimiter="\t")}
vis = {r["doi"]: r for r in csv.DictReader(open(D + "criterion2_visual.tsv"), delimiter="\t")}
# Screening-agent outcomes (one agent per candidate; details in <slug>/screening_notes.md)
agent = {
 "10.7554/elife.88206": ("excluded", "4", "Fig. 5—S1A", "n = 3 stated; the deposited CDKL5/tubulin membranes identify one replicate set only"),
 "10.1016/j.molcel.2021.06.009": ("excluded", "5", "Fig. S1A/B", "12 unlabelled lanes, no lane guide; mapping only possible from band signals"),
 "10.1016/j.nbd.2018.09.006": ("excluded", "4", "Fig. 2B", "n = 5 stated; GAPDH deposited for one 6-lane membrane only, ZO-1 blots missing"),
 "10.1016/j.isci.2023.107207": ("excluded", "4", "Fig. 3B (S3A)", "n = 6 stated; one membrane per antibody with 2 lanes per group"),
 "10.1038/s44318-025-00560-8": ("excluded", "2", "Fig. EV1C/D", "only a pseudo-coloured RGB composite; main-figure blots are co-IPs normalised to the bait"),
 "10.1016/j.molcel.2023.06.020": ("excluded", "4", "Fig. 1C (S1C/D)", "n = 3 stated; one replicate membrane per blot deposited (agent recorded criterion 3; recorded as 4 like ranks 76 and 84)"),
 "10.1038/s44321-026-00479-5": ("excluded", "2", "Fig. 1D", "deposited only as pseudo-coloured RGB TIFF and JPEG"),
 "10.1038/s44318-025-00587-x": ("excluded", "1", "Fig. 1G", "only blot is a qualitative band-shift assay, not quantified"),
 "10.1038/s44321-026-00497-3": ("excluded", "2", "Fig. 2D", "two of the panel's files are 8-bit"),
 "10.1016/j.molcel.2025.10.026": ("excluded", "2", "Fig. 3E–F", "panel files are 8-bit; the only 16-bit pair (S3H) has n = 1"),
 "10.1038/s44318-025-00437-w": ("excluded", "1", "—", "all blots shown as representative images, none quantified"),
 "10.1038/s41467-026-71753-w": ("**included**", "—", "Fig. 1e", "γH2AX / total protein stain, 4 groups × 6 mice, raw Odyssey scans; `10_1038_s41467_026_71753_w/`"),
 "10.1016/j.molcel.2022.08.018": ("excluded", "1", "—", "only northern blots quantified (main text not accessible; decided from figures and supplement)"),
 "10.1016/j.molcel.2025.08.015": ("excluded", "1", "—", "no western blot quantified; the .scn file is an RT-PCR gel"),
 "10.1038/s44321-025-00336-x": ("excluded", "2", "Fig. 4J", "pseudo-coloured TIFFs only"),
 "10.1038/s44318-026-00706-2": ("excluded", "1", "—", "DmdA blots not quantified"),
 "10.1016/j.celrep.2024.114831": ("excluded", "7", "Fig. 5E", "criteria 2–6 met; only plotted means ± SD, no per-replicate numbers"),
 "10.7554/elife.91083": ("excluded", "1", "Fig. 6B–D", "only phospho/total ratios quantified; no panel fits the unit of analysis"),
 "10.1038/s44319-026-00740-5": ("excluded", "1", "—", "blots shown one lane per condition, not quantified"),
}
not_peer = {"10.1101": "preprint", "10.57844": "Arcadia publication, not peer-reviewed"}
rows = sorted(csv.DictReader(open(D + "candidates.csv")), key=lambda r: int(r["rank"]))
out = []
for r in rows:
    d = r["doi"]; s = c2[d]
    if d in agent:
        dec, crit, panel, why = agent[d]
    elif s["raw_acquisition"] != "yes":
        dec, crit, panel = "excluded", "2", "—"
        why = "no TIFF, .scn or .gel file in the deposit" if s["raw_acquisition"] == "no image files" else f"no raw image among {s['image_files_checked']} TIFF/.scn/.gel files"
    elif any(d.startswith(p) for p in not_peer):
        dec, crit, panel = "excluded", "1", "—"
        why = next(v for p, v in not_peer.items() if d.startswith(p))
    elif d in vis and vis[d]["raw_blot"] == "no":
        dec, crit, panel, why = "excluded", "2", "—", "raw images are microscopy only; every blot file is 8-bit, colour or CMYK"
    else:
        dec, crit, panel, why = "pending", "", "", ""
    out.append(f"| {r['rank']} | {d} | {r['repositories']} | {dec} | {crit} | {panel} | {why} |")
n_inc = sum("**included**" in o for o in out); n_pend = sum("| pending |" in o for o in out)
print("""# Level 3: screening of the sampling frame

One row per candidate of [`candidates.csv`](candidates.csv), in screening order. "Criterion"
is the first inclusion criterion of [`docs/level3_protocol.md`](../../docs/level3_protocol.md)
that the candidate fails.

**How the criteria were checked.**

* **Criterion 2 first, for every candidate.** [`screen_files.py`](screen_files.py) listed each
  deposit and ran `blotverse inspect` on its image files ([`criterion2.tsv`](criterion2.tsv)).
  A candidate with no raw image at all fails criterion 2 whatever its article says, so
  criterion 1 was not assessed for these 193 candidates.
* **Raw images that are not blots.** For the 42 candidates with a raw image, a rescreen counted
  only files named as blots ([`criterion2_blot.tsv`](criterion2_blot.tsv)). The candidates
  without one were checked visually by an agent ([`criterion2_visual.tsv`](criterion2_visual.tsv)):
  their raw images are microscopy, EM or brightfield, and they fail criterion 2.
* **Criterion 1 without an agent.** Preprints and other non-peer-reviewed DOIs were
  excluded from the DOI alone.
* **Screening agents.** Every other candidate was screened by an agent following
  [`screening_brief.md`](screening_brief.md). Each agent's evidence is in
  `<doi-slug>/screening_notes.md`.

**Loading control.** The protocol accepts a housekeeping protein or total protein as the
loading control. Panels normalised to the total form of the same protein (phospho/total) or to
an immunoprecipitated bait (co-IP) are therefore not experiments under the protocol. Several
candidates had such panels quantified (ranks 19, 84, 87, 95, 209; rank 87 had no other); this is reported as a
limitation.
""")
print(f"**Yield:** {n_inc} included, {len(out) - n_inc - n_pend} excluded" + (f", {n_pend} pending" if n_pend else "") + f" of {len(out)} candidates.\n")
print("| Rank | DOI | Repository | Decision | Criterion | Panel | Reason |\n|---|---|---|---|---|---|---|")
print("\n".join(out))

# --- Amendment 2, point 5: first-frame rescreen under the revised rules -------------
rescreen = {
 "10.7554/elife.88206": ("excluded", "4", "Fig. 5A (pEB2 / EB2)", "phospho/total panel now first; same replicate problem as before"),
 "10.1016/j.molcel.2021.06.009": ("excluded", "5", "Fig. S1A/B", "unchanged"),
 "10.1016/j.nbd.2018.09.006": ("excluded", "4", "Fig. 2B", "unchanged"),
 "10.1016/j.isci.2023.107207": ("excluded", "4", "Fig. 1A (pSTAT3 / STAT3)", "phospho/total panel now first; three membranes deposited for n = 4-27 per group"),
 "10.7554/elife.91083": ("**included (provisional)**", "—", "Fig. 6B/6D (pS935 / LRRK2)", "6B/6C (pRab10/Rab10) fails 5; 6D counted as a separate experiment — decision for the auditor"),
 "10.1038/s44318-025-00560-8": ("excluded", "2", "Fig. EV1C/D", "phospho-paxillin panels are colour or 8-bit; unchanged"),
 "10.1016/j.molcel.2023.06.020": ("excluded", "4", "Fig. 1C (S1C/D)", "unchanged"),
 "10.1038/s44321-026-00479-5": ("excluded", "2", "Fig. 1D", "unchanged"),
 "10.1038/s44318-025-00587-x": ("excluded", "1", "Fig. 1G", "unchanged"),
 "10.1038/s44321-026-00497-3": ("excluded", "2", "Fig. 2D", "unchanged"),
 "10.1016/j.molcel.2025.10.026": ("excluded", "2", "Fig. 3E–F", "unchanged"),
 "10.1038/s44318-025-00437-w": ("excluded", "1", "—", "unchanged"),
 "10.1016/j.molcel.2022.08.018": ("excluded", "1", "—", "unchanged"),
 "10.1016/j.molcel.2025.08.015": ("excluded", "1", "—", "unchanged"),
 "10.1038/s44321-025-00336-x": ("excluded", "2", "Fig. 4J", "unchanged (Fig. 5H phospho/total pair is 8-bit)"),
 "10.1038/s44318-026-00706-2": ("excluded", "1", "—", "unchanged"),
 "10.1016/j.celrep.2024.114831": ("**included**", "—", "Fig. 5E", "deoxyhypusinated eIF5A / Ponceau S, 3 replicates; excluded before only on criterion 7"),
 "10.1038/s44319-026-00740-5": ("excluded", "1", "—", "unchanged"),
}
rank1 = {r["doi"]: r["rank"] for r in rows}
re_rows = sorted(rescreen.items(), key=lambda kv: int(rank1[kv[0]]))
print("\n## First frame rescreened under Amendment 2 (point 5)\n")
print("The 18 agent-screened exclusions above, screened again with criterion 7 no longer excluding "
      "and phospho / total accepted. Inclusions here form a separately reported group.\n")
print(f"**Yield:** {sum('included' in v[0] for _, v in re_rows)} included of {len(re_rows)}.\n")
print("| Rank | DOI | Decision | Criterion | Panel | Reason |\n|---|---|---|---|---|---|")
for d, (dec, crit, panel, why) in re_rows:
    print(f"| {rank1[d]} | {d} | {dec} | {crit} | {panel} | {why} |")

# --- Second frame (Amendment 2, point 4) -------------------------------------------
c2b = {r["doi"]: r for r in csv.DictReader(open(D + "criterion2_frame2.tsv"), delimiter="\t")}
vis2 = {r["doi"]: r for r in csv.DictReader(open(D + "criterion2_visual_frame2.tsv"), delimiter="\t")}
agent2 = {
 "10.1016/j.molcel.2018.08.017": ("excluded", "2", "Fig. 2E (S1D)", "NOP56/ACTB blots are 8-bit; the 16-bit files are northern blots"),
 "10.1016/j.chom.2018.11.008": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.molcel.2018.05.026": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.celrep.2018.03.054": ("excluded", "2", "Fig. 6G (P-RIPK1 / RIPK1)", "panel files are 8-bit"),
 "10.1016/j.molcel.2018.08.042": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.cryobiol.2018.07.004": ("pending", "", "", "article not accessible to the screening agent (closed access); deposit has 16-bit raw blots"),
 "10.1016/j.celrep.2017.09.051": ("**included**", "—", "Supp. Fig. 3C (Fabp5 / Gapdh)", "CR vs AL, n = 6/6; Fig. 3B fails 2 (16-bit PNG)"),
 "10.1016/j.molcel.2017.05.006": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.celrep.2018.04.070": ("excluded", "1", "—", "pull-down blots not quantified"),
 "10.1016/j.cell.2018.01.032": ("excluded", "4", "Fig. 1F", "one lane per group, 2 independent experiments"),
 "10.1016/j.biopha.2019.109053": ("**included**", "—", "Fig. 3G/H (MHC / GAPDH)", "control vs smoke 4/8/12 weeks, 3 rats per group; claim.yaml incomplete (article not accessible)"),
 "10.1016/j.cell.2018.04.039": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.molcel.2018.05.013": ("excluded", "1", "Fig. 3E", "no loading control; S4N normalised to the IP bait"),
 "10.1016/j.molcel.2018.01.027": ("excluded", "1", "Fig. 7C", "only densitometry is a pull-down without loading control"),
 "10.1016/j.molcel.2017.01.026": ("excluded", "1", "—", "blots not quantified; quantified bands are in-gel fluorescence"),
 "10.1016/j.molcel.2017.12.012": ("excluded", "2", "Fig. 1C", "pseudo-colour files"),
 "10.1016/j.molcel.2017.05.003": ("excluded", "1", "Fig. 4D", "normalised to the immunoprecipitated bait"),
 "10.1016/j.biopha.2018.11.124": ("excluded", "2", "—", "every quantified protein has 8-bit or colour replicates (article not accessible)"),
 "10.1016/j.molcel.2017.06.020": ("excluded", "1", "—", "blots not quantified"),
 "10.1016/j.molcel.2017.04.024": ("**included**", "—", "Fig. S7D (Cdc48 / Pgk1)", "cdc48-3 vs WT, n = 3, claim: no difference"),
 "10.1016/j.molcel.2017.11.016": ("excluded", "1", "—", "supplementary blots not quantified"),
}
rows2 = sorted(csv.DictReader(open(D + "candidates_frame2.csv")), key=lambda r: int(r["rank"]))
out2 = []
for r in rows2:
    d = r["doi"]; s = c2b[d]
    if d in agent2:
        dec, crit, panel, why = agent2[d]
    elif s["raw_acquisition"] != "yes":
        dec, crit, panel = "excluded", "2", "—"
        why = ("no TIFF, .scn or .gel file in the deposit" if s["raw_acquisition"] == "no image files"
               else f"no raw image among {s['image_files_checked']} TIFF/.scn/.gel files")
    elif d in vis2 and vis2[d]["raw_blot"] == "no":
        dec, crit, panel, why = "excluded", "2", "—", "raw images are not western blots; every blot file is 8-bit, colour, PNG or PDF"
    else:
        dec, crit, panel, why = "pending", "", "", ""
    out2.append(f"| {r['rank']} | {d} | {r['repositories']} | {dec} | {crit} | {panel} | {why} |")
print("\n## Second frame (Amendment 2, point 4: deposits 2010–2019)\n")
print("Screened under the revised rules from the start, with the same steps as the first frame "
      "([`criterion2_frame2.tsv`](criterion2_frame2.tsv), [`criterion2_blot_frame2.tsv`](criterion2_blot_frame2.tsv), "
      "[`criterion2_visual_frame2.tsv`](criterion2_visual_frame2.tsv)). In the visual check, ranks 63 and 64 "
      "(about 11 GB of microscopy stacks) were judged from their file names and one local file each.\n")
n2 = sum("included" in o for o in out2); p2 = sum("| pending |" in o for o in out2)
print(f"**Yield:** {n2} included, {len(out2) - n2 - p2} excluded, {p2} pending of {len(out2)} candidates.\n")
print("| Rank | DOI | Repository | Decision | Criterion | Panel | Reason |\n|---|---|---|---|---|---|---|")
print("\n".join(out2))
