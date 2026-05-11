# TM1650 Status-Sniffing

Ja, der ESP32 kann den aktuellen Controller-Status von einem TM1650-basierten
Panel lesen. Dazu lauscht der ESP32 passiv auf den TM1650-Leitungen `SCL` und
`SDA`, waehrend der originale Controller Display- und LED-Daten schreibt.

Das ist etwas anderes als Button-Mapping:

- Button-Mapping liest die Tastenmatrix von der TM1650-Seite
- Status-Mapping dekodiert Schreibzugriffe vom originalen Controller zum TM1650

## Was Erkannt Werden Kann

Der sichtbare Frontpanel-Status sollte aus TM1650-Display-RAM-Schreibungen
rekonstruierbar sein:

| Status | Erwartete Quelle |
| --- | --- |
| Pump aktiv | Pump-LED-Bit |
| Chlorinator aktiv | Chlorinator-LED-Bit |
| Boost aktiv | Boost-LED-Bit |
| Sleep aktiv | Sleep-LED-Bit |
| Pump low flow | Low-flow-LED-Bit |
| Low salt | Low-salt-LED-Bit |
| High salt | High-salt-LED-Bit |
| Service | Service-LED-Bit |
| Timer-/Displaywert | Siebensegment-Display-Bytes |

Die exakte Adresse und das Bit fuer jede LED muessen am Zielcontroller noch
erfasst werden.

## ESP32-Ausleseansatz

Der ESP32 kann elektrisch parallel zu den TM1650-Buseingaengen sitzen:

```text
Original controller SCL/SDA -> TM1650
                         \-> ESP32 input pins
```

Empfohlene erste Verdrahtung:

| TM1650-Signal | Kandidat ESP32 GPIO | Hinweise |
| --- | ---: | --- |
| SCL | 19 | Nur Eingang beim Sniffing |
| SDA | 18 | Nur Eingang beim Sniffing |
| GND | GND | Gemeinsame Niedervolt-Masse |

Bei 5-V-TM1650-Bus Level-Shifting verwenden. Ein 5-V-Signal nicht direkt mit
einem ESP32-Eingang verbinden.

## Decoder-Plan

Das TM1650-Protokoll ist I2C-aehnlich:

1. START erkennen: SDA faellt, waehrend SCL high ist.
2. SDA bei SCL high fuer jedes Bit sampeln.
3. Bits zu Bytes gruppieren.
4. ACK-Zyklen ignorieren oder mitschreiben.
5. STOP erkennen: SDA steigt, waehrend SCL high ist.
6. Display-Memory-Schreibzugriffe dekodieren.
7. Dekodierte Bytes als Diagnose-Frames veroeffentlichen.
8. Bekannte Bytes/Bits auf ESPHome-Status-Entities mappen.

Die erste Firmware-Implementierung soll passiv und log-only sein. Sie darf SCL
oder SDA nicht treiben.

## Capture-Checkliste

Diese Zustaende erfassen und beschriften:

| Zustand | Was notieren |
| --- | --- |
| Idle/off | Baseline-Frames |
| Pump on | Frames waehrend Pump-LED leuchtet |
| Chlorinator on | Frames waehrend Chlorinator-LED leuchtet |
| Boost on | Frames waehrend Boost-LED leuchtet |
| Sleep on | Frames waehrend Sleep-LED leuchtet |
| Self-clean active | Display- und LED-Frames |
| Low flow | Nur wenn sicher reproduzierbar |
| Low salt | Nur wenn sicher reproduzierbar |
| High salt | Nur wenn sicher reproduzierbar |
| Service | Nur wenn sicher reproduzierbar |

Bei jedem Capture den sichtbaren Panel-Zustand und die rohen Bytes notieren,
die zum TM1650 geschrieben werden.

## ESPHome-Status-Platzhalter

`intex-esphome.yaml` enthaelt bereits deaktivierte Platzhalter-Entities fuer
die erwarteten Zustaende:

- `Pump Active`
- `Chlorinator Active`
- `Boost Active`
- `Sleep Active`
- `Pump Low Flow`
- `Low Salt`
- `High Salt`
- `Service`

Sie geben absichtlich `false` zurueck, bis eine TM1650-Sniffer-Komponente
dekodierten Status veroeffentlicht. Die Mapping-Substitutions in der YAML
sollten erst eingetragen werden, nachdem Captures die echten LED-Bits
bestaetigen.

## Externe Komponente

Der erste passive Sniffer liegt in diesem Repository unter:

```text
components/tm1650_sniffer
```

`intex-esphome.yaml` laedt ihn ueber ESPHome `external_components` von:

```text
https://github.com/cococheaf/intex-esphome
```

Aktuelles Verhalten:

- laeuft dauerhaft, solange `tm1650_sniffer_enabled` `true` ist
- nur Eingang auf den konfigurierten SCL/SDA-Pins
- erkennt TM1650-aehnliche START- und STOP-Bedingungen
- sampelt SDA auf steigenden SCL-Flanken
- gruppiert Daten in Bytes
- loggt rohe Frames als `TM1650 frame (...)`
- treibt den Bus nicht
- veroeffentlicht noch keinen dekodierten Status

Laufzeitsteuerung:

```yaml
substitutions:
  # Passiven Sniffer vollstaendig aktivieren oder deaktivieren.
  tm1650_sniffer_enabled: "true"

  # Sniffing aktiv lassen, aber rohe Frame-Logs ein- oder ausschalten.
  tm1650_sniffer_log_raw: "true"
```

Mit `tm1650_sniffer_enabled: "true"` ist der Sniffer ab Boot aktiv und lauscht,
solange der ESP32 laeuft. Es braucht keinen separaten Scan-Befehl in der YAML.
Setze `tm1650_sniffer_log_raw` auf `false`, wenn der Bus sehr aktiv ist und die
rohen Frame-Logs im Normalbetrieb zu laut werden.
