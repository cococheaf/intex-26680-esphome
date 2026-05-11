# TM1650 Button Mapping

Some Intex panels use a TM1650 LED driver with keyboard scanning. For those
panels, button mapping is a hardware continuity task before it is a firmware
task.

## TM1650 Pins

| TM1650 pin | Signal |
| ---: | --- |
| 1 | DIG1 |
| 5 | DIG2 |
| 6 | DIG3 |
| 7 | DIG4 |
| 8 | A/KI1 |
| 9 | B/KI2 |
| 11 | C/KI3 |
| 12 | D/KI4 |
| 13 | E/KI5 |
| 14 | F/KI6 |
| 15 | G/KI7 |
| 2 | SCL |
| 3 | SDA |
| 4 | GND |
| 10 | VDD |
| 16 | DP/KP |

## Keycode Table

Find the `DIG` column and `KI` row for a button. Their intersection is the
TM1650 keycode.

| KI row | DIG4 | DIG3 | DIG2 | DIG1 |
| --- | ---: | ---: | ---: | ---: |
| A/KI1 | `0x47` | `0x46` | `0x45` | `0x44` |
| B/KI2 | `0x4F` | `0x4E` | `0x4D` | `0x4C` |
| C/KI3 | `0x57` | `0x56` | `0x55` | `0x54` |
| D/KI4 | `0x5F` | `0x5E` | `0x5D` | `0x5C` |
| E/KI5 | `0x67` | `0x66` | `0x65` | `0x64` |
| F/KI6 | `0x6F` | `0x6E` | `0x6D` | `0x6C` |
| G/KI7 | `0x77` | `0x76` | `0x75` | `0x74` |

## Measurement Workflow

1. Disconnect power from the controller.
2. Mark TM1650 pin 1 on the keypad/display board.
3. Put the multimeter in resistance or continuity mode.
4. Press and hold one physical button.
5. Find the two TM1650 matrix pins connected by that button.
6. Record one `DIG` pin and one `KI` pin.
7. Convert that pair to a keycode using the table above.
8. Repeat for every button on the target model.

The TM1650 documentation notes that `DIG` and `KI` are connected through
roughly 2 kOhm during button scanning. Depending on the board, continuity mode
may not beep reliably; resistance mode is often clearer.

## Mapping Tables

Fill one table for the hardware being measured.

### Intex 26670

| Button | DIG pin | KI pin | Keycode | Confirmed |
| --- | --- | --- | --- | --- |
| Power | | | | no |
| Lock/Unlock | | | | no |
| Timer | | | | no |
| Boost | | | | no |
| Self-clean | | | | no |

### Intex 26680

| Button | DIG pin | KI pin | Keycode | Confirmed |
| --- | --- | --- | --- | --- |
| Power | | | | no |
| Pump | | | | no |
| Lock/Unlock | | | | no |
| Chlorinator | | | | no |
| Boost | | | | no |
| Self-clean | | | | no |

## ESPHome Substitutions

After measuring, copy the keycodes into `intex-esphome.yaml`:

```yaml
substitutions:
  keycode_power: "0x00"
  keycode_pump: "0x00"
  keycode_lock_unlock: "0x00"
  keycode_chlorinator: "0x00"
  keycode_timer: "0x00"
  keycode_boost: "0x00"
  keycode_self_clean: "0x00"
```

Use `unmapped` for buttons that do not exist on the selected model.
