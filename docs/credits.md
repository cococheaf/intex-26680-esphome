# Credits

This project builds on excellent prior work from the Intex pool automation
community.

## tonyflores1006/intex-swg-iot

Repository:

- https://github.com/tonyflores1006/intex-swg-iot

The `tonyflores1006/intex-swg-iot` project demonstrated the core idea of
placing an ESP32 between the original Intex controller board and the original
control panel, then using that position to observe and automate the device.

That work is one of the foundations for `intex-esphome`. It helped shape the
overall interface-board direction, the ESP32 bridge approach and the early
thinking around reading panel state and forwarding or injecting button events.

## jingsno/intex-swg-pcb

Repository:

- https://github.com/jingsno/intex-swg-pcb

The `jingsno/intex-swg-pcb` project provided valuable PCB, protocol and capture
reference material. The reference board files and pulse-protocol notes in this
repository are based on that work and were reorganized here for the
`intex-esphome` project structure.

That work is especially important for the hardware bring-up notes, the panel
interface board reference and the non-TM1650 pulse-protocol reference material
kept under `docs/protocol/pulse-reference`.

## Attribution Notes

`intex-esphome` focuses on an ESPHome-based, all-in-one path for Intex 26670 and
Intex 26680 panel interface work. The project would not be starting from such a
useful position without the investigations, code, PCB files and documentation
published by the projects above.

If you use this repository, please also credit those upstream authors where
their work informed your hardware, firmware or documentation.
