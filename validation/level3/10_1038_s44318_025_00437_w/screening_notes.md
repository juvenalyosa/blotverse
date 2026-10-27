# Screening: 10.1038/s44318-025-00437-w

Dudziak et al. (2025) "Spc105/Kre28 promotes mitotic error correction by outer kinetochore
recruitment of Ipl1/Sli15", *The EMBO Journal* (peer-reviewed; PMID 40281358, PMC12170873).
Deposit: EMBO SourceData S-SCDT-10_1038-S44318-025-00437-W.

Sources read (in `article/`): Europe PMC full-text XML (`PMC12170873.xml`, flattened in
`fulltext.txt`), all figure legends (`figure_legends.txt`), deposit file list
(`deposit_filelist.tsv`, `deposit_full.json`).

## Western blot panels and quantification

Every western blot in the article is shown as a representative image only; none is
quantified (no band normalisation to a loading control, no densitometry, no bar chart, no
statistics). The text uses the blots qualitatively ("Western blotting confirmed that
wild-type and mutant proteins were expressed at comparable levels (Fig. 1D)"). The article's
quantifications (Fig. 2E, 3A-B, 4D, 7G, EV2D-E, EV3C) are of fluorescence microscopy,
chromosome-segregation scoring or an autoradiograph (kinase assay), not western blots.

| Panel | Content | Quantified by authors? |
|---|---|---|
| Fig. 1D | anti-Flag Kre28 WT/ΔZwint/ΔRWD, Pgk1 loading control (`Blot_antiFlag.tif`, `Blot_antiPGK1.tif`) | no: representative blot, expression "comparable" |
| Fig. 2A | Pds1-myc time course after α-factor release (WT, ΔZwint, mad1Δ), Pgk1 | no |
| Fig. 2C | Pds1-myc time course after nocodazole release, Pgk1 | no |
| Fig. 4B | anti-HA Mtw1-Sli15 fusions, anti-Flag Kre28, Pgk1 | no |
| Fig. 5C-E, 7D, EV3A | in vitro pull-downs, anti-His (Sli15) | no (Fig. 5E only "judged by protein band intensity") |
| Fig. 5F | TurboID streptavidin pull-down, anti-Flag/Strep | no |
| EV1B, EV2A-C, EV5C | expression checks of mutants, Pgk1 | no |

No panel meets the unit-of-analysis definition (target normalised to a loading control,
≥ 2 groups compared, quantification and conclusion reported), so no panel was examined
under criteria 2-8.

Note: `blotverse inspect` had already accepted `images/Blot_antiFlag.tif` as raw
(`criterion2_blot.tsv`), but criterion 2 is not reached.

## Decision

**Excluded. First failing criterion: 1** (peer-reviewed article, but no western blot panel
quantified by the authors).

No `claim.yaml` or `fetch.sh` written (no qualifying panel). `images/` holds the two files
downloaded by `screen_files.py` (`Blot_antiFlag.tif`, Fig. 1D; and
`WholeMount_Dissection_Kre28_deltaRWD.tif`, Fig. 1C tetrad plate). No file with the
authors' numbers was downloaded.

## Rescreen under Amendment 2

Rescreened on 2026-10-27 under Amendment 2, point 5 (criterion 7 no longer excludes; the
total form of a modification-specific target counts as a loading control when each lane can
be matched; normalisation to an immunoprecipitated bait is still not a loading control).
The evidence above is reused; neither revised rule touches it.

* **Phospho / total panels.** None. No western blot uses a phospho-specific antibody. The
  phosphorylation readouts (Ipl1 autophosphorylation and KMN substrates, Fig. EV3B-C) are
  kinase-assay autoradiographs, and the Ipl1/Sli15 mobility shift in Fig. 5E is a
  pull-down "judged by protein band intensity", with no band normalised to a total form or
  to a loading control.
* Every western blot remains a representative image (expression checks against Pgk1,
  time courses, pull-downs), with no densitometry, normalisation or statistics. The revised
  rules change what counts as a loading control and drop criterion 7, but do not create a
  quantified western blot panel.

**Decision under Amendment 2: excluded. First failing criterion: 1** (no western blot panel
quantified by the authors). Unchanged. No `fetch.sh` or `claim.yaml` written.
