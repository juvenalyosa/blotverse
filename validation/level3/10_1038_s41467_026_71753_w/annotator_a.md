status: annotated

# Annotator A log: Volk et al. 2026, Fig. 1e, gammaH2AX (14+0d)

Sources used: `claim.yaml`, `article/fulltext.xml` (legends, Methods),
`article/SupplementaryFig11_overview.png` (Supplementary Fig. 11b) and the three raw images in
`images/`. I did not open `authors/`, `screening_notes.md` or `fetch.sh`. I did not use any
author numbers.

## 1. Images

| Role | File | Evidence |
|---|---|---|
| Target (gammaH2AX) | `images/0000817_02_800.TIF`, bottom strip | claim.yaml deposit metadata: "Blot 66 - STAT1, PCNA, pH2AX (Odyssey CLx, 800 nm channel)", "bottom strip (~15 kDa)". In the paired 700 nm image (`0000817_02_700.TIF`, same size and same frame) the bottom strip carries the ladder markers that sit just above and below the band. Supp. Fig. 11b labels the gammaH2AX crop "15kDa", with the 15 kDa ladder blob at the bottom edge of the crop. |
| Loading control (Total Protein Stain) | `images/0000807_01_700.TIF` | claim.yaml: "Blot 66 - TPS (Odyssey CLx, 700 nm channel)". Legend: "Band intensities normalized to Total Protein Stain". Methods: "Membranes were stained with LI-COR Revert 700 Total Protein Stain Kit". The image is an uncut membrane with the same lane plan as the target image (see below). |
| Orientation aid only | `images/0000817_02_700.TIF` | 700 nm channel of the target acquisition: ladders and PCNA. |

All replicates sit on one membrane. Methods: "All biological replicates and group
comparisons were performed on the same blot." One blot covers all 4 groups x 6 mice.

`blotverse inspect`: all three are floating-point TIFFs rescaled to 16 bit. The 800 image
was scaled x67.0, the target-700 x238.3 and the TPS x50.2. Detector saturation cannot be
checked from pixel values. The TPS image is 1016x650 and the target 1010x585, so they are
different acquisitions. I therefore used `reference_image` + `reference_annotation`, not
`total_protein`, which needs the same geometry as the target.

`invert: false`. These are fluorescence (Odyssey) images: bands are bright on a dark
background. The raw pixel values are higher in the bands (e.g. JNDMA band about 3.5-4.3
against a background of about 2.8 in the 800 image).

## 2. Lane plan (membrane level)

Supp. Fig. 11b, gammaH2AX row, header from left to right:
"Juvenile [NT: M1 M2 M3 F1 F2 F3] [NDMA: M1 M2 M3 F1 F2 F3] | Adult [NT: M1 ... F3]
[NDMA: M1 ... F3]". The crop shows a ladder at the left edge, a ladder between each block
of six and a ladder at the right edge: 5 ladders and 24 sample lanes. The legend
(Supp. Fig. 11): "(b) 14+0d for γH2AX, p-p53, ... (Fig. 1e,g, ...)" and "All Western blots
used for densitometry in this study are shown here as cropped blots, including all
biological replicates".

The raw images give the same plan, plus one extra lane at the far left.

* Target bottom strip (ladder centroids from the 700 nm channel, 20 kDa / 15 kDa rows):
  ladders at x ≈ 46, 103-105, 300, 486-487, 672, 877-882. There is an unlabelled sample
  lane at x ≈ 75 between the first two ladders.
* TPS: ladders at x ≈ 80, 142, 332, 515, 697, 895. There is an unlabelled sample lane at
  x ≈ 110.

The figure crop starts at a ladder and shows only one ladder before Juvenile NT M1. So the
crop starts at the second ladder (x ≈ 103 target / 142 TPS), and the leftmost
ladder+lane pair is cropped out. **That extra lane is not in any of the four groups and is
excluded.**

Orientation (not mirrored). Evidence:

1. The extra lane is at the left in both raw images, and the figure crop has its edge
   ladder at the left.
2. The ladder spacing pattern matches. Figure gaps are 325/305/305/345 px. Raw target gaps
   are 197/187/186/206-210 px, so both scale by about 1.64 and both have the last
   (Adult NDMA) block widest. TPS gaps are 190/183/182/198 px, the same pattern.
3. The rightmost 15 kDa ladder blob is diffuse in both the figure and the raw image.
4. Non-band artefacts match. The figure shows a black dot just right of the third ladder
   (Adult NT M1 position); the raw 800 image has a dot at x ≈ 522, y ≈ 412. A vertical
   smudge at Adult NT M2 in the figure matches a raw spot at x ≈ 557, y ≈ 375.

I used these only to register the crop onto the raw image. Group identity comes from the
figure labels, not from band intensities.

## 3. Lane-by-lane evidence

