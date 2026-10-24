// E6 - Fiji reference measurements for the BlotVerse default background branch.
//
// For every analysed experiment directory (containing lanes.csv from
// `blotverse run`), this macro opens each blot image, converts it to 32-bit,
// runs Process > Subtract Background with radius = factor x band height
// (branch rb<factor>, smoothing on, dark background) and measures the raw
// integrated density of every band window listed in lanes.csv.
//
// Usage (headless):
//   fiji --headless -macro e6_fiji.ijm "<out_dir>|<image_dir>[|<radius_factor>]"
// radius_factor defaults to 2 (branch rb2). Writes <out_dir>/fiji_rb<factor>_rect.csv

args = split(getArgument(), "|");
outDir = args[0];
imageDir = args[1];
factor = 2;
if (args.length > 2) factor = parseInt(args[2]);
lines = split(File.openAsString(outDir + File.separator + "lanes.csv"), "\n");
header = split(lines[0], ",");
col = newArray(0);
for (i = 0; i < header.length; i++) col = Array.concat(col, header[i]);

function column(name) {
  for (i = 0; i < col.length; i++) if (col[i] == name) return i;
  exit("missing column " + name);
}

cImage = column("image"); cBlot = column("blot"); cLane = column("lane"); cBand = column("band");
cX0 = column("x0"); cY0 = column("y0"); cX1 = column("x1"); cY1 = column("y1"); cH = column("band_height");

setBatchMode(true);
result = "blot,lane,band,fiji_rb" + factor + "_rect\n";
current = "";
for (r = 1; r < lines.length; r++) {
  if (lengthOf(lines[r]) == 0) continue;
  f = split(lines[r], ",");
  if (f[cImage] != current) {
    if (current != "") close();
    open(imageDir + File.separator + f[cImage]);
    run("32-bit");
    radius = factor * parseInt(f[cH]);
    run("Subtract Background...", "rolling=" + radius);
    current = f[cImage];
  }
  x0 = parseInt(f[cX0]); y0 = parseInt(f[cY0]);
  makeRectangle(x0, y0, parseInt(f[cX1]) - x0, parseInt(f[cY1]) - y0);
  List.setMeasurements;
  result = result + f[cBlot] + "," + f[cLane] + "," + f[cBand] + "," + d2s(List.getValue("RawIntDen"), 4) + "\n";
}
if (current != "") close();
File.saveString(result, outDir + File.separator + "fiji_rb" + factor + "_rect.csv");
