# Exposure-invariance bench: results

Known truth taken from published data, without a laboratory. The same membrane imaged at
several exposures must keep the ratio between two lanes of one band as long as the detector
is linear.

* Protocol: [`exposure_protocol.md`](exposure_protocol.md) (v1.0 pre-specified, v1.1
  amended before pooling, v1.2 fixed before a new confirmatory set).
* Screening: [`validation/exposure/screening.md`](../validation/exposure/screening.md).
* Reproduce: `validation/exposure/run_all.sh`. Results go to `validation/results/exposure/`.

## Summary

| Protocol | Set | Series (membranes) | Exposures with a saturation flag | P1 misses (spec rule) | Criterion |
|---|---|---|---|---|---|
| v1.1 (pair rule) | confirmatory 1 | 4 (3) | 3 | 3 | not met |
| v1.2 (lane rule) | development = all v1.1 series | 6 (5) | 11 | 0 | not evidence (rule built on these) |
| **v1.2 (lane rule)** | **confirmatory 2** | **5 (4)** | **5** | **0** | **met** |

**v1.2 confirmatory set:** UFM1 blots, EMBO J 2026 (doi 10.1038/s44318-026-00753-9). Four
12-lane membranes, each at a normal and a lower exposure. 1 of the 16 pre-listed studies was
eligible; every other one deposited only 8-bit, RGB, CMYK or differently binned files (see
the screening log).

* **P1 (judged):** 0 misses in 40 usable exposure x band combinations. The saturation rule
  flagged the 5 exposures with saturated windows (the low continuous band). In 4 of the 5,
  the lane-level analysis shows the sub-linear lanes the saturation produces.
* **Candidate near-saturation rule:** 0 misses. It flagged no additional exposure, so this
  set does not test it. Promoting it to the spec stays undecided.
* **The v1.1 pair rule** would have counted 1 miss (UFM1 replicate 3, band r246, normal
  exposure). No lane moves beyond ±0.2, so it is not compression of any lane.
* **P2:** 147/148 compressive pairs carry a flag (Wilson 95 % 0.96–1.00).
* **S1:** confirms the development ranking on independent data. Rolling ball + rectangle
  preserves ratios best (`rb2/rect` q95 0.067, `rb4/rect` 0.060 log2). The spec default,
  `rb2/profile`, comes next (q95 0.125). Saturation damages the profile branches most
  (flagged median 0.37 vs 0.18 log2 for rect).

**Caveat on size.** One study, 4 membranes, 0 misses in 40 exposure x band
combinations. By the rule of three, the 95 % upper bound on the miss rate per combination is
about 3/40 = 7.5 %. However, only 5 combinations actually reached saturation. The claim
"compression is always flagged" therefore rests on 5 cases, and on those alone the upper
bound is about 3/5 = 60 %. More 16-bit exposure series would tighten this. Most journal
deposits do not contain them, which limits the bench.

The sections below report the v1.1 analysis that motivated v1.2.

## Data

| Set | Series | Membrane | Exposures |
|---|---|---|---|
| Development | `gtsf1_2026_series_a` | GTSF-1, EMBO Rep 2026 (doi 10.1038/s44319-026-00914-1), ChemiDoc `.scn` | 1, 50.75, 100.5, 150.25, 200 s (cumulative; a 4x4-binned 4.9 s capture is excluded by the grid check) |
| Development | `gtsf1_2026_series_b` | GTSF-1, same article | 4.5 s auto + 38.25, 75.5, 112.75, 150 s (cumulative) |
| Confirmatory | `set1ip_2026_gfp` | GFP-Set1 IP, EMBO Rep 2026 (doi 10.1038/s44319-026-00901-6), ChemiDoc raw16 | 20 s, 294.7 s (separate captures) |
| Confirmatory | `plcg_ip_2025_plc`, `_flag_left`, `_flag_right` | HEK IPs, EMBO J 2025 (doi 10.1038/s44318-025-00600-3) | 5 s, 20 s (separate captures) |

Screening found 2 eligible studies out of 16. Most deposits hold only 8-bit or colour
exports.

## Primary endpoint P1: does the saturation QC flag every compressed exposure?

**Criterion (G3 clause): zero misses. Not met.**

| Set | Usable exposure x band | With a saturation flag | Misses | Not evaluable (saturated at every exposure) |
|---|---|---|---|---|
| Development | 20 | 8 | 1 | 0 |
| Confirmatory | 32 | 3 | **3** | 3 bands |

### Saturation itself is always caught

* Every exposure with pixels at the detector maximum was flagged; this holds by
  construction of the rule.
* The informative result is that saturation always distorted the ratios:
  * In the development set, the top bands saturate in every lane and move pairs by
    0.4–2.2 log2 (median, by branch).
  * The confirmatory set has a partial-saturation band, `plc r264`, where 2 of 6 lanes
    saturate at 20 s. With the default branch, all 9 pairs that include a saturated lane are
    distorted and flagged (median 0.76 log2), while the 6 pairs of unsaturated lanes move by
    0.03 log2 (median).
