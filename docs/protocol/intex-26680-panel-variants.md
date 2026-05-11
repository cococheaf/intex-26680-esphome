# Intex 26680 Panel Variants

The 26680 panel must be treated as a measured target, not as a fixed known
interface. Community notes point to at least two related panel families:

- a 4-pin panel connector with a 16-pin TM1650-style LED/key driver
- a pulse-protocol panel without confirmed TM1650 traffic

Both variants can look similar from the outside. The first hardware step is to
identify which variant is present before implementing any active control.

## Variant A: TM1650-Style Panel

Expected signs:

- 4-pin connector between controller and keypad/display board
- 16-pin driver IC near the display/key matrix
- two bus lines that may behave like clock and data

The likely ESP32 signal split used by related projects is:

| Signal role | Candidate ESP32 GPIO |
| --- | ---: |
| Controller clock/data line 1 | 19 |
| Controller clock/data line 2 | 18 |
| Display-side clock/data line 1 | 17 |
| Display-side clock/data line 2 | 16 |

These GPIOs are only historical candidates. Confirm the real 26680 wiring with
a continuity check and logic capture before using them.

For this variant, button support is not just a pin mapping problem. Each
physical button needs a decoded key value and a matching ESPHome/Home Assistant
action. Related example code only handled a subset of buttons, so the 26680
buttons must be captured individually:

- Power
- Pump
- Lock/Unlock
- Chlorinator
- Boost
- Self-clean

## Variant B: Pulse-Protocol Panel

Expected signs:

- 4-pin connector between controller and keypad/display board
- no confirmed TM1650 bus traffic
- pulse-width encoded traffic for display/status and button states

For this variant, use the capture checklist in
`docs/protocol/intex-26680-pulse-protocol.md`.

## Bring-Up Decision Tree

1. Photograph both sides of the keypad/display board.
2. Identify the display/key driver package and markings.
3. Check continuity from the 4-pin connector to the driver IC pins.
4. Measure idle voltage on each connector pin.
5. Capture power-up traffic with a logic analyzer.
6. Press and release each button while capturing both signal lines.
7. Classify the panel as TM1650-style, pulse-protocol, or unknown.

Only after this classification should the firmware grow beyond passive line
diagnostics.
