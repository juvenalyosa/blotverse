status: annotated

# Annotator B log - eLife 10.7554/eLife.91083, Fig. 6B / 6D (LRRK2 pS935 / total LRRK2)

Output: annotator_b.yaml. 3 blots x 7 lanes (groups wt, R1441G, G2019S, I2020T, K1906M, T1343A, T1343A/G2019S; control wt). Bands are bright on dark (chemiluminescence), so no `invert`. Allowed sources only (claim.yaml, article/, images/); authors/ and any quantification not read.

## Images
- Rep 1: target = images/Figure6_source_data_1/A/2023-02-07_2x2-300s-07.tif, left set, top strip (label "pS935"). Reference = same file, left set, third strip (label "LRRK2 (24D8)"). Same-file reference uses `reference_image` = same path with its own `reference_annotation` (strips differ in row and slightly in x).
- Rep 2: same file A, right set, top strip ("LRRK2 pS935") and third strip ("LRRK2 (24D8)").
- Rep 3: target = B/2023-02-17_2x2-180s-05.tif, left membrane, top strip ("pS935"); reference = C/2023-02-20_24D8_2x2-180s-05.tif (single strip "LRRK2 (24D8)").
- Not used: B right membrane top strip (labelled 24D8, nearly blank), pRab10 / Rab10 strips (not this panel), focus.tif photos.
- Evidence for the replicate assignment: legend of Fig. 6-figure supplement 2 (quoted in claim.yaml): "(A-C) Biological replicates. Panel (C) shows the total-LRRK2 blot (24D8) for panel (B)"; panel A holds two complete labelled sets (left/right), n=3. Source: article/Figure6_source_data_1_Annotation.png, elife-91083-fig6-figsupp2-v1.jpg, claim.yaml.

## Lane mapping (identical for every strip)
Evidence: lane labels written above every membrane in article/Figure6_source_data_1_Annotation.png, left to right: wt, R1441G, G2019S, I2020T, K1906M, T1343A, T1343A/G2019S, Marker, PEI only, not transfected. The raw images show the 7 sample lanes at a regular ~30 px pitch, then a gap (marker/PEI lanes with no pS935/24D8 signal expected), then extra lanes 11-12 (left out). Lane i (1-7) = label i. Group mapping is therefore by position only, not by band intensity.

| lane | group | rep1 x (target / ref) | rep2 x (target / ref) | rep3 x (B / C) |
|---|---|---|---|---|
| 1 | wt | 63 / 62 | 637 / 635 | 80 / 270 |
| 2 | R1441G | 93 / 92 | 667 / 665 | 110 / 300 |
| 3 | G2019S | 123 / 123 | 697 / 694 | 139 / 329 |
| 4 | I2020T | 153 / 153 | 726 / 724 | 169 / 359 |
| 5 | K1906M | 183 / 183 | 756 / 753 | 199 / 389 |
| 6 | T1343A | 213 / 214 | 786 / 783 | 228 / 418 |
| 7 | T1343A/G2019S | 243 / 244 | 816 / 813 | 258 / 448 |

(x values rounded; exact numbers in the yaml. Lane width 22 = 0.75 x pitch.) Sample names are `<group>_rep<k>`.

## Geometry method
Approximate centres read by eye on rendered images; `refine_lanes.py` centroids were used as a cross-check, but for weak lanes (rep1/rep2 R1441G, I2020T; rep3 R1441G, I2020T) centroids are pulled by neighbours, so I fitted a regular grid (x0 + pitch*i) per strip to the strong lanes' centroids (pitch 29.6-30.3). Band row per lane = row maximum of the mean over x+-5 within +-22 rows; half-height from the FWHM of that profile (rounded up slightly). Checked by drawing the boxes on the images (rep1/rep2 overlay inspected; rep3 centroids agree with grid to <1 px). `blotverse inspect`: all three images 16-bit, raw=yes, 0 saturated pixels. `blotverse detect` accepted the file.
Half-heights: rep1 target 14, rep1 ref 10, rep2 target 6, rep2 ref 9, rep3 target 6, rep3 ref 6.

## Uncertainties
1. Rep1 pS935 (A left top) is a smeared doublet/broad band, slanting down to the left (row 97 at wt to 83 at lane 7). I put the box on the lower, denser band of the doublet per lane; the upper smear is partly outside the box. R1441G and I2020T bands there are narrow (FWHM ~9 vs ~19), so the boxes are wider than their bands. Total-LRRK2 strip in rep1 is also a doublet with a faint upper smear.
2. Very faint pS935 bands in rep3 (B left) lanes 2 and 4, and rep2 lane 2: the band row for these was set by the row maximum of a near-background profile and by neighbour trend; the row could be off by a few pixels.
3. Rep3 target and reference are different images/membranes (B vs C, repeat blot); lane matching relies on the labels being the same on every membrane (stated in claim.yaml and visible in the annotation figure). C lanes occupy x 270-448 while B lanes are x 80-258; no image registration between B and C is assumed or needed.
4. The "Marker" lane position in the raw images is visually empty on the pS935/24D8 strips; I could not verify from the raw images alone that lane 8 is the marker, only the label order. Irrelevant to the seven annotated lanes.
5. Rep1/rep2 reference strip row drifts (rep2: 402-407) across lanes (membrane tilt); per-lane rows were used.

## Choices between alternatives
- Same-file reference (A) handled via `reference_image` = target path plus `reference_annotation` instead of `total_protein`: the two strips are different membranes at different rows, not a total-protein image of the same lanes.
- Rep3 reference = image C (per legend), not B right membrane.
- Grid fit instead of per-lane centroids (see above).
- Lower band of the smeared doublet in rep1 target (alternative: full smear; would need half-height ~29).
- Single band_half_height per strip (format allows one value).
