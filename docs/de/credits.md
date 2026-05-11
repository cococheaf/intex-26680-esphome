# Credits

Dieses Projekt baut auf hervorragender Vorarbeit aus der
Intex-Pool-Automatisierungs-Community auf.

## tonyflores1006/intex-swg-iot

Repository:

- https://github.com/tonyflores1006/intex-swg-iot

Das Projekt `tonyflores1006/intex-swg-iot` hat die Grundidee gezeigt, einen
ESP32 zwischen die originale Intex-Controller-Platine und das originale
Control-Panel zu setzen und diese Position zum Beobachten und Automatisieren
des Geraets zu nutzen.

Diese Arbeit ist eine der Grundlagen fuer `intex-esphome`. Sie hat die
Interface-Board-Richtung, den ESP32-Bridge-Ansatz und die fruehen Ueberlegungen
zum Lesen von Panel-Status sowie zum Weiterleiten oder Injizieren von
Button-Events gepraegt.

## jingsno/intex-swg-pcb

Repository:

- https://github.com/jingsno/intex-swg-pcb

Das Projekt `jingsno/intex-swg-pcb` lieferte wertvolles PCB-, Protokoll- und
Capture-Referenzmaterial. Die Referenz-Board-Dateien und Pulse-Protokollnotizen
in diesem Repository basieren auf dieser Arbeit und wurden hier fuer die
`intex-esphome`-Projektstruktur neu organisiert.

Diese Arbeit ist besonders wichtig fuer die Hardware-Bring-up-Notizen, die
Panel-Interface-Board-Referenz und das Nicht-TM1650-Pulse-Protokollmaterial
unter `docs/protocol/pulse-reference`.

## Hinweise Zur Attribution

`intex-esphome` fokussiert sich auf einen ESPHome-basierten All-in-one-Weg fuer
Intex 26670 und Intex 26680 Panel-Interface-Arbeiten. Ohne die Untersuchungen,
Code-Beispiele, PCB-Dateien und Dokumentation der oben genannten Projekte waere
der Startpunkt deutlich schlechter.

Wenn du dieses Repository verwendest, gib bitte auch diesen Upstream-Autoren
Credit, sofern ihre Arbeit deine Hardware, Firmware oder Dokumentation
beeinflusst hat.
