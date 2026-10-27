# Screening notes: 10.1038/s44321-026-00497-3

Article: Van Hove JLK et al. (2026) "Comprehensive functional testing in fibroblasts has strong
utility to diagnose mitochondrial disease", *EMBO Mol Med* 18(10):4153-4172. PMID 42711551,
PMC13658156. Deposit: BioStudies S-SCDT-10_1038-S44321-026-00497-3 (released 2026-08-06).

The article text, legends and figures are saved in `article/` (Europe PMC full-text XML,
Fig 1 and Fig 2 images from PMC, extracted legends, BioStudies study JSON). The PMC and
Springer PDFs were behind bot checks and could not be downloaded.

## Criterion 1

Passes. It is a peer-reviewed research article. It has western blots quantified by the
authors: Methods, "Protein abundance by western blot": "Protein bands of interest were
quantified using ImageJ software and normalized to either citrate synthase or adenine
nucleotide translocase (ANT)". Results report MTCO1 as % of in-assay controls, and per-cell-line
values ("MTCO1 % control") are in Dataset EV2. Only the column headers of Dataset EV2 were
read.

## Panels

The article has two figures (Fig 1: diagnostic scheme; Fig 2: schematic of the methods with
representative examples), no EV figures and no Appendix figures. Tables EV1-EV3 and
Datasets EV1-EV3 are tables.

| Panel | Western blot? | In the list (target / loading control, ≥ 2 groups)? | First failing criterion | Evidence |
|---|---|---|---|---|
| Fig 2B, complex I assembly blot (BN-PAGE, NDUFS2) | yes | no | (not listed) | It has no loading control. The result is a pattern: holocomplex at 1050 kDa and intermediates at 230/400/460 kDa. The 230 kDa band is compared with the holocomplex of the same lane, not with a loading control. One lane each: Huh7+CAM, control, C12orf65, control. Raw file `Complex_I_assembly_western_blot_example_c12orf65.tif`: inspect gives 16-bit, raw = yes. That file is not used because the panel is not in the list. |
| Fig 2D, MTCO1 / citrate synthase (CS) | yes | yes: MTCO1 normalised to CS, patient lines against controls | **2** (and 4 for the 16-bit sub-blot) | See below. |

### Fig 2D

Legend: "The abundance of the MTCO1 subunit is detected by western blotting of whole cell
lysate with citrate synthase as a control. Representative decreased abundance in cell lines
with pathogenic variants in SURF1, NARS2, and MRPS16 is shown, whereas the cell lines with
variants in MRSP18C and MRPL39 showed normal abundance."

The panel has two sub-blots:

* **Upper (SURF1 against Control).** Files `MTCO1_western_SURF1.tif`,
  `Citrate_synthase_loading_control_MTCO1_western_SURF1.tif` and
  `MTCO1_western_SURF1_membrane_overlay.tif`. All three are 16-bit (inspect: 16/16 bits,
  raw = yes).
* **Lower (Control, NARS2, MRPS18C, MRPS16, MRPL39).** Files
  `mtco1_western_NARS2_etc_mtco1_result_original.tif` and
  `mtco1_western_NARS2_etc_loading_citrate_synthase_original.tif`. Inspect gives 8 bits,
  raw = **no**, reason "8-bit file" (RGB with identical channels).

* **Criterion 2: fails.** Two of the panel's five raw files are 8-bit exports, which BlotVerse
  rejects. The only acquisitions of the lower sub-blot (NARS2, MRPS18C, MRPS16 and MRPL39
  against control) are these 8-bit files.
* **The upper sub-blot alone would still fail criterion 4**, so the decision does not depend
  on how criterion 2 is read for a panel with two blots:
  * *Criterion 3* passes: the full membrane is deposited, with 9 lanes and a marker.
  * *Criterion 4* fails. The authors state no replicate number for the panel, which is
    labelled "representative". The figure shows 2 SURF1 lanes and 2 Control lanes. In the
    lower sub-blot each cell line has a single lane.
  * The uncropped membrane has 6 weak lanes and 3 strong lanes, and nothing in the article,
    legend, file names or the cropping PowerPoints (whose only text is "Figure 1D" and the
    marker sizes) says which lanes are SURF1. Criterion 5 would therefore also fail.
  * The per-replicate numbers for this blot are only the per-cell-line "MTCO1 % control"
    values in Dataset EV2, with no per-lane values for this membrane. Criterion 7 is doubtful.
  * The source-data txt files (Figure_1B/1C/1D.txt) only describe how the images were
    cropped.

## Decision

**Excluded.** First failing criterion: **2**. The first (and only) quantified western blot
panel, Fig 2D, includes 8-bit raw files: the NARS2/MRPS18C/MRPS16/MRPL39 MTCO1 and CS blots.
Its 16-bit SURF1 sub-blot shows 2 lanes per group in a representative example, with no stated
n ≥ 3, so it fails criterion 4.

No `fetch.sh` or `claim.yaml` was written, because no panel qualifies. `images/` still
holds the file put there by `screen_files.py`
(`Complex_I_assembly_western_blot_example_c12orf65.tif`, Fig 2B, SHA-256
`dd49054db742f9a8e99509da53a9f2ce6bf797c388fb6a22807254ce6ba3942d`). No files with the authors'
numbers were placed in this folder.
