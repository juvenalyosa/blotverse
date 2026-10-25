#!/usr/bin/env bash
# Level-3 reanalysis of AURORA A Fig. 3G: technical comparison with the
# authors' quantification, then the full multiverse.
# Usage: validation/level3/aurora_a_2026/run.sh [RESULTS_DIR]
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../../.." && pwd)
bv="$root/build/apps/blotverse"
out=${1:-$root/validation/results/level3_aurora_a_2026}
"$here/fetch.sh" >/dev/null
mkdir -p "$out"
for image in DICER_R1 HA_R1 DICER_R2 HA_R2 DICER_R3; do
  sed "s|^image: .*|image: $here/3G_WB_$image.tif|" "$here/measure_$image.yaml" > "$out/measure_$image.yaml"
  "$bv" dilution "$out/measure_$image.yaml" --out "$out/m_$image" --spec "$root/spec/multiverse.yaml" >/dev/null
done
python3 "$here/compare.py" "$out" "$here/3G_WB_quantification.xlsx" | tee "$out/comparison.md"
"$bv" run "$here/experiment.yaml" --out "$out/multiverse" --spec "$root/spec/multiverse.yaml"
