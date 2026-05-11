# Intex ESPHome

**Deutsch** | [English](README.en.md)

![Intex ESPHome Controller](docs/images/intex-pump-controller.jpg)

All-in-one ESPHome-Projekt fuer Interface-Arbeiten am Bedienfeld der Intex
26670 und Intex 26680.

Das Projekt laesst die originale Controller-Platine und das originale
Display-/Tastenfeld an ihrem Platz, setzt eine ESP32-Interface-Platine
dazwischen und startet bewusst in einem passiven Diagnosemodus. Aktive virtuelle
Tastensteuerung sollte erst aktiviert werden, nachdem die echte Tastenmatrix und
das Busverhalten an der Zielhardware gemessen wurden.

## Projektstatus

Das ist eine fruehe Hardware- und Firmware-Basis, keine fertige
Automatisierungs-Firmware.

Offene Aufgaben:

- Spannungspegel am Panel-Stecker messen
- Pinout und Signalrichtung des Steckers bestaetigen
- TM1650 SCL/SDA/VDD/GND am Zielpanel bestaetigen
- echten Busverkehr mit einem Logic Analyzer mitschneiden
- Button-Keycodes fuer das ausgewaehlte Modell mappen
- Display- und Status-LED-Frames mappen
- KiCad-Board unter `hardware/panel-interface-board` anpassen

## Modellauswahl

Eine YAML-Datei deckt beide unterstuetzten Modelle ab:

```yaml
substitutions:
  target_model: "26680"
```

Unterstuetzte Werte:

| Modell | Panel-Tasten |
| --- | --- |
| `26670` | Power, Lock/Unlock, Timer, Boost, Self-clean |
| `26680` | Power, Pump, Lock/Unlock, Chlorinator, Boost, Self-clean |

Die Modell-Substitution aendert aktuell nur Namen und Diagnosewerte. Protocol
Decoding und Button-Injection bleiben deaktiviert, bis die gemessenen Keycodes
eingetragen sind.

## Standard-ESPHome-Firmware

Die Standard-YAML benoetigt ESPHome 2026.4.0 oder neuer und stellt bereit:

- WiFi, API, OTA, Captive Portal und Webserver
- Geraetestatus
- WiFi-Signal und Uptime-Diagnose
- ausgewaehltes Zielmodell
- Platzhalter fuer gemessene Keycodes
- rohe GPIO-Diagnose fuer vier Kandidatenleitungen der Panel-Bridge
- deaktivierte virtuelle Button-Platzhalter fuer spaetere Steuerung
- passiven TM1650-Sniffer als externe Komponente aus diesem Repository
- ESP32 SRAM1-as-IRAM-Unterstuetzung auf klassischen ESP32-Zielen

Die einzige externe ESPHome-Komponente ist `tm1650_sniffer`. Sie wird ueber
`external_components` aus diesem GitHub-Repository geladen.

## Keycode-Mapping

Bei TM1650-basierten Panels ist der naechste Schritt eine Durchgangsmessung von
jeder physischen Taste zu den TM1650-Matrixpins. Workflow und Keycode-Tabelle
stehen in [TM1650 Button-Mapping](docs/de/protocol/tm1650-button-mapping.md).

Der aktuelle Status kann spaeter ebenfalls aus TM1650-Display-/LED-Schreibungen
gelesen werden, sobald der Bus passiv dekodiert ist. Der Capture-Plan steht in
[TM1650 Status-Sniffing](docs/de/protocol/tm1650-status-sniffing.md).

Nicht-TM1650-Pulse-Protokollmaterial bleibt als Referenz erhalten:

- [Intex 26670 Pulse-Referenz](docs/protocol/pulse-reference/intex-26670-pulse-protocol.md)
- [Intex 26680 Pulse-Referenz](docs/protocol/pulse-reference/intex-26680-pulse-protocol.md)
- [Arduino-Referenzdemos](docs/protocol/pulse-reference/reference-arduino-demos/README.md)

