#!/usr/bin/env bash
# Re-downloads every file used for 10.1016/j.molcel.2017.04.024 (Fig. S7D, Cdc48 / Pgk1,
# WT vs cdc48-3). Tsuchiya et al. (2017) Mol Cell 66:488-502. No PMC copy exists; the
# article and its supplement come from Elsevier's static file server (ars.els-cdn.com),
# the raw images from Mendeley Data t4xtrtj2gp (version 3, doi:10.17632/t4xtrtj2gp.3).
# article/  article + supplement PDFs, deposit metadata, overview crop of Fig. S7D
# images/   raw 16-bit TIFFs of the Fig. S7D membrane (Cdc48 strip and Pgk1 strip)
# authors/  every file with the authors' numbers (Tables S1-S6, xlsx; annotators never open this folder)
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images authors
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

get() { # url dest sha256 (sha "-" = served dynamically, hash not checked)
  curl -sSL -o "$2" "$1"
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}
ELS="https://ars.els-cdn.com/content/image/1-s2.0-S109727651730309X"
MEN="https://data.mendeley.com/public-files/datasets/t4xtrtj2gp/files"

# --- article: "Document S2. Article plus Supplemental Information" and "Document S1" (supplementary figures and legends)
get "$ELS-mmc8.pdf" article/article_plus_SI_mmc8.pdf f28ce3a1ffa6a115e0491a95cd83befe22adee0cf69a259f7fb1c57c9d14bc1b
get "$ELS-mmc1.pdf" article/DocumentS1_mmc1.pdf     f17b5d32053a239aa578bde54a1a4b3ec9e343d3a08501cde728a0937e1c4117
# deposit metadata (Mendeley public API; file list, names, sizes, hashes)
get "https://data.mendeley.com/public-api/datasets/t4xtrtj2gp" article/deposit_metadata.json -

# --- raw images of the panel (Mendeley file names: "Figure S7D IB-Cdc48 .tif", "Figure S7D IB-Pgk1.tif")
get "$MEN/6925930a-869a-4649-adf2-1559a49c10a0/file_downloaded" images/FigS7D_IB-Cdc48.tif a799ef5e66fb80339dbe0299d4da3a57c01fd4c94daad9b1a543a6262007ff01
get "$MEN/d5489c9c-faa0-47c5-8ec9-cb0fc8905f5e/file_downloaded" images/FigS7D_IB-Pgk1.tif  70a3bfbd3f3f75898d1322fac2326071d4741b4583c1b4e41e281632765cbed5

# --- authors' numbers: Tables S1-S6 (mmc2-mmc7). Table S6 (mmc7.xlsx, sheet "6D") holds Fig. S7D.
get "$ELS-mmc2.xlsx" authors/mmc2.xlsx 36afdfa0b430be3089d1ab5c0c422e645f258c37a79d06f7f506330e6e36e463
get "$ELS-mmc3.xlsx" authors/mmc3.xlsx d5580c7e3fa37bef12bbb595959d8e76664104bda1eda34de315cae6f96fa971
get "$ELS-mmc4.xlsx" authors/mmc4.xlsx 0851af9a5f9b2ed1b8b3568c4a433799da84a835e57327919ffa1bb6176a5071
get "$ELS-mmc5.xlsx" authors/mmc5.xlsx 2866f7fbb635e76a4bf0093ffb4436a1e8827b234c2c0757bad2a1ec948de535
get "$ELS-mmc6.xlsx" authors/mmc6.xlsx ae4f0e77027fbc42542e75b6067174790d1e0c856d01302ba15a65485f44c0bc
get "$ELS-mmc7.xlsx" authors/mmc7.xlsx 0824087cc877d78a987ee827e8df0556af01be9c020485fa14387af6d4d8af14

# --- overview for annotators: Fig. S7D blot crop (lane labels "WT", "cdc48-3"; no values,
# the bar chart is left out). Page 13 of Document S1 rendered at 6x with macOS PDFKit, then
# cropped with sips. Derived file; this renderer produced
# 702a71310b0d3c680acbc0fe7fecf54a9eb03d6daf9cdd481878085871a96f14.
cat > "$TMP/render.swift" <<'EOF'
import PDFKit
import AppKit
let d = PDFDocument(url: URL(fileURLWithPath: CommandLine.arguments[1]))!
let p = d.page(at: Int(CommandLine.arguments[2])! - 1)!
let r = p.bounds(for: .mediaBox); let sc: CGFloat = 6
let img = NSImage(size: NSSize(width: r.width*sc, height: r.height*sc))
img.lockFocus(); NSColor.white.set(); NSRect(x:0,y:0,width:r.width*sc,height:r.height*sc).fill()
let ctx = NSGraphicsContext.current!.cgContext; ctx.scaleBy(x: sc, y: sc); p.draw(with: .mediaBox, to: ctx); img.unlockFocus()
let rep = NSBitmapImageRep(data: img.tiffRepresentation!)!
try! rep.representation(using: .png, properties: [:])!.write(to: URL(fileURLWithPath: CommandLine.arguments[3]))
EOF
if command -v swift >/dev/null && command -v sips >/dev/null; then
  swift "$TMP/render.swift" article/DocumentS1_mmc1.pdf 13 "$TMP/p13.png"
  sips -c 700 900 --cropOffset 3350 2600 "$TMP/p13.png" --out article/FigS7D_overview.png >/dev/null
else
  echo "swift/sips not found: render page 13 of article/DocumentS1_mmc1.pdf (Fig. S7D) by hand" >&2
fi
echo "done"
