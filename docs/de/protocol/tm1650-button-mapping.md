# TM1650 Button-Mapping

Einige Intex-Panels verwenden einen TM1650 LED-Treiber mit Tastatur-Scanning.
Bei diesen Panels ist Button-Mapping zuerst eine Hardware-Durchgangsmessung und
erst danach eine Firmware-Aufgabe.

## TM1650-Pins

| TM1650-Pin | Signal |
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

## Keycode-Tabelle

Finde fuer eine Taste die passende `DIG`-Spalte und `KI`-Zeile. Der
Schnittpunkt ist der TM1650-Keycode.

| KI-Zeile | DIG4 | DIG3 | DIG2 | DIG1 |
| --- | ---: | ---: | ---: | ---: |
| A/KI1 | `0x47` | `0x46` | `0x45` | `0x44` |
| B/KI2 | `0x4F` | `0x4E` | `0x4D` | `0x4C` |
| C/KI3 | `0x57` | `0x56` | `0x55` | `0x54` |
| D/KI4 | `0x5F` | `0x5E` | `0x5D` | `0x5C` |
| E/KI5 | `0x67` | `0x66` | `0x65` | `0x64` |
| F/KI6 | `0x6F` | `0x6E` | `0x6D` | `0x6C` |
| G/KI7 | `0x77` | `0x76` | `0x75` | `0x74` |

## Messablauf

1. Controller spannungsfrei machen.
2. TM1650 Pin 1 auf der Keypad-/Display-Platine markieren.
3. Multimeter auf Widerstands- oder Durchgangsmessung stellen.
4. Eine physische Taste gedrueckt halten.
5. Die zwei TM1650-Matrixpins finden, die durch diese Taste verbunden werden.
6. Einen `DIG`-Pin und einen `KI`-Pin notieren.
7. Dieses Pinpaar mit der Tabelle oben in einen Keycode umwandeln.
8. Fuer jede Taste des Zielmodells wiederholen.

Die TM1650-Dokumentation beschreibt, dass `DIG` und `KI` beim Button-Scanning
ueber ungefaehr 2 kOhm verbunden werden. Je nach Board piept der
Durchgangsmodus daher nicht zuverlaessig; Widerstandsmessung ist oft klarer.

## Mapping-Tabellen

Eine Tabelle fuer die gemessene Hardware ausfuellen.

### Intex 26670

| Taste | DIG-Pin | KI-Pin | Keycode | Bestaetigt |
| --- | --- | --- | --- | --- |
| Power | | | | nein |
| Lock/Unlock | | | | nein |
| Timer | | | | nein |
| Boost | | | | nein |
| Self-clean | | | | nein |

### Intex 26680

| Taste | DIG-Pin | KI-Pin | Keycode | Bestaetigt |
| --- | --- | --- | --- | --- |
| Power | | | | nein |
| Pump | | | | nein |
| Lock/Unlock | | | | nein |
| Chlorinator | | | | nein |
| Boost | | | | nein |
| Self-clean | | | | nein |

## ESPHome-Substitutions

Nach der Messung die Keycodes in `intex-esphome.yaml` eintragen:

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

Fuer Tasten, die am ausgewaehlten Modell nicht existieren, `unmapped`
verwenden.