26680-spezifische Panel-Varianten sind in
[Intex 26680 Panel-Varianten](docs/de/protocol/intex-26680-panel-variants.md)
dokumentiert.

## Hardware-Sicherheit

Pooltechnik kombiniert Netzspannung, Wasser, Pumpen und chemische Erzeugung.
Bitte vorsichtig arbeiten.

- ESP32 niemals direkt mit unbekannten Spannungspegeln verbinden.
- Control-Panel-Bus messen, bevor etwas dauerhaft verdrahtet wird.
- Bei Bedarf Level-Shifting oder Isolation verwenden.
- Netzspannungsverdrahtung strikt von Niedervolt-Elektronik trennen.
- Manuelle Bedienung waehrend Tests verfuegbar halten.
- Keine unbeaufsichtigte Automatisierung betreiben, bevor Status-Mapping
  verifiziert ist.

## Hardware-Richtung

Der vorgesehene Interface-Pfad ist:

```text
Originale Controller-Platine <-> ESP32-Interface-Platine <-> Originales Display-/Tastenfeld
```

Standard-Diagnosepins:

| Signal | ESP32 GPIO | Zweck |
| --- | ---: | --- |
| Controller Clock Candidate | 19 | TM1650-Clock-Kandidat von der Controller-Seite |
| Controller Data Candidate | 18 | TM1650-Data-Kandidat von der Controller-Seite |
| Panel Clock Candidate | 17 | Clock-Kandidat auf Panel-/Display-Seite |
| Panel Data Candidate | 16 | Data-Kandidat auf Panel-/Display-Seite |

Siehe [Panel-Bridge-Board-Notizen](docs/de/hardware/panel-bridge-board.md) fuer
Hardware-Bring-up-Notizen.

## Build Und Flash

Voraussetzungen:

- ESPHome 2026.4.0 oder neuer
- Python-Umgebung, die von deiner ESPHome-Installation unterstuetzt wird
- ESP32-Development-Board

Lokale Secrets-Datei erstellen:

```bash
cp secrets.yaml.example secrets.yaml
```

`secrets.yaml` bearbeiten, `target_model` in `intex-esphome.yaml` setzen und
dann kompilieren:

```bash
esphome compile intex-esphome.yaml
```

Per USB flashen:

```bash
esphome run intex-esphome.yaml
```

Nach dem ersten Flash koennen OTA-Updates mit demselben Befehl ausgefuehrt
werden, sobald das Geraet online ist.

## Repository-Struktur

```text
intex-esphome.yaml             All-in-one ESPHome-Geraetekonfiguration
secrets.yaml.example           Beispiel fuer lokale WiFi-Secrets
docs/hardware/                 Hardware-Bring-up-Notizen auf Englisch
docs/images/                   Projektbilder
docs/protocol/                 Protokollnotizen und Capture-Plaene auf Englisch
docs/protocol/pulse-reference/ Nicht-TM1650-Referenzmaterial
docs/reference/                Referenzdokumente und Datenblaetter
docs/de/                       Deutsche Dokumentation
hardware/panel-interface-board/  KiCad-Referenz fuer Panel-Interface-Board
.github/social-preview.jpg     Quellbild fuer GitHub Social Preview
```

## Credits

Dieses Projekt baut auf hervorragender Intex-Automatisierungsarbeit von
[`tonyflores1006/intex-swg-iot`](https://github.com/tonyflores1006/intex-swg-iot)
und
[`jingsno/intex-swg-pcb`](https://github.com/jingsno/intex-swg-pcb)
auf. Details stehen in den [Credits](docs/de/credits.md).

## Beitragen

Siehe [CONTRIBUTING.de.md](CONTRIBUTING.de.md).

## Lizenz

Dieses Projekt ist unter der GNU General Public License v3.0 lizenziert. Siehe
[LICENSE](LICENSE).
