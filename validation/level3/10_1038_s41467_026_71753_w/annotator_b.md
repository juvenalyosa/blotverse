status: mapped (one residual uncertainty about which streak is the gammaH2AX band, see below)

# Annotator B log: Volk et al. 2026, Fig. 1e gammaH2AX (14+0d), first of four blots

## Images
- Target: `images/0000817_02_800.TIF`, bottom of three strips (800 nm channel; claim.yaml: Blot 66 STAT1/PCNA/pH2AX, bottom strip ~15 kDa).
- Ladder/orientation: `images/0000817_02_700.TIF` (same scan, 700 nm; ladders visible in all three strips).
- Loading control: `images/0000807_01_700.TIF` (total protein stain, 700 nm, claim.yaml: Blot 66 TPS).
- One blot, one replicate set: the n = 6 mice per group are the six lanes of each block (legend: n = 6 mice/group, 3 males, 3 females). Blot id B66.
- `blotverse inspect`: all three files are 16-bit, raw, flagged as floating-point rescaled (saturation cannot be checked from pixel values), no exposure metadata.
- Bands are bright on dark background: no `invert`.

## Lane order evidence
Source: `article/SupplementaryFig11_overview.png`, panel b (header bars Juvenile/Adult, then NT/NDMA, then M1 M2 M3 F1 F2 F3 per block, ladders between blocks) and claim.yaml `lane_layout_source`. Supplementary Fig. 11 legend: panel b = 14+0d, Fig. 1e.
Raw layout (both the 700 nm strip image and the TPS image): ladder | extra lane + ladder at far left | 6 lanes | ladder | 6 | ladder | 6 | ladder | 6 | ladder. So the four blocks of six between ladders, left to right, are Juvenile NT, Juvenile NDMA, Adult NT, Adult NDMA; within each block M1 M2 M3 F1 F2 F3 left to right. Each block is the 6 slots between two ladders (pitch ~27 px, 7 slots per ladder-to-ladder interval).

## Lane table (target x / TPS x, in pixels of each image)
| lane | group | sample | target x | TPS x | evidence |
|---|---|---|---|---|---|
| 1 | Juvenile NT | JNT-M1 | 131 | 170.4 | Supp Fig 11b header (Juvenile NT, M1); position 1 of block 1 between ladders |
| 2 | Juvenile NT | JNT-M2 | 159 | 197.7 | Supp Fig 11b header (Juvenile NT, M2); position 2 of block 1 between ladders |
| 3 | Juvenile NT | JNT-M3 | 187 | 229.9 | Supp Fig 11b header (Juvenile NT, M3); position 3 of block 1 between ladders |
| 4 | Juvenile NT | JNT-F1 | 215 | 256.8 | Supp Fig 11b header (Juvenile NT, F1); position 4 of block 1 between ladders |
| 5 | Juvenile NT | JNT-F2 | 244 | 285.1 | Supp Fig 11b header (Juvenile NT, F2); position 5 of block 1 between ladders |
| 6 | Juvenile NT | JNT-F3 | 272 | 310.9 | Supp Fig 11b header (Juvenile NT, F3); position 6 of block 1 between ladders |
| 7 | Juvenile NDMA | JNDMA-M1 | 327 | 363.7 | Supp Fig 11b header (Juvenile NDMA, M1); position 1 of block 2 between ladders |
| 8 | Juvenile NDMA | JNDMA-M2 | 353 | 389.3 | Supp Fig 11b header (Juvenile NDMA, M2); position 2 of block 2 between ladders |
| 9 | Juvenile NDMA | JNDMA-M3 | 380 | 415.8 | Supp Fig 11b header (Juvenile NDMA, M3); position 3 of block 2 between ladders |
| 10 | Juvenile NDMA | JNDMA-F1 | 407 | 441.9 | Supp Fig 11b header (Juvenile NDMA, F1); position 4 of block 2 between ladders |
| 11 | Juvenile NDMA | JNDMA-F2 | 433 | 468.9 | Supp Fig 11b header (Juvenile NDMA, F2); position 5 of block 2 between ladders |
| 12 | Juvenile NDMA | JNDMA-F3 | 460 | 495.8 | Supp Fig 11b header (Juvenile NDMA, F3); position 6 of block 2 between ladders |
| 13 | Adult NT | ANT-M1 | 514 | 544.0 | Supp Fig 11b header (Adult NT, M1); position 1 of block 3 between ladders |
| 14 | Adult NT | ANT-M2 | 540 | 570.1 | Supp Fig 11b header (Adult NT, M2); position 2 of block 3 between ladders |
| 15 | Adult NT | ANT-M3 | 567 | 596.1 | Supp Fig 11b header (Adult NT, M3); position 3 of block 3 between ladders |
| 16 | Adult NT | ANT-F1 | 594 | 623.2 | Supp Fig 11b header (Adult NT, F1); position 4 of block 3 between ladders |
| 17 | Adult NT | ANT-F2 | 620 | 649.3 | Supp Fig 11b header (Adult NT, F2); position 5 of block 3 between ladders |
| 18 | Adult NT | ANT-F3 | 647 | 676.3 | Supp Fig 11b header (Adult NT, F3); position 6 of block 3 between ladders |
| 19 | Adult NDMA | ANDMA-M1 | 703 | 724.4 | Supp Fig 11b header (Adult NDMA, M1); position 1 of block 4 between ladders |
| 20 | Adult NDMA | ANDMA-M2 | 733 | 751.5 | Supp Fig 11b header (Adult NDMA, M2); position 2 of block 4 between ladders |
| 21 | Adult NDMA | ANDMA-M3 | 763 | 778.5 | Supp Fig 11b header (Adult NDMA, M3); position 3 of block 4 between ladders |
| 22 | Adult NDMA | ANDMA-F1 | 793 | 804.7 | Supp Fig 11b header (Adult NDMA, F1); position 4 of block 4 between ladders |
| 23 | Adult NDMA | ANDMA-F2 | 822 | 832.9 | Supp Fig 11b header (Adult NDMA, F2); position 5 of block 4 between ladders |
| 24 | Adult NDMA | ANDMA-F3 | 852 | 860.8 | Supp Fig 11b header (Adult NDMA, F3); position 6 of block 4 between ladders |

