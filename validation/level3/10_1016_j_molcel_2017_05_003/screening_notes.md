# Screening notes: 10.1016/j.molcel.2017.05.003

Jaco I, Annibaldi A, Lalaoui N, Wilson R, Tenev T, Laurien L, Kim C, Jamal K, et al. (2017),
"MK2 Phosphorylates RIPK1 to Prevent TNF-Induced Cell Death", Molecular Cell 66(5):698-710.e5.
PMID 28506461, PMC5459754 (open access).
Deposit: Mendeley Data znt3g8r753 v1, "MK2 phosphorylates RIPK1 to prevent TNF induced cell
death. Jaco et al", published 2017-05-12: 34 zip files, one per figure panel (Figs 2B-5E,
S2-S6C; 6D = confocal images).

Screened 2026-10-27, second frame, under the brief with the Amendment 2 addition (criterion 7
does not exclude; phospho / total accepted when each lane can be matched; normalisation to an
immunoprecipitated bait is still not a loading control).

## Western blot panels and how they are quantified

Sources: Europe PMC full-text XML (all main legends, Methods), Fig. 4 and Fig. 5 images, and
Document S1 (Figures S1-S6 with legends), all in `article/`.

Almost every western blot panel (Figs 2B-G, 3A-E, 4A-C, 4E-F, 5A-B, 5E, S2-S6) is shown as a
representative blot and is **not quantified**. The Methods' only quantification statement is
for PI uptake by flow cytometry. Every graph with statistics (Figs 1, 5D, 6, S1) is a
cell-death, DEVDase or PLA assay. Two western blot panels carry a quantification:

| Panel | Quantification | Fits the unit of analysis? |
|---|---|---|
| **Fig 4D** | RIPK1 immunoprecipitated from WT and *Mk2*-/- MEFs ± TNF; "Quantification of the intensity of the P-S166 signal, normalized to total RIPK1" (one bar per lane, no error bars, no n, no statistics) | **No.** The reference band is total RIPK1 **in the RIPK1 immunoprecipitate**, i.e. the IP bait. Amendment 2 accepts phospho / total as a loading control only for lysate blots; "normalisation to an immunoprecipitated bait (co-IP) is still not a loading control". Same reading as 10.1038/s44318-025-00560-8 (p-paxillin normalised to IP'd paxillin). |
| Fig 5C | TUBE affinity purification ± USP21; "quantification of non-modified RIPK1 in the USP21-treated samples", y-axis "Ub-RIPK1 (Densitometry)", raw densitometry per lane | **No.** Not normalised to any loading control (only RIPK1 and ubiquitin are blotted); one lane per condition, no replicates. |

No panel fits *Unit of analysis* (target normalised to a housekeeping protein, total protein,
or, under Amendment 2, the total form of a phospho target in matched lanes, with >= 2 groups
compared). Criterion 1 therefore fails: there is no western blot panel quantified by the
authors in the sense of the protocol. This follows the precedent of 10.7554/elife.91083 in
`screening.md` (excluded, criterion 1, "no panel fits the unit of analysis").

## Panels examined (recorded although the candidate already fails criterion 1)

`build/apps/blotverse inspect` on the deposit files (zips from the Mendeley API; SHA-256 of
the zips match the record):

| Panel | First failing criterion | Evidence |
|---|---|---|
| **Fig 4D** | **1 (unit of analysis: IP-bait normalisation)** | Also: `Fig4D_P_RIPK1 S166.tif` colour (pseudo-coloured), `Fig4D_RIPK1.tif` 8-bit, so criterion 2 fails as well; the blot is a single replicate (criterion 4). |
| Fig 5C | 1 (no loading control) | `Fig5C_RIPK1 WB.tif`, `Fig5C_Ubiqutin WB.tif` are 16-bit, raw = yes, but there is no loading-control band and one lane per condition. |

For reference, the files the file screen put in `images/` (Figs 2B, 2D, 2E, 3B: unquantified
representative blots) are mostly 8-bit, JPEG or colour; only `Fig2B_pMK2.tif` and
`3B_P_p38 WB.tif` are 16-bit.

Criterion 7 (not excluding): the deposit holds images only; no per-replicate numbers exist
for Fig 4D or 5C (single bars without n).

## Decision

**Excluded, criterion 1 (Fig 4D).** The only quantified western blots are Fig 4D (phospho
RIPK1 normalised to the immunoprecipitated RIPK1 bait) and Fig 5C (unnormalised densitometry
of a TUBE pull-down). Neither is normalised to a loading control.

## Files

* `article/molcel-2017-05-003-PMC5459754.xml`: Europe PMC full text.
* `article/molcel-2017-05-003-mmc1-FiguresS1-S6.pdf`: Document S1 (supplementary figures and
  legends), from the Europe PMC supplementary-files endpoint.
* `article/fig4.jpg`, `article/fig5.jpg`: main Figs 4 and 5 (same source). The bar charts in
  4D and 5C print no values.
* `images/`, `archives/`: left as downloaded by `screen_files.py` (not curated, candidate
  excluded). The Fig 4D and 5C zips were downloaded to a scratch directory only.
* No `fetch.sh` or `claim.yaml` (excluded). No file with the authors' numbers was saved.
* No API that needs an e-mail address was used (Europe PMC, NCBI E-utilities, Mendeley
  public API, OpenAlex without a mailto parameter).
