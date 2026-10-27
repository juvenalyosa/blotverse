# Screening notes: 10.1038/s44321-025-00336-x

* Article: Nebeyu et al., "Inhibiting cholesterol synthesis halts rhabdomyosarcoma growth via
  ER stress and cell cycle arrest", EMBO Molecular Medicine (2025), PMID 41249736,
  PMC12686467. Peer-reviewed journal article (not a preprint).
* Deposit: EMBO SourceData S-SCDT-10_1038-S44321-025-00336-X (released 2025-11-14).
* Article saved: `article/europepmc_PMC12686467.xml` (Europe PMC full text with all
  legends), `article/Fig8_HTML.jpg` (Figure EV4). The PDF and the other figure JPEGs could
  not be fetched (publisher/PMC returned HTML bot-check pages); not needed for the decision.

## Criterion 1

Met. Western blot panels quantified by the authors exist: Figure 4J (numbers = relative
expression normalised to vinculin), Figure 5H (quantified, mean ± SEM, N = 3, t-tests) and
Figure EV4F (numbers normalised to vinculin).

## Quantified western blot panels, figure order

| Panel | Criterion failed | Evidence |
|---|---|---|
| Fig. 4J (DHCR7, Cyclin D1 / vinculin; shSCR vs shDHCR7 in RD, KLHEL1, RH30) | **2** | Deposit section f3_p9 files `RD_DHCR7.tif.tif.tif`, `RD_vincuin.tif.tif`, `KLHEL1_DHCR7.tif`, `KLHEL1_vincuin.tif.tif`, `RH30_DHCR7.tif`, `RH30_Cyclin_D1.tif`, `RH30_vincuin.tif`: `blotverse inspect` raw = no for all, "rejected: colour (pseudo-coloured) image". No other image files for this panel. |
| Fig. 5H (PERK, p-eIF2α, eIF2α, ATF4, CHOP; shSCR vs shDHCR7, RD/KLHEL1; stain-free total protein) | 2 | Deposit section f4_p7: every target image is 8-bit (`280824_blot1_perk_1.tif`, `280824_blot1_eif2alpha_6.tif`, `280824_bpot1alaviiva_peif2alpha_4.tif`, `280824_blot2alaviiva_CHOPrewash_2.tif`, `280824_blot2_ATF-4_2.tif`, `090424_blot1_PERK_4(Chemiluminescence).tif`, `090424_blot1_eIF2Alpha_2(Chemiluminescence).tif`, `100424_blot1_p-eIF2Alpha(Chemiluminescence).tif`, `090424_blot1_ATF-4_1(Chemiluminescence).tif`, `090424_blot1_CHOP_2(Chemiluminescence).tif`): raw = no, "8-bit file". Only the loading-control stain-free gel `090424gel1_rhabdo(Stain_Free_Gel).raw16.tif` (and its duplicate `..._1.tif`) is 16-bit, raw = yes; the target bands are not. |
| Fig. EV4F (DHCR7, HMGCR / vinculin; shSCR vs shDHCR7, RD/KLHEL1) | 2 | No image files deposited for any EV figure (deposit sections cover main figures only). |

The blot-named 16-bit file flagged by the file screen,
`090424gel1_rhabdo(Stain_Free_Gel).raw16.tif`, is the stain-free total-protein gel of
Fig. 5H; it is a loading-control image only, so it does not make the panel's raw acquisition
readable for the target.

## Decision

**Excluded.** First failing criterion of the candidate: **2**, at its first quantified panel
(Fig. 4J): the deposited blot images are pseudo-coloured exports rejected by
`blotverse inspect`.

No `fetch.sh` or `claim.yaml` written (no panel qualifies).

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules applied: criterion 7
no longer excludes; the total form of the target counts as a loading control for a
modification-specific target when each lane can be matched; normalisation to an
immunoprecipitated bait is still not a loading control.

The figure legends in `article/europepmc_PMC12686467.xml` were searched again for western
blot quantification, including phospho / total. The quantified western panels in figure
order are the same three: Fig. 4J, Fig. EV4F and Fig. 5H (main figures first: 4J, 5H; then
EV4F). Fig. 5H is the only panel with a phospho / total pair (p-eIF2α / eIF2α). No other
panel becomes eligible under the revised loading-control rule.

| Panel | Criterion failed (Amendment 2) | Evidence |
|---|---|---|
| Fig. 4J (DHCR7, Cyclin D1 / vinculin) | 2 | Unchanged: all deposited files (`RD_DHCR7.tif.tif.tif`, `RD_vincuin.tif.tif`, `KLHEL1_DHCR7.tif`, `KLHEL1_vincuin.tif.tif`, `RH30_*`) rejected by `blotverse inspect` as pseudo-coloured. Criterion 2 precedes the changed criteria 6 and 7. |
| Fig. 5H (p-eIF2α / eIF2α, PERK, ATF4, CHOP; stain-free) | 2 | eIF2α now counts as a loading control for p-eIF2α (criterion 6), but every target image, phospho and total alike (`280824_bpot1alaviiva_peif2alpha_4.tif`, `100424_blot1_p-eIF2Alpha(Chemiluminescence).tif`, `280824_blot1_eif2alpha_6.tif`, `090424_blot1_eIF2Alpha_2(Chemiluminescence).tif`, ...), is 8-bit: raw = no. Only the stain-free gel is 16-bit. |
| Fig. EV4F (DHCR7, HMGCR / vinculin) | 2 | Unchanged: no image files deposited for EV figures. |

Per-replicate numbers (criterion 7, now recorded only) were not re-examined, because every
panel fails criterion 2 first.

**Decision unchanged: excluded, first failing criterion 2 (Fig. 4J).** No `fetch.sh` or
`claim.yaml` written.
