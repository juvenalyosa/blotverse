#!/usr/bin/env bash
# Exposure-invariance bench: fetch every case, run `blotverse exposure` on each
# series config and pool the results (see docs/exposure_protocol.md).
# Usage: validation/exposure/run_all.sh [RESULTS_DIR]
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
bv="$root/build/apps/blotverse"
out=${1:-$root/validation/results/exposure}
mkdir -p "$out"
for case_dir in "$here"/*/; do
  case=$(basename "$case_dir")
  "$case_dir/fetch.sh" >/dev/null
  for config in "$case_dir"*.yaml; do
    name="${case}_$(basename "$config" .yaml)"
    "$bv" exposure "$config" --out "$out/$name" --spec "$root/spec/multiverse.yaml" >/dev/null
    echo "ran $name"
  done
done
# Protocol v1.2: the six v1.1 series are the development set; every other
# case is confirmatory. See docs/exposure_protocol.md.
dev=(gtsf1_2026 set1ip_2026 plcg_ip_2025)
python3 "$here/summarize.py" "$out" "${dev[@]}" > "$out/summary_development.md"
conf=()
for case_dir in "$here"/*/; do
  case=$(basename "$case_dir")
  [[ " ${dev[*]} " == *" $case "* ]] || conf+=("$case")
done
if ((${#conf[@]})); then
  python3 "$here/summarize.py" "$out" "${conf[@]}" | tee "$out/summary_confirmatory.md"
else
  echo "no confirmatory series yet" | tee "$out/summary_confirmatory.md"
fi
