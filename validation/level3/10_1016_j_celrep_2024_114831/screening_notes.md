# Screening notes: 10.1016/j.celrep.2024.114831

Article: Cell Reports 43, 114831 (2024), "ERK1/2 interaction with DHPS regulates eIF5A
deoxyhypusination independently of ERK kinase activity". PMID 39392755, PMC11544350.
Peer-reviewed journal article, not a preprint. Deposit: Zenodo 17700326.

Files in `article/`: PMC full-text XML (`PMC11544350.xml`), PMC figure images
(`nihms-2031309-f0001.jpg` is the graphical abstract, `f0002`–`f0008` are Figures 1–7),
publisher supplementary PDF (`1-s2.0-S2211124724011823-mmc1.pdf`, Figures S1–S8 and Tables S1–S4)
and the publisher article PDF (`1-s2.0-S2211124724011823-mmc2.pdf`).

## Deposit contents

Zenodo 17700326 has two files:

* `Raw membrane images from in vitro deoxyhypusination assays in tiff format.zip` (38 entries).
  It holds raw 16-bit ChemiDoc TIFFs (`*.raw16.tif`) for three panels only: Fig. 5E (time course,
  folders Replicate I/II/III, each with one chemiluminescence image of three cut strips "from the
  top control ERK2 BSA", plus one Ponceau S and one colorimetric "membrane cutting reference"
  image per condition), Fig. 5F (one chemiluminescence image of "replicates I II III", plus
  Ponceau S and colorimetric images per replicate) and Fig. S5E (one chemiluminescence image of
  replicates I–III, plus combined Ponceau S/cutting-reference image per replicate).
* `Numerical data for MST measurements ... in txt format.zip`: MST binding curves (Figs. 2, 4D,
  S2, S7), not western blot data.

`blotverse inspect` on all 31 TIFFs in the image zip: every file is 2750 x 2200, 16 bits,
16 effective bits, 0 saturated pixels, `raw = yes`.

No source-data table exists for any western blot: the article's "Data and code availability"
section lists only PDB/EMDB and MassIVE deposits; the only supplementary file is a PDF
(Figures S1–S8, Tables S1–S4: cryo-EM statistics, primers, MS settings, MS protein list);
the Zenodo deposit has numbers only for MST. No xlsx or csv with densitometry values was found.

## Quantified western blot panels (normalised to a loading control, ≥ 2 groups), in figure order

Not listed (normalised to the immunoprecipitated bait or primary-IP signal, which is neither a
housekeeping protein nor total protein): Fig. 3D, 3E (co-IP normalised to HA), 4A (to HA),
4B, 4C (to HIS), 5A, 5B, 5D (to HA), 5C (to primary IP); Fig. S4A (to HA), S4C (to HIS),
S5A (to eIF5A / ERK2 IP), S5B (to DHPS IP), S5C, S5D (to HA). None of these has any raw image in
the deposit, so each would fail criterion 2 if it were counted.

| Panel | Target / loading control | First failing criterion | Evidence |
|---|---|---|---|
| Fig. 5E | Deoxyhypusinated eIF5A / Ponceau S (total protein, same membrane); Control vs 100 µM ERK2 vs 100 µM BSA, 12 time points; triplicates | **7** | C2: all Fig. 5E TIFFs `raw = yes` (16-bit). C3: chemiluminescence images show the full cut strips, 12 lanes each, and Ponceau S images show the whole membrane with ladders. C4: legend "triplicates (A and D–F)"; Replicate I, II, III folders each contain all three conditions. C5: conditions from file name "from the top control ERK2 BSA" and per-condition Ponceau file names; replicate from folder; lane order = time points 0–60 min as in the figure. C6: per-condition, per-replicate Ponceau S image of the same membrane (methods: "Panceau S signal was used for chemiluminescent signal normalization"). C7 fails: the figure shows only mean ± SD per time point; no per-replicate values in the article, supplement or deposit. |
| Fig. 5F | Deoxyhypusinated eIF5A / Ponceau S; ERK2 0, 0.01, 0.1, 0.5, 1, 5, 10, 100 µM; triplicates; * at 100 µM | 7 | C2: TIFFs `raw = yes`. C3–C6: one chemiluminescence image with three strips "replicates I II III" from the top, 8 lanes each, and a Ponceau S image per replicate. C7 fails: the bar chart shows the three points per dose but no numbers are provided anywhere. |
| Fig. 6A | DHPS, eIF5A, eIF5A D/H / β-actin; Raf activation time course | 2 | No raw image of this panel in the deposit (deposit covers only Figs. 5E, 5F, S5E). |
| Fig. 6B | DHPS, eIF5A / β-actin; MEK1wt vs MEK1ca | 2 | No raw image in the deposit. |
| Fig. 6C | DHPS, eIF5A / β-actin; DMSO vs PLX4032 vs AZD6244 | 2 | No raw image in the deposit. |
| Fig. 6D | DHPS, eIF5A / β-actin; pLL3.7 vs shERK1 vs shERK2 | 2 | No raw image in the deposit. |
| Fig. 6E | DHPS, eIF5A / β-actin; Raf act. ± ERK inhibitors | 2 | No raw image in the deposit. |
| Fig. S5E | Deoxyhypusinated eIF5A / Ponceau S; ERK2 doses; triplicates | 7 | C2: TIFFs `raw = yes`; one chemiluminescence image of replicates I–III and a Ponceau S image per replicate. C7 fails: no per-replicate values anywhere. |
| Fig. S6A | DHPS, eIF5A / β-actin; BRAFwt vs BRAF V600E | 2 | No raw image in the deposit. |
| Fig. S6B | DHPS, eIF5A / β-actin; SK-MEL-28 DMSO vs PLX4032 vs AZD6244 | 2 | No raw image in the deposit. |
| Fig. S6D | DHPS, eIF5A / β-actin; shERK1 / shERK2 | 2 | No raw image in the deposit. |
| Fig. S6E | DHPS, eIF5A / β-actin; LNCaP shERK1/2 | 2 | No raw image in the deposit. |

