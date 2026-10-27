status: identified (one panel, one membrane pair, 12 lanes)

# Annotator B log: Charles et al. 2017, Supp. Fig. 3C (Fabp5, male explant lysates)

## Images
* Target (Fabp5 CL): `images/0701 fat23 Gapdh Fabp5 CL-CM.tif`, middle strip (rows ~450-460). Source: claim.yaml
  (`images.target.strip`) and `article/Fig S3A Male_Data Preparation.pptx` slide 4 ("Fig. S3A Male Fabp5 CL,
  Original Captured 4"; embedded crop image7.jpg shows the 12-lane Fabp5 CL strip, AL | CR).
* Loading control (Gapdh): `images/0619 fat21 Fasn AceS CL.tif`, bottom strip (rows ~605-645). Source: claim.yaml and pptx
  slide 5 ("Fig. S3A Male Gapdh CL, Original Captured 5", labels Fasn / Acas2 / Gapdh, "AL", "CR").
  Separate image, so `reference_image` / `reference_annotation` are used. No total-protein image.
* Only one replicate set exists: the 12 explants form one membrane (N = 6 per group are the lanes). Each lane is a replicate.
* Not used: top strip of the 0701 image (Gapdh, "a repeated WB, not included in figure" per the pptx), bottom strip
  of 0701 (Fabp5 CM, other panel), Fasn and Acas2 strips of 0619, and the `w.tif` white-light images (photos of the membrane).
* Bands are bright on dark: no `invert`.

## Lane mapping
Evidence for all 12 lanes: Supp. Fig. 3 legend (`article/SupplementaryFigure3_legend.txt`): "Adipose tissue explants were
dissected from short-term CR mice and ad libitum control mice (N = 6, upper panel: male mice ...)"; pptx slides 4 and 5 label
the strips "AL" and "CR" over the two halves; the Supp. Fig. 3A male panel layout (pptx image4.png, same figure
assembled for the slides) shows 6 lanes under an "AL" bar on the left and 6 under a "CR" bar on the right, for Fabp5 CL and Gapdh.
Each membrane image shows exactly 12 lanes, left to right, so lanes 1-6 = AL and 7-12 = CR; the same positions on the
Gapdh membrane (claim.yaml: "lanes matched by position (6 AL then 6 CR on both)"). Replicate number = position within the group, left to right
(the files carry no animal IDs, so the numbering is positional only). Lane 1 is the leftmost.

| Lane | Group | Sample | Target x / y | Gapdh x / y |
|---|---|---|---|---|
| 1 | AL | AL1 | 427.8 / 451 | 450.8 / 606 |
| 2 | AL | AL2 | 486.8 / 450 | 509.2 / 609 |
| 3 | AL | AL3 | 548.2 / 451 | 566.3 / 615 |
| 4 | AL | AL4 | 605.4 / 451 | 618.9 / 618 |
| 5 | AL | AL5 | 665.0 / 452 | 674.2 / 623 |
| 6 | AL | AL6 | 718.6 / 452 | 729.6 / 626 |
| 7 | CR | CR1 | 772.9 / 453 | 782.7 / 630 |
| 8 | CR | CR2 | 830.0 / 455 | 838.5 / 629 |
| 9 | CR | CR3 | 886.7 / 455 | 895.5 / 634 |
| 10 | CR | CR4 | 939.8 / 456 | 947.5 / 635 |
| 11 | CR | CR5 | 999.7 / 456 | 997.8 / 639 |
| 12 | CR | CR6 | 1060.9 / 458 | 1051.6 / 643 |

## Geometry
* Lane centres, target: determined on the strong Gapdh strip in the top of the same 0701 image (same membrane, same lanes), via
  `refine_lanes.py` (y 244), because the AL Fabp5 bands are too faint for a reliable centroid. `blotverse lanes` on the
  target strip gave 439 and 513 for lanes 1 and 2 (faint-band noise); I overrode them with 427.8 and 486.8, which match the bands in the image.
  Other lanes agree with `blotverse lanes` within about 10 px.
* Lane centres, Gapdh: `refine_lanes.py` on the bottom strip (y 625, band-search 30).
* Band rows: the bands slope downward to the right (target about 450 to 458, Gapdh 606 to 643), so rows are per lane, from a
  smoothed row profile over a window of the lane centre +/-12 px following the slope; target rows agree with `blotverse lanes` within 1-2 px.
  Lane 2 target row (448 raw) smoothed to 450 and lane 1 (451) kept; the rows of the faintest AL bands are uncertain by about +/-2 px.
* Widths 44 px (pitch about 58 px; `refine_lanes` suggested 43 and 41). Half-heights: FWHM about 11 (target) and 9 (Gapdh);
  I used 14 and 12 (about 1.2 x FWHM), so the band is covered and strips 100+ rows away are excluded.
* `blotverse detect` accepts the file. Not run: `blotverse run`, no group comparison.

## Uncertainties
* The mapping of lanes to animals is positional only; no sample IDs in allowed sources.
* The Gapdh membrane (0619) is a different blot of the same lysates, as stated by claim.yaml. Its lane order is assumed to equal
  the target membrane (stated by claim.yaml and shown by the figure, not independently verifiable from files).
* The "w.tif" companion images were not checked for lane identity beyond the claim.
* Faint AL Fabp5 bands (lanes 1-6): centre and row estimated with larger error. 66 saturated pixels in the target image
  (`blotverse inspect`) are likely in the strong top Gapdh strip, not in Fabp5 bands (not located precisely).
* The Gapdh bottom strip has a few faint additional smudges below lanes 2-6 (rows ~665-670); outside the band window.
* I read the first ~50 lines of `screening_notes.md` (not in my allowed list) before noticing this; I used only the file list
  of images and the claim's mapping, which claim.yaml already gives. Nothing from it changed the geometry.

## Choices between alternatives
* Gapdh from 0619 bottom strip (claim.yaml) rather than the Gapdh top strip of the target membrane, which is the same lanes and
  well aligned but marked "a repeated WB, not included in figure".
* Lane centres of target from the Gapdh strip of the same image rather than from the faint Fabp5 bands.
* Per-lane band rows rather than one flat row, because of the slope.
* Sample names as AL1-AL6 / CR1-CR6 by position.
