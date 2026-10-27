# Screening: 10.1016/j.molcel.2023.06.020

Prasad V, Cerikan B, Stahl Y, et al. *Enhanced SARS-CoV-2 entry via UPR-dependent
AMPK-related kinase NUAK2.* Mol Cell 83(14):2559-2577, 2023 (CC BY-NC-ND 4.0).
Deposit: Mendeley Data jsbnp3nyv3 (v1), "DataSet1", folders `WesternBlots/` and `ImagingData/`.
The deposit has no source-data tables (no xlsx/csv/pzfx).

## Sources (in `article/`)

| File | Source | SHA-256 |
|---|---|---|
| `Prasad2023_MolCell_published.pdf` | published version, UNIGE open archive, https://access.archive-ouverte.unige.ch/access/metadata/0e9a1334-3028-4ac8-b6e8-bb2d3db84730/download | e8c0bf094c3b81b84c5b0be5d263762ce216663d3b5eeae63e27f8f32d42b003 |
| `mmc1_supplementary_figures.pdf` | https://ars.els-cdn.com/content/image/1-s2.0-S1097276523004677-mmc1.pdf (Figures S1-S7 and legends) | 696384fb7d4553fffd2ed1b48ab7170892b5f6ce5488b6a98868a8132f3d3b7f |
| `*.txt` | text extracted from the two PDFs (PDFKit) | |
| `mendeley_jsbnp3nyv3_dataset.json` | https://data.mendeley.com/public-api/datasets/jsbnp3nyv3 | 394520450247057e2c1524ca279e32441fc8c95f870a825152e931de781c75cd |
| `deposit_Fig-*-highlighted.png` | authors' overview PNGs from the deposit (red boxes mark the lanes shown in the figure) | see `sha256` in the dataset JSON |

Publisher pages (cell.com, sciencedirect.com) return 403 to scripted access; the Elsevier API
returned metadata only. Europe PMC has no full text (PMID 37421942, no PMCID).

Criterion 1: peer-reviewed article (Mol Cell), with quantified western blots: **met**.

## Quantified western blot panels, in figure order

Panels where a target band is normalised to a loading control and ≥ 2 groups are compared.
Not counted: Fig 1A (UPR controls, not quantified); Fig 1D and Fig S1E (percent phosphorylated
IRE1α = p-IRE1α / (p-IRE1α + IRE1α) within one Phos-tag lane, no loading control);
Fig 6I-K (F-actin / G-actin fraction ratio, no loading control).

Criterion 2 (`blotverse inspect`, column `raw`) passes for every deposited WB TIFF
(all 16-bit greyscale, `raw = yes`): `Fig-1C-SARS-UPR/I_*-uncropped.tif` (8 files),
`Fig-4F-NUAK2-kinetics-SARS/NUAK2-kinetics.tif`, `Fig-S5C-siNUAK2-kd-levels/{siNUAK2-uncropped,GAPDH}.tif`,
`Fig-6I`, `Fig-6J/A1058F/{Actin-NUAK2-composite,GAPDH}.tif`, `Fig-1A` and `Fig-S2E` scans.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig 1C (p-eIF2α and GADD34 vs β-actin, time course NI/4/8/12/16/24 h; quantified in Fig S1C, S1D) | 3 | Methods: band intensities "normalized to the b-Actin loading control"; legend S1C/D: one-way ANOVA, "three biologically independent experiments". Deposit `Fig-1C-SARS-UPR/` holds one membrane per antibody, each with a single set of 8 sample lanes (NI, 4, 8, 12, 16, 24 h, ctr, DTT) = one replicate (`Uncropped-highlighted.png`). Replicates 2-3 are not deposited, so the deposited images do not show every lane of the quantified replicates (criterion 4 would also fail). |
| Fig 4F (NUAK2 vs GAPDH, A549-ACE2, SARS-CoV-2 0/4/8/12/24 h) | 3 | Legend: "mean of n = 3 experiments ± SD ... **p < 0.01; ***p < 0.001", one-way ANOVA. Deposit `Fig-4F-NUAK2-kinetics-SARS/NUAK2-kinetics.tif` (raw = yes, 16-bit) is one uncropped membrane with 5 sample lanes (NUAK2 top, GAPDH bottom, ladder right) = one replicate. Replicates 2-3 are not deposited. |
| Fig S4C (NUAK2 vs GAPDH, siControl vs siNUAK2; deposit folder named `Fig-S5C-siNUAK2-kd-levels`) | 3 | Legend: two-tailed unpaired t test, "three biologically independent experiments", ** p < 0.01. `siNUAK2-uncropped.tif` / `GAPDH.tif` (raw = yes) show 5 sample lanes; the overview marks only lanes 4-5 (one siControl/siNUAK2 pair). The other 3 lanes are unlabelled, and 5 lanes cannot hold 3 control/KD pairs: the 3 replicates are not all in the deposited images. |
| Fig S4I (NUAK2 vs GAPDH, Control vs DTT 4 h, A549-ACE2) | 2 | Legend: t test, n = 3, *** p < 0.001. No raw image of this blot in the deposit (the `Fig-1A-UPR-controls` scans are Calu-3 DTT/Tg 1 h/4 h, the Fig 1A panel), so there is no file for `blotverse inspect`. |

