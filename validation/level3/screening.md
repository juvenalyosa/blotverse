# Level 3: screening of the sampling frame

One row per candidate of [`candidates.csv`](candidates.csv), in screening order. "Criterion"
is the first inclusion criterion of [`docs/level3_protocol.md`](../../docs/level3_protocol.md)
that the candidate fails.

**How the criteria were checked.**

* **Criterion 2 first, for every candidate.** [`screen_files.py`](screen_files.py) listed each
  deposit and ran `blotverse inspect` on its image files ([`criterion2.tsv`](criterion2.tsv)).
  A candidate with no raw image at all fails criterion 2 whatever its article says, so
  criterion 1 was not assessed for these 193 candidates.
* **Raw images that are not blots.** For the 42 candidates with a raw image, a rescreen counted
  only files named as blots ([`criterion2_blot.tsv`](criterion2_blot.tsv)). The candidates
  without one were checked visually by an agent ([`criterion2_visual.tsv`](criterion2_visual.tsv)):
  their raw images are microscopy, EM or brightfield, and they fail criterion 2.
* **Criterion 1 without an agent.** Preprints and other non-peer-reviewed DOIs were
  excluded from the DOI alone.
* **Screening agents.** Every other candidate was screened by an agent following
  [`screening_brief.md`](screening_brief.md). Each agent's evidence is in
  `<doi-slug>/screening_notes.md`.

**Loading control.** The protocol accepts a housekeeping protein or total protein as the
loading control. Panels normalised to the total form of the same protein (phospho/total) or to
an immunoprecipitated bait (co-IP) are therefore not experiments under the protocol. Several
candidates had such panels quantified (ranks 19, 84, 87, 95, 209; rank 87 had no other); this is reported as a
limitation.

**Yield:** 1 included, 234 excluded of 235 candidates.

