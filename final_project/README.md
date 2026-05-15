# Smart Posture Monitor
**SIT210 Embedded Systems Development — Task 11.2P Final Project**
Student: Emilio Joseph Calma | s223976317 | Deakin University

---

## Project Overview

The Smart Posture Monitor is an embedded IoT system that helps desk workers and students become aware of poor posture before it causes pain or injury. The Arduino Nano 33 IoT reads tilt angle from its built-in LSM6DS3 IMU every second. If the tilt exceeds 30 degrees forward for more than 60 consecutive seconds, the system fires a local LED and buzzer alert, updates an I2C OLED display with a correction message, and sends an email notification via IFTTT Webhooks over WiFi. All posture events are logged to ThingSpeak for trend analysis.

**Research question:** Can a low-cost embedded system using inertial sensing reliably detect and alert a desk worker to sustained poor posture in a real home-office environment?

---

## Files

| File | Description |
|---|---|
| `SmartPostureMonitor.ino` | Main Arduino sketch |
| `README.md` | This file |

---

## Hardware

| Component | Interface | Pin(s) | Purpose |
|---|---|---|---|
| Arduino Nano 33 IoT | — | — | MCU + WiFi + built-in IMU |
| LSM6DS3 IMU (built-in) | I2C | SDA / SCL | Accelerometer for tilt detection |
| SSD1306 OLED 128x64 | I2C | A4 (SDA) / A5 (SCL) | Live posture status display |
| Red LED | Digital | D4 | Local visual alert |
| Piezo buzzer (PKM22EP-40) | Digital | D5 | Audible alert on bad posture |
| 330Ω resistor | — | — | Current limiting for LED |

---

## Wiring

```
Arduino 3.3V  →  OLED VCC
Arduino GND   →  OLED GND
Arduino A4    →  OLED SDA
Arduino A5    →  OLED SCL

Arduino D4    →  330Ω resistor  →  LED anode (+)
LED cathode (-) →  GND

Arduino D5    →  Buzzer (+)
Buzzer (-)    →  GND
```

The LSM6DS3 IMU is built into the Arduino Nano 33 IoT and shares the same I2C bus as the OLED. No external IMU wiring is required.

---

## Libraries Required

Install via Arduino IDE — Sketch → Include Library → Manage Libraries:

- `Arduino_LSM6DS3`
- `Adafruit_SSD1306`
- `Adafruit_GFX`
- `WiFiNINA`
- `ArduinoHttpClient`
- `Wire` (built-in, no install needed)

---

## Setup Instructions

### 1. IFTTT Webhook
1. Sign in at [ifttt.com](https://ifttt.com)
2. Create applet: **If** Webhooks "Receive a web request" → **Then** Email "Send me an email"
3. Set event name to `posture_alert`
4. Go to [ifttt.com/maker_webhooks](https://ifttt.com/maker_webhooks) → Documentation → copy your key

### 2. ThingSpeak Channel
1. Sign in at [thingspeak.com](https://thingspeak.com)
2. Create a new channel:
   - Field 1: `Tilt Angle (deg)`
   - Field 2: `Alert Count`
3. Copy the Write API Key from the API Keys tab

### 3. Configure credentials in the code
Open `SmartPostureMonitor.ino` and fill in:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* IFTTT_KEY     = "YOUR_IFTTT_KEY";
const char* TS_API_KEY    = "YOUR_THINGSPEAK_WRITE_KEY";
```

### 4. Upload
1. Open Arduino IDE
2. Select board: **Arduino Nano 33 IoT**
3. Select the correct port
4. Click Upload
5. Open Serial Monitor at **9600 baud**

---

## How It Works

1. On startup the system connects to WiFi, initialises the IMU and OLED
2. Every 1 second the IMU is read and tilt angle is calculated:
   ```
   tilt = atan2(ax, az) * 180 / PI
   ```
3. If tilt > 30 degrees, a sustained-posture timer starts
4. If the timer reaches 60 seconds without correction, the alert fires:
   - LED activates on D4
   - Buzzer beeps 3 times on D5
   - OLED updates to "CORRECT NOW!"
   - IFTTT email is sent via HTTPS
5. When the user corrects posture, the alert resets automatically
6. Every 60 seconds, tilt and alert count are posted to ThingSpeak

---

## Adjustable Parameters

| Parameter | Default | Description |
|---|---|---|
| `TILT_THRESHOLD` | 30.0 degrees | Forward tilt that triggers monitoring |
| `BAD_POSTURE_LIMIT` | 60000 ms | Duration before alert fires |
| `IMU_INTERVAL` | 1000 ms | IMU sampling rate |
| `CLOUD_INTERVAL` | 60000 ms | ThingSpeak posting rate |

For testing, set `BAD_POSTURE_LIMIT` to `5000` (5 seconds) for faster alerts.

---

## Serial Monitor Output

```
Connecting to WiFi...
WiFi connected!
IP: 192.168.x.x
IMU ready
Tilt: 2.45 deg
Tilt: 3.12 deg
Tilt: 45.67 deg
Watch posture...
ALERT: Poor posture detected!
IFTTT response: 200
Posture corrected - alert reset
Tilt: 1.89 deg
```

---

## Communication Protocols

| Protocol | Between | Purpose |
|---|---|---|
| I2C (Wire library) | Nano 33 IoT ↔ LSM6DS3 + OLED | Read IMU; update display |
| HTTPS (WiFiNINA) | Nano 33 IoT → IFTTT Webhook | Trigger email alert |
| HTTP (ThingSpeak API) | Nano 33 IoT → ThingSpeak | Log posture data |
| USB Serial 9600 baud | Nano 33 IoT → PC | Debug output |

---

## Hardware Note

The LSM6DS3 IMU is built into the Arduino Nano 33 IoT — no external IMU purchase was required. The Keyestudio LDR module used in previous tasks is not used in this project. WiFi credentials and API keys have been replaced with placeholders in this repository.
