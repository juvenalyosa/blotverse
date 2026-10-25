# Image-integrity screening

Findings are **regions to review**, not evidence of manipulation. Legitimate operations can produce them: a reused loading control that the authors declared, membrane or strip edges, exports with clipped or processed background. Review each one against the original acquisition and the methods.

## Images

| # | Image | Size | Bits | Grey levels | Usable patches | Pixel SHA-256 |
|---|---|---|---|---|---|---|
| 1 | 3G_WB_DICER_R1.tif | 1200x1648 | 16 | 44618 | 1920388 | `59234abe8808bba9...` |
| 2 | 3G_WB_DICER_R2.tif | 1200x1648 | 16 | 33233 | 1946393 | `7ea0130c1519418e...` |
| 3 | 3G_WB_DICER_R3.tif | 1200x1648 | 16 | 37896 | 1946393 | `33ca01dc812ce45d...` |
| 4 | 3G_WB_HA_R1.tif | 1200x1648 | 16 | 20019 | 1687254 | `52051b53b0767d6e...` |
| 5 | 3G_WB_HA_R2.tif | 1200x1648 | 16 | 20780 | 1938987 | `523c9a75a60cebf0...` |
| 6 | 3G_WB_HA_R3.tif | 1200x1648 | 16 | 27205 | 1944695 | `662047929f6cb51d...` |

* Image 1: flat-region test skipped: typical residual noise below 2.000000 quantisation steps
* Image 3: flat-region test skipped: typical residual noise below 2.000000 quantisation steps
* Image 4: flat-region test skipped: typical residual noise below 2.000000 quantisation steps
* Image 5: flat-region test skipped: typical residual noise below 2.000000 quantisation steps
* Image 6: flat-region test skipped: typical residual noise below 2.000000 quantisation steps

## Identical pixel data

No two images have the same pixel fingerprint.

## Duplicated regions (shared noise)

Residual correlation >= 0.5 over 12x12 patches, at least 16 connected patch positions; flipped and 180-degree copies included; every pair of images compared. Genuine bands of the same shape do not share noise; independent noise gives a correlation near 0 (standard deviation about 1/12).

None found.

## Straight seams (lower confidence)

A sharp step in the background along a single column or row, sustained over at least 64 pixels and at least 128 in total (robust z >= 10, step share 0.8-1.25). Pasted strips from another acquisition produce them; so can a straight membrane edge or a stitched acquisition.

None found.

## Flat regions (lower confidence)

8x8 windows whose residual noise is below 0.25 x the noise of the image's quietest decile of windows, excluding saturated and minimum-clipped windows. Erased or painted areas produce them; so can processed exports.

None found.

## Not covered

* Copies that were rescaled, rotated by other than 180 degrees, blurred, or re-noised beyond about the image's own noise level.
* Figures that went through JPEG compression or resampling: the noise this screening relies on is gone.
* Masks: `mask_N.tif` holds the finding number at each flagged pixel of image N.

Runtime 84.3 s.
