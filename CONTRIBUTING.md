# Contributing to Intex ESPHome

**English** | [Deutsch](CONTRIBUTING.de.md)

Thank you for helping improve this project.

This firmware observes and may later control real pool equipment through an
ESP32. Good contributions are careful, practical and easy to verify on
hardware.

## Project Goals

The project aims to:

- understand the Intex 26670 and Intex 26680 display/keypad bus variants
- provide reliable ESP32-based state reading
- support safe virtual button injection after mappings are verified
- expose useful ESPHome entities for Home Assistant
- keep the firmware understandable and maintainable
- document hardware findings clearly enough that others can reproduce them

## How To Contribute

Useful contributions include:

- bus captures from Intex 26670 or Intex 26680 hardware
- verified TM1650 button matrix mappings
- verified pulse-protocol button mappings
- verified LED/status mappings
- documentation improvements
- ESPHome compatibility updates
- safer GPIO and relay handling
- bug fixes and test reports

Small focused pull requests are preferred over large mixed changes.

## Hardware Reports

When reporting hardware behavior, please include:

- exact Intex model and region if known
- board photos if they are relevant
- ESP32 board type
- wiring diagram or pin mapping
- measured bus voltage
- logic analyzer sample rate
- decoded bytes or capture files
- what button was pressed or which display state was visible
- firmware commit used for the test

Please avoid conclusions based only on assumptions from another Intex model.

## Bug Reports

A good bug report should include:

- a clear description of the problem
- steps to reproduce it
- expected behavior
- actual behavior
- serial monitor logs
- API responses if relevant
- hardware wiring details
- ESPHome version
- firmware commit

For timing or protocol issues, include a logic analyzer capture when possible.

## Pull Requests

Before opening a pull request, please check that:

- the change has a clear purpose
- hardware assumptions are documented
- unrelated cleanup is not mixed with behavior changes
- public API changes are described
- README or docs are updated when needed
- ESPHome compile still works in your environment
- safety-sensitive changes are explained

## Development Guidelines

Please follow the style already used in the firmware unless a cleanup is part of
the actual change.

- Keep timing-sensitive bus code simple.
- Avoid hidden side effects in shared global state.
- Prefer named constants over magic bytes.
- Keep comments focused on non-obvious protocol or timing behavior.
- Do not move GPIO assignments without documenting the hardware reason.
- Do not add automation behavior that can unexpectedly run pool equipment.

## Safety Considerations

This firmware can affect pumps, chlorination and power switching.

Changes should avoid:

- bypassing manual controls
- assuming every Intex model uses the same protocol
- running equipment without verified flow or state information
- making relay outputs active by surprise
- hiding bus errors behind optimistic status values
- hardcoding hardware behavior that should be measured first

When in doubt, prefer read-only diagnostics over control behavior.

## Commit Messages

Use clear commit messages. Examples:

```text
docs: document 26680 keypad mapping workflow
fix: release SDA after virtual key response
feat: expose raw bus frames as diagnostics
refactor: name TM1650 command constants
build: update ESPHome validation
```

## Testing

Depending on the change, useful testing includes:

- `esphome config intex-esphome.yaml`
- `esphome compile intex-esphome.yaml`
- flashing to an ESP32 test board
- checking serial monitor output
- checking the ESPHome API entities in Home Assistant
- checking the ESPHome web server when enabled
- comparing decoded bytes with a logic analyzer trace
- testing WiFi fallback AP provisioning
- checking OTA upload on a non-critical test device

Describe what you tested in the pull request.

## License

This repository is licensed under the GNU General Public License v3.0. Do not
add third-party code unless its license is compatible with GPL-3.0 and the
source is documented.

When work is derived from or materially informed by another project, add or
update attribution in [credits](docs/credits.md).

## Questions

If you are unsure whether a change fits, open an issue first and describe the
hardware setup, the problem and the proposed approach.
