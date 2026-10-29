# Annotator A: 10.7554/eLife.91083, Fig. 6B / 6D (LRRK2 pS935 / total LRRK2)

status: annotated

3 blots (replicates), 7 lanes each (wt, R1441G, G2019S, I2020T, K1906M, T1343A,
T1343A/G2019S). Control group: wt. No `invert`, because the raw TIFFs show bright bands on a dark
background (raw Stella chemiluminescence, 16-bit; `blotverse inspect`: raw = yes, 0 saturated
pixels in all three signal images).

## Sources used

* `article/Figure6_source_data_1_Annotation.png` (deposit) and
  `article/elife-91083-fig6-figsupp2-v1.jpg` (Fig. 6-figure supplement 2). These are the same
  labelled figure. Every membrane is labelled left to right: "wt, R1441G, G2019S, I2020T, K1906M,
  T1343A, T1343A/G2019S, Marker, PEI only, not transfected".
* Legend of Fig. 6-figure supplement 2 (article XML): "Cell-based phospho-Rab assays, blot raw
  images (Stella imaging system, ECL+) used for quantification (ImageJ). (A–C) Biological
  replicates. Panel (C) shows the total-LRRK2 blot (24D8) for panel (B) (replication of the
  western blot B to allow quantification of the reference). ... the membranes shown in one
  panel were imaged together using the Stella system".
* Fig. 6 legend: "(B) Western blot for LRRK2 pS935, total LRRK2 ... (D) Relative LRRK2 pS935
  levels ... (n=3)".
* Deposit folder names `Figure6_source_data_1/A`, `/B`, `/C`, which match panels A, B and C of
  the supplement figure. Each folder has a signal image and a `focus.tif` white-light photo
  (membrane outlines and prestained marker).
* `claim.yaml` (panel, groups, image assignment).

## Images

| Blot | Target (pS935) | Loading control (24D8, total LRRK2) |
|---|---|---|
| rep1_A_left | `A/2023-02-07_2x2-300s-07.tif`, left set, top strip (rows ~60-110) | same image, left set, third strip (rows ~400-420) |
| rep2_A_right | `A/2023-02-07_2x2-300s-07.tif`, right set, top strip (rows ~65-82) | same image, right set, third strip (rows ~395-415) |
| rep3_B_C | `B/2023-02-17_2x2-180s-05.tif`, left membrane, top strip (rows ~145-162) | `C/2023-02-20_24D8_2x2-180s-05.tif`, single strip (rows ~210-228), given as `reference_image` |

In replicates 1 and 2 the 24D8 strip is a separate membrane piece, but it is in the same image
and its lanes line up with the pS935 lanes (centres agree within ~1.5 px on the left set and
~3 px on the right set). So one shared lane set is used, with separate `target_y` and
`reference_y`. In replicate 3 the reference is a separate image (C), so it has its own
`reference_annotation`. `total_protein` is not used, because 24D8 is a total-target antibody,
not a total-protein stain.

## Lane mapping and evidence

The same rule applies to every blot. The labels in the annotation figure give the order
wt -> T1343A/G2019S for sample lanes 1-7, followed by Marker, PEI only, not transfected. In each
raw image I located the prestained marker lane in the matching `focus.tif`. The 7 sample lanes
are the 7 lanes directly left of that marker, at the same lane pitch (~29.5 px). Each focus
image also shows a second marker at the far left of every membrane, one lane-pitch left of wt.
The annotation figure crops it off. It is a ladder, so it is excluded.

### Replicate 1: image A, left set
* Marker (focus.tif, top-left strip): ladder bands at x ~265 (lane 8) and at x ~30 (left edge).
  The sample lanes are at x 61.5, 90, 118.5, 148, 178, 207, 236.
* Lane 1 x=61.5: wt. Lane 2 x=90: R1441G. Lane 3 x=118.5: G2019S. Lane 4 x=148: I2020T.
  Lane 5 x=178: K1906M. Lane 6 x=207: T1343A. Lane 7 x=236: T1343A/G2019S.
  Evidence: the label order in Annotation.png panel A (left), counted from the first lane right
  of the left marker up to the lane before the marker at x ~265.
* Excluded lanes: x ~265 (Marker), ~295 (PEI only) and ~325 (not transfected). Also x ~355 and
  ~385. These two show bands in the raw image but sit to the right of "not transfected". In the
  annotation figure they are cut off: only a sliver of the x ~355 24D8 band shows at the right
  edge of panel A-left. They are not part of the experiment.
* Replicate identity: panel A, left set of membranes. The legend calls panels A–C biological
  replicates, with C only the reference for B. n = 3 therefore implies two replicates in A
  (left and right sets, each a complete, separately labelled set of pS935 / pRab10 / 24D8 /
  Rab10 membranes).

### Replicate 2: image A, right set
* Marker (focus.tif, top-right strip): ladder at x ~845 (lane 8) and x ~600 (left edge).
* Lanes at x 632, 661.5, 692, 721.5, 752, 782, 811.5 = wt, R1441G, G2019S, I2020T, K1906M, T1343A,
  T1343A/G2019S (label order in Annotation.png panel A right).
* Excluded: ~845 (Marker), ~875 (PEI only), ~905 (not transfected), and ~935 and ~960 (extra
  lanes outside the labelled set).

