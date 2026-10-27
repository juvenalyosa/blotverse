# Screening notes: 10.1038/s44318-025-00587-x

Article: Coulthurst lab et al., "A widely-occurring family of pore-forming effectors broadens
the impact of the Serratia Type VI secretion system", The EMBO Journal (2025), PMID 41120787,
PMC12669606. Peer-reviewed journal article (not a preprint).

Deposit: S-SCDT-10_1038-S44318-025-00587-X (EMBO SourceData, BioStudies).

## Sources examined (`article/`)

* `PMC12669606.xml`: Europe PMC full text with all figure legends (Fig. 1-8, EV1-EV3) and Methods.
* `44318_2025_587_Fig1_HTML.png`: Figure 1 (the only figure that contains an immunoblot).
* `44318_2025_587_MOESM1_ESM.pdf`: Appendix (Appendix Fig. S1-S7, Table S1).
* `44318_2025_587_MOESM13_ESM.pdf`: Expanded View figures (EV1-EV3).

## Western blot panels in figure order

The full text and supplements contain one western blot panel: Fig. 1G. "Immunoblot" appears
only in its legend, and the Methods describe anti-FLAG blotting only for that assay.
No main, EV or Appendix figure shows a western blot quantified against a loading control.
Appendix Fig. S3 shows protein purification/size-exclusion, not a quantified western blot.

| Panel | Criterion failed | Evidence |
|---|---|---|
| Fig. 1G (Sip4-FLAG mPEG-MAL cysteine accessibility, anti-FLAG; deposit files `VC_`, `No_Cys_`, `S18C_and_G43C_`, `G81C_`, `G145C_blot_(anti-FLAG).tif`, `Annotated_blots.png`) | 1 | Qualitative topology mapping: the legend says only that "Sip4-FLAG and Sip4-FLAG-mPEG-MAL species were detected by immunoblotting". The figure has no quantification, no bar chart, no loading control, no statistics and one lane per condition. The text draws a qualitative conclusion from band shift presence or absence ("were only labelled with mPEG-Mal in the presence of SDS"). The source data have no Fig. 1G table (Fig. 1 source data: `Figure_1D.xlsx` growth curves only). |

The other deposit images (`151019_AO03vsAO07-09_*.tiff`) are Fig. 1E time-lapse microscopy
frames, not blots. The file screen had already found them to be 8-bit or pseudo-coloured.

## Decision

**Excluded.** First failing criterion: **1**. The article has no western blot panel
quantified by the authors. Its only immunoblot (Fig. 1G) is a qualitative mPEG-MAL
band-shift topology assay with no normalisation to a loading control and no group
comparison.

Because no panel qualifies, `fetch.sh` and `claim.yaml` were not written. `images/` holds the
files that `screen_files.py` downloaded earlier and was left unchanged.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules: criterion 7 no longer
excludes; the total form of the target counts as a loading control for a modification-specific
target (phospho / total) when each lane can be matched; co-IP bait normalisation still does not.

The full text (`article/PMC12669606.xml`) was searched again: "phospho" occurs only as
"phospholipids"; there is no phospho-specific antibody, no total-protein reference, no
co-immunoprecipitation and no densitometry. The only immunoblot remains Fig. 1G, a qualitative
anti-FLAG mPEG-MAL band-shift assay with one lane per condition and no quantification. The
revised rules concern loading-control type and criterion 7; they do not create a quantified
western blot panel, so criterion 1 still fails.

| Panel | First failing criterion (Amendment 2) | Evidence |
|---|---|---|
| Fig. 1G (Sip4-FLAG mPEG-MAL, anti-FLAG) | 1 | unchanged: not quantified, no loading control, no group comparison |

Criterion 7: not applicable (no quantified western blot; Fig. 1 source data hold only the
Fig. 1D growth curves).

**Decision under Amendment 2: excluded**, unchanged. First failing criterion: **1**. No
`fetch.sh` or `claim.yaml` written.
