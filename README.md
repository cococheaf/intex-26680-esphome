# Intex 26670 ESPHome

![Intex 26670 ESPHome controller](docs/images/intex-pump-controller.jpg)

ESPHome project for an Intex 26670 panel interface board.

The goal of this branch is a clean 26670 starting point: keep the original
controller and original display/keypad board in place, add an ESP32 interface
board between them, and use that board to observe the panel lines before any
control logic is added.

## Project Status

This is an early hardware and firmware baseline, not a finished automation
firmware.

Open 26670 tasks:

- measure the panel connector voltage levels
- confirm connector pinout and signal direction
- capture real 26670 traffic with a logic analyzer
- confirm whether the panel uses TM1650-style traffic or the documented pulse
  protocol
- map the 26670 button frames
- map the 26670 status LED frames
- adapt the KiCad board under `hardware/intex-26670-panel-board`

## Default ESPHome Firmware

The default YAML intentionally uses ESPHome-native components only. It exposes:

- WiFi, API, OTA, captive portal and web server
- device status
- WiFi signal and uptime diagnostics
- raw GPIO line-state diagnostics for the two panel communication directions

Protocol decoding is not enabled in this branch yet. The first firmware target
is a stable bring-up image that can sit on the bench while the hardware and bus
levels are verified.

## 26670 Control Panel

The documented 26670-style panel has five physical buttons:

| Button | Function |
| --- | --- |
| Power | Main on/off button |
| Lock/Unlock | Panel lock button |
| Timer | Runtime/program timer button |
| Boost | Boost mode button |
| Self-clean | Self-clean mode button |

The known related panel has these status LEDs:

| LED | Meaning |
| --- | --- |
| Boost | Boost mode is active |
| Sleep | Sleep mode is active |
| Working | Active output state |
| Ozone | Ozone indicator on related boards |
| Pump low flow | Pump flow is too low |
| Low salt | Salt level is too low |
| High salt | Salt level is too high |
| Service | Service/error state |

## Hardware Safety

Pool equipment mixes mains voltage, water, pumps and chemical generation. Be
careful.

- Do not connect the ESP32 directly to unknown voltage levels.
- Measure the control-panel bus before wiring anything permanently.
- Use level shifting or isolation when needed.
- Keep mains wiring separate from low-voltage electronics.
- Keep the manual controls available during testing.
- Do not run unattended automation until the 26670 state mapping is verified.

## Hardware Direction

The intended panel interface path is:

```text
Original 26670 controller board <-> ESP32 interface board <-> Original 26670 display/keypad board
```

Current default diagnostic pins:

| Signal | ESP32 GPIO | Direction |
| --- | ---: | --- |
| Controller To Panel Line | 18 | Original controller -> ESP32 |
| Panel To Controller Line | 17 | Original panel -> ESP32 |

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

Edit `secrets.yaml`, then compile:

```bash
esphome compile intex-26670-esphome.yaml
```

Flash over USB:

```bash
esphome run intex-26670-esphome.yaml
```

After the first flash, OTA updates can be performed with the same command when
the device is online.

## Repository Layout

```text
intex-26670-esphome.yaml       ESPHome device configuration
secrets.yaml.example           Example local WiFi secrets
docs/hardware/                 Hardware bring-up notes
docs/images/                   Project images
docs/protocol/                 Protocol notes and capture plans
docs/reference/                Reference documents and datasheets
hardware/intex-26670-panel-board/  KiCad panel interface board reference
.github/social-preview.jpg     GitHub social preview image source
```

## Related Branches

- `intex-26670`: this branch, focused on the Intex 26670.
- `intex-26680`: separate branch for the Intex 26680 panel protocol and board
  adaptation.
