# Screening notes: 10.1016/j.molcel.2017.01.026

Bürmann et al., "Tuned SMC Arms Drive Chromosomal Loading of Prokaryotic Condensin",
Mol Cell 65 (2017). PMID 28238653, PMC5344682. Peer-reviewed journal article, not a
preprint. Deposit: Mendeley Data sxn48nykf3 v1 ("Original data of gels, Western blots and
dilution spot assays"). Second sampling frame (rank 105), screened 2026-10-27 under the
brief with the Amendment 2 addition.

## Sources

* `article/PMC5344682.xml`: Europe PMC full text (text, legends, STAR Methods).
  SHA-256 e5608bba708c5f843866624a76f7dde1931ecd212994c61047c3b17b8f3d3546.
* `article/mmc1_supplementary.pdf`: publisher Supplemental Information (Figures S1–S6,
  Tables S1–S5), `https://ars.els-cdn.com/content/image/1-s2.0-S1097276517300527-mmc1.pdf`,
  SHA-256 458dae5c91089fbfdcab89d86c76e6c52026777ac45609ab53b05957ba47eb9b.
* `article/gr2_lrg.jpg`: Figure 2 (publisher), checked for blot quantification.
* Deposit file list from the Mendeley public API: zips for Figures 2, 3, 4, 5, S2, S3, S5,
  S6. `screen_files.py` found raw files (criterion2_frame2.tsv: e.g. `2E_Western.tif`
  raw = yes; `S2E_right.tif` pseudo-coloured).

## Western blots and quantification (figure order)

* Western blots in the article: Fig. 2E and 2F ("Expression of the engineered alleles was
  probed by western blotting using polyclonal antibodies raised against full-length Smc
  ... Coomassie staining of extracts run on a separate SDS-PAGE gel is shown as a loading
  control"), Fig. 5E and Fig. S5C ("As in Figure 2E"). These are single expression checks
  under spot dilutions, one lane per strain, with no quantification, n or statistics
  (checked on Figure 2).
* Quantified gel bands: Fig. 3A, 3B, S3D, S5E, S5G are **in-gel HaloTag fluorescence** after
  in vivo BMOE cross-linking, quantified as the cross-linked fraction ("Protein bands were
  quantified in Wolfram Mathematica"; three separate reactions, mean ± SD and 95 % credible
  intervals). These are not western blots, and the readout is a within-lane fraction, not a
  target normalised to a loading control. Fig. 4C (chromosome entrapment) is also in-gel
  fluorescence. Figs. 3C and S6C are Coomassie gels of purified protein.
* Other graphs: colony-area screens (2C, 5D, S2B, S5B), ChIP-seq/qPCR (4A–B, 6C, S4,
  S5D), ATPase assays (3D, 6D, S3E–F), sequence statistics (1, S1).

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 2E (anti-Smc / CBB, short smc alleles) | 1 | Western blot shown as an expression check, not quantified by the authors; one lane per strain, no n or test. |
| Fig. 3A (hinge cross-linking, in-gel fluorescence) | 1 | Quantified, but not a western blot and not normalised to a loading control (cross-linked fraction within each lane). |

## Decision

**Excluded, criterion 1.** No western blot panel is quantified by the authors. The western
blots (Figs. 2E, 2F, 5E, S5C) are unquantified expression checks. The quantified bands are
in-gel fluorescence cross-linking fractions, which are not western blots normalised to a
loading control. Amendment 2 (criterion 7, phospho/total) does not change this.

No `fetch.sh` or `claim.yaml` was written. `images/` and `archives/` still hold the files
downloaded by `screen_files.py`.
