# 10.1016/j.celrep.2024.114831 (Fig. 5E): not analysable

**Outcome:** included (Amendment 2, first-frame rescreen group), annotated by both
annotators with the same mapping, but **not analysable with BlotVerse at the freeze
commit**. Counted as an analysis outcome, not as a screening exclusion.

**Why (found independently by annotator A and annotator B):**

* Each condition (Control, 100 µM ERK2, 100 µM BSA) was run on its own membrane with its
  own Ponceau S image. The ERK2 vs Control comparison is between membranes by design.
  BlotVerse requires a control lane in every blot (its joint test permutes within blots),
  so `blotverse detect` / `run` stop with "blot ... has no 'Control' lane".
* One blot per replicate cannot hold the three conditions either: a blot takes one
  reference image, and each condition has its own Ponceau image with different lane
  positions (so `total_protein` does not apply).
* The 12 lanes of each strip are a time course (0–60 min), not interchangeable replicates.
  Even if the file loaded, BlotVerse would treat the time points as samples of one group.

Annotations: `validation/level3/10_1016_j_celrep_2024_114831/annotator_{a,b}.{yaml,md}`.
No `experiment.yaml` was reconciled and no run was made. BlotVerse code was not changed
(the protocol fixes the freeze commit).