| Rank | DOI | Repository | Decision | Criterion | Panel | Reason |
|---|---|---|---|---|---|---|
| 1 | 10.1038/s44319-025-00651-x | EMBO SourceData | excluded | 2 | — | no raw image among 90 TIFF/.scn/.gel files |
| 2 | 10.1152/ajpendo.00385.2025 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 3 | 10.1038/s44318-025-00607-w | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 4 | 10.1038/s44318-026-00885-y | EMBO SourceData | excluded | 2 | — | no raw image among 38 TIFF/.scn/.gel files |
| 5 | 10.1038/s44321-025-00302-7 | EMBO SourceData | excluded | 2 | — | no raw image among 134 TIFF/.scn/.gel files |
| 6 | 10.1038/s44321-025-00244-0 | EMBO SourceData | excluded | 2 | — | no raw image among 9 TIFF/.scn/.gel files |
| 7 | 10.1093/cei/uxad092 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 8 | 10.3344/kjp.26061 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 9 | 10.1101/2022.07.02.498543v1 | Mendeley Data | excluded | 2 | — | no raw image among 3 TIFF/.scn/.gel files |
| 10 | 10.1186/s12974-023-02739-4 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 11 | 10.1038/s44321-026-00423-7 | EMBO SourceData | excluded | 2 | — | no raw image among 42 TIFF/.scn/.gel files |
| 12 | 10.1080/22221751.2025.2544720 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 13 | 10.7554/elife.77987 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 14 | 10.1038/s44321-025-00247-x | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 15 | 10.1038/s44319-026-00861-x | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 16 | 10.1038/s44320-025-00172-8 | EMBO SourceData | excluded | 2 | — | no raw image among 49 TIFF/.scn/.gel files |
| 17 | 10.1038/s44319-026-00815-3 | EMBO SourceData | excluded | 2 | — | no raw image among 222 TIFF/.scn/.gel files |
| 18 | 10.2139/ssrn.3634264 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 19 | 10.7554/elife.88206 | Harvard Dataverse | excluded | 4 | Fig. 5—S1A | n = 3 stated; the deposited CDKL5/tubulin membranes identify one replicate set only |
| 20 | 10.1038/s44319-025-00578-3 | EMBO SourceData | excluded | 2 | — | no raw image among 21 TIFF/.scn/.gel files |
| 21 | 10.1126/science.adm9805 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 22 | 10.1038/s44318-025-00599-7 | EMBO SourceData | excluded | 2 | — | no raw image among 65 TIFF/.scn/.gel files |
| 23 | 10.1016/j.biochi.2019.07.020 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 24 | 10.1016/j.celrep.2020.107715 | Mendeley Data | excluded | 2 | — | no raw image among 19 TIFF/.scn/.gel files |
| 25 | 10.1038/s44321-025-00243-1 | EMBO SourceData | excluded | 2 | — | no raw image among 99 TIFF/.scn/.gel files |
| 26 | 10.1210/clinem/dgae732 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 27 | 10.1101/2025.01.14.632970v1 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 28 | 10.1038/s44318-025-00447-8 | EMBO SourceData | excluded | 2 | — | no raw image among 28 TIFF/.scn/.gel files |
| 29 | 10.1007/s00360-025-01641-0 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 30 | 10.1038/s44318-026-00761-9 | EMBO SourceData | excluded | 2 | — | no raw image among 55 TIFF/.scn/.gel files |
| 31 | 10.1101/2022.08.13.503663 | Zenodo | excluded | 1 | — | preprint |
| 32 | 10.1210/endocr/bqaf124 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 33 | 10.1126/science.adh2771 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 34 | 10.1111/j.1471-4159.2008.05778.x | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 35 | 10.1038/s44318-025-00647-2 | EMBO SourceData | excluded | 2 | — | no raw image among 75 TIFF/.scn/.gel files |
| 36 | 10.1038/s44321-025-00341-0 | EMBO SourceData | excluded | 2 | — | no raw image among 114 TIFF/.scn/.gel files |
| 37 | 10.1016/j.cellsig.2019.109501 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 38 | 10.1038/s44321-025-00354-9 | EMBO SourceData | excluded | 2 | — | no raw image among 20 TIFF/.scn/.gel files |
| 39 | 10.1016/j.celrep.2022.111538 | Mendeley Data | excluded | 2 | — | no raw image among 11 TIFF/.scn/.gel files |
| 40 | 10.1016/j.peptides.2020.170432 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 41 | 10.1016/j.isci.2025.111864 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 42 | 10.1038/s44321-026-00379-8 | EMBO SourceData | excluded | 2 | — | no raw image among 35 TIFF/.scn/.gel files |
| 43 | 10.1016/j.molcel.2021.06.009 | Mendeley Data | excluded | 5 | Fig. S1A/B | 12 unlabelled lanes, no lane guide; mapping only possible from band signals |
| 44 | 10.1007/s12035-025-05475-3 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 45 | 10.1038/s44321-025-00322-3 | EMBO SourceData | excluded | 2 | — | no raw image among 4 TIFF/.scn/.gel files |
| 46 | 10.1210/endocr/bqaa065 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 47 | 10.1038/s44321-025-00249-9 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 48 | 10.1038/s44321-026-00496-4 | EMBO SourceData | excluded | 2 | — | no raw image among 26 TIFF/.scn/.gel files |
| 49 | 10.1038/s44319-026-00963-6 | EMBO SourceData | excluded | 2 | — | no raw image among 40 TIFF/.scn/.gel files |
| 50 | 10.1038/s44321-025-00321-4 | EMBO SourceData | excluded | 2 | — | no raw image among 26 TIFF/.scn/.gel files |
| 51 | 10.1002/0471140856.tx1707s24 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 52 | 10.1038/s44319-026-00756-x | EMBO SourceData | excluded | 2 | — | no raw image among 102 TIFF/.scn/.gel files |
| 53 | 10.1016/j.intimp.2020.106445 | Mendeley Data | excluded | 2 | — | no raw image among 72 TIFF/.scn/.gel files |
| 54 | 10.1016/j.atherosclerosis.2020.01.029 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 55 | 10.1038/s44318-025-00530-0 | EMBO SourceData | excluded | 2 | — | no raw image among 33 TIFF/.scn/.gel files |
| 56 | 10.1038/s44318-025-00679-8 | EMBO SourceData | excluded | 2 | — | no raw image among 177 TIFF/.scn/.gel files |
| 57 | 10.1038/s44319-026-00922-1 | EMBO SourceData | excluded | 2 | — | no raw image among 155 TIFF/.scn/.gel files |
| 58 | 10.1038/s44321-026-00547-w | EMBO SourceData | excluded | 2 | — | no raw image among 313 TIFF/.scn/.gel files |
| 59 | 10.1038/s44319-025-00497-3 | EMBO SourceData | excluded | 2 | — | no raw image among 167 TIFF/.scn/.gel files |
| 60 | 10.1038/s44321-026-00391-y | EMBO SourceData | excluded | 2 | — | no raw image among 23 TIFF/.scn/.gel files |
| 61 | 10.5534/wjmh.200085 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 62 | 10.1016/j.cell.2024.02.013 | Mendeley Data | excluded | 2 | — | no raw image among 10 TIFF/.scn/.gel files |
| 63 | 10.1101/2025.09.01.673462 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 64 | 10.1038/s44319-026-00728-1 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 65 | 10.1039/d5na00706b | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 66 | 10.1038/s44318-026-00734-y | EMBO SourceData | excluded | 2 | — | no raw image among 32 TIFF/.scn/.gel files |
| 67 | 10.1038/s44321-026-00403-x | EMBO SourceData | excluded | 2 | — | no raw image among 70 TIFF/.scn/.gel files |
| 68 | 10.1016/j.aca.2020.07.012 | Mendeley Data | excluded | 2 | — | no raw image among 4 TIFF/.scn/.gel files |
| 69 | 10.1038/s44319-026-00805-5 | EMBO SourceData | excluded | 2 | — | no raw image among 277 TIFF/.scn/.gel files |
| 70 | 10.1038/s44319-026-00761-0 | EMBO SourceData | excluded | 2 | — | no raw image among 15 TIFF/.scn/.gel files |
| 71 | 10.1038/s44321-026-00503-8 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 72 | 10.1016/j.bioorg.2020.103874 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 73 | 10.1101/2025.11.02.686110 | Mendeley Data | excluded | 1 | — | preprint |
| 74 | 10.1038/s44318-025-00686-9 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 75 | 10.3390/cells14030226 | Harvard Dataverse | excluded | 2 | — | no raw image among 180 TIFF/.scn/.gel files |
| 76 | 10.1016/j.nbd.2018.09.006 | Mendeley Data | excluded | 4 | Fig. 2B | n = 5 stated; GAPDH deposited for one 6-lane membrane only, ZO-1 blots missing |
| 77 | 10.1111/imb.70059 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 78 | 10.3389/fimmu.2020.603569 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 79 | 10.1074/jbc.ra118.003656 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 80 | 10.1002/1873-3468.70303 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 81 | 10.1111/j.1471-4159.2010.07096.x | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 82 | 10.1038/s44318-025-00515-z | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 83 | 10.1016/j.cell.2023.11.009 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 84 | 10.1016/j.isci.2023.107207 | Mendeley Data | excluded | 4 | Fig. 3B (S3A) | n = 6 stated; one membrane per antibody with 2 lanes per group |
| 85 | 10.1073/pnas.2523183123 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 86 | 10.1101/2022.10.28.514299 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 87 | 10.7554/elife.91083 | Zenodo | excluded | 1 | Fig. 6B–D | only phospho/total ratios quantified; no panel fits the unit of analysis |
| 88 | 10.5534/wjmh.210180 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 89 | 10.1126/science.adg7942 | Mendeley Data | excluded | 2 | — | no raw image among 2 TIFF/.scn/.gel files |
| 90 | 10.1038/s44319-026-00823-3 | EMBO SourceData | excluded | 2 | — | no raw image among 85 TIFF/.scn/.gel files |
| 91 | 10.1016/j.neuron.2023.06.002 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 92 | 10.3390/pathogens10070794 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 93 | 10.1038/s41467-022-32562-z | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 94 | 10.1038/s44319-025-00510-9 | EMBO SourceData | excluded | 2 | — | no raw image among 43 TIFF/.scn/.gel files |
| 95 | 10.1038/s44318-025-00560-8 | EMBO SourceData | excluded | 2 | Fig. EV1C/D | only a pseudo-coloured RGB composite; main-figure blots are co-IPs normalised to the bait |
| 96 | 10.1016/j.ymben.2019.11.007 | Zenodo | excluded | 2 | — | no raw image among 1 TIFF/.scn/.gel files |
| 97 | 10.1101/2025.05.12.650695v2 | Mendeley Data | excluded | 1 | — | preprint |
| 98 | 10.1038/s41598-025-91153-2 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 99 | 10.1016/j.jstrokecerebrovasdis.2018.11.018 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 100 | 10.1016/j.molcel.2023.06.020 | Mendeley Data | excluded | 4 | Fig. 1C (S1C/D) | n = 3 stated; one replicate membrane per blot deposited (agent recorded criterion 3; recorded as 4 like ranks 76 and 84) |
| 101 | 10.1038/s44318-026-00913-x | EMBO SourceData | excluded | 2 | — | no raw image among 1 TIFF/.scn/.gel files |
| 102 | 10.1002/biof.1774 | Mendeley Data | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 103 | 10.1016/j.cmet.2020.06.002 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 104 | 10.1371/journal.pone.0282859 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 105 | 10.1038/s41598-024-76862-4 | Zenodo | excluded | 2 | — | no raw image among 11 TIFF/.scn/.gel files |
| 106 | 10.1038/s44319-025-00616-0 | EMBO SourceData | excluded | 2 | — | no raw image among 139 TIFF/.scn/.gel files |
| 107 | 10.1210/clinem/dgaa161 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 108 | 10.1038/s44321-026-00479-5 | EMBO SourceData | excluded | 2 | Fig. 1D | deposited only as pseudo-coloured RGB TIFF and JPEG |
| 109 | 10.1038/s44318-025-00587-x | EMBO SourceData | excluded | 1 | Fig. 1G | only blot is a qualitative band-shift assay, not quantified |
| 110 | 10.1016/j.bbr.2026.116111 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 111 | 10.1016/j.jep.2020.112828 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 112 | 10.1128/jcm.00267-25 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 113 | 10.1152/ajpgi.00145.2024 | Dryad | excluded | 2 | — | no raw image among 1 TIFF/.scn/.gel files |
| 114 | 10.1016/j.neuron.2021.10.032 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 115 | 10.1101/2025.01.08.631787v1 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 116 | 10.1210/endocr/bqaa157 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 117 | 10.3344/kjp.23039 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 118 | 10.1016/j.mce.2020.110990 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 119 | 10.1371/journal.pone.0103587 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 120 | 10.1038/s44320-026-00229-2 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 121 | 10.33696/neurol.5.103 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 122 | 10.1038/s44319-026-00713-8 | EMBO SourceData | excluded | 2 | — | no raw image among 9 TIFF/.scn/.gel files |
| 123 | 10.1016/j.molcel.2020.12.042 | Mendeley Data | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 124 | 10.1038/s44321-026-00497-3 | EMBO SourceData | excluded | 2 | Fig. 2D | two of the panel's files are 8-bit |
| 125 | 10.1038/s44318-026-00703-5 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 126 | 10.1016/j.intimp.2020.106334 | Mendeley Data | excluded | 2 | — | no raw image among 8 TIFF/.scn/.gel files |
| 127 | 10.1038/s44318-025-00604-z | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 128 | 10.1038/s44319-026-00724-5 | EMBO SourceData | excluded | 2 | — | no raw image among 166 TIFF/.scn/.gel files |
| 129 | 10.1016/j.molcel.2025.10.026 | Mendeley Data | excluded | 2 | Fig. 3E–F | panel files are 8-bit; the only 16-bit pair (S3H) has n = 1 |
| 130 | 10.1038/s44321-026-00534-1 | EMBO SourceData | excluded | 2 | — | no raw image among 47 TIFF/.scn/.gel files |
| 131 | 10.1038/s44318-025-00519-9 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 132 | 10.1038/s44318-026-00770-8 | EMBO SourceData | excluded | 2 | — | no raw image among 102 TIFF/.scn/.gel files |
| 133 | 10.1126/sciadv.adh1134 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 134 | 10.3390/ijms25137457 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 135 | 10.1093/toxsci/kfag049 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 136 | 10.7554/elife.82809 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 137 | 10.1155/2014/971750 | Mendeley Data | excluded | 2 | — | no raw image among 4 TIFF/.scn/.gel files |
| 138 | 10.1038/s44321-026-00516-3 | EMBO SourceData | excluded | 2 | — | no raw image among 10 TIFF/.scn/.gel files |
| 139 | 10.1016/j.celrep.2022.111547 | Mendeley Data | excluded | 2 | — | no raw image among 25 TIFF/.scn/.gel files |
| 140 | 10.1101/2023.02.09.527880v4.abstract | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 141 | 10.1111/bph.15367 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 142 | 10.1038/s41598-020-59085-1 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 143 | 10.1101/2020.07.14.195297 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 144 | 10.1038/s44319-025-00552-z | EMBO SourceData | excluded | 2 | — | no raw image among 5 TIFF/.scn/.gel files |
| 145 | 10.1016/j.neulet.2021.135690 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 146 | 10.1016/j.molcel.2021.07.020 | Mendeley Data | excluded | 2 | — | no raw image among 7 TIFF/.scn/.gel files |
| 147 | 10.1186/s12886-021-02166-0 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 148 | 10.1038/s44319-026-00838-w | EMBO SourceData | excluded | 2 | — | no raw image among 4 TIFF/.scn/.gel files |
| 149 | 10.57844/arcadia-1mmp-30hh | Zenodo | excluded | 1 | — | Arcadia publication, not peer-reviewed |
| 150 | 10.1016/j.metabol.2019.153962 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 151 | 10.1038/s44319-025-00667-3 | EMBO SourceData | excluded | 2 | — | no raw image among 30 TIFF/.scn/.gel files |
| 152 | 10.1038/s44319-026-00699-3 | EMBO SourceData | excluded | 2 | — | no raw image among 95 TIFF/.scn/.gel files |
| 153 | 10.1038/s44321-025-00287-3 | EMBO SourceData | excluded | 2 | — | no raw image among 77 TIFF/.scn/.gel files |
| 154 | 10.1096/fj.202601959r | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 155 | 10.1038/s44318-025-00437-w | EMBO SourceData | excluded | 1 | — | all blots shown as representative images, none quantified |
| 156 | 10.1038/s41467-026-71753-w | Zenodo | **included** | — | Fig. 1e | γH2AX / total protein stain, 4 groups × 6 mice, raw Odyssey scans; `10_1038_s41467_026_71753_w/` |
| 157 | 10.1016/j.redox.2020.101592 | Mendeley Data | excluded | 2 | — | no raw image among 10 TIFF/.scn/.gel files |
| 158 | 10.1093/jxb/erae250 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 159 | 10.1038/s44318-026-00757-5 | EMBO SourceData | excluded | 2 | — | no raw image among 96 TIFF/.scn/.gel files |
| 160 | 10.3389/fimmu.2026.1860823 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 161 | 10.1038/s44318-026-00833-w | EMBO SourceData | excluded | 2 | — | no raw image among 36 TIFF/.scn/.gel files |
| 162 | 10.1016/j.jmb.2023.168353 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 163 | 10.1016/j.molcel.2022.08.018 | Mendeley Data | excluded | 1 | — | only northern blots quantified (main text not accessible; decided from figures and supplement) |
| 164 | 10.1038/s44319-026-00867-5 | EMBO SourceData | excluded | 2 | — | no raw image among 61 TIFF/.scn/.gel files |
| 165 | 10.1002/1873-3468.70196 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 166 | 10.1038/s44321-026-00474-w | EMBO SourceData | excluded | 2 | — | no raw image among 93 TIFF/.scn/.gel files |
| 167 | 10.3390/biomedicines8120626 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 168 | 10.3389/fendo.2024.1282231 | Dryad | excluded | 2 | — | no raw image among 19 TIFF/.scn/.gel files |
| 169 | 10.1038/s44321-026-00385-w | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 170 | 10.1038/s44318-026-00891-0 | EMBO SourceData | excluded | 2 | — | no raw image among 35 TIFF/.scn/.gel files |
| 171 | 10.1016/j.molcel.2025.08.015 | Mendeley Data | excluded | 1 | — | no western blot quantified; the .scn file is an RT-PCR gel |
| 172 | 10.1212/wnl.0000000000009233 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 173 | 10.1038/s44321-025-00364-7 | EMBO SourceData | excluded | 2 | — | no raw image among 52 TIFF/.scn/.gel files |
| 174 | 10.1038/s44321-026-00520-7 | EMBO SourceData | excluded | 2 | — | no raw image among 2 TIFF/.scn/.gel files |
| 175 | 10.1016/j.ejphar.2020.173036 | Mendeley Data | excluded | 2 | — | no raw image among 2 TIFF/.scn/.gel files |
| 176 | 10.7554/elife.86913 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 177 | 10.1016/j.cell.2021.04.001 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 178 | 10.1016/j.celrep.2021.110056 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 179 | 10.1038/s44319-025-00504-7 | EMBO SourceData | excluded | 2 | — | no raw image among 69 TIFF/.scn/.gel files |
| 180 | 10.1038/s44318-025-00532-y | EMBO SourceData | excluded | 2 | — | no raw image among 87 TIFF/.scn/.gel files |
| 181 | 10.1038/s44319-025-00597-0 | EMBO SourceData | excluded | 2 | — | no raw image among 43 TIFF/.scn/.gel files |
| 182 | 10.1212/nxg.0000000000000636 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 183 | 10.1016/j.stem.2022.06.004 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 184 | 10.1101/2021.08.02.454729v2 | Mendeley Data | excluded | 2 | — | no raw image among 20 TIFF/.scn/.gel files |
| 185 | 10.1038/s44319-026-00694-8 | EMBO SourceData | excluded | 2 | — | no raw image among 22 TIFF/.scn/.gel files |
| 186 | 10.1038/s44321-025-00336-x | EMBO SourceData | excluded | 2 | Fig. 4J | pseudo-coloured TIFFs only |
| 187 | 10.1038/s44318-026-00760-w | EMBO SourceData | excluded | 2 | — | no raw image among 17 TIFF/.scn/.gel files |
| 188 | 10.7554/elife.72593 | Dryad;Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 189 | 10.1016/j.chembiol.2023.06.017 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 190 | 10.1016/j.actbio.2019.02.024 | Mendeley Data | excluded | 2 | — | no raw image among 4 TIFF/.scn/.gel files |
| 191 | 10.7554/elife.100527.2 | Dryad | excluded | 2 | — | no raw image among 5 TIFF/.scn/.gel files |
| 192 | 10.1016/j.bbr.2022.114077 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 193 | 10.1016/j.cca.2006.07.029 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 194 | 10.1038/s44319-025-00614-2 | EMBO SourceData | excluded | 2 | — | no raw image among 27 TIFF/.scn/.gel files |
| 195 | 10.1038/s44318-026-00706-2 | EMBO SourceData | excluded | 1 | — | DmdA blots not quantified |
| 196 | 10.1074/jbc.m112.353961 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 197 | 10.3389/fimmu.2022.822996 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 198 | 10.1038/s44321-025-00326-z | EMBO SourceData | excluded | 2 | — | no raw image among 82 TIFF/.scn/.gel files |
| 199 | 10.1111/apha.14078 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 200 | 10.1038/s44321-026-00452-2 | EMBO SourceData | excluded | 2 | — | no raw image among 130 TIFF/.scn/.gel files |
| 201 | 10.1038/s44318-026-00876-z | EMBO SourceData | excluded | 2 | — | no raw image among 41 TIFF/.scn/.gel files |
| 202 | 10.1038/s44321-026-00513-6 | EMBO SourceData | excluded | 2 | — | no raw image among 72 TIFF/.scn/.gel files |
| 203 | 10.1155/2021/6632411 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 204 | 10.7554/elife.97327 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 205 | 10.1128/jvi.00507-23 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 206 | 10.1371/journal.pone.0326687 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 207 | 10.3389/fphar.2024.1389654 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 208 | 10.5534/wjmh.250105 | Harvard Dataverse | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 209 | 10.1016/j.celrep.2024.114831 | Zenodo | excluded | 7 | Fig. 5E | criteria 2–6 met; only plotted means ± SD, no per-replicate numbers |
| 210 | 10.1038/s44319-025-00457-x | EMBO SourceData | excluded | 2 | — | no raw image among 6 TIFF/.scn/.gel files |
| 211 | 10.1523/jneurosci.0610-22.2023 | Mendeley Data | excluded | 2 | — | no raw image among 1 TIFF/.scn/.gel files |
| 212 | 10.1186/s12933-022-01486-9 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 213 | 10.1038/s44319-026-00797-2 | EMBO SourceData | excluded | 2 | — | no raw image among 35 TIFF/.scn/.gel files |
| 214 | 10.1016/j.celrep.2023.112265 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 215 | 10.1242/bio.059190 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 216 | 10.1038/s44318-026-00873-2 | EMBO SourceData | excluded | 2 | — | no raw image among 1 TIFF/.scn/.gel files |
| 217 | 10.1111/febs.15274 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 218 | 10.1038/s44321-025-00318-z | EMBO SourceData | excluded | 2 | — | no raw image among 161 TIFF/.scn/.gel files |
| 219 | 10.1038/s44321-025-00264-w | EMBO SourceData | excluded | 2 | — | no raw image among 10 TIFF/.scn/.gel files |
| 220 | 10.1016/j.bbadis.2019.165606 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 221 | 10.1002/1873-3468.70069 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 222 | 10.1101/2024.07.19.604361 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 223 | 10.1016/j.xgen.2025.100986 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 224 | 10.1038/s44319-026-00776-7 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 225 | 10.1038/s44319-025-00583-6 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 226 | 10.1038/s44321-025-00360-x | EMBO SourceData | excluded | 2 | — | no raw image among 144 TIFF/.scn/.gel files |
| 227 | 10.3390/cancers14133074 | Zenodo | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 228 | 10.1038/s44319-026-00740-5 | EMBO SourceData | excluded | 1 | — | blots shown one lane per condition, not quantified |
| 229 | 10.1038/s44321-026-00383-y | EMBO SourceData | excluded | 2 | — | no raw image among 217 TIFF/.scn/.gel files |
| 230 | 10.1038/s44318-026-00871-4 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 231 | 10.1038/s44318-025-00422-3 | EMBO SourceData | excluded | 2 | — | raw images are microscopy only; every blot file is 8-bit, colour or CMYK |
| 232 | 10.1016/j.isci.2023.106695 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 233 | 10.1016/j.molcel.2026.03.013 | Mendeley Data | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
| 234 | 10.1038/s44319-025-00660-w | EMBO SourceData | excluded | 2 | — | no raw image among 74 TIFF/.scn/.gel files |
| 235 | 10.1007/s12195-026-00884-3 | Dryad | excluded | 2 | — | no TIFF, .scn or .gel file in the deposit |
