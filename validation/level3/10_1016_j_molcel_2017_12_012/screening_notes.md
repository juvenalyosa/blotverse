# Screening notes: 10.1016/j.molcel.2017.12.012

Neal S, Jaeger PA, Duttke SH, Benner C, Glass CK, Ideker T, Hampton RY (2018), "The Dfm1
Derlin Is Required for ERAD Retrotranslocation of Integral Membrane Proteins", Molecular Cell
69(2):306-320. PMID 29351849, PMC6049073 (NIH author manuscript NIHMS928720).
Deposit: Mendeley Data ym9mtgmrwh v1, "The Dfm1 derlin is required for ERAD retrotranslocation
of integral membrane proteins, Neal et al", published 2018-02-23, description "Western blot
data of biochemical studies done on Dfm1". 52 single image files, named by figure panel.

Screened 2026-10-27, second frame, under the brief with the Amendment 2 addition (criterion 7
does not exclude; phospho / total accepted; co-IP bait is not a loading control).

## Criterion 1

Peer-reviewed article (Mol Cell), not a preprint. Western blots are quantified by the authors:
Methods, "ImageJ (NIH) was used for all western blot quantifications ... a representative
western blot was shown and band intensities were normalized to PGK1 loading control and
quantified." Passes. Text and legends: `article/molcel-2017-12-012-PMC6049073.xml` (PMC XML via
NCBI E-utilities; Europe PMC has no full text for this manuscript).

## Quantified western blot panels (figure order)

Panels whose legend states normalisation of a target band to a loading control (PGK1) and
compares >= 2 groups (strains), all cycloheximide chases:

| Panel | Legend | Deposit files |
|---|---|---|
| **Fig 1C** | SUS-GFP chase in WT, *ubc7*Δ, *cdc48-2*; "normalized to PGK1 loading control", >= 3 experiments | `Fig1C.tiff`, `Fig1C_overexp..tiff` |
| Fig 2A | SUS-GFP chase, WT vs *dfm1*Δ; (A-B) normalised to PGK1 | `Fig2A_GFPblot.tiff` |
| Fig 2B | Hmg2-GFP chase, WT vs *dfm1*Δ | `Fig2B_PGKblot.tiff` (no GFP file) |
| Fig 3A, 3B, 3C, 3E, 3H | ERAD-M/-C/-L substrate chases; (A-C, E, H) normalised to PGK1 | `Fig3A_HAblot`, `Fig3A_PGKblot`, `Fig3B_GFPblot`, `Fig3B_PGKblot`, `Fig3C_PGK blot.tif` (no target file), `Fig3E_HAblot.tiff` + `Fig3E_PGKblot.png`, `Fig3H_MYCblot.tiff` + `Fig3H_PGKblot.png` |
| Fig 4C, 4E | Dfm1 variants / Hmg2-GFP chases; (C, E) normalised to PGK1 | `Fig4C_HAblot.tiff` + `Fig4C_PGKblot.png`; `Fig4E_PGKblot.png` (no GFP file) |
| Fig 5B, 5D | Dfm1 variants / Hmg2-GFP chases; (B, D) normalised to PGK1 | `Fig5B_HAblot.tiff` + `Fig5B_PGK blot.tif`; `Fig5D_GFPblot.png` + `Fig5D_PGKblot.tif` |
| Supplementary S1B, S4C | (supplement not opened; deposit files named by panel) | `FigS1B_GFPblot.tif` + `FigS1B_PGK blot.tif`; `FigS4C_HAblot.tiff` + `FigS4C_PGKblot.tif` |

Not experiments under *Unit of analysis*: Fig 4B/4D, 5C, 6E (Cdc48 "percentage ... bound to
pellet fraction", a fraction ratio, not normalised to a loading control); Figs 1D, 2C-F, 3D,
3I, 4F, 5E, 6D (ubiquitin / IP retrotranslocation blots, not quantified); Fig 6B, 6C, 6G (the
legend states no quantification or normalisation; 6G, `Fig6G_PGK_Mycblot.tif`, is 16-bit but
holds one membrane of 9 lanes, i.e. a single replicate, so it would fail criterion 4 anyway).

## Panels examined

`build/apps/blotverse inspect` on the panel files (SHA-256 of every file matches the Mendeley
API record):

| Panel | First failing criterion | Evidence |
|---|---|---|
| **Fig 1C** | **2** | `Fig1C.tiff` and `Fig1C_overexp..tiff`: raw = no, "rejected: colour (pseudo-coloured) image". No greyscale acquisition of Fig 1C is deposited. |
| Fig 2A | 2 | `Fig2A_GFPblot.tiff`: colour (pseudo-coloured), raw = no |
| Fig 2B | 2 | `Fig2B_PGKblot.tiff`: colour, raw = no; no target file |
| Fig 3A | 2 | `Fig3A_HAblot.tiff` 8-bit; `Fig3A_PGKblot.tiff` colour |
| Fig 3B | 2 | `Fig3B_GFPblot.tiff`, `Fig3B_PGKblot.tiff`: colour |
| Fig 3C | 2 | only `Fig3C_PGK blot.tif` (16-bit, raw = yes) is deposited; no target image |
| Fig 3E | 2 | `Fig3E_HAblot.tiff` colour; `Fig3E_PGKblot.png` not a TIFF |
| Fig 3H | 2 | `Fig3H_MYCblot.tiff` colour; `Fig3H_PGKblot.png` not a TIFF |
| Fig 4C | 2 | `Fig4C_HAblot.tiff` 8-bit; `Fig4C_PGKblot.png` not a TIFF |
| Fig 4E | 2 | only `Fig4E_PGKblot.png` (not a TIFF); no target file |
| Fig 5B | 2 | `Fig5B_HAblot.tiff` 8-bit (PGK 16-bit) |
| Fig 5D | 2 | `Fig5D_GFPblot.png` not a TIFF (PGK 16-bit) |
| Fig S1B | 2 | `FigS1B_GFPblot.tif` 8-bit (PGK 16-bit) |
| Fig S4C | 2 | `FigS4C_HAblot.tiff` 8-bit (PGK 16-bit) |

Criterion 7 (not excluding): the deposit holds images only; the supplementary tables are
strain lists and screen hits. No per-replicate numbers were found.

## Decision

**Excluded, criterion 2 (Fig 1C).** No quantified panel has a raw greyscale 16-bit image of
both the target and the loading control.

## Files

* `article/molcel-2017-12-012-PMC6049073.xml`: PMC full text (text, methods, legends).
* `images/`: left as downloaded by `screen_files.py` (not curated, candidate excluded). The
  other panel files examined above were downloaded to a scratch directory only.
* No `fetch.sh` or `claim.yaml` (excluded). No file with the authors' numbers was saved.
* No API that needs an e-mail address was used or skipped (NCBI E-utilities, Europe PMC, Mendeley public API).
