# TM1650 Status Sniffing

Yes, the ESP32 can read the current controller status from a TM1650-based
panel. The ESP32 does this by passively listening to the TM1650 `SCL` and `SDA`
lines while the original controller writes display and LED data.

This is different from button mapping:

- button mapping reads the key matrix from the TM1650 side
- status mapping decodes writes from the original controller to the TM1650

## What Can Be Detected

The visible front-panel state should be recoverable from TM1650 display RAM
writes:

| Status | Expected source |
| --- | --- |
| Pump active | Pump LED bit |
| Chlorinator active | Chlorinator LED bit |
| Boost active | Boost LED bit |
| Sleep active | Sleep LED bit |
| Pump low flow | Low-flow LED bit |
| Low salt | Low-salt LED bit |
| High salt | High-salt LED bit |
| Service | Service LED bit |
| Timer/display value | Seven-segment display bytes |

The exact address and bit for each LED still need to be captured on the target
controller.

## ESP32 Readout Approach

The ESP32 can sit electrically in parallel with the TM1650 bus inputs:

```text
Original controller SCL/SDA -> TM1650
                         \-> ESP32 input pins
```

Recommended first-pass wiring:

| TM1650 signal | Candidate ESP32 GPIO | Notes |
| --- | ---: | --- |
| SCL | 19 | Input only while sniffing |
| SDA | 18 | Input only while sniffing |
| GND | GND | Common low-voltage ground |

Use level shifting if the TM1650 bus is 5 V. Do not connect a 5 V signal
directly to an ESP32 input.

## Decoder Plan

The TM1650 protocol is I2C-like:

1. Detect START: SDA falls while SCL is high.
2. Sample SDA on SCL high for each bit.
3. Group bits into bytes.
4. Ignore or record ACK cycles.
5. Detect STOP: SDA rises while SCL is high.
6. Decode display memory writes.
7. Publish decoded bytes as diagnostic frames.
8. Map known bytes/bits to ESPHome status entities.

The first firmware implementation should be passive and log-only. It should
not drive SCL or SDA.

## Capture Checklist

Capture and label these states:

| State | What to record |
| --- | --- |
| Idle/off | Baseline frames |
| Pump on | Frames while Pump LED is on |
| Chlorinator on | Frames while Chlorinator LED is on |
| Boost on | Frames while Boost LED is on |
| Sleep on | Frames while Sleep LED is on |
| Self-clean active | Display and LED frames |
| Low flow | Only if safely reproducible |
| Low salt | Only if safely reproducible |
| High salt | Only if safely reproducible |
| Service | Only if safely reproducible |

For every capture, note the visible panel state and the raw bytes written to
the TM1650.

## ESPHome Status Placeholders

`intex-esphome.yaml` already contains disabled placeholder entities for the
expected states:

- `Pump Active`
- `Chlorinator Active`
- `Boost Active`
- `Sleep Active`
- `Pump Low Flow`
- `Low Salt`
- `High Salt`
- `Service`

They intentionally return `false` until a TM1650 sniffer component publishes
decoded state. The mapping substitutions in the YAML should be filled only
after captures confirm the real LED bits.
