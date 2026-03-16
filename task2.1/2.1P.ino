#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define DHT_PIN     2
#define DHT_TYPE    DHT11
#define LDR_PIN     A0

const char* ssid     = "Bikini Bottom";
const char* password = "Nobita88";

unsigned long channelID = 3301172;
const char* apiKey      = "FV3BGGTZZI7L8D0I";

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient client;

// Connect to WiFi
void setupWiFi() {
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }
  Serial.println("\nConnected!");
}

// Read temperature from DHT11, returns Celsius
float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Temperature read failed!");
    return 0;
  }
  return temp;
}

// Read light level from LDR, returns 0-1023
int readLight() {
  return analogRead(LDR_PIN);
}

// Send both sensor readings to ThingSpeak
void sendToThingSpeak(float temperature, int light) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, light);

  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent!");
  } else {
    Serial.println("Send failed: " + String(response));
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  ThingSpeak.begin(client);
  setupWiFi();
}

// Read sensors and send to ThingSpeak every 30 seconds
void loop() {
  float temperature = readTemperature();
  int light         = readLight();

  Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.print("Light: ");
  Serial.println(light);

  sendToThingSpeak(temperature, light);
  delay(30000);
}