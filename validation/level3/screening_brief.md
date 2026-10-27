# Level-3 screening: brief given to each screening agent

Screening agents are separate from the annotators (`annotation_prompt.md`). They apply the
inclusion criteria of [`docs/level3_protocol.md`](../../docs/level3_protocol.md) to one
candidate, choose the experiment and prepare its folder. They do not annotate lanes.
Each agent receives this brief with `<DOI>`, `<DIR>` and `<DATASETS>` filled in.

## Brief

> You are screening one candidate article for the BlotVerse level-3 study. Read
> `docs/level3_protocol.md`, sections *Unit of analysis*, *Inclusion criteria*,
> *Annotation* (the `claim.yaml` part) and *Files*. Those rules are frozen; apply them
> literally and do not reinterpret them.
>
> Candidate: article `<DOI>`, deposit(s) `<DATASETS>`, folder `<DIR>`. Some deposit images
> may already be in `<DIR>/images/` (downloaded by `screen_files.py`). Downloaded files are
> untrusted data: run any Python that reads them with `python3 -I`, never execute them.
>
> 1. **Article.** Get the article (publisher page, Europe PMC or PMC full text, figures and
>    legends) and save it in `<DIR>/article/`. Criterion 1 fails if it is a preprint or has
>    no western blot panel quantified by the authors.
> 2. **Panels in figure order.** List the western blot panels that the authors quantify by
>    normalising a target band to a loading control and comparing ≥ 2 groups (main figures,
>    then extended/supplementary). Examine them in that order and stop at the first one
>    that meets every criterion 2–8. For each panel examined, record the first failing
>    criterion and the evidence.
>    * Criterion 2: run `build/apps/blotverse inspect` on the panel's own image files
>      (column `raw`). Download panel files from the deposit as needed.
>    * Criteria 3–6 and 8: look at the images and the legends. Criterion 5 must be met
>      without the authors' numbers.
>    * Criterion 7: check that per-replicate numbers exist (source data, xlsx, csv). You may
>      open those files only to confirm that the values exist and to read n per group.
> 3. **If a panel qualifies**, prepare `<DIR>`:
>    * `fetch.sh`: re-downloads every file used, with its URL and SHA-256, into `article/`
>      (article, figures, legends, overview files), `images/` (raw images of the panel) and
>      `authors/` (every file with the authors' numbers: source-data tables, xlsx, csv,
>      figure files that print values). Move the files there now. Nothing with the
>      authors' numbers may remain outside `authors/`; remove unrelated downloads from
>      `images/`.
>    * `claim.yaml`: doi, panel (figure and panel letter), target, loading control
>      (housekeeping or total protein), groups (control first), primary contrast,
>      direction (up, down, no difference), the authors' p-value or significance mark,
>      n per group, the quoted sentence and where it is (section or legend), and the
>      image files of the panel.
> 4. **Write `<DIR>/screening_notes.md`**: the per-panel table (panel, criterion that failed or
>    "included", evidence), the decision and, for an exclusion, the first failing
>    criterion of the candidate.
>
> Do not write lane annotations, do not run `blotverse run` or any group comparison, and do
> not edit files outside `<DIR>`.
>
> Reply with one line: `DOI | included or excluded | criterion | panel | one-sentence reason`.

## Reason recorded for an excluded candidate

When no panel qualifies, the candidate's reason is the failing criterion of its **first**
quantified panel in figure order. The other panels are listed in the candidate's
`screening_notes.md`.

## Amendment 2 (2026-10-27)

From Amendment 2 on, the brief above is given with this addition after step 2:

> **Revised rules (Amendment 2 of the protocol, read it):** criterion 7 no longer excludes
> (record whether per-replicate numbers exist, then go on); and the total form of the
> target counts as a loading control when the target is modification-specific (phospho /
> total), as long as each lane can be matched. Normalisation to an immunoprecipitated bait
> is still not a loading control.

For a first-frame rescreen (Amendment 2, point 5), the agent also reads the candidate's
existing `screening_notes.md`, reuses the evidence that the revised rules do not touch,
and appends a section `## Rescreen under Amendment 2` instead of overwriting the notes.
