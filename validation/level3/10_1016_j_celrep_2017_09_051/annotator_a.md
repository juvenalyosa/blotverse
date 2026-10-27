# Annotator A log: 10.1016/j.celrep.2017.09.051, Supp. Fig. 3C (Fabp5 CL, male)

status: annotated (mapping identified from the allowed sources)

Sources used: `claim.yaml`; `article/SupplementaryFigure3_legend.txt`;
`article/Fig S3A Male_Data Preparation.pptx` (slides 1, 4 and 5, with their embedded
media); `images/` (4 TIFFs). Not opened: `authors/`, `screening_notes.md`, any other
annotator's files. No author numbers were read. Lane identity was never inferred from
band intensity.

## Images

| Role | File | Region |
|---|---|---|
| Target, Fabp5 cell lysate (CL) | `images/0701 fat23 Gapdh Fabp5 CL-CM.tif` (1392x1040, 16-bit) | middle membrane strip, band row y ~447-458 |
| Loading control, Gapdh CL | `images/0619 fat21 Fasn AceS CL.tif` (1392x1040, 16-bit) | bottom membrane strip, band row y ~606-643 (slanted) |
| White-light images (strip outlines and handwriting only) | `... CL-CM w.tif`, `... CL w.tif` | not quantified |

Evidence for the files:
* pptx slide 4 is titled "Fig. S3A Male / Fabp5 CL", "Original Captured 4". Its embedded raw image (`ppt/media/image5.tif`) is pixel-identical to `0701 fat23 Gapdh Fabp5 CL-CM.tif` (checked with `numpy.array_equal`). The strip labels on the slide read, from top to bottom, "Gapdh / A repeated WB, not included in figure", "Fabp5", "Fabp5". The rounded "Invert & Crop" box (rotated 90 degrees) maps to raw rows ~362-538, which is the middle strip.
* pptx slide 5 is titled "Fig. S3A Male / Gapdh CL", "Original Captured 5". Its embedded raw image (`image8.tif`) is pixel-identical to `0619 fat21 Fasn AceS CL.tif`. The labels read "Fasn", "Acas2", "Gapdh", and the crop box maps to raw rows ~551-727, which is the bottom strip and its lowest band.
* `inspect`: all four files are raw 16-bit. `0701` has 66 pixels at 65535. They lie in rows 246-247, x 472-512, which is the top strip (the repeated Gapdh, not used). The Fabp5 and 0619 Gapdh windows contain no saturated pixels.
* The images are chemiluminescence, with bright bands on a dark background, so there is **no `invert`**. Because the loading control is a separate image of a different membrane, it is given as `reference_image` / `reference_annotation`. It is not `total_protein`.

## Lane mapping

Figure layout (pptx slide 1, the Supp. Fig. 3A image `ppt/media/image1.png`, and the slide 4/5 panel image `image4.png`): the "Male" panel has 12 lanes under a bar "AL" over lanes 1-6 and a bar "CR" over lanes 7-12. Rows: Fabp4 CM, Fabp4 CL, Fabp5 CM, Fabp5 CL, Gapdh. The legend says "Adipose tissue explants were dissected from short-term CR mice and ad libitum control mice (N = 6, upper panel: male mice ...)", and panel C is "Density analysis ... in adipose explants described in Panel A Upper Panel". This gives 6 AL + 6 CR, one lane per mouse, so each lane is one biological replicate. Slides 4 and 5 repeat the "AL" and "CR" labels with arrows over the left and right halves of the cropped strip.

Orientation (no mirror):
* The slides say only "Invert & Crop". No flip or rotation step is stated.
* The handwritten labels on the white-light images ("23 CL", "fat23 CM", "fat21 CL") read upright and left to right, with the marker lanes at the left edge.
* As a geometric check, I registered the authors' displayed crops (`image7.jpg` for Fabp5 CL, `image9.jpg` for Gapdh CL) to the raw strips, unflipped and mirrored. This is a whole-strip pattern correlation, used only to fix orientation and position. It did not assign lanes from intensities.
  * Fabp5 CL: r = 0.95 unflipped vs 0.19 mirrored.
  * Gapdh CL (after a 3.5 degree de-skew): r = 0.86 unflipped vs 0.61 mirrored.

  So the raw left-to-right order equals the figure's left-to-right order.

Lanes excluded: the molecular-weight marker lanes. On 0701's middle strip they are at x ~375 and ~1110; on 0619's bottom strip at x ~400, which is visible on the white-light images. No other lanes are present: the strips carry exactly 12 sample lanes.

| Lane (left to right) | Group | Sample | Fabp5 x (0701) | Gapdh x (0619) | Evidence |
|---|---|---|---|---|---|
| 1 | AL | AL1 | 430.0 | 452.0 | 1st of 6 lanes under "AL" (Supp. Fig. 3A male panel; slides 4/5) |
| 2 | AL | AL2 | 495.5 | 510.0 | 2nd lane under "AL" |
| 3 | AL | AL3 | 557.5 | 567.0 | 3rd lane under "AL" |
| 4 | AL | AL4 | 611.5 | 621.0 | 4th lane under "AL" |
| 5 | AL | AL5 | 669.0 | 676.0 | 5th lane under "AL" |
| 6 | AL | AL6 | 724.5 | 730.0 | 6th lane under "AL" |
| 7 | CR | CR1 | 777.0 | 783.5 | 1st of 6 lanes under "CR" |
| 8 | CR | CR2 | 832.0 | 839.5 | 2nd lane under "CR" |
| 9 | CR | CR3 | 887.0 | 895.0 | 3rd lane under "CR" |
| 10 | CR | CR4 | 939.5 | 947.0 | 4th lane under "CR" |
| 11 | CR | CR5 | 998.5 | 997.5 | 5th lane under "CR" |
| 12 | CR | CR6 | 1060.5 | 1053.0 | 6th lane under "CR" |

