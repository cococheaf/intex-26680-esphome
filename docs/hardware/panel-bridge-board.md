# Panel Bridge Board Notes

This project targets an ESP32 panel interface board for Intex 26670 and Intex
26680 controllers.

The board should sit between the original controller board and the original
display/keypad board. The first revision should be transparent: it should let
the original signals pass while making those signals available to an ESP32 for
measurement and later decoding.

## Target Topology

```text
Original controller board <-> ESP32 interface board <-> Original display/keypad board
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
| Controller board -> ESP32 | `CONTROLLER_CLOCK` | 19 | Candidate TM1650 clock from controller side |
| Controller board -> ESP32 | `CONTROLLER_DATA` | 18 | Candidate TM1650 data from controller side |
| Display board -> ESP32 | `PANEL_CLOCK` | 17 | Candidate forwarded/display-side clock |
| Display board -> ESP32 | `PANEL_DATA` | 16 | Candidate forwarded/display-side data |

For pulse-protocol panels, only two of these lines may be meaningful. Confirm
the real signal roles with a continuity check and logic capture.

## Electrical Design Checks

Before a permanent board:

- measure the panel connector voltage levels
- confirm idle levels and pull-up locations
- confirm whether either side actively drives high
- decide whether level shifters, series resistors or digital isolators are
  needed
- provide a simple bypass path for early hardware tests
- avoid boot-sensitive ESP32 pins for relays or critical bus lines

## KiCad Reference

The KiCad reference lives under:

- `hardware/panel-interface-board`

It is the starting point for a shared 26670/26680 PCB adaptation.
