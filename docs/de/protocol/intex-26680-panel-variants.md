# Intex 26680 Panel-Varianten

Das 26680-Panel muss als Messobjekt behandelt werden, nicht als fest bekannte
Schnittstelle. Community-Notizen deuten mindestens auf zwei verwandte
Panel-Familien hin:

- 4-poliger Panel-Stecker mit 16-poligem TM1650-aehnlichem LED-/Key-Treiber
- Pulse-Protokoll-Panel ohne bestaetigten TM1650-Verkehr

Beide Varianten koennen von aussen aehnlich aussehen. Der erste Hardware-Schritt
ist daher, die vorhandene Variante zu identifizieren, bevor aktive Steuerung
implementiert wird.

## Variante A: TM1650-aehnliches Panel

Erwartete Hinweise:

- 4-poliger Stecker zwischen Controller und Keypad-/Display-Platine
- 16-poliger Treiber-IC nahe Display-/Tastenmatrix
- zwei Busleitungen, die sich wie Clock und Data verhalten koennen

Die wahrscheinlich von verwandten Projekten verwendete ESP32-Signalaufteilung
ist:

| Signalrolle | Kandidat ESP32 GPIO |
| --- | ---: |
| Controller Clock/Data Leitung 1 | 19 |
| Controller Clock/Data Leitung 2 | 18 |
| Display-seitige Clock/Data Leitung 1 | 17 |
| Display-seitige Clock/Data Leitung 2 | 16 |

Diese GPIOs sind nur historische Kandidaten. Die echte 26680-Verdrahtung muss
per Durchgangsmessung und Logic-Capture bestaetigt werden.

Bei dieser Variante ist Button-Support nicht nur ein Pin-Mapping-Problem. Jede
physische Taste braucht einen dekodierten Keywert und eine passende
ESPHome-/Home-Assistant-Aktion. Verwandter Beispielcode behandelte nur einen
Teil der Tasten; deshalb muessen die 26680-Tasten einzeln erfasst werden:

- Power
- Pump
- Lock/Unlock
- Chlorinator
- Boost
- Self-clean

## Variante B: Pulse-Protokoll-Panel

Erwartete Hinweise:

- 4-poliger Stecker zwischen Controller und Keypad-/Display-Platine
- kein bestaetigter TM1650-Busverkehr
- pulsbreitencodierter Verkehr fuer Display/Status und Button-Zustaende

Fuer diese Variante die Capture-Checkliste in
`docs/protocol/pulse-reference/intex-26680-pulse-protocol.md` verwenden.

## Bring-up-Entscheidungsbaum

1. Beide Seiten der Keypad-/Display-Platine fotografieren.
2. Display-/Key-Treibergehaeuse und Markings identifizieren.
3. Durchgang vom 4-poligen Stecker zu den Treiber-IC-Pins pruefen.
4. Idle-Spannung an jedem Steckerpin messen.
5. Power-up-Verkehr mit Logic Analyzer mitschneiden.
6. Jede Taste druecken und loslassen, waehrend beide Signalleitungen
   mitgeschnitten werden.
7. Panel als TM1650-aehnlich, Pulse-Protokoll oder unbekannt klassifizieren.

Erst nach dieser Klassifizierung sollte die Firmware ueber passive
Leitungsdiagnose hinaus wachsen.
