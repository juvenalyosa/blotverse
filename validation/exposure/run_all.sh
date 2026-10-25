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
python3 "$here/summarize.py" "$out" | tee "$out/summary.md"
