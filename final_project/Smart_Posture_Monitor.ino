/*
 * Smart Posture Monitor
 * SIT210 Embedded Systems Development
 * Student: Emilio Joseph Calma
 *
 * Hardware:
 *   - Arduino Nano 33 IoT (built-in LSM6DS3 IMU)
 *   - SSD1306 OLED 128x64 (I2C: SDA=A4, SCL=A5)
 *   - Red LED + 330ohm resistor (D4)
 *   - Piezo buzzer (D5)
 *
 * Libraries required:
 *   - Arduino_LSM6DS3
 *   - Adafruit_SSD1306
 *   - Adafruit_GFX
 *   - WiFiNINA
 *   - ArduinoHttpClient
 */

#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
const char* WIFI_SSID     = "WIFI NAME";
const char* WIFI_PASSWORD = "WIFI PASSWORD";

// IFTTT Webhook
const char* IFTTT_HOST    = "maker.ifttt.com";
const char* IFTTT_KEY     = "IFTTT API KEY";
const char* IFTTT_EVENT   = "posture_alert";

// ThingSpeak
const char* TS_HOST       = "api.thingspeak.com";
const char* TS_API_KEY    = "THING SPEAK API KEY";

// Pin definitions
#define LED_PIN     4
#define BUZZER_PIN  5

// ── OLED setup
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Thresholds
const float TILT_THRESHOLD    = 30.0;   // degrees forward
const unsigned long BAD_POSTURE_LIMIT = 60000; // 60 seconds in ms

// Timing (non-blocking)
const unsigned long IMU_INTERVAL    = 1000;   // read IMU every 1s
const unsigned long CLOUD_INTERVAL  = 60000;  // post to cloud max every 60s

unsigned long lastIMUTime    = 0;
unsigned long lastCloudTime  = 0;
unsigned long badPostureStart = 0;

// State
bool badPostureActive  = false;
bool alertFired        = false;
int  alertCount        = 0;
float currentTilt      = 0.0;

// Function prototypes
void connectWiFi();
float readTiltAngle();
void updateOLED(float tilt, bool alert);
void triggerAlert(float tilt);
void resetAlert();
void postToIFTTT(float tilt);
void postToThingSpeak(float tilt);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Posture Monitor");
  display.println("Starting up...");
  display.display();

  // IMU
  if (!IMU.begin()) {
    Serial.println("IMU init failed");
    while (true);
  }
  Serial.println("IMU ready");

  // WiFi
  connectWiFi();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Ready!");
  display.display();
  delay(1000);
}

void loop() {
  unsigned long now = millis();

  // Read IMU every 1 second
  if (now - lastIMUTime >= IMU_INTERVAL) {
    lastIMUTime = now;

    currentTilt = readTiltAngle();
    Serial.print("Tilt: ");
    Serial.print(currentTilt);
    Serial.println(" deg");

    if (currentTilt > TILT_THRESHOLD) {
      // Bad posture detected
      if (!badPostureActive) {
        badPostureActive  = true;
        badPostureStart   = now;
        alertFired        = false;
      }

      unsigned long duration = now - badPostureStart;

      if (duration >= BAD_POSTURE_LIMIT && !alertFired) {
        triggerAlert(currentTilt);
        alertFired = true;
      }

    } else {
      // Good posture - reset
      if (badPostureActive) {
        resetAlert();
      }
      badPostureActive = false;
      alertFired       = false;
    }

    updateOLED(currentTilt, alertFired);
  }

  // Post to ThingSpeak every 60s (rate limit)
  if (now - lastCloudTime >= CLOUD_INTERVAL) {
    lastCloudTime = now;
    postToThingSpeak(currentTilt);
  }
}

// Read tilt angle from LSM6DS3 accelerometer
// Returns forward tilt in degrees
float readTiltAngle() {
  float ax, ay, az;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    // Calculate tilt angle from vertical using arctan
    float tilt = atan2(ax, az) * 180.0 / PI;
    if (tilt < 0) tilt = -tilt;
    return tilt;
  }
  return 0.0;
}

// Update OLED display
void updateOLED(float tilt, bool alert) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Posture Monitor");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.print("Tilt: ");
  display.print(tilt, 1);
  display.println(" deg");

  display.setCursor(0, 26);
  if (alert) {
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.println("CORRECT");
    display.println(" NOW!");
  } else if (tilt > TILT_THRESHOLD) {
    display.setTextSize(1);
    display.println("Watch posture...");
    unsigned long elapsed = (millis() - badPostureStart) / 1000;
    display.print("Duration: ");
    display.print(elapsed);
    display.println("s");
  } else {
    display.setTextSize(1);
    display.println("GOOD POSTURE :)");
  }

  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("Alerts: ");
  display.print(alertCount);

  display.display();
}

// Fire the alert: LED + buzzer + IFTTT
void triggerAlert(float tilt) {
  alertCount++;
  Serial.println("ALERT: Poor posture detected!");

  // LED on
  digitalWrite(LED_PIN, HIGH);

  // Buzzer beep pattern
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1000, 300);
    delay(500);
  }

  // IFTTT email
  postToIFTTT(tilt);
}

// Reset alert outputs
void resetAlert() {
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  Serial.println("Posture corrected - alert reset");
}

// Send IFTTT Webhook POST
void postToIFTTT(float tilt) {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  WiFiSSLClient wifi;
  HttpClient client = HttpClient(wifi, IFTTT_HOST, 443);

  String path = "/trigger/";
  path += IFTTT_EVENT;
  path += "/with/key/";
  path += IFTTT_KEY;

  String body = "{\"value1\":\"";
  body += String(tilt, 1);
  body += " degrees\",\"value2\":\"";
  body += String(alertCount);
  body += " alerts today\",\"value3\":\"Correct your posture!\"}";

  client.beginRequest();
  client.post(path);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", body.length());
  client.beginBody();
  client.print(body);
  client.endRequest();

  int statusCode = client.responseStatusCode();
  Serial.print("IFTTT response: ");
  Serial.println(statusCode);
}

// Post tilt data to ThingSpeak
void postToThingSpeak(float tilt) {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, TS_HOST, 80);

  String path = "/update?api_key=";
  path += TS_API_KEY;
  path += "&field1=";
  path += String(tilt, 1);
  path += "&field2=";
  path += String(alertCount);

  client.get(path);

  int statusCode = client.responseStatusCode();
  Serial.print("ThingSpeak response: ");
  Serial.println(statusCode);
}

// Connect to WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  int attempts = 0;
  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi failed - continuing offline");
  }
}
