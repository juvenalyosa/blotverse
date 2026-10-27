# Annotator B log: Tsuchiya 2017, Fig. S7D (Cdc48 / Pgk1, WT vs cdc48-3)

status: mapped (not unidentifiable; two residual assumptions listed under Uncertainties)

## Images
- Target: `images/FigS7D_IB-Cdc48.tif` (918x1024, 16-bit, dark bands on light background; `blotverse inspect` also advises invert). Marker strip at left with handwritten "aCDC".
- Loading control: `images/FigS7D_IB-Pgk1.tif` (1080x574, separate image, different size; handwritten "aPGK"). Used as `reference_image` with its own `reference_annotation`; not `total_protein`.
- Both are one membrane cut into strips (legend: Cdc48 and Pgk1 immunoblots of the same total lysate); each has six sample bands in three pairs separated by an empty lane, marker at left.

## Lane mapping
Evidence for all lanes: claim.yaml `lane_layout_source` and the Fig. S7D overview (`article/FigS7D_overview.png`), which shows two adjacent lanes labelled "WT" (left) and "cdc48-3" (right). Legend: "means ± SEM of three biologically independent experiments". The raw images show three pairs of two bands. Replicate numbering is by position left to right (arbitrary).

| Lane | x Cdc48 | x Pgk1 | Group | Sample | Evidence |
|---|---|---|---|---|---|
| 1 | 275.0 | 257.5 | WT | WT-1 | left of pair 1; overview order WT, cdc48-3 |
| 2 | 323.5 | 306.8 | cdc48-3 | cdc48-3-1 | right of pair 1 |
| 3 | 419.2 | 403.5 | WT | WT-2 | left of pair 2 |
| 4 | 470.9 | 455.7 | cdc48-3 | cdc48-3-2 | right of pair 2 |
| 5 | 568.2 | 553.7 | WT | WT-3 | left of pair 3 |
| 6 | 621.4 | 605.3 | cdc48-3 | cdc48-3-3 | right of pair 3 |

Lane i of the Cdc48 strip is paired with lane i of the Pgk1 strip by position (same pair layout, 6 bands each, same left-to-right order). Marker lane excluded. The empty lanes between pairs and the empty area right of lane 6 are excluded. Group assignment was never based on band intensities.

## Geometry
- Tools: `blotverse inspect`; `validation/level3/refine_lanes.py` (run once per image); then a short numpy script that repeats the refine_lanes.py procedure on the inverted image (65535 - pixel), because refine_lanes.py does not support dark-on-light images (run as-is it clipped the signal at 0 and gave a wrong Pgk1 band row, 135 vs the real ~150, and shifted Cdc48 centres by up to 21 px). I did not run `blotverse lanes` or `detect`, which need a CLI-style invert-aware setup; values were checked by eye against the images.
- Cdc48: lane pitch ~48-52 px within pairs; lane half-max width ~34-35, set width 34. Band rows 490-494 (slight upward slope to the right); FWHM ~10 rows, half-height 10 (approximately 1x FWHM, covers the thin band; neighbouring faint bands lie ~50 px below and the marker ladder is outside the lanes).
- Pgk1: band half-max width ~49-54 (bands almost touch within a pair, pitch 49); width 44 chosen to avoid overlap. Rows 148-152; FWHM 13 rows, half-height 14.

## Uncertainties
1. Which lane of each pair is WT: taken as left, per the published crop and claim.yaml; the raw deposit has no lane labels. A swap would change the sign of the result.
2. Replicate order (1-3) is arbitrary, and it is not known whether the three pairs are the experiments in the same order on both strips (assumed by position; irrelevant to group means, only matters for pairing, and here both strips are the same membrane).
3. Cdc48 strip: a fainter band below (~y 550) and smear above the main band; I took only the ~100 kDa main band (y ~491). Pgk1 bands may be near saturation (strongly dark, 49 saturated pixels in the file).
4. The image has a clipped white/black region at the left (marker strip scanned with different contrast); lanes lie in the light region so no effect expected.

## Choices
- Pgk1 as separate reference image (different dimensions) rather than total_protein.
- invert: true for both images (set on the blot; applies to the reference as well per BlotVerse convention; check with the pipeline owner if it does not).
- Lane widths narrower than the pitch (34 and 44) to avoid neighbours.
- Own inverted refinement rather than refine_lanes.py output (see Geometry).
