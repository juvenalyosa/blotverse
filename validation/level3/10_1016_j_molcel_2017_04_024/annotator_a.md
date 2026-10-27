status: annotated

# Annotator A: 10.1016/j.molcel.2017.04.024, Fig. S7D (Cdc48 / Pgk1, WT vs cdc48-3)

Sources used: `claim.yaml`, `article/FigS7D_overview.png`, `article/deposit_metadata.json`,
`article/DocumentS1_mmc1.pdf` and `article/article_plus_SI_mmc8.pdf` (text only),
`images/`. I did not open `authors/` or `screening_notes.md`, did not read any other
annotator's file, and did not use band intensities to assign lanes.

## 1. Images

| Role | File | Deposit name (Mendeley t4xtrtj2gp v3) | Size |
|---|---|---|---|
| Target (Cdc48) | `images/FigS7D_IB-Cdc48.tif` | "Figure S7D IB-Cdc48 .tif" | 918 x 1024, uint16 |
| Loading control (Pgk1) | `images/FigS7D_IB-Pgk1.tif` | "Figure S7D IB-Pgk1.tif" | 1080 x 574, uint16 |

* The deposit lists exactly two files for S7D, named by figure panel and antibody. The S7F files
  ("Figure S7F IB-Cdc48.tif", "...IB-Pgk1.tif") belong to another panel and were not used.
* `blotverse inspect` on both: "bright background: the image looks inverted (dark bands)". So
  `invert: true`.
* Pgk1 is a separate image of a different size, so I used `reference_image` +
  `reference_annotation`, not `total_protein`.
* Both images are composites. The left part (x < ~237 in Cdc48, x < ~213 in Pgk1) is a
  white-light photo of the marker strip with handwritten labels "αCDC" and "αPGK". The
  chemiluminescence part is to the right. All lanes I annotated lie in the chemiluminescence part.

## 2. Lane mapping

Legend of Fig. S7D (DocumentS1_mmc1.pdf): "Relative expression levels of Cdc48 between
wild-type (WT) and cdc48-3 mutant. Total lysate prepared by the mild alkali method was
subjected to immunoblotting with anti-Cdc48 and anti-Pgk1 antibodies. Signal intensities of
Cdc48 were quantified and normalized to Pgk1 and represented as means ± SEM of three
biologically independent experiments."

Published crop (`FigS7D_overview.png`): two adjacent lanes, labelled left to right "WT",
"cdc48-3". Cdc48 is at ~98 kDa and Pgk1 just below 49 kDa.

Raw membrane: both images show six sample lanes, arranged as three pairs of two adjacent
lanes. An empty lane separates each pair (Cdc48 x ≈ 345-395 and 495-545; Pgk1 x ≈ 335-375 and
485-525). There are no further sample lanes right of x ≈ 645 (Cdc48) or ≈ 635 (Pgk1).
Six lanes = 3 experiments x 2 strains, which matches "three biologically independent
experiments" with one WT and one cdc48-3 lysate each.

| Lane (L→R) | Cdc48 x | Pgk1 x | Group | Sample | Evidence |
|---|---|---|---|---|---|
| 1 | 275.1 | 256.8 | WT | exp1 | Left lane of pair 1. The published crop puts WT left of cdc48-3 within an adjacent pair |
| 2 | 323.5 | 306.8 | cdc48-3 | exp1 | Right lane of pair 1 (crop order) |
| 3 | 419.4 | 403.4 | WT | exp2 | Left lane of pair 2 (crop order) |
| 4 | 470.3 | 455.7 | cdc48-3 | exp2 | Right lane of pair 2 (crop order) |
| 5 | 568.6 | 554.5 | WT | exp3 | Left lane of pair 3 (crop order) |
| 6 | 621.7 | 606.3 | cdc48-3 | exp3 | Right lane of pair 3 (crop order) |

Lanes left out: the marker strip in the white-light overlay (left edge of each image), and
the two empty separator lanes.

The Cdc48 and Pgk1 images map lane-for-lane. Both show the same pattern: pair, gap, pair,
gap, pair. Both have the marker at the left. The claim and the handwritten labels describe
them as the upper and lower strips of one membrane. Lane k in one image is lane k in the other.

## 3. Geometry

Method: I inverted copies of both images in the scratchpad, because `refine_lanes.py` and
`blotverse lanes` expect bright bands.

* **Lane centres:** `refine_lanes.py` on the inverted images.
  * Cdc48: approximate x 274 323 420 469 569 621, y 492, `--search 20 --band-search 12`.
  * Pgk1: approximate x 257 307 404 456 555 607, y 150, `--search 20 --band-search 15`.
