# Screening notes: 10.1016/j.cell.2018.01.032

Hoshii et al., "A Non-catalytic Function of SETD1A Regulates Cyclin K and the DNA Damage
Response", Cell 172 (2018) 1007-1021, PMID 29474905, PMC6052445 (author manuscript).
Deposit: Mendeley Data hhjhsrgkz7 v1 (2018-02-02), "Raw data of giemsa images, western blot
images and karyotyping images", 14 zips (Figure 1-7, Figure S1-S7). Second frame, screened under
Amendment 2 (2026-10-27).

## Sources

* `article/PMC6052445.xml`: PMC XML (NCBI efetch; Europe PMC fullTextXML returned HTTP 500).
  Main text, methods and main-figure legends.
* `article/nihms939010f2.jpg`, `f4`-`f7`: main figures from PMC. Fig. 1 (`nihms939010f1.jpg`) prints
  the authors' fold-change values under the Fig. 1F blot; it was viewed and then deleted from this
  folder (URL: https://pmc.ncbi.nlm.nih.gov/articles/instance/6052445/bin/nihms939010f1.jpg).
* `article/mendeley_hhjhsrgkz7.json`: deposit metadata.
* The supplementary PDF (Figures S1-S7 and their legends, `NIHMS939010-supplement-1.pdf`) could not
  be downloaded: PMC and cell.com return a bot-check page / HTTP 403 to scripts, and the
  Elsevier API gives metadata only.

## Criterion 1

Peer-reviewed article (Cell). Fig. 1F is a western blot quantified by the authors: "Quantified
intensity is normalized by GAPDH signal and fold changes are shown as a relative value to Control."
Passes.

## Quantified western blot panels (figure order)

Only one main-figure legend describes a quantified blot: **Fig. 1F** (SETD1A, H3K4me1/2/3, H3 /
GAPDH; shRNA groups Ren/Ren, Ren/Setd1b, Setd1a/Ren, Setd1a/Setd1b; fold change printed under each
lane). Other main-figure blots (2B, 4H, 5B-G co-IP, 6G co-IP, 6H, 7E) are shown without
quantification. Supplementary legends could not be read (see above).

## Panels examined

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 1F (SETD1A etc. / GAPDH) | **4** | C2 passes: `blotverse inspect` on the six `Figure 1/1F_*.tif` files (GAPDH, H3, H3K4me1, H3K4me2, H3K4me3, SETD1A): 1536 × 1024, 16-bit, raw = yes. C3 passes (whole membranes). C4 fails: the figure shows one lane per group and the legend says "Representative images from 2 independent experiments are shown"; no ≥ 3 biological replicates per group are stated, and the quantification is a single fold change per lane, with no statistics. |
| Supplementary blots (S1, S2D/E, S3B, S4C, S5B, S6D, S7A/C deposited) | not assessed against legends | Legends not obtainable. From the deposited images: S4C files are JPEG (C2 would fail); S1P, S2D, S7A, S3B and S6D show sets of 4-6 lanes per membrane (S3B, S6D rotated 90°), with no layout of ≥ 3 replicate lanes per group. S5B/S7C are co-IP. |

## Decision

**Excluded, criterion 4 (Fig. 1F)**: the first quantified panel shows a single lane per group
(representative of 2 experiments). No `fetch.sh` or `claim.yaml` written. `images/` and
`archives/` are left as downloaded by `screen_files.py`. No file with the authors' numbers is
kept in this folder.

Caveat: the supplementary legends were not read. If a supplementary blot were quantified with
≥ 3 replicates per group it could still qualify; nothing in the deposited images suggests one.
