status: annotated, but the experiment file does not load in BlotVerse as is (schema limitation, see "Blocking issue")

# Annotator A log: 10.1016/j.celrep.2024.114831, Fig. 5E

Sources used: `claim.yaml`, `article/PMC11544350.xml` (legend, Results, STAR Methods),
`article/Fig5E_overview.png` (lane labels only), file names in `images/`, and the raw images.
I did not open `authors/` or `screening_notes.md`, and I did not use any author numbers. No
lane was assigned from band intensity.

## Blocking issue: the design does not fit the BlotVerse schema

* Each condition (Control, 100 uM ERK2, 100 uM BSA) was run on **its own gel and membrane**.
  Every condition has its own Ponceau S image, with its own two ladders and 12 sample lanes, and its
  own "membrane cutting reference" image. After cutting, the three 17 kDa strips were exposed together
  in one chemiluminescence image ("from the top control ERK2 BSA").
* The faithful encoding is one blot per replicate x condition (9 blots). Each blot has:
  * the chemiluminescence image, with the 12 lanes of that condition's strip;
  * `reference_image` = that condition's Ponceau S image, with its own 12-lane `reference_annotation`.
  `annotator_a.yaml` is written this way.
* BlotVerse requires a Control lane in every blot, so `blotverse detect` stops with:
  `blot 'repI_ERK2' has no 'Control' lane`. Before that check, all 9 blots parsed and their
  annotation and reference annotation geometry passed validation.
* Why not one blot per replicate, with all 36 lanes in the chemiluminescence image? A blot takes only
  one `reference_image`, but the three conditions are normalised to three different Ponceau images.
  Using one condition's Ponceau for all 36 lanes would be wrong.
* `total_protein` does not apply. The Ponceau images are the same pixel size (2750 x 2200) as the
  target image, but their lane positions are not the same.
* So the Control vs ERK2 comparison is between membranes by design. Each membrane is normalised to its
  own Ponceau. The reconciler or auditor must decide whether BlotVerse can support blots without a
  Control lane, or whether the experiment is excluded. I did not change any code.

## Images (task step 1)

| Replicate | Target (chemiluminescence, 3 strips) | Loading control (Ponceau S, one per condition) |
|---|---|---|
| I | `I_Chemiluminescence_from the top control ERK2 BSA.raw16.tif` | `I_control_Ponceau S`, `I_ERK2_Ponceau S`, `I_BSA_Ponceau S` (.raw16.tif) |
| II | `II_Chemiluminescence_from the top control ERK2 BSA.raw16.tif` | `II_control_…`, `II_ERK2_…`, `II_BSA_…` |
| III | `III_Chemiluminescence_from the top control ERK2 BSA.raw16.tif` | `III_control_…`, `III_ERK2_…`, `III_BSA_…` |

* `blotverse inspect` on all files: 2750 x 2200, 16-bit, raw = yes, 0 saturated pixels.
* Inspect warns "bright background, set invert" for the Ponceau images. This comes from the white
  paper/plate around the membrane. On the membrane itself, the Ponceau bands are **brighter** than the
  dark membrane. I checked crops and raw values: membrane about 39,000; eIF5A band peak about
  46,000–47,000.
* The chemiluminescence bands are also bright on a dark background. So `invert` is not set. (It would
  apply to both images of a blot.)
* The colorimetric "membrane cutting reference" images were used only to confirm that each membrane
  was cut into strips. They are not in the experiment file.

## Lane mapping (task step 2): evidence

**Replicate**

* The replicate is the Roman-numeral prefix of every file name (I, II, III). In `claim.yaml`, the
  deposit has one folder per replicate (Replicate I, II, III).
* n = 3 matches the legend: "triplicates (A and D–F)". Methods: "All experiments were done in
  triplicates."

**Strip to group (chemiluminescence image)**

* The file name says "from the top control ERK2 BSA". Each image has exactly three horizontal strips:
  * top strip = Control (rows ~880–960);
  * middle strip = 100 uM ERK2 (rows ~1075–1150);
  * bottom strip = 100 uM BSA (rows ~1295–1345).
* `Fig5E_overview.png` shows the same order (Control, 100 µM ERK2, 100 µM BSA). The legend says:
  "Control stands for reaction without ERK2. BSA was used as a control for molecular crowding
  effects."

