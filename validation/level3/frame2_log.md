# Level-3 sampling frame frame2: search log (2026-10-27, deposits 2010-01-01 to 2019-12-31)

Written by `build_frame.py`. *Hits*: records returned. *Kept*: in the date window and
matching the text filter (EMBO: with a blot image file). *Linked*: linked to an article DOI.

| Repository | Query | Hits | Kept | Linked |
|---|---|---|---|---|
| EMBO SourceData | collection search 'blot' | 1167 |  |  |
| EMBO SourceData | collection search 'WB' | 191 |  |  |
| EMBO SourceData | studies in window / with a blot image file | 62 | 0 | 0 |
| Zenodo | "western blot" AND (raw OR uncropped OR original) AND publication_date:[2010-01-01 TO 2019-12-31] | 20 | 15 | 15 |
| Mendeley Data | "western blot" | 3034 | 236 | 108 |
| Figshare | :title: "western blot" (datasets) | 2 | 1 | 0 |
| Dryad | "western blot" | 98 | 29 | 29 |
| Harvard Dataverse | "western blot" (datasets) | 68 | 8 | 5 |
| UNC Dataverse | "western blot": search API refused (401) | 0 | 0 | 0 |

Articles (merged by DOI): 145; dropped as development datasets: 0; already in an earlier frame: 0; **candidates: 145**.