* **Band rows:** row maximum per lane, measured over the central 24 columns.
* **Checks against the image:** column and row profiles, plus overlay renders of every box on
  the image.

Cross-checks:

* `blotverse lanes` on the raw Cdc48 image: "found no lane signal" (bands are not inverted).
  On the raw Pgk1 image it returned evenly spaced lanes at the wrong positions (139 px rows).
  I rejected both results.
* `blotverse detect` on a draft with `invert: true` gave Cdc48 target rows 493/494/492/491/490/490.
  These are identical to my rows. Its lane centres (274/333/417/478/562/624, width 53)
  assume even spacing and misplace the paired lanes, so I rejected them.

Final values:

* **Cdc48 lanes:** centres 275.1, 323.5, 419.4, 470.3, 568.6, 621.7.
  * Width 36. The lanes measure ~37-40 px at the band row; for example, lane 1 spans x 256-293
    and lane 2 spans 304-342. `refine_lanes` suggested 40. I narrowed it to 36 so that boxes
    do not reach the ~10 px gap between paired lanes, where the lane edges are soft.
* **Cdc48 band rows:** 493, 494, 492, 491, 490, 490 (a slight tilt).
  * Half-height 8. The per-lane FWHM is 6-7 px. The mean row profile is above background
    from ~483 to ~500.
  * `refine_lanes` gave 11, but that value comes from pooling lanes with different peak
    rows, which broadens the profile. 8 covers the band and keeps out the faint lower band
    at y ≈ 548-552.
* **Pgk1 lanes:** centres 256.8, 306.8, 403.4, 455.7, 554.5, 606.3.
  * Width 40. The bands are ~43-47 px wide and paired lanes nearly touch (gap ~4 px at x ≈ 280,
    430, 580), so 40 keeps each box inside its own lane.
* **Pgk1 band rows:** 151, 152, 149, 148, 148, 148.
  * Half-height 15 (from `refine_lanes`). FWHM is 11-12 px. The mean row profile is above
    background from ~134 to ~166, so ±15 covers the band.
* The overlay renders show every box centred on its band.

## 4. Uncertainties

1. **WT/cdc48-3 order within each pair is inferred, not stated.** No source labels the
   deposited lanes. The order rests on the published crop, which shows "WT" left of
   "cdc48-3" in two adjacent lanes. I assume the crop is one of the three pairs, and that all
   pairs were loaded in the same order. If a pair were loaded in reverse, that replicate's
   groups would swap. I judge this the standard reading of the layout, not a guess between
   equally likely options. Still, it is the main assumption.
2. **The replicate number (exp1/exp2/exp3) for each pair is arbitrary.** No source says which
   experiment each pair is, or which pair is the published crop. The pairing itself (one WT
   and one cdc48-3 lane per experiment) is supported by the gap layout. Only the labels are
   arbitrary.
3. **Saturation.** Pixels at value 0 in these dark-band images are at the detector limit.
   * Cdc48 band windows: 75, 50, 68, 43, 4 and 24 such pixels in lanes 1-6.
   * Pgk1 band windows: ~210-270 such pixels in every lane.
   * The loading control is therefore clipped in all lanes, and the target in most.
   * Note: `blotverse inspect` reports only the 65535-valued pixels before inversion (254 and 49).
     It counts the zeros only after `invert: true`.
4. **Same membrane.** I assume Cdc48 and Pgk1 are strips of the same membrane, based on the
   claim, the legend and the matching layout. The two images have different sizes and
   offsets (Pgk1 lanes sit ~15-18 px left of the Cdc48 lanes), so they were annotated
   independently.
5. **Lane 5 Cdc48 smear.** Pair 3 (lanes 5-6) shows a somewhat weaker background smear above
   the band. This is only noted; it was not used for mapping.

## 5. Choices between alternatives

* `reference_image`/`reference_annotation` rather than `total_protein`: Pgk1 is a separate image
  of a different size, and it is a housekeeping protein, not a total-protein stain.
* `invert: true` for both images, following the `blotverse inspect` warning and the visible dark
  bands.
* Lane centres from `refine_lanes.py` rather than `blotverse detect`, because the even-spacing
  assumption fails for the paired layout.
* Cdc48 width 36 instead of the suggested 40, and half-height 8 instead of 11 (reasons in §3).
* Pgk1 width 40 instead of 39, which is effectively the same.
* Per-lane band rows instead of one shared row, because of the visible 3-4 px tilt across the
  membrane.
* Sample labels `exp1`-`exp3` in pair order from left to right.
