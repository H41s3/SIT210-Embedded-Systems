/*
 * Task 3.1P - Create Trigger/Notification based on Sensor Data
 * SIT210 - Embedded Systems Development
 * 
 * Description:
 *   Reads light level from a Keyestudio LDR module on pin A0.
 *   When sunlight is detected (LDR value above threshold), sends
 *   an IFTTT webhook trigger to notify the user that sunlight has
 *   started hitting the terrarium.
 *   When sunlight stops, sends a second IFTTT trigger to notify
 *   the user that sunlight exposure has ended.
 * 
 * Hardware:
 *   - Arduino Nano 33 IoT
 *   - Keyestudio LDR Module (Signal -> A0, VCC -> 3.3V, GND -> GND)
 * 
 * Libraries Required:
 *   - WiFiNINA
 * 
 * Pin Assignments:
 *   - A0: LDR Signal
 */

#include <WiFiNINA.h>

// WiFi Credentials
const char* WIFI_SSID     = "Bikini Bottom";
const char* WIFI_PASSWORD = "Nobita88";

// ── IFTTT Configuration ───────────────────────────────────────
const char* IFTTT_KEY         = "bVslGLOlac8Wx51SWqDLzM";
const char* IFTTT_EVENT_ON    = "sunlight_start";   // Event name for sunlight detected
const char* IFTTT_EVENT_OFF   = "sunlight_stop";    // Event name for sunlight stopped
const char* IFTTT_HOST        = "maker.ifttt.com";

// ── Sensor Configuration ──────────────────────────────────────
const int   LDR_PIN           = A0;
const int   LIGHT_THRESHOLD   = 600;  // Values above this = sunlight detected
                                      // Adjust based on your environment

// ── Timing ───────────────────────────────────────────────────
const unsigned long CHECK_INTERVAL = 5000;  // Check every 5 seconds (ms)

// ── State Tracking ────────────────────────────────────────────
bool sunlightActive   = false;  // Current sunlight state
unsigned long lastCheckTime = 0;

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupPins();
  setupWiFi();
}

// ─────────────────────────────────────────────────────────────
void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = currentTime;

    int lightLevel = readLightLevel();
    Serial.print("Light Level: ");
    Serial.println(lightLevel);

    bool sunlightDetected = isSunlight(lightLevel);

    if (sunlightDetected && !sunlightActive) {
      // Sunlight just started
      sunlightActive = true;
      Serial.println("Sunlight DETECTED - sending IFTTT trigger...");
      sendIFTTTTrigger(IFTTT_EVENT_ON, lightLevel);

    } else if (!sunlightDetected && sunlightActive) {
      // Sunlight just stopped
      sunlightActive = false;
      Serial.println("Sunlight STOPPED - sending IFTTT trigger...");
      sendIFTTTTrigger(IFTTT_EVENT_OFF, lightLevel);
    }
  }
}

// ─────────────────────────────────────────────────────────────
// Sets up pin modes
void setupPins() {
  pinMode(LDR_PIN, INPUT);
  Serial.println("Pins initialised.");
}

// ─────────────────────────────────────────────────────────────
// Connects to WiFi network
void setupWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ─────────────────────────────────────────────────────────────
// Reads and returns the current LDR sensor value (0-1023)
int readLightLevel() {
  return analogRead(LDR_PIN);
}

// ─────────────────────────────────────────────────────────────
// Returns true if the light level indicates sunlight
bool isSunlight(int lightLevel) {
  return lightLevel > LIGHT_THRESHOLD;
}

// ─────────────────────────────────────────────────────────────
// Sends an IFTTT Webhook trigger for the given event name.
// Passes the current light level as value1.
void sendIFTTTTrigger(const char* eventName, int lightLevel) {
  WiFiSSLClient client;

  Serial.print("Connecting to IFTTT... ");

  if (!client.connect(IFTTT_HOST, 443)) {
    Serial.println("Connection failed.");
    return;
  }

  // Build JSON payload with light level as value1
  String payload = "{\"value1\":\"" + String(lightLevel) + "\"}";

  // Build the HTTP POST request
  String url = "/trigger/" + String(eventName) + "/with/key/" + String(IFTTT_KEY);

  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + String(IFTTT_HOST));
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + String(payload.length()));
  client.println("Connection: close");
  client.println();
  client.println(payload);

  // Wait for and print the response
  delay(1000);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  client.stop();
  Serial.println("IFTTT trigger sent: " + String(eventName));
}
