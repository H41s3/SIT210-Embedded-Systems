# Task 2.1P: Sending Temperature and Light Data to the Web

## System Description

This project reads temperature and light level data from Linda's room using an Arduino Nano 33 IoT and sends the data to ThingSpeak every 30 seconds via WiFi. The data is displayed as live graphs on the ThingSpeak web dashboard, allowing room conditions to be monitored remotely.

---

## Hardware Used

- Arduino Nano 33 IoT
- DHT11 Temperature and Humidity Sensor
- Keyestudio LDR Light Sensor Module
- Breadboard and jumper wires
- Micro-USB to USB-A cable

## Wiring

| Component        | Arduino Pin       |
|-----------------|-------------------|
| DHT11 VCC       | 3.3V (via rail)   |
| DHT11 GND       | GND (via rail)    |
| DHT11 DATA      | D2                |
| LDR V (VCC)     | 3.3V (via rail)   |
| LDR G (GND)     | GND (via rail)    |
| LDR S (Signal)  | A0                |

---

## Code Structure & Function Explanations

### `setupWiFi()`
Connects the Arduino to the WiFi network using the provided SSID and password. Keeps retrying every 3 seconds until a connection is established.

### `readTemperature()`
Reads the current temperature in Celsius from the DHT11 sensor. Returns 0 if the reading fails.

### `readLight()`
Reads the analogue light level from the LDR sensor on pin A0. Returns a value between 0 (dark) and 1023 (bright).

### `sendToThingSpeak()`
Sends the temperature and light values to the ThingSpeak channel using the Write API Key. Field 1 = Temperature, Field 2 = Light.

### `setup()`
Initialises serial communication, the DHT11 sensor, ThingSpeak client, and connects to WiFi.

### `loop()`
Calls readTemperature() and readLight() to get sensor data, then calls sendToThingSpeak() to upload the data. Waits 30 seconds before repeating.

---

## Software & Libraries

- Arduino IDE 2
- DHT sensor library (Adafruit)
- ThingSpeak library
- WiFiNINA library
- Language: C/C++ (Arduino framework)

---

## ThingSpeak Channel

- Channel Name: RoomConditions
- Field 1: Temperature
- Field 2: Light