Order note: the Fig 1C blots are shown in a main figure and quantified in Fig S1C/D. Whether
they are ranked as Fig 1C (main) or as Fig S1C (after Fig 4F), the first quantified panel fails
criterion 3, so the candidate's reason does not depend on that choice.

Criterion 7 was not reached for any panel. Note: the deposit contains no per-replicate numbers,
and the article has no source-data file.

## Decision

**Excluded.** First failing criterion of the first quantified panel (Fig 1C, quantified in
Fig S1C): **criterion 3**: the deposit contains a single replicate membrane for each
quantified blot (Figs 1C, 4F, S4C), whereas the authors quantify n = 3 biological replicates.

No `fetch.sh` or `claim.yaml` was written (no panel qualifies). `images/` was left as
downloaded by `screen_files.py`; it holds no authors' numbers.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules: criterion 7 no longer
excludes; the total form of the target counts as a loading control when the target antibody is
modification-specific (phospho / total) and each lane can be matched; co-IP bait normalisation
is still not a loading control. Evidence above on criteria 1-6 and 8 is reused: the revised
rules do not touch criteria 2-5.

**Phospho panels reconsidered.**

* Fig 1C / S1C (p-eIF2α): the Methods state that band intensities were "normalized to the
  b-Actin loading control" (then to the membrane sum), so p-eIF2α is quantified against
  β-actin, not total eIF2α. Point 3 does not change the panel; total eIF2α
  (`I_eIF2alpha-uncropped.tif`) would not help anyway, since it is the same single replicate.
* Fig 1D and Fig S1E (% phosphorylated IRE1α, Phos-tag): previously not counted. The blot uses
  a pan-IRE1α antibody on a Phos-tag gel, and the value is p-IRE1α / (p-IRE1α + IRE1α) within
  one lane (Methods, "Mn2+ Phos-tag gel analysis"). The target is not a modification-specific
  antibody, so point 3 (read literally) does not make this a loading-control normalisation and
  the panels stay outside the unit of analysis. Even if they were counted, they rank after
  Fig 1C and fail criterion 3: the only Phos-tag image deposited (folder
  `Fig-S2E-IRE1-Phos-tag`, `221201_SarsCoV2 version 2 IV IRE1a,2-2. Scan.tif`,
  SHA-256 89235e93…f86fb, `blotverse inspect`: 16-bit, raw = yes) shows 2 membrane strips of
  7 lanes each, i.e. one experiment, whereas the legends state n = 3 biologically independent
  experiments (overview `Uncropped-highlighted.png`, SHA-256 5fe9690c…aa02e, looked at in the
  scratchpad, not kept).
* No other phospho / total or co-IP quantification in the article or Figures S1-S7.

| Panel | First failing criterion (Amendment 2) | Evidence |
|---|---|---|
| Fig 1C (p-eIF2α, GADD34 vs β-actin; quantified in Fig S1C/D) | 3 | unchanged: one replicate membrane per antibody deposited, n = 3 quantified |
| Fig 1D / S1E (% p-IRE1α, Phos-tag) | not an eligible panel (not a modification-specific antibody); would fail 3 | single 2 × 7-lane Phos-tag scan, n = 3 quantified |
| Fig 4F (NUAK2 vs GAPDH) | 3 | unchanged |
| Fig S4C (NUAK2 vs GAPDH, siNUAK2) | 3 | unchanged |
| Fig S4I (NUAK2 vs GAPDH, DTT) | 2 | unchanged: no raw image deposited |

Criterion 7 (recorded, no longer excluding): no per-replicate numbers exist (no source-data
file in the article or the deposit).

**Decision under Amendment 2: excluded**, unchanged. First failing criterion of the first
quantified panel (Fig 1C): **3**. No `fetch.sh` or `claim.yaml` written.
