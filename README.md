# Intex ESPHome

![Intex ESPHome controller](docs/images/intex-pump-controller.jpg)

All-in-one ESPHome project for Intex 26670 and Intex 26680 control-panel
interface work.

The project keeps the original controller and display/keypad board in place,
adds an ESP32 interface board between them, and starts in a passive diagnostic
mode. Active virtual button control should only be enabled after the real
button matrix and bus behavior have been measured on the target hardware.

## Project Status

This is an early hardware and firmware baseline, not a finished automation
firmware.

Open tasks:

- measure the panel connector voltage levels
- confirm connector pinout and signal direction
- confirm TM1650 SCL/SDA/VDD/GND wiring on the target panel
- capture real traffic with a logic analyzer
- map the button keycodes for the selected model
- map display and status LED frames
- adapt the KiCad board under `hardware/panel-interface-board`

## Model Selection

Use one YAML file for both supported models:

```yaml
substitutions:
  target_model: "26680"
```

Supported values:

| Model | Panel buttons |
| --- | --- |
| `26670` | Power, Lock/Unlock, Timer, Boost, Self-clean |
| `26680` | Power, Pump, Lock/Unlock, Chlorinator, Boost, Self-clean |

The model substitution currently changes naming and diagnostics only. Protocol
decoding and button injection stay disabled until the measured keycodes are
added.

## Default ESPHome Firmware

The default YAML requires ESPHome 2026.4.0 or newer and exposes:

- WiFi, API, OTA, captive portal and web server
- device status
- WiFi signal and uptime diagnostics
- selected target model
- measured keycode placeholders
- raw GPIO line-state diagnostics for four candidate bridge lines
- disabled virtual button placeholders for later control work
- a passive TM1650 sniffer loaded as an external component from this repository
- ESP32 SRAM1-as-IRAM support on classic ESP32 targets

The only external ESPHome component is `tm1650_sniffer`, loaded from this
GitHub repository through `external_components`.

## Keycode Mapping

For TM1650-style panels, the next step is a continuity map from each physical
button to the TM1650 matrix pins. The mapping workflow and keycode table are in
[TM1650 button mapping](docs/protocol/tm1650-button-mapping.md).

Current status can also be read from TM1650 display/LED writes once the bus is
passively decoded. The capture plan is in
[TM1650 status sniffing](docs/protocol/tm1650-status-sniffing.md).

Non-TM1650 pulse-protocol material is kept as reference only:

- [Intex 26670 pulse reference](docs/protocol/pulse-reference/intex-26670-pulse-protocol.md)
- [Intex 26680 pulse reference](docs/protocol/pulse-reference/intex-26680-pulse-protocol.md)
- [Reference Arduino demos](docs/protocol/pulse-reference/reference-arduino-demos/README.md)

The 26680-specific panel variant notes are in
[Intex 26680 panel variants](docs/protocol/intex-26680-panel-variants.md).

## Hardware Safety

Pool equipment mixes mains voltage, water, pumps and chemical generation. Be
careful.

- Do not connect the ESP32 directly to unknown voltage levels.
- Measure the control-panel bus before wiring anything permanently.
- Use level shifting or isolation when needed.
- Keep mains wiring separate from low-voltage electronics.
- Keep the manual controls available during testing.
- Do not run unattended automation until state mapping is verified.

## Hardware Direction

The intended panel interface path is:

```text
Original controller board <-> ESP32 interface board <-> Original display/keypad board
```

Default diagnostic pins:

| Signal | ESP32 GPIO | Purpose |
| --- | ---: | --- |
| Controller Clock Candidate | 19 | Candidate TM1650 clock from controller side |
| Controller Data Candidate | 18 | Candidate TM1650 data from controller side |
| Panel Clock Candidate | 17 | Candidate forwarded/display-side clock |
| Panel Data Candidate | 16 | Candidate forwarded/display-side data |

See [panel bridge board notes](docs/hardware/panel-bridge-board.md) for the
hardware bring-up notes.

## Build And Flash

Prerequisites:

- ESPHome
- Python environment supported by your ESPHome installation
- ESP32 development board

Create your local secrets file:

```bash
cp secrets.yaml.example secrets.yaml
```

Edit `secrets.yaml`, set `target_model` in `intex-esphome.yaml`, then compile:

```bash
esphome compile intex-esphome.yaml
```

Flash over USB:

```bash
esphome run intex-esphome.yaml
```

After the first flash, OTA updates can be performed with the same command when
the device is online.

## Repository Layout

```text
intex-esphome.yaml             All-in-one ESPHome device configuration
secrets.yaml.example           Example local WiFi secrets
docs/hardware/                 Hardware bring-up notes
docs/images/                   Project images
docs/protocol/                 Protocol notes and capture plans
docs/protocol/pulse-reference/ Non-TM1650 reference material
docs/reference/                Reference documents and datasheets
hardware/panel-interface-board/  KiCad panel interface board reference
.github/social-preview.jpg     GitHub social preview image source
```

## Credits

This project builds on excellent prior Intex automation work by
[`tonyflores1006/intex-swg-iot`](https://github.com/tonyflores1006/intex-swg-iot)
and
[`jingsno/intex-swg-pcb`](https://github.com/jingsno/intex-swg-pcb).
See [credits](docs/credits.md) for details.

## License

This project is licensed under the GNU General Public License v3.0. See
[LICENSE](LICENSE).
