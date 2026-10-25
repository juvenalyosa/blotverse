#!/usr/bin/env bash
# Raw LI-COR Odyssey CLx acquisition of a 7-point lysate dilution series
# (40-1.25 ug; cPARP, beta-actin, GAPDH, COXIV), from:
#   Pillai-Kastoori L. et al. (2020) A systematic approach to quantitative Western
#   blot analysis. Anal Biochem 593:113608. doi:10.1016/j.ab.2020.113608
# Data: Mendeley Data doi:10.17632/vwcy56z5jh.1 (CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
url="https://data.mendeley.com/public-files/datasets/vwcy56z5jh/files/1af1a81c-3a14-4cad-a669-451bc409d72f/file_downloaded"
tmp=$(mktemp -d)
curl -sL -o "$tmp/linear_range.zip" "$url"
unzip -q -o "$tmp/linear_range.zip" -d "$tmp/x"
cp "$(find "$tmp/x" -name '0000453_03_800.TIF' | head -1)" "$here/0000453_03_800.TIF"
cp "$(find "$tmp/x" -name '0000453_03_700.TIF' | head -1)" "$here/0000453_03_700.TIF"
rm -rf "$tmp"
echo "fetched into $here"
