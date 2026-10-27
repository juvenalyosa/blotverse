#!/usr/bin/env bash
# Re-downloads every file used for 10.1038/s41467-026-71753-w (Fig. 1e, gammaH2AX).
# article/  article text, legends, overview figure (Supplementary Fig. 11, page 13 of the SI)
# images/   raw Odyssey CLx images of Blot 66 (target strip and total-protein stain)
# authors/  every file with the authors' numbers (annotators never open this folder)
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images authors
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

get() { # url dest sha256 (sha "-" = page served dynamically, hash not checked)
  # nature.com needs a browser user agent; zenodo.org refuses one (403), so only send it to nature/springer
  case "$1" in *nature.com*|*springernature.com*) curl -sSL -A "Mozilla/5.0" -o "$2" "$1" ;; *) curl -sSL -o "$2" "$1" ;; esac
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}
check() { echo "$2  $1" | shasum -a 256 -c -; }

# --- article (HTML and Europe PMC XML can change when re-rendered; hashes recorded for reference)
get "https://www.nature.com/articles/s41467-026-71753-w" article/article.html -   # fec60496e64f0dc93890476698734dfa2f59c97d8017dd3438a1091479fe7fce
get "https://www.ebi.ac.uk/europepmc/webservices/rest/PMC13254281/fullTextXML" article/fulltext.xml -   # 2ec158faa350849f4e9c5b0ce19c8ba40592cebc251f4cf0195d5264bebaf457

# --- Supplementary Information PDF: prints per-replicate dot plots and p-values -> authors/
SI_URL="https://media.springernature.com/original/springer-static/esm/art%3A10.1038%2Fs41467-026-71753-w/MediaObjects/41467_2026_71753_MOESM1_ESM.pdf"
get "$SI_URL" authors/41467_2026_71753_MOESM1_ESM.pdf d9b31ab4c63ca33a99724c9ed811d19c5a83e6029b3f5282b6f1ec7cb3cd4188

# Overview figure for annotators: SI page 13 (Supplementary Fig. 11, cropped blots with
# lane labels, no values) rendered at 3x with macOS PDFKit. Derived file; the hash below is
# what this renderer produced (2e2446d638e4014f7504b89173d86424a6dcf5846b9bf722ed64b5f1ae23b556).
cat > "$TMP/render.swift" <<'EOF'
import PDFKit
import AppKit
let d = PDFDocument(url: URL(fileURLWithPath: CommandLine.arguments[1]))!
let p = d.page(at: Int(CommandLine.arguments[2])! - 1)!
let r = p.bounds(for: .mediaBox); let sc: CGFloat = 3
let img = NSImage(size: NSSize(width: r.width*sc, height: r.height*sc))
img.lockFocus(); NSColor.white.set(); NSRect(x:0,y:0,width:r.width*sc,height:r.height*sc).fill()
let ctx = NSGraphicsContext.current!.cgContext; ctx.scaleBy(x: sc, y: sc); p.draw(with: .mediaBox, to: ctx); img.unlockFocus()
let rep = NSBitmapImageRep(data: img.tiffRepresentation!)!
try! rep.representation(using: .png, properties: [:])!.write(to: URL(fileURLWithPath: CommandLine.arguments[3]))
EOF
swift "$TMP/render.swift" authors/41467_2026_71753_MOESM1_ESM.pdf 13 article/SupplementaryFig11_overview.png

# Main Fig. 1 (prints per-replicate points and p-values) -> authors/
get "https://media.springernature.com/full/springer-static/image/art%3A10.1038%2Fs41467-026-71753-w/MediaObjects/41467_2026_71753_Fig1_HTML.png" \
    authors/Fig1_HTML.png 955d91dc391c9e37925b4259d6674b68336494cc35520f103944c6939f199dfd

# --- Zenodo deposit 10.5281/zenodo.19122404
get "https://zenodo.org/api/records/19122404/files/Western_Blot.zip/content" "$TMP/Western_Blot.zip" \
    fe2b16099029ec384435b7ba393c6b27ef831bc3706741ce4611d4ab031cc357
get "https://zenodo.org/api/records/19122404/files/Source_Data.zip/content" "$TMP/Source_Data.zip" \
    92ac4f6ed432ca7969312ece5b6757bbc948b53ed43c566e024cb6b2f092df5f

# Raw images of Blot 66 (Image Studio project "WS RGAM D0")
#   0000817_02 = acquisition "Blot 66 - STAT1, PCNA, pH2AX" (gammaH2AX = bottom strip, 800 channel)
#   0000807_01 = acquisition "Blot 66 - TPS" (total protein stain, 700 channel)
#   (0000817_03 is a byte-identical duplicate of 0000817_02 and is not used)
IMG="Image_Studio_Files/WS RGAM D0/Images"
unzip -j -o -q "$TMP/Western_Blot.zip" "$IMG/0000817_02/0000817_02_800.TIF" "$IMG/0000817_02/0000817_02_700.TIF" \
    "$IMG/0000807_01/0000807_01_700.TIF" -d images
check images/0000817_02_800.TIF 0e83010dc46348dcf1b1a2bf64447b358da8849667409150a0436a72cc296e37
check images/0000817_02_700.TIF 5ad0f8203e335c1803c02b2632508944e7309fa568aadfeb3c8a981043a8eaeb
check images/0000807_01_700.TIF 286ead1bbe2a2525432137084a47911901cc11acd3f41d9334f4e8a39437bb22

# Authors' numbers: Image Studio database (per-shape signals), Image Studio exports, results workbook
unzip -j -o -q "$TMP/Western_Blot.zip" "Image_Studio_Files/WS RGAM D0/WS RGAM D0.db" \
    "Results/DW_RGAM_0d/WS_D0_RGAM_Blot66_pH2AX.xls" "Results/DW_RGAM_0d/WS_D0_RGAM_Blot66_TPS.xls" \
    "Results/DW_RGAM_0d/WS_D0_RGAM_Results.xlsx" -d authors
check "authors/WS RGAM D0.db" b41075cd2e9f8544ba0ff93061a3ccb161523297b72642e6ddc6763edaab474f
check authors/WS_D0_RGAM_Blot66_pH2AX.xls a6c80ff95a7cad2da0c9aa858e34dc1b508bb3f28cebbe586e6084338be317f9
check authors/WS_D0_RGAM_Blot66_TPS.xls b75e35ff63924d2e4c2a7c8a43375fc236fe4555b5460d648b1c6badf05c451d
check authors/WS_D0_RGAM_Results.xlsx 04c9b1631037e2be0c069aab341c530ad102e23ce2f0fe76fdfedfe926783a9f

unzip -j -o -q "$TMP/Source_Data.zip" Source_Data_Fig1_R2.xlsx -d authors
check authors/Source_Data_Fig1_R2.xlsx c002482e31e36ef07145788d3ccb6b0899b30fba2312ad43124717c764d6b5b9
