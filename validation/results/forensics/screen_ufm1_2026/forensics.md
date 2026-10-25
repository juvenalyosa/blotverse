# Image-integrity screening

Findings are **regions to review**, not evidence of manipulation. Legitimate operations can produce them: a reused loading control that the authors declared, membrane or strip edges, exports with clipped or processed background. Review each one against the original acquisition and the methods.

## Images

| # | Image | Size | Bits | Grey levels | Usable patches | Pixel SHA-256 |
|---|---|---|---|---|---|---|
| 1 | WB_UFM1.tif | 687x550 | 16 | 13775 | 357417 | `89369efce2edc286...` |
| 2 | WB_UFM1_lower_exposure.tif | 687x550 | 16 | 3287 | 364364 | `23b05a4c3e98cc6a...` |
| 3 | WB_UFM1_lower_exposure_replicate_2.tif | 687x550 | 16 | 4413 | 364364 | `b042d5893d6a8aee...` |
| 4 | WB_UFM1_lower_exposure_replicate_2_1.tif | 916x733 | 16 | 7354 | 653410 | `5f5d9b75d7ce7633...` |
| 5 | WB_UFM1_lower_exposure_replicate_3_1.tif | 916x733 | 16 | 9831 | 653410 | `0c00e31d8e107f0b...` |
| 6 | WB_UFM1_replicate_2.tif | 687x550 | 16 | 15709 | 355972 | `7f681acd479f3f3c...` |
| 7 | WB_UFM1_replicate_2_1.tif | 916x733 | 16 | 13135 | 644935 | `7e415c2437912b4e...` |
| 8 | WB_UFM1_replicate_3_1.tif | 916x733 | 16 | 16532 | 642833 | `d18a2ef6eeef788c...` |


## Identical pixel data

No two images have the same pixel fingerprint.

## Duplicated regions (shared noise)

Residual correlation >= 0.5 over 12x12 patches, at least 16 connected patch positions; flipped and 180-degree copies included; every pair of images compared. Genuine bands of the same shape do not share noise; independent noise gives a correlation near 0 (standard deviation about 1/12).

| # | Region (x0,y0-x1,y1) | Image | Matches | Image | Orientation | Area px | Mean corr. | Identical px | Gain | Offset |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0,0-916,733 | 4 | 0,0-916,733 | 7 | identity | 666955 | 0.770 | 0.5 % | 0.139 | 545.2 |
| 2 | 0,0-916,733 | 5 | 0,0-916,733 | 8 | identity | 665135 | 0.777 | 0.3 % | 0.141 | 553.1 |
| 3 | 0,0-687,550 | 1 | 0,0-687,550 | 2 | identity | 371441 | 0.790 | 0.3 % | 18.504 | -11086.9 |
| 4 | 0,0-687,550 | 3 | 0,0-687,550 | 6 | identity | 362964 | 0.768 | 0.2 % | 0.018 | 647.8 |

## Straight seams (lower confidence)

A sharp step in the background along a single column or row, sustained over at least 64 pixels and at least 128 in total (robust z >= 10, step share 0.8-1.25). Pasted strips from another acquisition produce them; so can a straight membrane edge or a stitched acquisition.

| Image | Direction | Position | Extent | Step (counts) | Step / noise | z | Share |
|---|---|---|---|---|---|---|---|
| 1 | row | 225 | 274-412 | -655.5 | -8.6 | 34.0 | 0.85 |
| 1 | row | 301 | 274-549 | -1416.4 | -18.5 | 87.4 | 1.12 |
| 1 | row | 304 | 274-480 | 1811.5 | 23.7 | 96.0 | 1.02 |
| 2 | row | 412 | 274-412 | 810.5 | 20.9 | 240.8 | 0.97 |
| 2 | row | 413 | 480-618 | 1980.0 | 51.0 | 430.8 | 0.88 |

## Flat regions (lower confidence)

8x8 windows whose residual noise is below 0.25 x the noise of the image's quietest decile of windows, excluding saturated and minimum-clipped windows. Erased or painted areas produce them; so can processed exports.

None found.

## Not covered

* Copies that were rescaled, rotated by other than 180 degrees, blurred, or re-noised beyond about the image's own noise level.
* Figures that went through JPEG compression or resampling: the noise this screening relies on is gone.
* Masks: `mask_N.tif` holds the finding number at each flagged pixel of image N.

Runtime 9.1 s.