The sample names are positional (AL1 = 1st AL lane). No mouse IDs are given in the allowed sources. Target lane k and Gapdh lane k are taken to be the same lysate. The evidence: claim.yaml ("probed on a second membrane loaded with the same 12 lysates ... lanes matched by position"), the figure stacking the Gapdh row under the Fabp5 CL row in the same 12 columns, and the identical AL | CR layout on slides 4 and 5.

## Geometry

Steps:
* `blotverse lanes` on each image with 12 lanes. Fabp5: target-y 455, lanes-x 428,1062. Gapdh: target-y 622, lanes-x 452,1052.
* `refine_lanes.py` with eyeballed centres. It returned widths 43 and 41, and half-heights 16 (Fabp5) and 22 (Gapdh). The Gapdh 22 is inflated because the band is slanted and the script averages one profile across lanes.
* Per-lane checks with my own helper scripts in the scratchpad: the band row (row maximum) and FWHM per lane, and the band's 30%-of-max horizontal extent per lane.
* Overlays of all boxes on contrast-stretched crops, inspected by eye.

Final values:
* **Fabp5 (0701):**
  * Centres are the midpoints of each band's 30% extent. The refine centroids were 2-9 px to the left on lanes 2, 3, 4 and 12, where the bands are asymmetric.
  * Width 47. Pitch ~52-65 px; bands are 39-57 px wide at 30% of max.
  * Per-lane rows 447-458 (slight slant).
  * Half-height 16 = ceil(1.1 x max FWHM 14), which matches refine_lanes.
* **Gapdh (0619):**
  * Centres from the same midpoint rule. They agree with refine and with `blotverse lanes` to within about 4 px.
  * Width 42. Pitch ~50-58 px.
  * Per-lane rows 606-643. The band slopes down to the right by about 37 px over the strip.
  * Half-height 10 = ceil(1.1 x FWHM 8) plus about 1 px for the within-lane slope.
* `blotverse detect` on the file parses and validates. The annotation is kept as written.

## Uncertainties

1. **Two membranes.** Target (membrane "fat23", image 0701) and loading control (membrane "fat21", image 0619) come from different blots. That both were loaded in the same lane order with the same 12 lysates rests on the slide titles ("Fabp5 CL" and "Gapdh CL", both "Fig. S3A Male"), the shared AL | CR labels, and the figure layout. No lane-by-lane sample list is given.
2. **Positional sample names.** Within a group, which mouse is AL1..AL6 / CR1..CR6 is unknown. It is irrelevant to an unpaired group comparison.
3. **Faint AL bands.** The Fabp5 band is faint in lanes 1-6, so their per-lane rows (447-453) come from a low-signal maximum. They agree with the neighbouring strong lanes to within a few pixels.
4. **Band overrun.** The right ends of the Fabp5 bands in lanes 2 and 12 are smeared and extend a few pixels past the 47-px box. The width was not increased, to avoid overlapping neighbouring lanes, since lanes 11 and 12 are only ~62 px apart and bands are up to 57 px wide.
5. **Gapdh slant.** The Gapdh band is tilted (~3.5 degrees). A rectangular window of half-height 10 contains the band in every lane, as checked on the overlay, but its edges include a little background where the band slopes.
6. **Faint secondary bands** lie ~45-60 px below the Gapdh band in lanes 2-6 of 0619. They are outside the windows (±10 px).

## Choices between alternatives

* **Loading control:**
  * Chosen: the Gapdh CL on `0619` (bottom strip). This is the image the authors used in the figure (slide 5) and the one claim.yaml names.
  * Rejected: the top strip of `0701` (Gapdh on the same membrane as Fabp5 CL). It is labelled "A repeated WB, not included in figure" and is partly saturated (lane 2).
* **Target strip:**
  * Chosen: the middle strip of 0701 (Fabp5 CL), following the slide 4 crop box.
  * Rejected: the bottom strip (Fabp5 CM, conditioned medium), which belongs to another sub-panel.
* **Image type:** `reference_image` / `reference_annotation`, not `total_protein`. The control is a housekeeping protein imaged on a separate membrane.
* **No invert:** the images have bright bands on a dark background. The authors' figure inverts them only for display.
* **Lane centres:** midpoints of the 30% band extent rather than the refine_lanes centroids, because asymmetric bands biased the centroids. The difference is at most 9 px; the remaining lanes differ by 0-5 px.
* **Gapdh half-height:** 10 (per-lane FWHM) rather than refine_lanes' 22, which is inflated by the slant across lanes.
* **One blot entry** with 12 lanes. This is one membrane per antibody; there are no replicate blots.
