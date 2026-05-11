# Intex Panel Interface Board

KiCad reference files for a shared Intex 26670/26680 ESP32 panel interface
board.

This hardware is a starting point and still needs adaptation for the exact
connector, enclosure and signal levels of the target controller.

## Files

- `pcb/` contains the KiCad project and board files.
- `docs/panel-interface-board.pdf` contains the schematic export.
- `docs/schematic.png` contains a schematic preview.
- `docs/poster.png` contains a board preview.
- `protocol/` contains pulse-protocol reference notes and Arduino capture demos
  from the reference board.

## Parts List

| Part | Notes |
| --- | --- |
| ESP32-DevkitC external antenna | ESP32 controller module |
| IRM-03-5 | 5 V power module |
| 5 V / 3.3 V level shifter | Logic-level translation |
| 3.3 V relay module | Optional relay output |
| XH2.54 4-pin connectors | Panel/controller wiring |
| Fuse 3 A / 250 V | Power protection |
| Screw terminals | Power and relay wiring |
| Jumper headers | Bring-up and bypass options |
