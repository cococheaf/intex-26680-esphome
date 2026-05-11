# Zu Intex ESPHome Beitragen

[English](CONTRIBUTING.md) | **Deutsch**

Danke, dass du hilfst, dieses Projekt zu verbessern.

Diese Firmware beobachtet und kann spaeter reale Pooltechnik ueber einen ESP32
steuern. Gute Beitraege sind vorsichtig, praktisch und auf Hardware gut
ueberpruefbar.

## Projektziele

Das Projekt soll:

- die Display-/Keypad-Busvarianten der Intex 26670 und Intex 26680 verstehen
- zuverlaessiges ESP32-basiertes Statuslesen bereitstellen
- sichere virtuelle Button-Injection nach verifizierten Mappings ermoeglichen
- nuetzliche ESPHome-Entities fuer Home Assistant bereitstellen
- Firmware verstaendlich und wartbar halten
- Hardware-Erkenntnisse so dokumentieren, dass andere sie reproduzieren koennen

## Wie Du Beitragen Kannst

Nuetzliche Beitraege sind:

- Bus-Captures von Intex 26670 oder Intex 26680 Hardware
- verifizierte TM1650-Button-Matrix-Mappings
- verifizierte Pulse-Protokoll-Button-Mappings
- verifizierte LED-/Status-Mappings
- Dokumentationsverbesserungen
- ESPHome-Kompatibilitaetsupdates
- sichereres GPIO- und Relais-Handling
- Bugfixes und Testberichte

Kleine fokussierte Pull Requests sind besser als grosse gemischte Aenderungen.

## Hardware-Berichte

Bei Hardware-Verhalten bitte angeben:

- exaktes Intex-Modell und Region, falls bekannt
- Board-Fotos, wenn relevant
- ESP32-Boardtyp
- Verdrahtungsdiagramm oder Pin-Mapping
- gemessene Busspannung
- Logic-Analyzer-Samplerate
- dekodierte Bytes oder Capture-Dateien
- welche Taste gedrueckt wurde oder welcher Displayzustand sichtbar war
- Firmware-Commit, der fuer den Test verwendet wurde

Bitte keine Schluesse nur aus Annahmen von einem anderen Intex-Modell ziehen.

## Bugreports

Ein guter Bugreport enthaelt:

- klare Problembeschreibung
- Schritte zum Reproduzieren
- erwartetes Verhalten
- tatsaechliches Verhalten
- Serial-Monitor-Logs
- API-Antworten, falls relevant
- Hardware-Verdrahtungsdetails
- ESPHome-Version
- Firmware-Commit

Bei Timing- oder Protokollproblemen nach Moeglichkeit einen
Logic-Analyzer-Capture beilegen.

## Pull Requests

Vor einem Pull Request bitte pruefen:

- die Aenderung hat einen klaren Zweck
- Hardware-Annahmen sind dokumentiert
- unrelated Cleanup ist nicht mit Verhaltensaenderungen vermischt
- Public-API-Aenderungen sind beschrieben
- README oder Docs sind bei Bedarf aktualisiert
- ESPHome-Compile funktioniert in deiner Umgebung weiterhin
- sicherheitsrelevante Aenderungen sind erklaert

## Entwicklungsrichtlinien

Bitte dem bestehenden Firmware-Stil folgen, sofern Cleanup nicht Teil der
eigentlichen Aenderung ist.

- Timing-sensitiven Buscode einfach halten.
- Versteckte Nebeneffekte in gemeinsamem globalem Zustand vermeiden.
- Benannte Konstanten statt Magic Bytes bevorzugen.
- Kommentare auf nicht-offensichtliches Protokoll- oder Timing-Verhalten
  fokussieren.
- GPIO-Zuweisungen nicht ohne dokumentierten Hardware-Grund verschieben.
- Keine Automatisierung ergaenzen, die Pooltechnik ueberraschend starten kann.

## Sicherheitsaspekte

Diese Firmware kann Pumpen, Chlorinator und Power-Switching beeinflussen.

Aenderungen sollen vermeiden:

- manuelle Bedienung zu umgehen
- anzunehmen, dass jedes Intex-Modell dasselbe Protokoll nutzt
- Technik ohne verifizierte Flow- oder Statusinformation laufen zu lassen
- Relaisausgaenge ueberraschend aktiv zu machen
- Busfehler hinter optimistischen Statuswerten zu verstecken
- Hardware-Verhalten hart zu kodieren, das zuerst gemessen werden muss

Im Zweifel lieber read-only Diagnose als Steuerverhalten.

## Commit Messages

Klare Commit Messages verwenden. Beispiele:

```text
docs: document 26680 keypad mapping workflow
fix: release SDA after virtual key response
feat: expose raw bus frames as diagnostics
refactor: name TM1650 command constants
build: update ESPHome validation
```

## Testing

Je nach Aenderung sind nuetzliche Tests:

- `esphome config intex-esphome.yaml`
- `esphome compile intex-esphome.yaml`
- Flash auf ein ESP32-Testboard
- Serial-Monitor-Ausgabe pruefen
- ESPHome-API-Entities in Home Assistant pruefen
- ESPHome-Webserver pruefen, wenn aktiviert
- dekodierte Bytes mit Logic-Analyzer-Trace vergleichen
- WiFi-Fallback-AP-Provisioning testen
- OTA-Upload auf einem unkritischen Testgeraet pruefen

Beschreibe im Pull Request, was du getestet hast.

## Lizenz

Dieses Repository ist unter der GNU General Public License v3.0 lizenziert.
Fuege keinen Drittanbieter-Code hinzu, wenn dessen Lizenz nicht mit GPL-3.0
kompatibel ist oder die Quelle nicht dokumentiert ist.

Wenn Arbeit von einem anderen Projekt abgeleitet oder wesentlich beeinflusst
wurde, ergaenze oder aktualisiere die Attribution in den
[Credits](docs/de/credits.md).

## Fragen

Wenn du unsicher bist, ob eine Aenderung passt, oeffne zuerst ein Issue und
beschreibe Hardware-Setup, Problem und vorgeschlagenen Ansatz.
