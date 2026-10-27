# Screening notes: 10.7554/elife.91083

Candidate: Gilsbach, Ho, et al. 2024, *Intramolecular feedback regulation of the LRRK2 Roc G
domain by a LRRK2 kinase-dependent mechanism*, eLife 12:RP91083. eLife version of record
(VOR published 2024-12-19, PMID 39699947, PMC11658767).
Deposit: Zenodo 10.5281/zenodo.14055918 (concept DOI 10.5281/zenodo.11242229; the article cites
the concept DOI), "Supplemental data for ... (Gilsbach et al., eLife 2024)", published
2024-11-18. Metadata in `article/zenodo_14055918_record.json`.

## Sources and where each file comes from

| File | Source | Notes |
|---|---|---|
| `article/elife-91083-PMC11658767.xml` | Europe PMC full-text XML | text, methods, legends |
| `article/elife-91083-v1.pdf`, `article/elife-91083-figures-v1.pdf` | eLife CDN | article and figures PDF |
| `article/elife-91083-fig6-v1.jpg`, `article/elife-91083-fig6-figsupp2-v1.jpg` | eLife IIIF | Fig. 6 and Fig. 6—figure supplement 2 |
| `article/zenodo_14055918_record.json` | Zenodo API | deposit metadata and description |
| `article/Figure6_source_data_1_Annotation.png` | **Zenodo** (`Figure6_source_data_1.zip/Annotation.png`) | lane annotation; identical in content to Fig. 6—figure supplement 2 |
| `images/Figure6_source_data_1/{A,B,C}/*.tif` | **Zenodo** (`Figure6_source_data_1.zip`, MD5 b92d5687b6a820c6682c1a6ebcdf3add, matches the record) | re-extracted with folder names kept (see below) |
| `archives/*.zip` | Zenodo | from `screen_files.py` |

eLife itself publishes only two data files for this article: `elife-91083-fig4-data1-v1.zip`
(Figure 4—source data 1, statistics of the GTPase kinetics) and `elife-91083-supp1-v1.docx`
(Supplementary file 1, a table of Michaelis–Menten parameters, K_M / k_cat ± error). Neither
belongs to the western blots, and eLife has no Figure 6 source-data file: the name
"Figure6_source_data_1" exists only in the Zenodo deposit. Every image used here comes from
Zenodo. (Supplementary file 1 was opened only to see what it contains; it is not kept in this
folder.)

**Re-extraction.** The earlier flattened extraction made `images/Figure6_source_data_1__focus.tif`
from both `A/focus.tif` and `B/focus.tif` (they differ: SHA-256 57bfc167… vs 78d4db90…). The
flattened files were removed and the zip was re-extracted with its folders:
`A/2023-02-07_2x2-300s-07.tif`, `A/focus.tif`, `B/2023-02-17_2x2-180s-05.tif`, `B/focus.tif`,
`B/B_focus.tif` (byte-identical to `B/focus.tif`), `C/2023-02-20_24D8_2x2-180s-05.tif`,
`C/2023-02-20_24D8_focus.tif`.

## Criterion 1

