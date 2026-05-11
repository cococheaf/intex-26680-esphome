# Panel Bridge Board Notes

This branch targets a panel interface board for the Intex 26670.

The board should sit between the original 26670 controller board and the
original 26670 display/keypad board. The first revision should be transparent:
it should let the original signals pass while making those signals available to
an ESP32 for measurement and later decoding.

## Target Topology

```text
Original 26670 controller board <-> ESP32 interface board <-> Original 26670 display/keypad board
```

The ESP32 side should eventually be able to:

- observe display and status traffic from the controller
- observe button traffic from the keypad
- forward the original signals with minimal delay
- publish decoded state through ESPHome after the protocol is verified
- inject button events only after passive forwarding is stable

## Candidate Signal Layout

The current board reference uses four low-voltage logic paths:

| Direction | Working name | ESP32 GPIO in reference | Purpose |
| --- | --- | ---: | --- |
| Controller board -> ESP32 | `FROM_CONTROLLER` | 18 | Receive display/status data |
| ESP32 -> Display board | `TO_PANEL` | 16 | Forward display/status data |
| Display board -> ESP32 | `FROM_PANEL` | 17 | Receive keypad data |
| ESP32 -> Controller board | `TO_CONTROLLER` | 19 | Forward keypad data |

The default ESPHome YAML currently exposes only the two receive-side line states
for safe bring-up:

| Diagnostic entity | ESP32 GPIO |
| --- | ---: |
| Controller To Panel Line | 18 |
| Panel To Controller Line | 17 |

## Electrical Design Checks

Before a permanent board:

- measure the 26670 panel connector voltage levels
- confirm idle levels and pull-up locations
- confirm whether either side actively drives high
- decide whether level shifters, series resistors or digital isolators are
  needed
- provide a simple bypass path for early hardware tests
- avoid boot-sensitive ESP32 pins for relays or critical bus lines

## KiCad Reference

The license-free KiCad reference lives under:

- `hardware/intex-26670-panel-board`

It is the starting point for the 26670-specific PCB adaptation.