Target lane centres come from the ladder positions in `0000817_02_700.TIF` bottom strip (ladders at x ~ 47, 103, 300, 487, 673, 882, measured at y ~ 415), dividing each ladder-to-ladder interval into 7 equal slots and taking slots 1-6. TPS lane centres come from `validation/level3/refine_lanes.py` run on approximate positions read off the image (band row 290, search +-8 px), checked on an overlay against the lane valleys. Lane width 22 px (pitch ~27) in both.

## Band geometry
- Target: a single diffuse, horizontally tilted streak at y ~ 393 (range 390-398 across the strip); band_half_height 10. Rows used per block: 396, 394, 393, 391 (left to right). Only where the streak is visible (second and fourth blocks) was it measured directly (row maxima 393-394 in block 2, ~390 in block 4); rows for the other blocks are interpolated along the streak line, because no band is resolvable there. This is a geometric choice, not a group assignment.
- TPS: whole-lane signal, reference_y 290, band_half_height 140 (rows 150-430, the stained lane body above the dye-front smile). `blotverse detect` accepted the file.
- Not run: `blotverse run`; no group comparison computed.

## Uncertainties
1. The gammaH2AX strip is a very high background, smeared image with no individually resolved lane bands. Lane x positions are therefore from the ladders (regular grid), not from band centroids. Automatic centroiding on the target gave unstable x (e.g. 800 and 817 in block 4), so it was not used. Lane x could be off by a few px (up to ~5 px at block edges; the last ladder at 882 is broad and the last block's grid pitch, ~30 px, is larger than in the TPS, ~27 px).
2. Two parallel streaks are present (y ~ 372 and y ~ 393). I chose the lower, stronger one at y ~ 393. The choice rests on position, not on matching the authors' result: the figure crop (Supp Fig 11b) shows one band near the top of the crop above the 15 kDa marker, but the vertical scale of the crop could not be calibrated. If the upper streak is the real gammaH2AX band, target_y should be ~372. The marker ladder rows at 700 nm are y ~ 350, 415, 477 (assumed 25, 20, 15 kDa; not verified) so both streaks lie above the lowest marker.
3. Block 1 and 3 target rows are interpolated (no visible band), see above.
4. Total protein lanes differ in width of the stained pattern and have a dye-front smile at the bottom and an over-bright top edge; rows 150-430 chosen to avoid both.
5. TPS image and target strip were not co-registered; the TPS lane x positions are independent of the target x positions (different membrane crops, 1016 vs 1010 px wide). Same-membrane claim relies on claim.yaml (Blot 66).
6. An additional sample lane sits between the first two ladders at the far left of both images (not in Supp Fig 11b); it is excluded (not one of the four groups).
7. Supp Fig 11b shows the same layout; panel d (WT mice) and panel c (7 d) are different experiments and were not used.

## Choices between alternatives
- Ladder-grid lane centres vs. band centroids for the target: grid chosen (see 1).
- Lower vs upper streak: lower chosen (see 2).
- Single-row (flat) vs tilt-following target_y: per-block rows with 1-5 px tilt.
- TPS measured over a large vertical window (whole lane) instead of a single band.
- No `invert` (fluorescence, bright bands).
