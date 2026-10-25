# Image-integrity screening

Findings are **regions to review**, not evidence of manipulation. Legitimate operations can produce them: a reused loading control that the authors declared, membrane or strip edges, exports with clipped or processed background. Review each one against the original acquisition and the methods.

## Images

| # | Image | Size | Bits | Grey levels | Usable patches | Pixel SHA-256 |
|---|---|---|---|---|---|---|
| 1 | 0000453_03_700.TIF | 497x426 | 16 | 1529 | 201690 | `0b9a570dc17a9f2d...` |
| 2 | 0000453_03_800.TIF | 497x426 | 16 | 4689 | 201690 | `6b44cff3ea39bed8...` |

* Image 1: floating-point image rescaled to 16-bit (x1535.226940); detector saturation cannot be checked from pixel values
* Image 2: floating-point image rescaled to 16-bit (x211.403226); detector saturation cannot be checked from pixel values
* Image 2: flat-region test skipped: typical residual noise below 2.000000 quantisation steps

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

Runtime 0.5 s.
