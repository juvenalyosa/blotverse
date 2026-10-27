# Level-3 annotation: agents and prompt

Frozen with [`docs/level3_protocol.md`](../../docs/level3_protocol.md). It does not change
during the study; any change is an amendment.

**Changes:** Amendment 2 (2026-10-27) added the `total_protein` item in step 4. Experiments
annotated before it used the prompt without that item.

| Annotator | Model | Context |
|---|---|---|
| A | `claude-opus-5-5` | fresh agent per experiment |
| B | `claude-sonnet-5-5` | fresh agent per experiment, separate from A |

Each agent receives only the prompt below, with `<DIR>` replaced by the experiment's folder
and `<X>` by `a` or `b`.

## Prompt

> You are annotating one published western blot experiment for BlotVerse, a tool that
> quantifies blots. Work only in `<DIR>`.
>
> **Allowed sources:** `<DIR>/article/` (article text, figures, legends, overview files)
> and `<DIR>/images/` (raw images). Also `<DIR>/claim.yaml`, which names the panel, the
> groups and the primary contrast.
>
> **Forbidden:** `<DIR>/authors/` and any other numeric quantification by the authors,
> including numbers printed in figures or tables. Never infer which lane is which from
> band intensities that would match the authors' results. Do not read any other
> annotator's files.
>
> **Task:**
>
> 1. **Identify the images.** Find the raw image(s) of the target band and of the loading
>    control for every replicate of the panel in `claim.yaml`.
> 2. **Map the lanes.** Map every lane of the panel's groups to its group and replicate,
>    using only:
>    * figure layout, legends and overview files;
>    * file names and deposit metadata.
>
>    Leave out ladders, inputs, IgG and lanes of other groups.
> 3. **Measure the geometry.** Use `blotverse inspect`, `blotverse lanes`,
>    `blotverse detect` and `validation/level3/refine_lanes.py` to measure:
>    * lane centres and widths;
>    * band rows and half-heights.
>
>    Check every value against the image.
> 4. **Write the experiment file.** Write `<DIR>/annotator_<X>.yaml`, a BlotVerse
>    experiment file:
>    * blots, lanes (group, sample), `annotation` blocks;
>    * `reference_image` and `reference_annotation` when the loading control is a
>      separate image;
>    * `total_protein` instead, when the loading control is a total-protein image with the
>      same size and lane positions as the target image;
>    * `invert: true` when bands are dark on a light background.
> 5. **Write the log.** Write `<DIR>/annotator_<X>.md`. For each lane, give the evidence
>    for its group and replicate (source and quote or description). Also list:
>    * every uncertainty;
>    * every choice between alternatives.
> 6. **When the mapping is ambiguous**, do not guess. This covers a lane, a strip or a
>    replicate that cannot be identified from the allowed sources. Write
>    `status: unidentifiable` and the reason at the top of the log, and stop.
>
> Do not run `blotverse run` and do not compute any group comparison.