**Ponceau image to group**

* The file name gives the group (`*_control_*`, `*_ERK2_*`, `*_BSA_*`).
* The images agree with it: the ERK2 membranes carry a strong extra band at ~42 kDa and the BSA
  membranes a strong band at ~66 kDa, as expected from 100 µM ERK2 or BSA in the reaction. The control
  membranes have neither. This is a check on the file names only, not on target intensity.

**Lane to time point (same rule for all 9 strips and all 9 Ponceau images)**

* The overview figure gives 12 lanes per condition, labelled left to right
  0, 1:15, 2:30, 3:45, 5, 6:15, 7:30, 8:45, 10, 15, 30, 60 min. `claim.yaml` gives the same order.
* Each Ponceau membrane shows 12 sample lanes between two ladders. Lane 1 is next to the left ladder,
  which sits by the cut top-left corner. Lane 1 is the 0 min sample.
* In the chemiluminescence strips the ladders were cut off. I placed the 12 lanes from these, which do
  not depend on target intensity:
  * the inter-lane valleys of the column profile along the band;
  * the regular lane pitch (80–89 px);
  * in the ERK2 strips, a weak second band below the target (rows ~1118 in rep I, ~1178 in rep III).
    It shows lane positions in lanes where the target band is absent, including lane 1.
  * Strip I_BSA shows 12 separate band positions. The other strips show 10 or 11 target bands plus
    1–2 empty lane positions at the left end, at the regular pitch.

| Blot | Lanes (left to right) | Group | Evidence |
|---|---|---|---|
| repI_control | 1–12 = 0 … 60 min | Control | file name "I_", top strip ("from the top control"); overview lane labels |
| repI_ERK2 | 1–12 = 0 … 60 min | 100 uM ERK2 | "I_", middle strip; overview; lower band marks lanes 1–2 |
| repI_BSA | 1–12 = 0 … 60 min | 100 uM BSA | "I_", bottom strip; overview |
| repII_control | 1–12 | Control | "II_", top strip |
| repII_ERK2 | 1–12 | 100 uM ERK2 | "II_", middle strip |
| repII_BSA | 1–12 | 100 uM BSA | "II_", bottom strip |
| repIII_control | 1–12 | Control | "III_", top strip |
| repIII_ERK2 | 1–12 | 100 uM ERK2 | "III_", middle strip; lower band marks lanes 1–2 |
| repIII_BSA | 1–12 | 100 uM BSA | "III_", bottom strip |

* For each blot, the reference lanes 1–12 are the 12 sample lanes of `<rep>_<condition>_Ponceau S`,
  left to right, with the ladders excluded.
* Excluded lanes:
  * the Ponceau ladders: left ~850–890 and right ~1940–1970, varying by image;
  * in II_ERK2 Ponceau, a faint spot at x ≈ 905 next to the left ladder (ladder, not a sample lane).

## Geometry (task step 3)

**Method**

* `refine_lanes.py` was run on every strip and Ponceau row, with `--search 20` and approximate centres
  read from the valley positions.
* A first pass with `--search 35` drifted by up to half a lane in III_ERK2 and III_BSA. I rejected it
  after checking an overlay.
* Lane width = round(0.75 × median pitch), as `refine_lanes.py` does.
* The bands are tilted (smile or slope up to ~35 px across a strip), so I give `target_y` **per lane**:
  the row maximum near the refined row.
* `band_half_height` = ceil(1.1 × median FWHM). This follows `refine_lanes.py`, but I measured the
  FWHM per lane, centred on each lane's own row. The script's pooled profile adds the tilt and gave
  inflated values (up to 40).
* `blotverse lanes` was not used for the final values. It takes a single band row, but the strips are
  tilted and three strips share one image. The valley and refine method was checked by overlay
  instead.
* Every box was checked on overlays of all 9 strips and all 9 Ponceau rows.

**Target (chemiluminescence)**

