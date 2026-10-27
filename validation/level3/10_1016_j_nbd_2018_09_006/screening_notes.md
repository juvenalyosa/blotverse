# Screening: 10.1016/j.nbd.2018.09.006

Stamatovic et al. (2018) "Decline in Sirtuin-1 expression and activity plays a critical
role in blood-brain barrier permeability in aging", *Neurobiology of Disease* (peer-reviewed;
PMID 30196051, PMC6401345, NIH author manuscript). Deposit: Mendeley Data xs4hrrz26y v1
(published 2026-08-03), one file `original images .zip` (SHA-256 70706422...3e67, matches
the Mendeley API hash).

Sources read (in `article/`): PMC full-text HTML (`PMC6401345.html`), figure legends
(`figure_legends.txt`), figures 1-5 (`nihms-1507927-f000N.jpg`), deposit metadata
(`deposit_metadata.json`). The two PMC supplementary files (`NIHMS1507927-supplement-1.docx`,
`-supplement-2.tif`) could not be downloaded (PMC and Europe PMC return an HTML
download-gate page); this does not affect the decision, which is set by the first
quantified panel.

Deposit content (zip, extracted to a scratch directory only):

* `original _Uncropped_blots _manuscript all 5 replicants for Fig 2B/`:
  `BV_CLDN5_YOUNG_YOUNG_OLD_OLD_60SSEC.tiff`, `CLDN5 young old bv.tif`,
  `GADPH young old_bv.tif`, `COcldn5 zo1_y_o_SAMP first.tif`, and a PDF scan of a film
  annotated "Y O Y O Y O" (`Scanned from a Xerox ... cldn5 young old -bv.pdf`).
* `Biological_Replicates_2017_Repeats/`: two IP blots (`IP_Cldn5blot_ZO-1 Y_O_Y_O.tiff`,
  `IP_ZO1blot_Cldn5_Y_O_Y_O.tiff`), i.e. Fig. 2C.
* `Historical_Grant_Source_Files/`: pilot blots (occludin, claudin-1, ZO-1/claudin-5
  y/o/SAMP, VASP/pVASP in an AD model) described by the depositor as grant pilots, not
  article figures.
* No source-data table, xlsx or csv.

## Quantified western blot panels, in figure order

| Panel | Content | Result | Evidence |
|---|---|---|---|
| Fig. 2B | claudin-5 and ZO-1 / GAPDH, young (2-6 m) vs old (16-22 m) mouse brain microvessels; n = 5; claudin-5 *** p < 0.001 | **criterion 4 failed** | See below. |
| Fig. 3D | Sirt1 / beta-tubulin, young vs middle vs old microvessels | criterion 2 failed | No raw image of this blot in the deposit (no Sirt1 or beta-tubulin file). |
| Fig. 5A | claudin-5 and ZO-1 / beta-tubulin, wt vs Sirt1Tg and wt vs Sirt1flox Cldn5icre; n = 3 | criterion 2 failed | No raw image of this blot in the deposit. |

Not quantified (not panels under the unit of analysis): Fig. 2C (IP, "representative
images"), Fig. 4A and 4B (representative Sirt1 blots, quantification is of permeability).

### Fig. 2B in detail

* Criterion 1: met (peer-reviewed; legend "Protein levels were normalized to GAPDH ...
  n=5; ***p<0.001 compared to young age group").
* Criterion 2: met. `blotverse inspect` on the four TIFFs of the Fig. 2B folder:
  all 16-bit, effective 16 bits, 0 saturated px, `raw = yes` (claudin-5 60 s, claudin-5
  "young old bv", GAPDH "young old_bv", claudin-5/ZO-1 "SAMP first").
* Criterion 3: met. The images show whole membranes with edges and all lanes.
* Criterion 4: **failed.** The authors state n = 5 per group (legend: "one of five
  independent experiments"). The deposit holds a single loading-control image,
  `GADPH young old_bv.tif`, with 6 lanes (at most 3 young + 3 old). The other claudin-5
  lanes (`BV_CLDN5_YOUNG_YOUNG_OLD_OLD_60SSEC.tiff`, 2 young + 2 old, separate membrane
  with ladder; `COcldn5 zo1_y_o_SAMP first.tif`, young/old/SAMP) have no GAPDH image.
  On the 6-lane claudin-5 membrane (`CLDN5 young old bv.tif`), lane 4 (an "O" lane per
  the scanned film annotation, marked "?") shows no band. The ZO-1 blots of Fig. 2B are
  not deposited at all. So not every one of the 5 replicates per group is present for
  both the target and the loading control.
* Criteria 5-8 not evaluated (stop at first failure). For the record, criterion 7 would
  also fail: no per-replicate numbers exist in the deposit or the article.

## Decision

**Excluded. First failing criterion: 4** (Fig. 2B, the first quantified panel: n = 5 per
group stated, but GAPDH is deposited for only one 6-lane membrane, and ZO-1 is not
deposited).

No `claim.yaml` or `fetch.sh` written (no qualifying panel). `images/` holds the one file
placed by `screen_files.py` (`BV_CLDN5_YOUNG_YOUNG_OLD_OLD_60SSEC.tiff`, Fig. 2B) and
`archives/` the deposit zip. No file with the authors' numbers was downloaded (none exists).

## Rescreen under Amendment 2

Rescreened 2026-10-27 under Amendment 2, point 5. Two rules changed: criterion 7 no longer
excludes, and phospho / total counts as a loading control when each lane can be matched.
Co-IP bait normalisation is still not a loading control.

* **Phospho / total.** The article has no phospho- or other modification-specific western blot (checked in the
  legends and full text). The only other blot is Fig. 2C, a claudin-5/ZO-1 IP, which is "representative" and not
  quantified. It would not count anyway, because normalisation to an immunoprecipitated bait is excluded. The VASP/pVASP files
  in the deposit are grant pilot blots and do not belong to any article figure. No new panel enters, and the order is unchanged:
  Fig. 2B, 3D, 5A.
* **Criterion 7.** Criterion 7 no longer excludes. As recorded above, neither the deposit nor the article has per-replicate numbers.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 2B (claudin-5, ZO-1 / GAPDH; young vs old) | **4** | Reused from above, because the amendment does not touch it. The legend gives n = 5 per group. GAPDH is deposited for only one 6-lane membrane (at most 3 + 3), the other claudin-5 membranes have no GAPDH image, and ZO-1 is not deposited at all. |
| Fig. 3D (Sirt1 / β-tubulin) | 2 | Unchanged: no raw image is deposited. |
| Fig. 5A (claudin-5 / β-tubulin) | 2 | Unchanged: no raw image is deposited. |

**Decision under Amendment 2: still excluded, criterion 4** (Fig. 2B, the first quantified panel).
No `fetch.sh` or `claim.yaml` was written.