* P2 (compressive pairs that carry a flag, default branch):
  * development: 92/94 (Wilson 95 % 0.93–0.99);
  * confirmatory: 9/20 (0.26–0.66).
  * The unflagged pairs are those of the misses below.

### The 4 misses: no pixel reached saturation

In every miss the brightest pixel of the band windows stayed at 16–79 % of full scale.

| Miss | What happens | Likely cause |
|---|---|---|
| GTSF-1 B `lower`, 150 s | ratios of lane 6 against the separate 4.5 s auto capture move by 0.4–1.5 log2 | lane 6 is faint (prominence 11 at 4.5 s, just above the floor), and the 4.5 s capture is a separate acquisition |
| GFP `doublet`, 295 s | every pair with lane 9 moves by +0.2–0.65 log2 | lane 9 is faint at 20 s (prominence 15): low-signal under-measurement at the short exposure |
| PLC `r444`, 20 s | pairs with lanes 5–6 move by +0.24–0.34 log2 (profile branch only) | lanes 4–6 are faint at 5 s (prominence 19–31) |
| PLC `r404`, 20 s | lanes 1–3 move by −0.27 log2 (profile branch) | lane 3 reaches 51 537 counts (79 % of full scale) at 20 s: possibly detector non-linearity below saturation |

**Reading.** In this bench, the saturation rule caught all saturation. It did not see:

* **Compression of faint lanes against a short exposure.** The "compressive" signature
  cannot tell low-signal under-measurement at the short exposure apart from compression at
  the long one.
* **One case of possible non-linearity near the top of the range.**

Under the pre-specified rule, both count as misses, and they are reported as such.

**For a future protocol version, to be tested on new series, not these:**

* a stricter SNR requirement at the shorter exposure of each pair;
* a near-saturation warning (for example any pixel above 75 % of the detector maximum).

## Secondary endpoints: how well each branch preserves ratios (S1–S3)

Clean pairs only (no flagged lane, above the SNR floor): \|Δ log2 ratio\| between
exposures.

| Branch | Dev n | Dev median | Dev q95 | Conf n | Conf median | Conf q95 | Conf > 0.2 |
|---|---|---|---|---|---|---|---|
| rb4/rect | 27 | 0.020 | **0.069** | 82 | **0.043** | 0.255 | 10 |
| rb2/rect | 27 | 0.022 | 0.443 | 82 | 0.044 | **0.213** | **6** |
| rb1/rect | 27 | 0.014 | 0.393 | 82 | 0.060 | 0.238 | 6 |
| rb1/mask | 27 | 0.019 | 0.331 | 82 | 0.056 | 0.256 | 11 |
| rb4/mask | 27 | 0.020 | 0.249 | 82 | 0.069 | 0.396 | 14 |
| rb2/mask | 27 | 0.042 | 0.362 | 82 | 0.080 | 0.617 | 16 |
| rb2/profile (default) | 27 | 0.029 | 1.050 | 82 | 0.065 | 0.476 | 13 |
| rb4/profile | 27 | 0.011 | 0.441 | 82 | 0.069 | 0.422 | 14 |
| rb1/profile | 27 | 0.044 | 0.527 | 82 | 0.059 | 0.451 | 14 |
| local/rect | 27 | 0.022 | 0.405 | 82 | 0.057 | 0.429 | 13 |
| local/mask | 27 | 0.038 | 0.387 | 82 | 0.060 | 0.317 | 16 |
| local/profile | 27 | 0.015 | 0.503 | 82 | 0.052 | 0.383 | 11 |

Damage from saturation (median \|Δ\| of flagged pairs):

* development: profile 2.17, rect 0.57–0.96, mask 0.50–0.88;
* confirmatory: profile 0.75, rect 0.23–0.42, mask 0.06–0.17.

## What this means for BlotVerse

* **Within the linear range, typical ratio error is 3–8 %** (median 0.04–0.08 log2), so
  the median ratio stays put in every branch. The tails differ:
  * rolling-ball + fixed rectangle (`rb2/rect`, `rb4/rect`) has the lightest tails;
  * the **profile-peak branch (the spec's default band definition)** has heavier tails in
    both sets, and saturation damages it most, because a clipped peak loses its height.
  * This is evidence for revisiting the default band definition in a later spec version.
    The spec is not changed here: that would be a post-hoc choice.
* **The saturation QC rule is necessary but not sufficient.**
  * It catches true saturation.
  * Faint bands near the noise floor and near-saturation non-linearity distort ratios
    without tripping it.
  * The multiverse is where those show up: as disagreement between band-definition and
    background branches.

## Limitations

* **Small sample:** 3 membranes in the confirmatory set, 2 exposures each.
* **One annotator, not blinded.**
* **Protocol amended after screening:** see v1.1 in the protocol file. The amendment widens
  the endpoints rather than narrowing them.
* **Separate captures** (all confirmatory series) can differ chemically between exposures.
  Such differences cannot be told apart from measurement error here.