### Replicate 3: image B (target) + image C (reference)
* B focus.tif, top-left strip: ladder at x ~285 (lane 8) and x ~50 (left edge). The pS935
  sample lanes are at x 80, 110, 138.5, 168, 197.5, 225, 255.5 = wt ... T1343A/G2019S (label
  order in Annotation.png panel B left, labelled "pS935"). Lanes at x ~315, ~345 (PEI only,
  not transfected) and ~375-405 (extra) are excluded.
* C focus.tif: ladder at x ~478 (lane 8) and x ~240 (left edge). The 24D8 sample lanes are at
  x 269.5, 299.5, 328.5, 356, 388, 416.5, 447.5 = wt ... T1343A/G2019S (Annotation.png panel C).
  The bands at x ~568 and ~600 are right of "not transfected" and outside the labelled crop, so
  they are excluded.
* B and C are matched by lane label, not by pixel position. C is a separate membrane ("the
  replication of the western blot B") with a different lane offset in the image.
* Not used: image B, right membrane, top strip (labelled "LRRK2 (24D8)" in panel B). The
  legend says panel C is the total-LRRK2 blot used for B.

## Geometry (measured, checked on zoomed renders)

Lane centres come from `refine_lanes.py` (centroid of the background-subtracted column signal)
and my own per-lane horizontal profile at the band row. They were cross-checked with
`blotverse lanes` on A-right and C, which agreed within ~1.5 px. Lane width is 22 px in every
blot, which is 0.75 x the lane pitch (~29.5 px), as `refine_lanes.py` suggests. Band rows are
the per-lane vertical peak, because every strip is slightly tilted. `blotverse detect` parses the
file without error (output written only to the scratchpad).

| Blot | target rows | target FWHM | reference rows | ref FWHM | band_half_height |
|---|---|---|---|---|---|
| rep1 | 96, 98, 93, 92, 89, 86, 85 | 8-23 px (smear) | 413, 412, 412, 410, 408, 409, 409 | 14-18 px | 12 |
| rep2 | 74, 74, 74, 72, 73, 73, 74 | 8-9 px | 402, 403, 404, 405, 406, 407, 407 | 9-14 px | 11 |
| rep3 | 158, 157, 155, 153, 151, 150, 148 | 7-8 px | (image C) 219, 219, 219, 219, 218, 220, 219 | 7-8 px | 8 (target), 9 (reference) |

In rep3 the main annotation's `reference_y` repeats the target rows, which are unused because
the reference comes from `reference_annotation`. In `reference_annotation`, `target_y` =
`reference_y` = the 24D8 rows, because the pipeline reads the reference image band through the
annotation's target window.

## Choices between alternatives

1. **Rep1 band window.** The A-left pS935 bands have a strong upward smear (rows ~65-105) in
   lanes 1, 3, 5, 6 and 7, and lanes 2 and 4 are narrow bands. I centred on each lane's peak
   (the band core) with half-height 12. This covers the core and the lower part of the smear,
   not the whole smear. A larger half-height (~20) would take in more smear but would also widen
   the 24D8 window, which shares the same `band_half_height`. I did not set up a same-image
   `reference_image` with its own half-height, because the loading control is in the same image.
2. **Shared lane set for target and 24D8 in image A** (separate membrane pieces imaged
   together), not a second annotation of the same file. The lane offsets are ≤3 px against a
   22 px window.
3. **Faint lanes.** The rows for the faint lanes (rep3 lanes 2 and 4, rep2 lane 2) follow the
   strip tilt of the neighbouring lanes. Their measured peaks agree with that tilt (157 and 153
   in rep3; 74 in rep2). The centroid for rep2 lane 2 in the pS935 strip was unreliable (signal
   too weak), so I took that centre from the 24D8 strip and the lane pitch (661.5).
4. **Lane width** 22 px. The alternative was 23 px (`blotverse lanes`). The difference is
   negligible.
5. **Group label "T1343A/G2019S"**, quoted, kept exactly as in claim.yaml. The sample IDs use
   "_".

## Uncertainties

* **Replicate structure (main).** That the left and right sets of image A are two separate
   biological replicates is inferred from "(A–C) Biological replicates", from C being only the
   reference for B, and from n = 3. No source states explicitly "left = replicate 1,
   right = replicate 2". The replicate numbering (left = 1) is arbitrary, and it does not affect
   the mapping of lanes to groups.
* **The A-left pS935 smear** makes the band window (and so the target integral) depend on the
  half-height and background choice. See choice 1.
* **Replicate 3 uses a reference from a different membrane** (image C, a repeat blot of the
  same lysates). Lanes are matched by label only. Loading differences between the two gels
  cannot be checked from the images.
* **The left-edge marker lanes** (x ~30 / ~600 in A, ~50 in B, ~240 in C) are not labelled in
  the annotation figure, which crops them off. I identified them as ladders from the focus
  images (multi-band prestained pattern). This sets the reading direction: the labelled order
  starts at the first lane to the right of that marker and ends at the marker at lane 8, which is
  also labelled "Marker". Both markers agree on 7 sample lanes in between.
* The two unlabelled lanes right of "not transfected" in every image are excluded. Their
  content is unknown.
