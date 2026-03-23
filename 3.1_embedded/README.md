# Task 3.1P – Create Trigger/Notification based on Sensor Data

## Overview

This task extends the embedded system built in previous tasks to generate real-time notifications based on light sensor data. The system monitors sunlight exposure for a terrarium using a Keyestudio LDR (Light Dependent Resistor) module connected to an Arduino Nano 33 IoT. When sunlight is detected or stops, the Arduino sends an HTTP POST request to the IFTTT Webhooks service, which triggers an email notification.

---

## Hardware

| Component | Connection |
|---|---|
| Arduino Nano 33 IoT | – |
| Keyestudio LDR Module | Signal → A0, VCC → 3.3V, GND → GND |
| Breadboard + Jumper Wires | – |

---

## Software & Services

- **Arduino IDE**
- **WiFiNINA library** (built-in for Nano 33 IoT)
- **IFTTT** – Webhooks applet configured with two events:
  - `sunlight_start` – triggered when sunlight is detected
  - `sunlight_stop` – triggered when sunlight stops

---

## How It Works

1. The Arduino connects to WiFi on startup using the `setupWiFi()` function.
2. Every 5 seconds, `readLightLevel()` reads the analog value from the LDR on pin A0 (range: 0–1023).
3. `isSunlight()` compares the reading against a configurable threshold (default: 600).
4. If the light state changes (dark → bright or bright → dark), `sendIFTTTTrigger()` sends an HTTPS POST request to the IFTTT Maker Webhooks endpoint.
5. IFTTT receives the trigger and sends an email notification with the current light level.

---

## Configuration

Before uploading, update the following constants in the `.ino` file:

```cpp
const char* WIFI_SSID       = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD   = "YOUR_WIFI_PASSWORD";
const char* IFTTT_KEY       = "YOUR_IFTTT_WEBHOOK_KEY";
const int   LIGHT_THRESHOLD = 600;  // Adjust to match your environment
```

> **Note:** WiFi credentials and the IFTTT key have been replaced with placeholders in this repository.

---

## IFTTT Setup

1. Log in to [IFTTT](https://ifttt.com) and create two new Applets.
2. For each Applet:
   - **If This:** Choose *Webhooks → Receive a web request*
   - Enter the event name (`sunlight_start` or `sunlight_stop`)
   - **Then That:** Choose *Email → Send me an email*
   - Customise the subject/body (you can include `{{Value1}}` to show the light level)
3. Find your Webhook key at [https://ifttt.com/maker_webhooks](https://ifttt.com/maker_webhooks) → *Documentation*

---

## Folder Structure

```
Task3.1Trigger/
├── Task3.1Trigger.ino   # Main Arduino sketch
└── README.md            # This file
```

---

## Notes

- The Keyestudio LDR module was used in place of the BH1750 sensor (unavailable). The LDR provides an analog voltage reading proportional to light intensity, which is sufficient to detect the presence or absence of direct sunlight.
- Circuit was prototyped and verified in Tinkercad using an Arduino Uno R3 as a substitute for the Nano 33 IoT (not available in Tinkercad).
