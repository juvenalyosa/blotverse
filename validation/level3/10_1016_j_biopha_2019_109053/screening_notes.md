# Screening notes: 10.1016/j.biopha.2019.109053

"The effect of various durations of cigarette smoke exposure on muscle fibre remodeling in rat
diaphragms", Biomedicine & Pharmacotherapy 117 (2019) 109053, PMID 31176169, gold OA
(CC BY-NC-ND). Deposit: Mendeley Data m34zcvp3nd v1 (2019-06-05), one file,
`supplementary figures.zip`. Second frame, screened under Amendment 2 (2026-10-27).

## Sources

| File | Source | Notes |
|---|---|---|
| `article/europepmc_31176169.json` | Europe PMC REST | metadata and abstract (no full text in Europe PMC/PMC) |
| `article/mendeley_m34zcvp3nd*.json` | Mendeley public API | deposit description (lane order, n, loading control) and file list |
| `article/gr1, gr2, gr5 (+ _lrg).jpg` | ars.els-cdn.com | figures without quantification (histology, EM) |
| `article/fig3G_lane_labels.jpg` | crop of Fig. 3 (see `fetch.sh`) | Fig. 3G blots and lane labels only |
| `authors/gr3, gr4, gr6, gr7 (+ _lrg).jpg` | ars.els-cdn.com | figures with the WB bar charts |
| `images/MHC.tif, BAF8.tif, F59.tif, Gapdh(fig3).tif` | Mendeley zip | Fig. 3G raw membranes |
| `archives/supplementary figures.zip` | Mendeley | from `screen_files.py` |

**Limitation.** The article HTML/PDF on ScienceDirect returned HTTP 403 to `curl` and WebFetch
(Elsevier API without a key gives metadata only; Unpaywall, OpenAlex and Semantic Scholar list
only the ScienceDirect PDF). The figure images were obtained from the Elsevier CDN, but the text
and the figure legends were not read. Everything below rests on the figures, the abstract and
the deposit description. The claim's primary contrast and quote are marked PENDING in
`claim.yaml` and must be filled from the article text before annotation.

## Criterion 1

Peer-reviewed journal article (not a preprint). Western blots quantified by the authors:
Fig. 3G/H, 4A/B, 6A/B, 7 (bar charts "Relative density unit" with significance marks). Passes.

## Quantified western blot panels (figure order)

Deposit description (authors): "original bands of western blot detection for
MHC-I/MHC-II/MHC/PGC-1α/COX4/GRP78/CHOP/caspase-12/p53/p21. GAPDH served as a loading control.
The sequence from left to right side represents the control group, the CS(cigarette smoke)4w
group, the CS8w group, the CS12w group. ... western blot detection was performed among randomly
selected three samples from each group of total four groups for the purpose of quantitative
analysis via one-way analysis of variance (ANOVA)."

1. Fig. 3G/H: MHC, MHC-I, MHC-II / GAPDH, 4 groups × 3 rats.
2. Fig. 4A/B: PGC-1α, COX4 / GAPDH.
3. Fig. 6A/B: GRP78, CHOP, caspase-12 / GAPDH.
4. Fig. 7: p53, p21 / GAPDH.

## Panels examined

| Panel | Result | Evidence |
|---|---|---|
| Fig. 3G/H (MHC / GAPDH) | **included** | **C2:** `blotverse inspect` on `MHC.tif`, `BAF8.tif`, `F59.tif`, `Gapdh(fig3).tif`: 1376 × 1104, 16-bit, effective 16 bits, raw = yes (warning: bright background, set `invert: true`). **C3:** each image shows one full strip with all 12 lanes, membrane edges visible. **C4:** n = 3 per group stated by the authors (deposit description; lane labels C-1..12W-3); all 12 lanes present in the target and GAPDH images. **C5:** lane order from the deposit description and the Fig. 3G labels (control ×3, CS4w ×3, CS8w ×3, CS12w ×3), no ladder lane; images are not mirrored (lane shapes of C-1/C-2 in MHC.tif and F59.tif match the figure). File-to-target mapping: `MHC.tif` = MHC (name), `Gapdh(fig3).tif` = GAPDH of Fig. 3 (name), `BAF8.tif` = MHC-I and `F59.tif` = MHC-II (antibody clone names). **C6:** GAPDH image of the same 12 lanes. **C7 (recorded, not excluding):** no per-replicate numbers; the deposit holds images only. **C8:** lanes vertical. |
| Fig. 4A/B, 6A/B, 7 | not examined | Later panels (one experiment per article). For the record: `CHOP.tif` is RGB (raw = no). |

## Decision

**Included: Fig. 3G/H, MHC normalised to GAPDH, control vs CS4w/CS8w/CS12w, n = 3 per group.**
`fetch.sh` (tested in a scratch copy: every hash OK) and `claim.yaml` written. The authors' bar
charts (Figs 3, 4, 6, 7) are in `authors/`; no source-data table exists. The flattened images left
by `screen_files.py` (`BAF8`, `Gapdh(fig3)`) were replaced by the four Fig. 3G files under their
deposit names.

**Open item before annotation:** read the article text (https://doi.org/10.1016/j.biopha.2019.109053)
and fill `primary_contrast`, `quote`, `quote_location` and the legend's significance thresholds in
`claim.yaml`; save the PDF in `authors/` (it shows the bar charts).