Source for every lane: Supp. Fig. 11b gammaH2AX row. Group labels come from the header bars
"Juvenile"/"Adult" over "NT"/"NDMA". Replicate labels come from the per-lane labels
"M1 M2 M3 F1 F2 F3" (males 1-3, females 1-3; legend "n = 6 mice/group; 3 males, 3 females").
Positions in the raw images come from section 2: block k lies between ladders k and k+1,
counting from the second raw ladder. Sample names in the YAML are `<group abbrev>_<label>`.

| # | Group | Sample | Target x (800) | Target y | TPS x | Evidence |
|---|---|---|---|---|---|---|
| 1 | Juvenile NT | JNT_M1 | 131 | 392 | 168.3 | 1st lane after ladder 2 (raw); figure "Juvenile / NT / M1" |
| 2 | Juvenile NT | JNT_M2 | 159 | 392 | 195.7 | figure "NT M2" |
| 3 | Juvenile NT | JNT_M3 | 187 | 392 | 222.0 | figure "NT M3" |
| 4 | Juvenile NT | JNT_F1 | 215 | 392 | 249.6 | figure "NT F1" |
| 5 | Juvenile NT | JNT_F2 | 243 | 392 | 277.4 | figure "NT F2" |
| 6 | Juvenile NT | JNT_F3 | 271 | 392 | 304.3 | figure "NT F3", last lane before ladder 3 |
| 7 | Juvenile NDMA | JNDMA_M1 | 326.6 | 394 | 356.0 | 1st lane after ladder 3; figure "Juvenile / NDMA / M1" |
| 8 | Juvenile NDMA | JNDMA_M2 | 352.4 | 394 | 383.2 | figure "NDMA M2" |
| 9 | Juvenile NDMA | JNDMA_M3 | 379.8 | 394 | 409.3 | figure "NDMA M3" |
| 10 | Juvenile NDMA | JNDMA_F1 | 405.8 | 394 | 435.4 | figure "NDMA F1" |
| 11 | Juvenile NDMA | JNDMA_F2 | 432.3 | 394 | 462.4 | figure "NDMA F2" |
| 12 | Juvenile NDMA | JNDMA_F3 | 459.4 | 394 | 488.6 | figure "NDMA F3", before ladder 4 |
| 13 | Adult NT | ANT_M1 | 513 | 394 | 540.2 | 1st lane after ladder 4; figure "Adult / NT / M1" |
| 14 | Adult NT | ANT_M2 | 540 | 394 | 566.2 | figure "NT M2" |
| 15 | Adult NT | ANT_M3 | 566 | 394 | 592.0 | figure "NT M3" |
| 16 | Adult NT | ANT_F1 | 592 | 394 | 618.4 | figure "NT F1" |
| 17 | Adult NT | ANT_F2 | 619 | 394 | 645.7 | figure "NT F2" |
| 18 | Adult NT | ANT_F3 | 645 | 394 | 671.9 | figure "NT F3", before ladder 5 |
| 19 | Adult NDMA | ANDMA_M1 | 701 | 390 | 724.5 | 1st lane after ladder 5; figure "Adult / NDMA / M1" |
| 20 | Adult NDMA | ANDMA_M2 | 728 | 390 | 752.7 | figure "NDMA M2" |
| 21 | Adult NDMA | ANDMA_M3 | 756 | 390 | 780.6 | figure "NDMA M3" |
| 22 | Adult NDMA | ANDMA_F1 | 784 | 390 | 808.9 | figure "NDMA F1" |
| 23 | Adult NDMA | ANDMA_F2 | 813 | 390 | 836.7 | figure "NDMA F2" |
| 24 | Adult NDMA | ANDMA_F3 | 842 | 390 | 865.3 | figure "NDMA F3", before the last ladder |

Excluded lanes: all ladders (5 in the figure, plus the cropped one at the far left), and the
unlabelled far-left sample lane (target x ≈ 75, TPS x ≈ 110), which is outside the figure.

## 4. Geometry

### Target (800 nm, bottom strip)

* Lane width 20 px. That is about 0.75 x the lane pitch of 26-28 px; `refine_lanes.py` also
  gives 20-21.
* Juvenile NDMA lane centres: `refine_lanes.py` on the 800 image (y=394, search 6-8). The
  results (326.6, 352.4, 379.8, 405.8, 432.3, 459.4) equal the ladder-to-ladder even
  spacing (300 -> 487, pitch 26.7) to within 1 px.
* Juvenile NT and Adult NT have no visible band, so the 800 centroid is noise there
  (`refine_lanes.py` gave 128/160/181/221/243/269 for JNT). I used even spacing between the
  flanking ladders instead:
  * JNT: 102.8 -> 299.9, pitch 28.2.
  * ANT: 486.8 -> 672.6, pitch 26.5.
  * Cross-check: the lane-front "teeth" in the 700 nm bottom strip (rows 445-465) give
    JNT 133/158/186/214/–/271 and ANT 514/540/565/594/623/655. The first five ANT lanes
    and all JNT lanes agree to within about 3 px.
