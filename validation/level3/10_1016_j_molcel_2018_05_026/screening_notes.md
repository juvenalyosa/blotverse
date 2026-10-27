# Screening notes: 10.1016/j.molcel.2018.05.026

Lemmens et al., "DNA Replication Determines Timing of Mitosis by Restricting CDK1 and PLK1
Activation", *Molecular Cell* 71:117-128 (2018). Second frame, rank 9. Deposit: Mendeley
Data 47s44zbm7c v1 ("Raw image data Lemmens et al. 2018", one archive). Screened
2026-10-27 under the Brief with the Amendment 2 addition.

## Sources

* Europe PMC full text `article/PMC6039720.xml` (all main figure legends).
* Elsevier files: `article/1-s2.0-S1097276518304003-mmc4.pdf` = Document S2, article plus
  supplemental information (SHA-256
  ae5316ad97a4c9111c13d6ee938477ae5b82ece1438cd7e4b8ab8e02667152c6) and
  `article/1-s2.0-S1097276518304003-mmc1.pdf` = Document S1, Figures S1-S7 (SHA-256
  af4333cf9ca543e261357216272f1819618c22dfe56e0f16c957b0011c8847f0); rendered pages
  `article/figure2_page.png` and `article/figureS3_page.png`.
* Deposit: `archives/Lemmens2018_raw_image_data.zip` (SHA-256
  6e7ba299c9f184a9ab94c68b1eb3df2475521cf7a8d4bb309f92b4441115098c; listed with
  `python3 -I` zipfile, not executed). Its only western blot files are
  `Fig2B/1B_raw_blots_CDC6_GAPDH.tif` and `Fig2B/1B_raw_blots_OsTIR.tif`; the rest are
  microscopy stacks (Fig. 2C, 4A, ...). `blotverse inspect` on
  `images/Lemmens2018_raw_image_data__Fig2B__1B_raw_blots_CDC6_GAPDH.tif`: 16-bit, raw = yes.
* Deposit file list from the Mendeley public API: `article/deposit_metadata.json`.

## Criterion 1

Peer-reviewed article, but **no western blot panel is quantified by the authors**.

* Fig. 2B: "RPE CDC6d cells were treated as outlined, and cell lysates were probed against
  indicated proteins" (CDC6-mAID, myc-OsTIR, KAP1); one representative blot, no
  densitometry, no n, no statistics.
* Fig. S3A and S3B: western blots of CDC6 degradation ("Cell lysates were probed for CDC6
  protein and GAPDH for loading"), shown only; no quantification.
* Every quantification in the article (Figs 1-6, S1-S7) is from high-content
  microscopy, live-cell FRET imaging or EdU/IF; the methods mention western blotting only
  to verify degron tagging. Nothing is normalised to a loading control and compared
  between groups.

| Panel | Result | Evidence |
|---|---|---|
| Fig. 2B (CDC6-mAID, OsTIR, KAP1) | criterion 1 failed | qualitative, not quantified |
| Fig. S3A / S3B (CDC6, GAPDH) | criterion 1 failed | qualitative, not quantified |

## Decision

**Excluded.** First failing criterion of the candidate: **criterion 1** (no western blot
panel quantified by the authors). No `fetch.sh` or `claim.yaml` was written.
