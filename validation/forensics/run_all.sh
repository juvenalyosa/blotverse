#!/usr/bin/env bash
# Image-integrity screening validation (docs/forensics_protocol.md): development
# injection bench, then the confirmatory endpoints E1-E4.
# Usage: validation/forensics/run_all.sh [RESULTS_DIR] [development|confirmatory|all]
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
bv="$root/build/apps/blotverse"
out=${1:-$root/validation/results/forensics}
part=${2:-all}
ex="$root/validation/exposure"
mkdir -p "$out"

if [[ $part == development || $part == all ]]; then
  "$ex/gtsf1_2026/fetch.sh" >/dev/null
  "$ex/set1ip_2026/fetch.sh" >/dev/null
  "$ex/plcg_ip_2025/fetch.sh" >/dev/null
  "$bv" forensics-bench "$ex"/set1ip_2026/gfp_20s.tif "$ex"/set1ip_2026/gfp_295s.tif \
    "$ex"/plcg_ip_2025/*.tif "$ex"/gtsf1_2026/08.03.anti_gtsf1_50sec.scn \
    "$ex"/gtsf1_2026/anti_GTSF1_75sec.scn "$ex"/gtsf1_2026/anti_GTSF1_5secauto.scn \
    --out "$out/development" --repeats 5 --seed 1 2>/dev/null >/dev/null
  echo "development bench -> $out/development/forensics_bench.md"
fi

if [[ $part == confirmatory || $part == all ]]; then
  "$ex/ufm1_2026/fetch.sh" >/dev/null
  "$root/validation/level3/aurora_a_2026/fetch.sh" >/dev/null
  "$root/validation/real_data/pillai_kastoori_2020/fetch.sh" >/dev/null
  ufm1=("$ex"/ufm1_2026/*.tif)
  aurora=("$root"/validation/level3/aurora_a_2026/*.tif)
  pillai=("$root"/validation/real_data/pillai_kastoori_2020/*.TIF)
  # E1/E2: every study screened with all its images together.
  "$bv" forensics "${ufm1[@]}" --out "$out/screen_ufm1_2026"
  "$bv" forensics "${aurora[@]}" --out "$out/screen_aurora_a_2026"
  "$bv" forensics "${pillai[@]}" --out "$out/screen_pillai_kastoori_2020"
  # E3/E4: injection bench, seed 2.
  "$bv" forensics-bench "${ufm1[@]}" "${aurora[@]}" "${pillai[@]}" \
    --out "$out/confirmatory" --repeats 5 --seed 2 2>/dev/null >/dev/null
  echo "confirmatory bench -> $out/confirmatory/forensics_bench.md"
fi