The article is a peer-reviewed eLife version of record. It has one western blot figure, Fig. 6B,
which the authors quantify (Fig. 6C relative pRab10, Fig. 6D relative pS935; "ANOVA followed by
a post hoc test (n=3, *p=0.05)"). All other figures are enzyme kinetics, mass photometry and
AlphaFold models.

## How the western blots are quantified

Methods (Cell-based Rab assay): "Samples were subsequently subjected to SDS-PAGE and western blot
analysis to determine LRRK2 pS935 and Rab10 T73 phosphorylation levels ... **Total LRRK2 and
Rab10 levels were determined as a reference for normalization.**" The Zenodo description says
the same: "The lines corresponding to LRRK2 pS935, total LRRK2, Rab10 pT73 and total Rab10 were
used for the quantification shown in Figure 6."

So Fig. 6C is pRab10(T73) / total Rab10 and Fig. 6D is LRRK2 pS935 / total LRRK2 (24D8). These
are phospho-to-total ratios of the same protein. No housekeeping protein and no total-protein
stain is probed or mentioned. Under *Unit of analysis* the target must be normalised to a loading
control (housekeeping protein or total protein, which criterion 6 specifies as "a total-protein
image of the same membrane"), so these panels are not experiments. This follows the reading used
for the other screened candidates (10.1016/j.isci.2023.107207 p/t STAT3, 10.7554/elife.88206
pEB2/EB2).

## Panels in figure order

| Panel | Result | Evidence |
|---|---|---|
| Fig. 6B/6C: pRab10(T73) / total Rab10; wt, R1441G, G2019S, I2020T, K1906M, T1343A, T1343A/G2019S; n = 3 | **not an experiment** (no loading-control normalisation) | Methods and Zenodo text: total Rab10 is the normalisation reference. No housekeeping band or total-protein stain on any membrane. |
| Fig. 6B/6D: LRRK2 pS935 / total LRRK2 (24D8); same groups; n = 3 | **not an experiment** | Same; total LRRK2 is the reference. |

No other western blot panel exists in the main figures or the figure supplements (Fig. 6—figure
supplement 2 shows the raw blots of Fig. 6B–D, not a new quantification).

### For the record only: Fig. 6B–D against criteria 2–8, had it counted

These observations do not change the decision.

* **Criterion 2** would pass. `blotverse inspect` on the three Zenodo chemiluminescence files:
  `A/2023-02-07_2x2-300s-07.tif`, `B/2023-02-17_2x2-180s-05.tif`,
  `C/2023-02-20_24D8_2x2-180s-05.tif` are all 1092 × 736, 16-bit, `raw` = yes, 0 saturated px
  (grey levels 19 630 / 3 559 / 4 021). The white-light marker photos (`focus.tif` files) are
  also 16-bit `raw` = yes, with 3 694–7 045 saturated px (not used for quantification).
* **Criterion 4/5** would be doubtful. The legend of Fig. 6—figure supplement 2 says
  "(A–C) Biological replicates" but also "Panel (C) shows the total-LRRK2 blot (24D8) for
  panel (B)", so C is not a third replicate; panel A holds two side-by-side membranes, which
  would have to be replicates 1 and 2 for n = 3. That mapping is not stated. In panel B the
  right-hand pS935-region strip is labelled LRRK2 (24D8) but is nearly blank (the usable total
  LRRK2 for B is the separate image C).
* **Criterion 7** would fail on the files seen: no per-replicate values for Fig. 6C/D were found
  in the Zenodo deposit (its other zips hold HPLC kinetics, mass photometry and AlphaFold
  models) or among eLife's files (Figure 4—source data 1 and Supplementary file 1 cover the
  kinetics only).

## Decision

**Excluded, criterion 1.** The article is peer-reviewed. Its only quantified western blots
(Fig. 6C, 6D) are phospho-to-total ratios (pRab10/total Rab10, pS935/total LRRK2), with no
housekeeping or total-protein loading control. So no panel fits the *Unit of analysis*, and
there is no first qualifying panel to which criteria 2–8 can be applied.

Caveat: if a total-protein band of the same protein (total Rab10, total LRRK2) were accepted as
"total protein", Fig. 6B–D would become the first panel. The raw 16-bit Zenodo images pass
criterion 2. The panel would then most likely fail criterion 4 or 5 (replicate mapping of panels
A–C) and criterion 7 (no per-replicate numbers found).

No `fetch.sh` or `claim.yaml` was written, because no panel qualified. Nothing with the
authors' numbers is kept in this folder.

## Rescreen under Amendment 2

Rescreened 2026-10-27 under Amendment 2, point 5. Two rules changed: criterion 7 no longer
excludes, and phospho / total counts as a loading control when each lane can be matched.
Evidence the revised rules do not touch is reused from above: criterion 1 (peer-reviewed VOR),
Fig. 6B as the only western blot figure, and criterion 2 (`blotverse inspect`: images A, B and C
are 1092 x 736, 16-bit, raw = yes, 0 saturated px).

**Effect of the revised rules.** Fig. 6C (pRab10 / total Rab10) and Fig. 6D (LRRK2 pS935 / total
LRRK2, 24D8) now fit the unit of analysis. Both compare 7 LRRK2 variants with wt, report n = 3 and
show significance marks. In figure order, 6B/6C comes first, then 6B/6D.

**New evidence (raw images rendered and read in scratch; band x-positions taken from column
profiles, not from intensities):**

* Every membrane has 12 lanes at a pitch of ~30 px. The labels (deposit `Annotation.png` =
  Fig. 6—figure supplement 2) cover lanes 1–10: wt, R1441G, G2019S, I2020T, K1906M, T1343A,
  T1343A/G2019S, Marker, PEI only, not transfected. Lanes 11–12 carry LRRK2, pRab10 and Rab10
  bands but no label. The annotation crops them off, so they are left out. Lanes 9–10 have
  Rab10 only, which fits PEI and untransfected controls. This confirms that the labels start
  at lane 1.
* Image A holds two complete label sets side by side. Each set has four strips (pS935, pRab10,
  LRRK2 24D8, Rab10), i.e. a phospho membrane and a total membrane, both cut at 140 kDa. Image B
  has a phospho membrane on the left (pS935 and pRab10) and a total membrane on the right
  (24D8, nearly blank, and Rab10). Image C is the repeat 24D8 blot for B, with 7 bands in lanes
  1–7 and 2 in lanes 11–12.
* **Replicate-3 pRab10 strip (image B, left, lower strip) is offset from its labels.** On the
  pS935 strip above it, lane 1 (wt) sits at x ≈ 79 px. On the pRab10 strip, the lane streaks
  and the first band start at x ≈ 100 px, ~0.7 lane to the right. The lane count (bands in
  lanes 11–12 at x ≈ 405/437) fits lane 1 = x ≈ 100. In the authors' own overview
  (`Annotation.png`, panel B), however, the pRab10 bands fall one label to the right. That
  reading gives a pRab10 band to the kinase-dead K1906M lane and to the Marker lane, and none to
  wt. The two readings give different groups for every pRab10 lane of replicate 3. Choosing
  between them means preferring the strip geometry over the authors' labelling, or using the
  expected band pattern. That is an ambiguous mapping.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 6B/6C: pRab10(T73) / total Rab10; wt vs 6 variants; n = 3 | **5** | C2 passes (above). C3 passes: whole membranes with all 12 lanes. C4 passes: n = 3 per group, with replicates in A-left, A-right and B, each with pRab10 and Rab10. C5 **fails**: the replicate-3 pRab10 strip is offset by ~0.7 lane from its labels, and the authors' overview assigns its bands one lane to the right of the strip-geometry reading (above). So the lane-to-group mapping of replicate 3 is ambiguous. C6 would pass (Rab10 on every membrane, or on the parallel membrane in B). C7 (no longer excluding): no per-replicate values. Supplementary file 1c gives only mean ± error per variant (it was opened only to see what it contains, now in `authors/`). |
| **Fig. 6B/6D: LRRK2 pS935 / total LRRK2 (24D8); wt vs 6 variants; n = 3** | **included** | C1, C2 pass (above). **C3:** whole membranes; lanes 1–7 of every quantified replicate are shown for pS935 and for 24D8. **C4:** n = 3 per group (legend). Replicates 1 and 2 are A-left and A-right (pS935 strip + 24D8 strip each); replicate 3 is B-left pS935 + C 24D8. Every replicate has the target and the reference. **C5:** groups come from the labels in `Annotation.png` / Fig. 6—S2. These are identical on every membrane, and the bands on the pS935 strips of A and B and on the 24D8 strips of A and C fall on the labelled lanes 1–7 (lane pitch and lane 11–12 positions agree). Replicates come from the legend of Fig. 6—S2: "(A–C) Biological replicates. Panel (C) shows the total-LRRK2 blot (24D8) for panel (B)". With n = 3, panel A holds two replicates, and each side of A is a complete, separately labelled set. No authors' numbers were used. **C6:** total LRRK2 for every replicate: A-left and A-right 24D8 strips (parallel membranes in the same image), and C for replicate 3 (repeat blot of the same lysates). Lanes are matched by label (Amendment 2 point 3). **C7** (recorded only): no per-replicate numbers. Supplementary file 1c has mean ± error per variant. **C8:** vertical lanes, no rotation. |

Notes for the reviewer:

* The split into 6B/6C and 6B/6D follows the per-quantification rows used above in the
  first-frame notes (6C and 6D are separate panels with separate targets). If Fig. 6B were
  treated as one experiment whose primary contrast is the first stated one (pRab10, R1441G
  vs wt), it would fail criterion 5, and the candidate would be excluded.
* The primary contrast of 6D is R1441G vs wt. It is the first contrast cited for Fig. 6C and D
  in the text ("R1441G and I2020T showed an approximately eightfold increase in Rab10
  phosphorylation (Figure 6C and D)"), and the first significance bracket in Fig. 6D. The text
  gives no direction for pS935, so the direction (down) is taken from Fig. 6D.
* The first-frame notes say that eLife's data files hold only kinetics. That is incomplete:
  Supplementary file 1 also has table 1c ("Cell-based phospho-Rab assays. Quantification of
  Western Blots": mean ± error of pRab10 and pS935 per variant). It is now in `authors/`.

**Decision under Amendment 2: included, Fig. 6B/6D** (pS935 / total LRRK2, primary contrast
R1441G vs wt, direction down, *, n = 3). Folder prepared:

* `fetch.sh`: tested in a scratch copy, where every hash checked out.
* `claim.yaml`.
* `article/`: Europe PMC XML, Fig. 6—figure supplement 2, deposit `Annotation.png`, Zenodo
  record JSON.
* `images/Figure6_source_data_1/{A,B,C}`: raw 16-bit images and focus photos. The duplicate
  `B/B_focus.tif` was removed.
* `authors/`: article PDF and figures PDF (Fig. 6C/D bar charts), the Fig. 6 image and
  Supplementary file 1.
* `archives/` was removed. It held the deposit zips from `screen_files.py`: the Fig. 6 zip is
  re-downloaded by `fetch.sh`, and the others are kinetics raw data and AlphaFold models,
  unrelated to the experiment.
