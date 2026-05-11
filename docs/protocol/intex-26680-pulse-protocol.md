# Intex 26680 Pulse Protocol Candidate

Some related Intex control panels use a one-wire pulse protocol between the
controller board and the display/keypad board. The 26680 still needs to be
measured, so this document is a candidate protocol and capture checklist, not a
confirmed specification.

## Physical Direction

Candidate traffic direction:

```text
Controller board -> Display board  display digits and status LEDs
Display board    -> Controller board  button state
```

Known related panels idle high and encode data as pulse widths.

## Candidate Bit Timing

| Symbol | Timing |
| --- | --- |
| Start | low pulse around 200 us |
| `0` bit | high pulse around 200 us, then low spacer around 200 us |
| `1` bit | high pulse around 800 us, then low spacer around 200 us |
| End | return to idle high |

The first 26680 capture should verify whether these timings match.

## Candidate Button Frames

Related panels send a 16-bit frame. The first byte is the inverse of the second
byte, so the effective button value is the low byte.

Candidate values from related hardware:

| Button | Value |
| --- | ---: |
| Release | `0x00` |
| Lock/Unlock | `0x01` |
| Timer | `0x02` |
| Power | `0x04` |
| Self-clean | `0x08` |
| Boost | `0x10` |

The 26680 has Pump and Chlorinator buttons, so the real 26680 button values
must be captured.

## Candidate Display Frames

Related panels send a 32-bit display/status frame:

```text
left digit, right digit, LED/status high byte, LED/status low byte
```

Candidate seven-segment bit order:

```text
DP G F E D C B A
```

Candidate LED bits from related hardware:

| LED | Mask |
| --- | ---: |
| Service | `0x0001` |
| High salt | `0x0002` |
| Low salt | `0x0020` |
| Pump low flow | `0x0040` |
| Ozone | `0x0400` |
| Power/working | `0x0800` |
| Sleep | `0x1000` |
| Boost | `0x8000` |

The 26680 front panel has separate Pump and Chlorinator LEDs. Their bit
positions still need to be measured.

## Capture Plan

Capture these events on a powered low-voltage panel connection:

- power-up sequence
- no button pressed
- each physical button pressed and released
- pump active
- chlorinator active
- boost active
- self-clean active
- simulated low-flow/salt/service states if safely reproducible

Record raw pulse timings first. Add decoded ESPHome entities only after the
frame format is confirmed.