## Decision

**Excluded.** No panel meets criteria 2–8. The first quantified panel in figure order, Fig. 5E,
fails **criterion 7**: the raw 16-bit images, Ponceau S loading controls, three replicates and
condition labels are all deposited, but the authors' quantification exists only as plotted
means ± SD, with no per-replicate numbers in the article, supplement or Zenodo deposit.

Note on panel order: if the co-IP panels normalised to the HA bait (Fig. 3D, first quantified
blot in the article) were counted as loading-control normalisations, the first quantified panel
would be Fig. 3D, which fails criterion 2 (no raw image deposited). The decision (excluded) is
the same either way.

No `fetch.sh` or `claim.yaml` was written because no panel qualified. `images/` and `archives/`
still hold the files downloaded by `screen_files.py`.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules applied: criterion 7
no longer excludes (recorded only); the total form of a modification-specific target counts
as a loading control when each lane can be matched; normalisation to an immunoprecipitated
bait is still not a loading control. Evidence above for criteria 2–6 and 8 is reused; only
the panel list and criterion 7 are revisited.

**Panel list under the revised rules.** No phospho / total panel is quantified anywhere in
the article: pERK1/2, pRSK and ERK1/2 appear only as unquantified input / IP rows (Figs. 3A,
3D, 3E, 5A–5D, 6E); in Fig. 3D the text mentions a higher pERK/ERK ratio in the IP fraction,
but the plotted densitometry is ERK1/2 co-IP normalised to HA (bait). The co-IP panels
(Figs. 3D, 3E, 4A–4C, 5A–5D, S4A, S4C, S5A–S5D) remain outside the panel list (bait
normalisation). Fig. 3B (thiophosphate in vitro kinase assay, biological duplicates) is not a
phospho / total normalisation and has no raw image in the deposit (it would fail criterion 2,
and criterion 4 with n = 2). So the first quantified panel in figure order is still Fig. 5E.

| Panel | Revised outcome | Evidence |
|---|---|---|
| Fig. 5E | **included** | C1: peer-reviewed, quantified. C2: all 21 Fig. 5E TIFFs `raw = yes` (2750 x 2200, 16 bit, 0 saturated px; re-run 2026-10-27). C3: chemiluminescence images show the full cut strips (12 lanes each) and Ponceau S images the whole membrane with ladders on both sides. C4: "triplicates (A and D–F)", Methods "All experiments were done in triplicates"; Replicate I, II, III folders each contain Control, ERK2 and BSA. C5: condition from file name "from the top control ERK2 BSA" and per-condition Ponceau / cutting-reference names; replicate from folder; time point of each lane from the figure's lane labels (0 … 60 min, left to right), no authors' numbers used. C6: Ponceau S of the same membrane per condition per replicate (total protein), 12 lanes in the same order. C7 (recorded, no longer excludes): **no per-replicate numbers** in article, supplement or deposit (only plotted mean ± SD). C8: lanes vertical in both chemiluminescence and Ponceau S images (checked on previews). |

Panels after Fig. 5E are not examined (stop at the first qualifying panel).

**Primary contrast:** 100 µM ERK2 vs Control (Results: "Western blot analysis of the reaction
product indicated that ERK2 can competitively inhibit the DHPS-mediated reaction (Figure
5E)"), direction down, no p-value or significance mark for Fig. 5E, n = 3 per group.

### Decision (rescreen)

**Included, Fig. 5E** (first-frame rescreen group, reported separately per Amendment 2
point 5). The only change from the first screen is criterion 7, which no longer excludes;
T1/T2 cannot be computed for this experiment (no per-replicate numbers).

Folder prepared:
* `fetch.sh` re-downloads everything with SHA-256 (tested in a scratch copy: all hashes OK,
  overview crop reproduced byte-identical).
* `article/`: `PMC11544350.xml` (text, legends, methods) and `Fig5E_overview.png` (crop of
  the Fig. 5E representative blot from page 10 of the article PDF: time and condition
  labels only, no graph).
* `images/`: the 21 Fig. 5E TIFFs (3 chemiluminescence, 9 Ponceau S, 9 colorimetric
  cutting-reference). The earlier `screen_files.py` download (a renamed copy of
  `I_BSA_Colorimetric...`) and `archives/` (whole zip, also Fig. 5F / S5E) were removed.
* `authors/`: publisher article PDF (`mmc2`) and supplement (`mmc1`), which print the plotted
  points, means and p-values. The PMC figure JPGs (`nihms-2031309-f000*.jpg`) printed plotted
  values and could not be re-downloaded by URL (PMC returns HTML), so they were deleted; the
  same figures are in `authors/…-mmc2.pdf`.
* `claim.yaml`.
