# Annotator B log: Fig. 5E (Cell Rep 2024, doi 10.1016/j.celrep.2024.114831)

status: identified (mapping unambiguous); STRUCTURAL PROBLEM for the loader, see "Tool limitation" below.

## Images
- Target (Hpu98 chemiluminescence), one 2750x2200 16-bit TIFF per replicate, holding three cut strips:
  `I_/II_/III_Chemiluminescence_from the top control ERK2 BSA.raw16.tif`. File name: "from the top control ERK2 BSA";
  claim.yaml: "three cut strips, from the top control ERK2 BSA". Top strip = Control, middle = ERK2, bottom = BSA.
- Loading control: one Ponceau S image per replicate and condition (`<rep>_<control|ERK2|BSA>_Ponceau S.raw16.tif`), 9 images,
  taken of the membrane before cutting. Different lane x and band rows from the strips, so `total_protein` is not valid;
  `reference_image` + `reference_annotation` used, one per blot.
- Not used: the 9 "Colorimetric membrane cutting reference" images (no quantifiable band; used only to understand the cut).
- `blotverse inspect`: all files 16-bit, raw=yes, 0 saturated px. Chemiluminescence: bright bands on dark, no invert. Ponceau
  images: the inspect warning ("bright background") is about the white paper around the membrane; bands are brighter than the
  dark membrane, so no `invert`.

## Lane mapping (all 9 strips alike)
Evidence: article/Fig5E_overview.png (lane labels 0, 1:15, 2:30, 3:45, 5, 6:15, 7:30, 8:45, 10, 15, 30, 60 min, left to right; rows
Control, 100 uM ERK2, 100 uM BSA) and claim.yaml lane_layout_source (12 lanes per strip, time order left to right; one folder per replicate:
Replicate I, II, III = file prefix I_, II_, III_). The deposit's file prefixes give the replicate; the strip order within the chemi
image gives the condition. Lane k (1..12, left to right) = time point k. Ponceau images: 12 sample lanes between a ladder on each side;
left to right = 0 ... 60 min (same layout stated in claim.yaml). Ladders (outer lanes of Ponceau) are excluded.
Blot ids: I_control, I_ERK2, I_BSA, II_*, III_*; sample = `<rep>-<cond>-<time>` (e.g. I-ERK2-6m15). Groups: Control, ERK2_100uM, BSA_100uM.
Every lane of the 36 per replicate is listed (12 time points x 3 conditions x 3 replicates = 108 target lanes, 108 Ponceau lanes).

## Geometry (all measured, then checked on overlays)
- Lane centres: weighted column centroid in the band rows, linear fit over lanes (x = x0 + pitch*i), residuals <= ~5 px for lanes with signal.
  Pitch differs per strip (77 to 88 px in the chemi strips) because strips were cut/placed separately; x0 about 920-970.
  The 0-min lane is often invisible/very faint: its position comes from the linear fit (extrapolated), not from signal.
- Band rows per lane: row maximum of the lane profile; for lanes with signal below 2% of the strip maximum, a quadratic fit over the strip
  (strips are tilted) replaced the maximum. Half-height: chemi 16 (14 for ERK2 strips, FWHM half-widths 6-17 px); Ponceau 20 (FWHM half 13-20).
- Lane width = round(0.75 * pitch).
- `refine_lanes.py` was run first (I strips); its centroids agreed with the fit within a few px, but it prints one band row per strip and
  a single band_half_height from the brightest lane, so I used my own per-lane fit (same method) for tilted strips.
- reference_y = target_y in each annotation (both rows are the band row of that image).

## Choices between alternatives
1. Ponceau signal: only one sample band is visible per lane (just above the membrane's lower edge); used it as the total-protein signal
   of the lane. Whole-lane Ponceau would need other rows; none carry lane signal. Band row differs per image (1380-1600).
2. `reference_image` per blot instead of `total_protein`: positions and rows differ from the strip image.
3. One blot per replicate x condition (9 blots), because each condition has its own Ponceau image and the loader allows one reference image per blot.
4. ERK2 strips show lower sub-bands (~40 px below the main band); the annotation takes the main (upper) band at the position of the
   other strips. Which band is Dhp-eIF5A: the one aligned with the Control and BSA strip bands and with the Fig5E overview (single band at -17 kDa).
5. Strip order: by file name and claim.yaml. Not checked against intensities (forbidden).
6. Ponceau ladders at both edges were excluded; I assumed the 12 lanes between them are the 12 time points.

## Uncertainties
- 0 min lanes (and 1:15 in ERK2 strips) have no visible band; positions extrapolated. Lane x of the strip's first lane could be off by a few px.
- Lane indexing relies on counting 12 slots leftwards from the brightest (60 min) lane; strips II-BSA and III-BSA have a smeared 30/60 min
  pair (lane 12 has a blob/wing to the right), so the lane 12 centre is less certain there (~5-10 px).
- Whether the Ponceau lane n sits at the same physical lane as strip lane n relies on the claim.yaml lane statement; both have 12 lanes.
- Ponceau bands in III_ERK2 and I_ERK2 are tilted/curved; rows follow a quadratic fit, accurate to a few px.
- Ponceau band for III_BSA lane 12 is faint; box placed by fit.

## Tool limitation (important)
`blotverse` (src/experiment.cpp, parse_experiment) requires every blot to contain a lane of the control group. Here Control, ERK2 and BSA
strips each need a different Ponceau image, so a blot with a single reference_image can hold one condition only. `annotator_b.yaml` is therefore
the faithful annotation (9 blots) but the loader rejects it with "blot 'I_ERK2' has no 'Control' lane". A scratch copy in which the first lane of
each blot was relabelled Control parsed and passed `blotverse detect` (lane counts and bounds are valid), so geometry is loadable. The caller
must decide how to encode this (e.g. relax the per-blot control check, or per-replicate normalisation across blots). I did not alter lane groups.
`blotverse run` was not executed and no comparison computed.
