# Screening notes: 10.1016/j.celrep.2017.09.051

Charles et al., "Uncoupling of Metabolic Health from Longevity through Genetic Alteration of
Adipose Tissue Lipid-Binding Proteins", Cell Reports 21:393–402 (2017), PMC5682597. Deposit:
Mendeley Data rp8s5rd98p v1 ("Original data of images and Western blot"). Second frame, rank 27.
I screened it under the brief with the Amendment 2 addition: criterion 7 is recorded but no longer
excludes, and phospho/total counts as a loading control.

## Criterion 1

The article is a peer-reviewed journal article (Cell Reports), not a preprint. Its western blots
are quantified by the authors (Supp. Fig. 3B, 3C and 3D, each "normalized to Gapdh"), so it passes.

The article text and figures come from PMC (author manuscript). The PMC supplement link returned a
reCAPTCHA page, and Europe PMC reports the article as not open access. I therefore took the
Supplemental Information from the Elsevier CDN (`ars.els-cdn.com`, PII S2211124717313414: mmc1 =
Supplemental Information, mmc2 = article plus SI). No API that needs an email address was used.

## Quantified western blot panels in figure order

| Panel | Quantification | Unit of analysis? |
|---|---|---|
| Fig. 3B (quantified in Supp. Fig. 3B) | Fabp4, Fabp5, Acc, Acly "normalized to Gapdh"; subcutaneous adipose tissue, AL vs CR, N = 3 (liver AL lanes are a reference only) | yes |
| Supp. Fig. 3A | secretion index = CM band / CL band of the same protein (Fabp4, Fabp5), AL vs CR, N = 6, male and female | no: the reference is the cell-lysate band of the same protein, which is neither a housekeeping protein, total protein nor the total form of a modification-specific target |
| Supp. Fig. 3C | male explant lysates (the explants of S3A, upper panel): Fabp4, Fabp5, Acc, Acly, Acas2 "normalized to Gapdh", AL vs CR, N = 6 | yes |
| Supp. Fig. 3D | Srebp1c mature/precursor "normalized to Gapdh", AL vs CR, N = 3 | yes (not examined: S3C qualified first) |
| Supp. Fig. 3E | WT vs Fabp4/5−/− blot, no quantification | no |

Figs. 1, 2 and 4 contain no western blots. The deposit's Fig. 2 and Fig. 4B files are histology,
EM and photos.

## Panels examined

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 3B / S3B | **2** | The deposit files for Fig. 3B are 16-bit greyscale **PNG** (`ML001_099_2016-08-03_CR24_Fabp4_1/2_16bit.png`, `ML001_099_2016-08-11_CR28_Acc1_Fabp5_1/2_16bit.png`, `ML001_100_2016-08-19_CR30_Gapdh-bTub-Hsp90_1/3_16bit.png`). `blotverse inspect` gives raw = no on all six: "rejected: not a readable TIFF file". The deposit has no TIFF or native imager file for Fig. 3B. Criterion 2 accepts only `.scn`/`.gel` or 16-bit/float TIFF. |
| Supp. Fig. 3A | not a unit | CM/CL secretion index (see above). |
| **Supp. Fig. 3C (Fabp5)** | **included** | See the per-criterion record below. |

### Supp. Fig. 3C, criterion by criterion

* **Primary contrast.** The first contrast of the panel stated in the text is in Results ("Caloric
  restriction mimics adipose tissue lipidome changes observed in Fabp deficiency"): "Conversely,
  CR increased the expression of Fabp5 in adipose tissue without altering Fabp4 (Figure 3B and
  Supplementary Figure 3B–C)." This gives target Fabp5, CR vs AL, direction up. The plot marks
  Fabp5 with *** (unpaired two-way Student's t-test). Fabp4, Acc, Acly and Acas2 are secondary.
* **C2.** Target membrane `0701 fat23 Gapdh Fabp5 CL-CM.tif`: 1392×1040, 16-bit, raw = yes,
  66 saturated pixels (QC will decide which lanes they touch). Loading-control membrane
  `0619 fat21 Fasn AceS CL.tif`: 16-bit, raw = yes, 0 saturated pixels. The two white-light
  companions (`… w.tif`) are also raw = yes.
* **C3.** Both images show the whole membrane strips with all 12 lanes.
* **C4.** The legend gives N = 6 mice per group (S3A, "N = 6, upper panel: male mice"; S3C:
  "explants described in Panel A Upper Panel"). Both images contain 6 AL and 6 CR lanes, for
  Fabp5 CL and for Gapdh.
* **C5.** The deposit's overview `Fig S3A Male_Data Preparation.pptx` identifies the lanes without
  any values. Slide 4, "Fig. S3A Male Fabp5 CL, Original Captured 4", places a crop box over the
  middle strip of image 0701. Slide 5, "Fig. S3A Male Gapdh CL, Original Captured 5", places one
  over the bottom strip of image 0619, labelled Fasn / Acas2 / Gapdh. Both slides label the lanes
  AL | CR with "Invert & Crop" and no flip, and the figure (S3A upper panel) shows 6 AL then 6 CR.
  In the 0701 image the top strip is Gapdh, labelled "A repeated WB, not included in figure", so
  the figure's Gapdh is the 0619 one. The bottom strip of 0701 is Fabp5 CM (slide 3). A visual
  check agrees: lane 2 is strong and lane 4 weak in the 0619 Gapdh, as in the figure's Gapdh row.
  I did not use any of the authors' numbers.
* **C6.** Gapdh, a housekeeping protein, is present for every lane on a second membrane loaded with
  the same 12 lysates (the S3A figure shows it as the loading row). Lanes match by position, so it
  goes in as `reference_image`.
* **C7 (recorded, not excluding).** No per-replicate numbers exist. The deposit holds images and
  pptx only, and S3C is a bar chart of mean ± SEM. T1 and T2 cannot be computed.
* **C8.** The lanes are vertical, with no rotation needed.

**Decision: included, Supp. Fig. 3C (Fabp5 / Gapdh, CR vs AL).**

## Files

* `fetch.sh` re-downloads everything listed below and checks the SHA-256 of each file. I ran it
  in a scratch copy and every hash matched.
* `article/` holds:
  * `pmc.html` (the PMC author manuscript);
  * main figures `nihms910522f1–f4.jpg`;
  * `SupplementaryFigure3_legend.txt` (legend text only, from mmc1 page 4);
  * the deposit overview `Fig S3A Male_Data Preparation.pptx` (no values).

  The article text quotes ELISA values and the S3A secretion fold (2.0 ± 0.2). It quotes none for
  S3C.
* `images/` holds `0701 fat23 Gapdh Fabp5 CL-CM.tif` (target), `0619 fat21 Fasn AceS CL.tif`
  (Gapdh) and their white-light images. I removed the 18 unrelated files that `screen_files.py`
  had downloaded (EM and histology TIFFs from Figs. 2 and 4, and the `0618` Fabp4 CM image).
* `authors/` holds `S2211124717313414-mmc1.pdf` (Supplemental Information, which prints the
  S3B/S3C bar charts) and `S2211124717313414-mmc2.pdf` (article plus SI).
* The Fig. 3B PNGs and the other pptx files were downloaded to a scratch directory only, for the
  criterion-2 check.