| Blot | x lane 1 → 12 | width | target_y range | half-height |
|---|---|---|---|---|
| repI_control | 946.0 → 1909.3 | 66 | 881–906 | 24 |
| repI_ERK2 | 910 → 1883.1 | 66 | 1075–1111 | 22 |
| repI_BSA | 932.9 → 1893.2 | 66 | 1303–1329 | 26 |
| repII_control | 913.8 → 1843.7 | 63 | 909–945 | 26 |
| repII_ERK2 | 902.5 → 1803.7 | 60 | 1103–1141 | 24 |
| repII_BSA | 894.7 → 1781.6 | 60 | 1296–1322 | 31 |
| repIII_control | 963 → 1919.4 | 64 | 945–963 | 26 |
| repIII_ERK2 | 963 → 1874.0 | 63 | 1137–1149 | 21 |
| repIII_BSA | 963 → 1859.3 | 61 | 1336–1342 | 26 |

* In empty lanes I set x by extrapolating the pitch and y from the trend of the neighbouring lanes,
  not from a refined value:
  * lane 1 of repI_control, repII_ERK2, repIII_control and repIII_BSA;
  * lanes 1–2 of repI_ERK2 and repIII_ERK2.

**Reference (Ponceau S)**

* The reference band is the low-MW band (eIF5A1 substrate, 17 kDa) at rows ~1380–1610, depending on
  the image.
* Width 60–66, half-height 20–24, per-lane y.
* Manual fixes:
  * I_BSA lane 1: y set to 1511, the profile peak. The automatic row was at the window edge.
  * III_BSA lane 5: y set to 1583, avoiding a dark speck.
  * III_BSA lane 12: x 1814.7 and y 1588. This lane is narrower, and the valley-midpoint guess of 1835
    was rejected after the column profile peaked at ~1816.
  * III_BSA half-height set to 24. The automatic FWHM median was pulled down by specks.

## Uncertainties

1. **Schema:** see "Blocking issue". This is the main problem. The mapping itself is clear.
2. **Chemiluminescence strip lanes 1 (and 2 for ERK2) are empty, and the strip edges are not visible.**
   Their positions are extrapolated from the pitch. In the ERK2 strips they are also supported by the
   weak lower band. If a strip were offset by one lane, every time point would shift.
   * Against an offset: every strip has exactly 12 lane positions between its left membrane edge and
     its last band.
   * Also against it: the 12 Ponceau lanes of each membrane have the same pitch.
3. **Strip orientation:** I assumed no strip was flipped left–right. The overview figure shows all three
   in the same left (0 min) to right (60 min) order, and the Ponceau cut corner is top-left on every
   membrane.
4. **Which Ponceau band was used:** Methods only say "Panceau S signal was used for chemiluminescent
   signal normalization". The band, lane region or whole-lane total used by the authors is not stated.
5. **Large late-time bands:** the 30–60 min target bands in several strips are wider and taller than
   the lane box (e.g. repI_ERK2 lane 12, repI_BSA lanes 11–12, repII_BSA lanes 11–12). Lane 12 of
   repII_BSA looks like a merged or smeared blob.
6. **Lower band in the ERK2 strips:** a second band about 40 px below the target in repI_ERK2 and
   repIII_ERK2. It is outside the target window, but close to it at the right-hand lanes.
7. **Ponceau speckle:** dark specks in III_BSA and dust on several membranes. Lane boxes were placed to
   avoid them where possible.
8. **Time course as groups:** the claim compares groups. Each group is a 12-point time course, so its
   lanes are not exchangeable replicates of one condition. How time points should be pooled is not
   specified in the experiment file. Each lane's `sample` carries the replicate, condition and time
   point.

## Choices between alternatives

* One blot per replicate x condition (chosen), versus one blot per replicate with 36 lanes. The latter
  cannot hold three reference images.
* `reference_image` (chosen) versus `total_protein`: the lane positions differ, so `total_protein` is
  not allowed.
* Reference band = eIF5A substrate band (chosen). Rejected alternatives:
  * the ERK2 or BSA band, which is absent from control membranes;
  * whole-lane Ponceau, which BlotVerse does not model as a separate image.
* Target = upper (main) chemiluminescence band in each strip, the one shown in the overview. The weak
  lower band in the ERK2 strips was not annotated as target.
* `invert: false` for all blots, because bands are bright on dark in both images.
* Per-lane `target_y` was used instead of a single row, because of the band tilt.
* `refine_lanes.py --search 20` was used instead of 35, because the wider search drifted to
  neighbouring lanes.
