# Screening notes: 10.1038/s44321-026-00479-5

Article: Vagner lab (Fabbri et al.), "Selective mRNA translation determines adaptative
mutability of melanoma cells to anti-BRAF/MEK combination therapy", EMBO Molecular Medicine
(2026), PMID 42477455, PMC13470486. Peer-reviewed journal article (not a preprint).

Deposit: S-SCDT-10_1038-S44321-026-00479-5 (EMBO SourceData, BioStudies). It covers the main
figures only (sections f0-f6 = Fig. 1-7). The Expanded View source data were taken from the
publisher's ESM zips (MOESM12 = Fig. EV1, MOESM16 = Fig. EV5, MOESM19 = Fig. EV8), opened in
the scratchpad only.

## Sources examined (`article/`)

* `PMC13470486.xml`: Europe PMC full text with all figure legends (Fig. 1-7, EV1-EV9).
* Deposit overview JPEGs `Western_A375_M249_gH2AX.jpg`, `Western_A375_M249_53BP1.jpg`
  (looked at, not kept).

## Western blot panels quantified against a loading control, in figure order

| Panel | Criterion failed | Evidence |
|---|---|---|
| Fig. 1D (γH2AX / GAPDH or Tubulin, A375 and M249; DMSO, BRAFi/MEKi, BRAFi/MEKi rel; n = 3) | 2 | Deposit files `gH2AX_M249.Tif`, `GAPDH_M249_.tif`: `blotverse inspect` raw = no, "colour (pseudo-coloured) image". A375 files `h2ax.jpg`, `tub.jpg` and overview `Western_A375_M249_gH2AX.jpg` are JPEG (rejected). No greyscale acquisition deposited. |
| Fig. 1E (53BP1 / GAPDH, A375 and M249; DMSO, BRAFi/MEKi, BRAFi/MEKi rel; n = 3) | 2 | M249 files `M249__53BP1.Tif`, `M249_GAPDH.Tif`: raw = no (pseudo-coloured RGB). A375 files `53bp1_membrane_2.Tif` and `gapdh_membrane_2.Tif` are 16-bit greyscale (raw = yes; GAPDH has 923 saturated px). Even for the A375 half alone, criterion 4 fails: the deposit has one A375 membrane (11 lanes), on which the overview marks only the last 3 lanes as DMSO / BRAFi/MEKi / BRAFi/MEKi rel; the other 8 lanes are unlabelled and no other A375 replicate membranes are deposited, whereas the legend states three independent experiments. |
| Fig. 3F (53BP1 / GAPDH, A375 WT and Δ5'UTR clones 1.3, 1.7, 1.11; DMSO vs BRAFi/MEKi; n = 4) | 2 | `3F_Western_blot.tiff`, `53bp1.Tif`, `53bp1_1.Tif` (identical SHA-256 to `53bp1.Tif`), `53bp1_2.Tif`, `gapdh.Tif`, `gapdh_1.Tif`, `gapdh_2.Tif`: all raw = no (pseudo-coloured). |
| Fig. 5A/B (53BP1 / GAPDH, A375 DMSO vs BRAFi/MEKi with 0-30 nM eFT226; n = 4) | 2 | `Western_eFT226.tiff`, `53bp1_.Tif`, `GAPDH.Tif`: all raw = no (pseudo-coloured). |
| Fig. EV1B (53BP1, XRCC4, KU70, RAD51, LIG3 / GAPDH, A375 DMSO vs BRAFi/MEKi; n = 3) | 2 | Only image in ESM zip: `EV1B_western.tiff`, raw = no (pseudo-coloured). |
| Fig. EV5B (53BP1, M249 DMSO/BRAFi/MEKi with eFT226; n = 3) | 2 | ESM zip contains only `EV5B_Western_M249_eFT226.xlsx`; no blot image deposited. |
| Fig. EV8B (Cyclin D1 / Tubulin, A375 with RBX0901 or eFT226; n = 3) | 2 | `20230901 A375 H358 CD1 2.Tif`, `20230901 A375 H358 TUB 2.Tif`: raw = no (pseudo-coloured). |

Not listed: Fig. EV4C/D (western blots shown as siRNA knockdown controls with no
quantification or statistics).

## Decision

**Excluded.** First failing criterion: **2**, on the first quantified panel (Fig. 1D). Its
deposited images are pseudo-coloured RGB TIFFs and JPEGs. Every later quantified western
blot panel also fails, and none qualifies. The only 16-bit greyscale blot files
(`53bp1_membrane_2.Tif`, `gapdh_membrane_2.Tif`, A375 half of Fig. 1E) hold one replicate
membrane out of three.

Because no panel qualifies, `fetch.sh` and `claim.yaml` were not written. `images/` holds the
files that `screen_files.py` downloaded earlier. Files downloaded during this screening were
removed, and no file with the authors' numbers is kept in this folder.
