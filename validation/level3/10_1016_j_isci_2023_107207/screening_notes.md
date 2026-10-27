# Screening: 10.1016/j.isci.2023.107207

Szanda et al. (2023) "Cannabinoid receptor type 1 (CB1R) inhibits hypothalamic leptin
signaling via β-arrestin1 in complex with TC-PTP and STAT3", *iScience* 26:107207
(peer-reviewed research article; PMID 37534180, PMC10392084). Deposit: Mendeley Data
53jfh7rd2b v1 (doi:10.17632/53jfh7rd2b.1), 64 files in per-figure folders (Fig. 1-5 TIFFs and
one `Fig.N_datasets.xlsx` per figure). No files for the supplementary figures.

Sources read (in `article/`): Europe PMC full-text XML (`PMC10392084.xml`, text extract
`fulltext.txt`), figure legends (`figure_legends.txt`), figures 1-5 (`gr1.jpg` ... `gr5.jpg`,
Elsevier high-resolution), supplement Document S1 (`mmc1.pdf`, text extract
`mmc1_text.txt`), deposit metadata from the Mendeley API (`deposit_metadata.json`).

Methods (Western blotting): "the ratio of phosphorylated to total protein (p/t ratio) was
regarded as degree of activation". All STAT3 quantifications are pSTAT3 / total STAT3.
That is not normalisation to a loading control (housekeeping protein or total-protein
stain), so these panels are not experiments under the *Unit of analysis*.

## Western blot panels, in figure order

| Panel | Content | Result | Evidence |
|---|---|---|---|
| Fig. 1A | pSTAT3 / tSTAT3, WT and CB1R−/− mice, leptin ± ACEA | not an experiment | Quantified as p/t STAT3 (y-axis "p/t STAT3"; Methods), no loading-control normalisation. The screen's file `Fig.1_CB1KO_actin.tif` belongs to this blot, but actin is not used in the quantification. |
| Fig. 2A, 2B | pSTAT3 / tSTAT3, GT1-7 and N2a cells | not an experiment | p/t STAT3 dose-response curves. |
| Fig. 3B (quantified in Fig. S3A) | TC-PTP and PTP1B / actin, siRNA (2 sequences) or PTP1B siRNA vs non-silencing dsRNA; n = 6 per group | **criterion 4 failed** | See below. |
| Fig. 4A | β-arrestin1 and β-arrestin2 / actin, control dsRNA vs β-arr1 siRNA vs β-arr2 siRNA; n = 7-7-4 (β-arr1) and 24-18-4 (β-arr2); * p < 0.001 | criterion 4 failed | See below. |
| Fig. 4C | pSTAT3 / tSTAT3 (β-arr1 knockdown) | not an experiment | p/t STAT3. |
| Fig. 4D | β-arr1, β-arr2, vinculin in HEK293 | not quantified | "blot representative for 3 independent experiments". |
| Fig. 5C | pSTAT3 / tSTAT3, refeeding, WT and CB1R−/− | not an experiment | p/t STAT3. |
| Fig. 5D | β-arr1 / actin blot (left); pSTAT3 / tSTAT3 (middle, right) | not quantified / not an experiment | Left blot has no bar graph; right graph is p/t STAT3. |
| Fig. S1A, S1B, S1G, S1H | CB1R verification; pSTAT3 blots | not quantified | "representative". |
| Fig. S2A | p/t STAT3 vs translocation | not an experiment | p/t STAT3. |
| Fig. S2E | CB1R / β-actin, dsRNA vs cnr1 siRNA vs CB1R plasmid; n = 4-4-8 | criterion 2 failed | No raw image of this blot in the deposit (no supplementary-figure files). |
| Fig. S3A | quantification of the Fig. 3B blots | see Fig. 3B | Same experiment as Fig. 3B. |
| Fig. S4E | co-IP | not quantified | "representative". |

### Fig. 3B (first quantified panel) in detail

* Criterion 1: met. Legend of Fig. S3A: "Protein abundance was normalized to actin ...
  n=6 for all groups; *p<0.05 and **p<0.01 vs appropriate control non-silencing dsRNA".
* Criterion 2: met. `blotverse inspect` on `Fig.3b_TCPTP.tif`, `Fig.3b_PTP1b.tif`,
  `Fig.3b_actin.tif` (and their `_membrane` images): all 16-bit, effective 16 bits,
  0 saturated px, `raw = yes`.
* Criterion 3: met for the deposited membrane (whole membrane with edges, ladder and all
  12 lanes in `Fig.3b_TCPTP_membrane.tif`).
* Criterion 4: **failed.** The authors state n = 6 per group. The deposit holds one
  membrane per antibody with 12 lanes, 2 per group (sequence 1 dsRNA/siRNA, sequence 2
  dsRNA/siRNA, PTP1B dsRNA/siRNA, as in Fig. 3B). Four of the six replicates per group are
  not deposited.
* Criteria 5-8 not evaluated.

### Fig. 4A in detail

Relevant if Fig. 3B is counted as a supplementary panel (its bar graph is in Fig. S3A);
the outcome is the same.

* Criteria 1-3 met: legend "β-arrestin to actin protein abundance was normalized to that in
  the control dsRNA treated group ... ∗p < 0.001 vs. control dsRNA"; `blotverse inspect` on
  `Fig.4a_Barr1.tif`, `Fig.4a_Barr1_actin.tif`, `Fig.4a_Barr2.tif`, `Fig.4a_Barr2_actin.tif`
  and the `_membran` images: all 16-bit, 0 saturated px, `raw = yes`; one 12-lane membrane
  shown in full.
* Criterion 4: **failed.** n = 7-7-4 (β-arr1 : actin) and 24-18-4 (β-arr2 : actin). The
  deposit holds a single 12-lane membrane (3 dsRNA + 3 β-arr1 siRNA + 3 dsRNA + 3 β-arr2
  siRNA, as in Fig. 4A). For β-arr1 : actin, only 3 of the 7 β-arr1 siRNA replicates and at
  most 6 of the 7 controls are present. For β-arr2 : actin, at most 6 of the 24 controls are
  present.

## Decision

**Excluded. First failing criterion: 4** (Fig. 3B / S3A, the first panel normalised to a
loading control: n = 6 per group stated, but the deposit holds one representative membrane
with 2 lanes per group). Fig. 4A fails the same way.

No `claim.yaml` or `fetch.sh` written (no qualifying panel). `images/` holds the two files
placed by `screen_files.py` (`Fig.1_CB1KO_actin.tif`, `Fig.1_CB1KO_actin_comp.tif`). Fig. 3B
and 4A images were downloaded to a scratch directory only. No file with the authors'
numbers (`Fig.N_datasets.xlsx`) was downloaded or opened.
