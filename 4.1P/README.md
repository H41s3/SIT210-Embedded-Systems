# Task 4.1P — Interrupt-Based Automatic Lighting System
**SIT210 — Embedded Systems Development**
Student: Emilio Joseph Calma (s223976317)

---

## Overview
This project implements a hardware interrupt-driven lighting system using the Arduino Nano 33 IoT. The system automatically activates two LEDs when motion is detected in dark conditions, with a manual slider switch override.

---

## Hardware
| Component | Pin |
|---|---|
| PIR motion sensor OUT | D2 |
| Slider switch C (common) | D3 |
| LED 1 (via 220Ω resistor) | D4 |
| LED 2 (via 220Ω resistor) | D5 |
| LDR module signal | A0 |
| PIR + LDR VCC | 3.3V |

**Notes:**
- BH1750 I2C sensor substituted with Keyestudio LDR analog module
- 3.3V used instead of 5V — VUSB requires soldering on Nano 33 IoT
- Tinkercad circuit diagram uses Arduino Uno R3 as substitute

---

## How It Works
- **PIR interrupt (D2, RISING)** — fires when motion is detected. LEDs turn on only if LDR reading > DARK_THRESHOLD (dark conditions)
- **Slider switch interrupt (D3, CHANGE)** — fires on every slide. Toggles LEDs regardless of light level (manual override)
- **ISR flag pattern** — ISRs only set a flag. All real logic runs in loop() to avoid conflicts with analogRead and Serial

---

## Files
- `Task4.1Interrupts.ino` — main Arduino sketch

---

## How to Run
1. Wire components as per the wiring table above
2. Open `Task4.1Interrupts.ino` in Arduino IDE
3. Select board: **Arduino Nano 33 IoT**
4. Select correct port and upload
5. Open Serial Monitor at **9600 baud**
6. Wait 60 seconds for PIR to warm up
7. Cover LDR and wave at PIR to trigger lights
8. Slide switch to manually toggle lights

---

## Serial Monitor Output
```
=== Task 4.1P: Interrupt-Based Lighting System ===
Waiting for motion or switch input...
[PIR] Motion detected. LDR: 5
[PIR] Dark — LEDs switched ON.
[PIR] Motion detected. LDR: 1
[PIR] Bright — LEDs remain OFF.
[SWITCH] Slider activated — LEDs now: ON
[SWITCH] Slider activated — LEDs now: OFF
```
