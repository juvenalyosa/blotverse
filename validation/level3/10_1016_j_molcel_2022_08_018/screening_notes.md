# Screening: 10.1016/j.molcel.2022.08.018

Li, Ikeuchi et al. (2022) "Sensing of individual stalled 80S ribosomes by Fap1 for
nonfunctional rRNA turnover", *Molecular Cell* 82 (peer-reviewed; PMID 36113412; Elsevier
open archive, not in PMC). Deposit: Mendeley Data ymdmwg5hw3 (v1), described as "original
blot images of western blot and northern blot".

Sources read (in `article/`):

* `gr1.jpg`-`gr6.jpg`: all six main figures (Elsevier CDN, `ars.els-cdn.com`, `_lrg`).
* `mmc1.pdf` (text in `mmc1.txt`): Figures S1-S7 with legends.
* `Gel_processing_summary.pdf`: the deposit's map of raw images to figure panels.
* `mendeley_dataset.json`: the deposit's metadata and file list.
* `elsevier_api_metadata.xml`: Elsevier API record (metadata only).

The cell.com and ScienceDirect full-text pages returned HTTP 403 to non-browser clients, so
the main-text legends and Results were not read. The panel decision rests on the figures
themselves and the supplementary legends. They show no WB densitometry: no
normalisation, bar chart, ratio or statistics on any western blot.

## Western blot panels and quantification

Only northern blots are quantified in this article: 18S NRD half-lives after transcription
shut-off (probe "18S tag") in Fig. 2C, 3D and 5E, and the polysome-fraction distribution
percentages in Fig. 2F. Every western blot is a representative image with no
quantification.

| Panel | Content | Quantified by authors? |
|---|---|---|
| Fig. 2A | Mag2 pull-down, IB HA (uS3-3HA ubiquitination), uL14, FLAG | no |
| Fig. 2D | MS2-purified ribosomes, IB HA (uS3-Ub), uL14 | no |
| Fig. 2E | in vitro ubiquitination of uS3-3HA, IB HA | no |
| Fig. 3A, 3C | sucrose-gradient fractions, IB FLAG/uL14, HA (Yil161w, Fpr1) | no |
| Fig. 5F | sucrose-gradient fractions of Fap1-FLAG mutants, IB Asc1/FLAG | no |
| Fig. 6B | in vitro ubiquitination of uS3-3HA, IB HA | no |
| Fig. S2E | polysome profile, Mag2-3HA (`Supplementary Figure2E_20211029_SDG-WB_MAG2-3HA_7.tif`) | no |
| Fig. S2H, S2I, S2K | uS3 ubiquitination (pull-downs, in vitro), IB HA/uL14 | no |
| Fig. S3C | sucrose-gradient distribution of Fap1-FLAG variants | no |
| Fig. 2C, 2F, 3D, 5E | **northern** blots (18S tag probe): half-life, % per fraction | yes, but not western blots |

No panel fits the unit of analysis: none normalises a target band to a loading
control, compares ≥ 2 groups and reports a quantification. So no panel was examined
under criteria 2-8, and `blotverse inspect` was not run.

## Decision

**Excluded. First failing criterion: 1.** The article is peer-reviewed but has no western
blot panel quantified by the authors. Its only quantified blots are northern blots.

No `claim.yaml` or `fetch.sh` was written because no panel qualifies. `images/` holds the
two deposit files downloaded by `screen_files.py`:
`Figure2C_mag2D_20171208_1652_11.tif` (a Fig. 2C northern blot) and
`Supplementary Figure2E_20211029_SDG-WB_MAG2-3HA_7.tif` (Fig. S2E WB). `archives/` holds the
deposit zip for Figure 2 / Figure S2. No file with the authors' numbers was downloaded.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules applied: criterion 7
no longer excludes; the total form of the target counts as a loading control for a
modification-specific target when each lane can be matched; normalisation to an
immunoprecipitated bait is still not a loading control.

Evidence reused unchanged: the figure and legend review above (gr1-gr6, `mmc1.txt`,
`Gel_processing_summary.pdf`). A fresh search of the supplementary legends (`mmc1.txt`) for
quantification, normalisation, ratios or phospho-specific detection found nothing on any
western blot. The article has no phospho / total panel, and its western blots (pull-downs,
in vitro ubiquitination, sucrose-gradient fractions) are representative images with no
densitometry, whatever the reference band. The only quantified blots are northern blots
(Fig. 2C, 2F, 3D, 5E).

Neither revised rule touches criterion 1 here: there is still no western blot panel
quantified by the authors, so no panel enters the examination of criteria 2-8.

**Decision unchanged: excluded, first failing criterion 1.** No `fetch.sh` or
`claim.yaml` written.
