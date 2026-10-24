#!/usr/bin/env bash
# E6: does the default background branch match Fiji on simple blots?
#
# Usage: validation/fiji/run_e6.sh WORK_DIR [FIJI_EXECUTABLE]
# Without a Fiji executable it only prepares the blots and prints the command.
set -euo pipefail

work=${1:?usage: run_e6.sh WORK_DIR [FIJI_EXECUTABLE]}
fiji=${2:-}
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
bv="$root/build/apps/blotverse"

mkdir -p "$work"
dirs=()
for seed in $(seq 1 10); do
  "$bv" simulate --out "$work/sim_$seed" --level clean --seed "$seed" --gels 2 --reps 3 >/dev/null
  "$bv" run "$work/sim_$seed/experiment.yaml" --out "$work/out_$seed" --permutations 0 \
    --spec "$root/spec/multiverse.yaml" >/dev/null
  dirs+=("$work/out_$seed")
done

if [[ -z "$fiji" ]]; then
  echo "Blots ready. Run Fiji on each directory, then compare:"
  for seed in $(seq 1 10); do
    echo "  ImageJ-macosx --headless -macro $here/e6_fiji.ijm \"$work/out_$seed|$work/sim_$seed\""
  done
  echo "  python3 $here/compare_e6.py ${dirs[*]}"
  exit 0
fi

for seed in $(seq 1 10); do
  "$fiji" --headless -macro "$here/e6_fiji.ijm" "$work/out_$seed|$work/sim_$seed"
done
python3 "$here/compare_e6.py" "${dirs[@]}"
