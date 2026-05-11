# Panel-Bridge-Board-Notizen

Dieses Projekt zielt auf eine ESP32-Panel-Interface-Platine fuer Intex 26670
und Intex 26680 Controller.

Die Platine soll zwischen der originalen Controller-Platine und dem originalen
Display-/Tastenfeld sitzen. Die erste Revision soll transparent sein: Sie soll
die originalen Signale durchreichen und diese Signale gleichzeitig fuer einen
ESP32 messbar machen, damit sie spaeter dekodiert werden koennen.

## Ziel-Topologie

```text
Originale Controller-Platine <-> ESP32-Interface-Platine <-> Originales Display-/Tastenfeld
```

Die ESP32-Seite soll perspektivisch koennen:

- Display- und Statusverkehr vom Controller beobachten
- Button-Verkehr vom Keypad beobachten
- originale Signale mit minimaler Verzoegerung weiterleiten
- nach verifiziertem Protokoll dekodierten Status ueber ESPHome veroeffentlichen
- Button-Events erst injizieren, wenn passives Forwarding stabil ist

## Kandidaten-Signallayout

Die aktuelle Board-Referenz verwendet vier Niedervolt-Logikpfade:

| Richtung | Arbeitsname | ESP32 GPIO in Referenz | Zweck |
| --- | --- | ---: | --- |
| Controller-Platine -> ESP32 | `CONTROLLER_CLOCK` | 19 | TM1650-Clock-Kandidat von Controller-Seite |
| Controller-Platine -> ESP32 | `CONTROLLER_DATA` | 18 | TM1650-Data-Kandidat von Controller-Seite |
| Display-Platine -> ESP32 | `PANEL_CLOCK` | 17 | Clock-Kandidat auf Display-/Panel-Seite |
| Display-Platine -> ESP32 | `PANEL_DATA` | 16 | Data-Kandidat auf Display-/Panel-Seite |

Bei Pulse-Protokoll-Panels sind moeglicherweise nur zwei dieser Leitungen
relevant. Die echten Signalrollen muessen mit Durchgangsmessung und
Logic-Capture bestaetigt werden.

## Elektrische Design-Checks

Vor einer dauerhaften Platine:

- Spannungspegel am Panel-Stecker messen
- Idle-Pegel und Pull-up-Positionen bestaetigen
- klaeren, ob eine Seite aktiv high treibt
- entscheiden, ob Level-Shifter, Serienwiderstaende oder digitale Isolatoren
  benoetigt werden
- einfachen Bypass-Pfad fuer fruehe Hardwaretests vorsehen
- bootkritische ESP32-Pins fuer Relais oder kritische Busleitungen vermeiden

## KiCad-Referenz

Die KiCad-Referenz liegt unter:

- `hardware/panel-interface-board`

Sie ist der Startpunkt fuer eine gemeinsame 26670/26680-PCB-Anpassung.