* Adult NDMA: lanes are **not** evenly spaced between ladders 672 and 877; the right ladder
  sits further out. I took the centres from two features that agree:
  * band maxima in the 800 band profile at 730/757/786/812;
  * lane-front teeth at 699/728/753/782/817/844.

  The result is 701, 728, 756, 784, 813, 842; `refine_lanes.py` gave
  701/726/754/784/811/842.
* Band rows:
  * JNDMA: measured per lane at 393-396; 394 used for the block.
  * ANDMA: measured at 389-391; 390 used.
  * JNT and ANT have no band. I used the strip curvature from the 20 kDa ladder row
    (348 @ x105, 353 @ x300, 353 @ x486, 352 @ x672) and the faint row-profile shoulders:
    JNT 389-394, ANT bump at 395. That gives JNT 392 and ANT 394.
* band_half_height 8. `refine_lanes.py` on JNDMA gave 8 (1.1 x FWHM; FWHM ≈ 7 rows);
  on ANDMA it gave 11 (weaker, noisier band). With ±8, every window covers the band core
  and stays clear of the weaker upper band at about 372.
* `blotverse lanes` per block (with `--lanes-x` guesses) placed JNDMA at
  330.6-446.4, consistent with the above. `blotverse detect` with no annotation fitted 24
  evenly spaced lanes across the ladders: lane 7 at x=285, which is in the Juvenile NT
  block / ladder gap. That fit is wrong because the comb ignores the interleaved ladders,
  so I used the manual annotation instead.
* `blotverse detect annotator_a.yaml` parses and validates the file, and an overlay check
  showed every target box on the band row and inside its lane.

### Loading control (TPS)

* Lane centres: `refine_lanes.py` at y=300 (search 10, band-search 40). Results at y=220
  and y=370 agree to within about 2 px, except JNT F2 at y=370, which is noise. The
  inter-lane gap minima (rows 260-350) give the same centres to within about 2 px. Width
  is 20.
* Window: a whole-lane total-protein window, rows 173-397 (target_y = reference_y = 285,
  band_half_height 112). This starts below the wells / top edge, which runs from about
  y 135 at the left to about y 165 at the right. It ends above the dense low-MW dye-front
  smear with lane-front artefacts (y ≳ 400). The overlay confirmed the window is inside
  every lane.

## 5. Uncertainties

1. **Which gammaH2AX band.** In Juvenile NDMA the 800 strip has a strong band at y ≈ 394
   and a weaker diffuse band at y ≈ 372. I annotated 394. It is the dominant band. Its
   position relative to the 15 kDa marker (about 412-416) matches the figure crop, where
   the band sits about 20 px above the 15 kDa ladder blob. Whether the authors' box also
   took in the upper band is not stated.
2. **Band row in the NT blocks** is interpolated (no visible band), with an uncertainty of
   about ±3 rows. The ±8 window absorbs this.
3. **Adult NDMA lane centres** come from band and lane-front features, not ladder spacing.
   If the even ladder spacing were right instead, F1-F3 would move by +6 px
   (790/819/848), about a third of a lane width.
4. **TPS window**: the authors' TPS region (whole lane, or a band range) is not given. I
   chose rows 173-397 and excluded the wells and the dye front.
5. **TPS image = Blot 66.** This rests on the deposit metadata in claim.yaml and on the
   identical lane architecture: an extra far-left lane, 5 ladders, and the widest last
   block. The image numbering (0000807 vs 0000817) differs, as expected for a TPS
   acquired before immunodetection.
6. **Saturation** cannot be checked: the images are floating-point exports rescaled by
   `inspect`.
7. **Replicate order** assumes the figure is an uncropped-order crop of this membrane. The
   legend says these are "cropped blots, including all biological replicates", and the
   artefacts register the crop onto the raw image.

## 6. Choices between alternatives

* `reference_image`/`reference_annotation` (separate TPS image) over `total_protein`
  (needs identical geometry).
* `invert: false` (fluorescence) over `invert: true`.
* Lower gammaH2AX band (y ≈ 394) over the upper band (y ≈ 372), or over a window covering
  both.
* Ladder-even lane spacing for JNT/ANT over the noisy 800 centroids. Feature-based spacing
  for ANDMA over ladder-even spacing.
* One band row per block over per-lane argmax rows: argmax is noise in blocks without a
  band.
* Whole-lane TPS window over a single TPS band.
* Lane width 20 over 18-21. Band half-height 8 over 11 (the ANDMA estimate) or 16
  (`refine_lanes.py` over all 24 lanes, inflated by the empty NT lanes).
* The far-left unlabelled lane is excluded, since it is not in the figure.
