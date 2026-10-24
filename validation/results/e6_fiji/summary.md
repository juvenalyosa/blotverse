# E6: BlotVerse rolling ball vs Fiji

Fiji `fiji-2.18.1-SNAPSHOT.jar`, ImageJ `ij-1.54p.jar` (macOS arm64), run headless with `validation/fiji/run_e6.sh`.
10 simulated blot sets per artefact level (2 gels x 3+3 lanes, target and reference band: 240 bands).
Measure: raw integrated density of each band window after Process > Subtract Background on the 32-bit image.

## Level: clean

```
branch rb1_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
branch rb2_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
branch rb4_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
```

## Level: moderate

```
branch rb1_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
branch rb2_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
branch rb4_rect: bands compared: 240
Pearson r = 1.00000  (target >= 0.99: PASS)
median relative difference = 0.0000%, max = 0.0000%
```

Target r >= 0.99 (plan E6 / gate G1): met for every branch and level.
