# Screening notes: 10.1016/j.molcel.2018.01.027

Annibaldi et al., "Ubiquitin-Mediated Regulation of RIPK1 Kinase Activity Independent of
IKK and MK2", Mol Cell 69 (2018). PMID 29452637, PMC5823975. Peer-reviewed journal
article, not a preprint. Deposit: Mendeley Data f559yfv4h4 v1 ("Original data of images
and Western blot"). Second sampling frame (rank 102), screened 2026-10-27 under the brief
with the Amendment 2 addition.

## Sources

* `article/PMC5823975.xml`: Europe PMC full text (text, legends, STAR Methods).
  SHA-256 132c43253f6c18101903813bef8c15b5e2d5c86795d74da1308a026391af4afa.
* `article/mmc1_supplementary.pdf`: publisher Supplemental Information (Figures S1–S5 with
  legends), `https://ars.els-cdn.com/content/image/1-s2.0-S109727651830056X-mmc1.pdf`,
  SHA-256 120b07709cfc5484c3105d140e08bac94ef20302f3cd5d905e1d54f248b80385.
* `article/gr2_lrg.jpg`, `gr3_lrg.jpg`, `gr5_lrg.jpg`, `gr6_lrg.jpg`, `gr7_lrg.jpg`: figures
  examined for blot quantification (publisher `...-grN_lrg.jpg`).
* Deposit file list from the Mendeley public API: 33 zips, one per panel (Figs. 1C–F, 2E–G,
  3C–G, 4E, 5B, 5D, 5F, 6A–C, 7B–E, S1A–C, S2E–F, S3F, S4A, S5B–D). `screen_files.py`
  found raw files among them (criterion2_frame2.tsv: e.g. `Fig3C_cIAP1.tif`,
  `Fig3C_tubulin.tif` raw = yes; `Fig2E_Tubulin.tif`, `Fig2E p100:p52.tif` pseudo-coloured).
  No xlsx/csv with blot values exists in the deposit or the supplement.

## Western blot panels and quantification (figure order)

Western blots appear in Figs. 2E–G, 3C–G, 5D, 5F, 6A–C, 7B–E and S2E–F, S3F, S4A, S5B–D.
Every one is shown as a representative image ("Representative immunoblots are shown of
three independent experiments", "Representative images of at least three independent
experiments are shown", etc.). The graphs in the article are ITC (2A–C), qPCR/ELISA
(3H–K), body temperature/survival/LDH/TUNEL (4), PI-positive cells and PLA speckles
(5A–C, 5E, 7F, S3A–E), Ub-AQUA mass spectrometry (7A, S5A). The legend's "Graphs show
mean ± SD, *p < 0.05" in Fig. 5D/F refers to the PI graphs (5C/5E); Figure 5 has no
densitometry plot (checked on the figure). The methods describe no densitometry or
loading-control normalisation.

The only densitometry is an inset in **Fig. 7C**: "Absolute intensity (arbitrary unit)" of
ubiquitylated RIPK1 in a TUBE pull-down versus time (cIAP2-/- vs cIAP1UBAmut MEFs), one
curve per genotype, no error bars, no n and no statistics.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 7C inset (Ub-RIPK1 in TUBE pull-down, 0–6 h TNF) | 1 (no panel fits the unit of analysis) | Absolute band intensity, not normalised to a loading control (no housekeeping, total protein or total form; the TUBE pull-down is an enrichment, and the anti-Ub blot of the pull-down is not used as a reference). One representative blot, no replicates or test, so no claim with n per group. Not a phospho/total ratio, so Amendment 2 point 3 does not apply. C7 (recorded only): no per-replicate numbers anywhere. |

## Decision

**Excluded, criterion 1.** All western blots are shown as representative images. The only
quantified blot (Fig. 7C inset) gives absolute intensities from one blot, with no
normalisation to a loading control. No panel fits the unit of analysis, and Amendment 2
(criterion 7, phospho/total) does not change this: the article has no quantified
phospho/total ratio (p-p65, p-p38, p-JNK, p-ERK in Fig. 3G, P-RIPK1/P-MK2 in Fig. 7E are
unquantified).

No `fetch.sh` or `claim.yaml` was written. `images/` and `archives/` still hold the files
downloaded by `screen_files.py`.
