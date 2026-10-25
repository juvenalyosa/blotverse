# Image-integrity screening: injection bench

9 real raw images; 340 trials. Settings: patch 12, min correlation 0.5, copy error <= 0.1 x noise, min patches 16, seam z >= 10, seam length >= 128, flat ratio 0.25.

| Manipulation | Trials | Detected | Rate (Wilson 95 %) | Trials with a false finding | Rate (Wilson 95 %) | False findings |
|---|---|---|---|---|---|---|
| none | 45 | - | - | 10 | 0.222 (0.125-0.363) | 10 |
| copy | 45 | 44 | 0.98 (0.88-1.00) | 10 | 0.222 (0.125-0.363) | 10 |
| copy_gain | 45 | 45 | 1.00 (0.92-1.00) | 11 | 0.244 (0.142-0.387) | 11 |
| copy_flip | 45 | 45 | 1.00 (0.92-1.00) | 11 | 0.244 (0.142-0.387) | 11 |
| copy_noise | 45 | 0 | 0.00 (0.00-0.08) | 10 | 0.222 (0.125-0.363) | 10 |
| cross_copy | 45 | 45 | 1.00 (0.92-1.00) | 21 | 0.467 (0.329-0.609) | 25 |
| splice | 25 | 25 | 1.00 (0.87-1.00) | 5 | 0.200 (0.089-0.391) | 5 |
| erase | 45 | 30 | 0.67 (0.52-0.79) | 10 | 0.222 (0.125-0.363) | 10 |

All trials: 88 / 340 with at least one false finding (0.259, Wilson 95 % 0.215-0.308).

* `copy`: a band region (24-64 x 16-32 px) pasted elsewhere in the same image. `copy_gain`: the same with brightness/contrast changed (gain 0.6-1.5, offset +-5 noise). `copy_flip`: flipped or rotated 180 degrees. `copy_noise`: with Gaussian noise of 0.5 x the image noise added (outside the stated scope of the copy test; reported to show the limit).
* `cross_copy`: a band region of an image from another study pasted into this one; both images are screened together.
* `splice`: a full-height strip (40-120 px) replaced by the same columns of an image from another study.
* `erase`: a band region (32-64 x 16-32 px) filled with the median of its border.
* A detection needs a finding of the right kind on the injected region (copies: at least 25 % of both the pasted and the source region; seams: within 2 px of a strip edge; flat: 25 % of the erased area). Flat regions inside a spliced strip are related findings, not false. Any other finding is false.

Runtime 776 s.
