# Screening notes: 10.1016/j.molcel.2017.05.006

Di Marco et al., "RECQ5 Helicase Cooperates with MUS81 Endonuclease in Processing Stalled
Replication Forks at Common Fragile Sites during Mitosis", Molecular Cell 66:658–671 (2017).
Deposit: Mendeley Data kvs9dtvdmy v1 (13 zip files, "Di Marco et al Figure 1–6, S1–S7"). Second
frame, rank 53. Screened under the brief with the Amendment 2 addition.

The article is not in PMC; Europe PMC lists it as subscription only. The article and
Supplemental Information came from the Elsevier CDN (`ars.els-cdn.com`, PII S1097276517303192):
mmc1 is the Supplemental Information and mmc2 is the article plus SI. Both are saved in `article/`.
No API that needs an email address was used.

## Criterion 1: no western blot panel quantified by the authors

The article is peer reviewed (Mol Cell). I read every legend in the main figures (1–6) and the
supplementary figures (S1–S7), and I looked at the figure pages (Fig. 1 and Fig. 6 rendered). No
western blot is quantified by densitometry and normalised to a loading control, or quantified at
all:

| Panel | What it is | Quantified? |
|---|---|---|
| Fig. 1A–D, 1I; S5A–B; S6A–C; S7D–G | nuclease, D-loop and EMSA assays on DNA gels (native PAGE / agarose) | yes, but as % product formation. These are DNA gels, not western blots |
| Fig. 1E, S1A, S1C, S1E | GST pull-down, SDS-PAGE stained gel (U/B fractions) | no |
| Fig. 1F, 6A | co-IP western blots (RECQ5 / MUS81) | no |
| Fig. 1H, S1D, S1F, S3A, S6D, S7B–C | pull-down western blots | no |
| Fig. 2A, 4C, 4E, 5B, 6D, 6E, 6F; S3B, S4A, S7A | western blots of depletion, expression or phosphorylation checks (MCM7, TFIIH and β-tubulin shown as loading) | no: no densitometry, no plot |
| Fig. 2B–E, 3, 4D/F, 5A/C–G, 6G–K; S2 | chromosome spreads, imaging, ChIP | yes, but not western blots |

The data-availability statement covers "Original imaging data and data from chromosome spread
analyses". The criterion-2 screen found one 16-bit TIFF (`4C_b-tubulin WB.tif`, Figure 4.zip). It
belongs to Fig. 4C, an expression check that is not quantified.

**Decision: excluded, criterion 1.** No panel meets the unit of analysis, so criteria 2–8 were
not examined. Amendment 2 changes nothing here: there is no quantified phospho/total blot, and
the co-IP blots are not quantified either.

## Files

* `article/`: `S1097276517303192-mmc1.pdf` and `S1097276517303192-mmc2.pdf`. They contain no
  western blot values.
* `images/` and `archives/` were left as downloaded by `screen_files.py`. No `fetch.sh` or
  `claim.yaml` was written.
* No file with the authors' numbers for a western blot exists, and none was downloaded.
