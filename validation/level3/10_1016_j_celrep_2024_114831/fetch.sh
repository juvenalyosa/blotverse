#!/usr/bin/env bash
# Re-downloads every file used for 10.1016/j.celrep.2024.114831 (Fig. 5E, deoxyhypusinated eIF5A).
# article/  Europe PMC full text (legends, methods) and an overview crop of the Fig. 5E blot
#           (lane labels = time points, strip labels = conditions; no values)
# images/   raw 16-bit ChemiDoc TIFFs of Fig. 5E, replicates I-III (chemiluminescence,
#           Ponceau S and colorimetric membrane-cutting reference per condition)
# authors/  every file with the authors' numbers (annotators never open this folder)
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images authors
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

get() { # url dest sha256 (sha "-" = page served dynamically, hash not checked)
  case "$1" in *els-cdn.com*) curl -sSL -A "Mozilla/5.0" -o "$2" "$1" ;; *) curl -sSL -o "$2" "$1" ;; esac
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}
check() { echo "$2  $1" | shasum -a 256 -c -; }

# --- article text (Europe PMC XML can change when re-rendered; hash recorded for reference)
get "https://www.ebi.ac.uk/europepmc/webservices/rest/PMC11544350/fullTextXML" article/PMC11544350.xml -   # e35199665f86a3cecef58eb8fa99fb7fb9aa7a3817852c2dce6b0fa0c918d25d

# --- publisher files: both print the plotted per-replicate points / means and p-values -> authors/
#   mmc1 = Supplemental information (Figures S1-S8, Tables S1-S4)
#   mmc2 = article PDF (Figures 1-7)
get "https://ars.els-cdn.com/content/image/1-s2.0-S2211124724011823-mmc1.pdf" authors/1-s2.0-S2211124724011823-mmc1.pdf \
    7b7dbf374160049cbec14e7fa48bc192ef6bb321d60e10cd2e1257da88096b40
get "https://ars.els-cdn.com/content/image/1-s2.0-S2211124724011823-mmc2.pdf" authors/1-s2.0-S2211124724011823-mmc2.pdf \
    1ef6e74c6ba3431cf707317f125479b8ea923f511cdc4c4e266795e14c92b04a

# Overview for annotators: page 10 of the article PDF (Figure 5) rendered at 3x with macOS
# PDFKit, then cropped with sips to the Fig. 5E representative blot only (time labels and
# condition labels, no graph). Derived file; the hash below is what this renderer produced
# (be2c23382b3211a6974ee2780b1151942af395fe59af6e1427ddd0b528a7b10b).
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
swift "$TMP/render.swift" authors/1-s2.0-S2211124724011823-mmc2.pdf 10 "$TMP/page10.png"
sips -c 470 1480 --cropOffset 3290 1600 "$TMP/page10.png" --out article/Fig5E_overview.png >/dev/null

# --- Zenodo deposit 17700326 (10.5281/zenodo.17700326): raw membrane images
ZIP_NAME="Raw membrane images from in vitro deoxyhypusination assays in tiff format.zip"
get "https://zenodo.org/api/records/17700326/files/Raw%20membrane%20images%20from%20in%20vitro%20deoxyhypusination%20assays%20in%20tiff%20format.zip/content" \
    "$TMP/raw.zip" 9025c05e7797cd4bc4fa18901c86e918918734baba87372402a2bfb2f5f7ec66
# Only the Fig. 5E folder (Replicate I, II, III); Fig. 5F and Fig. S5E images are not used.
unzip -j -o -q "$TMP/raw.zip" "Time course in vitro eIF5A deoxyhypusination assay determining DHPS inhibition by ERK2 related to Figure 5_E/*" -d images

check "images/I_Chemiluminescence_from the top control ERK2 BSA.raw16.tif"      acea8f31b3d39cfa9d95d86c0640b432727a8c2701e87f19a7d07228712ad5e0
check "images/I_control_Ponceau S.raw16.tif"                                    3cd2f4feca8e972e7a757c39959a18f93487c7efef21c88bc53da11ef0e9a9d0
check "images/I_ERK2_Ponceau S.raw16.tif"                                       20495f33df0ccc45b25e43ebe9b81c8235485c8e94852518d6a5210bf8adca06
check "images/I_BSA_Ponceau S.raw16.tif"                                        68efd13ec7f4d32e1fcf988f64a870b88a5f81a234b3381785463fb1c57b2802
check "images/I_control_Colorimetric_membrane cutting reference_.raw16.tif"     d7a944d2bba8f6391f8ef7e8aa0a0f617871155ded26024e728f86287cebb71f
check "images/I_ERK2_Colorimetric_membrane cutting reference.raw16.tif"         f3070265187a4631bcf209646e5f8c5d2a4c2f9729d70fdf2b5b1f587ccd6c33
check "images/I_BSA_Colorimetric_membrane cutting reference.raw16.tif"          19ae2566a47184b92cd3b18632e51df1a276e8fb941a164c2bc11e4530aedba6
check "images/II_Chemiluminescence_from the top control ERK2 BSA.raw16.tif"     e1a6f2a45a344560322c9781ec513a5cd6e189fb1edbbe1582115a135b5c1210
check "images/II_control_Ponceau S.raw16.tif"                                   544cd47406a170543a720aeaf08b19d0306bb9b26d340cebb3187e09dcc14a73
check "images/II_ERK2_Ponceau S.raw16.tif"                                      63f6ca5059d32aa6d94e5c92497c2acd54227041a17e075095e37d87adaa4b9e
check "images/II_BSA_Ponceau S.raw16.tif"                                       53c3d32c764c19e418f3f89c2f2421d605e8400b313b227926c64d74653cdd17
check "images/II_control_Colorimetric_membrane cutting reference.raw16.tif"     d97d83c3521a5c1219e08c7ca38ee40088a00ef601203b18fe0233d7bcb345af
check "images/II_ERK2_Colorimetric_membrsne cutting reference.raw16.tif"        b5a3b8362c58d692201bd1af6c5ecdea98b8b696e340deb55b4febe282f19825
check "images/II_BSA_Colorimetric_membrane cutting reference.raw16.tif"         cd729a5ed097b8f1cd1cdba5ec03fa7c2c3126fb0d65df8d0a8e9331861aa1f5
check "images/III_Chemiluminescence_from the top control ERK2 BSA.raw16.tif"    822df003746859cf125892ec3bf62d7aa40b47009e69ff77483b4aff0eaed9fc
check "images/III_control_Ponceau S.raw16.tif"                                  5307f11aae42d9e5286821418da8c67f7913521eff00a912f53acea55c86fe66
check "images/III_ERK2_Ponceau S.raw16.tif"                                     5525d99753c4d841c9b8cf0babaf00395d38e36aa719dff073458f37d03c9545
check "images/III_BSA_Ponceau S.raw16.tif"                                      1a578d539ff6a23d23c570e63d76546b4ea0ae1dd3505e54712df434b6573c0a
check "images/III_control_Colorimetric_membrane cutting reference.raw16.tif"    5de550a0792c0294cd1323e78956cd7e73fbd23a7d41a6c77f0611318383e320
check "images/III_ERK2_Colorimetric_membrane cutting reference.raw16.tif"       4ea658022b758172cc5be8784710989022dfca3ab51dec67c44dae8c281f512c
check "images/III_BSA_Colorimetric_membrane cutting reference.raw16.tif"        5c35f094cd88a062649b10e4469e8ffe2a7dd43a71bb3dcba23c4eecd16338e4

# No source-data table exists for any western blot (article, supplement and deposit checked);
# the second Zenodo file (MST curves, txt) is not western blot data and is not downloaded.
