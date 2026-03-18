#define BUTTON_PIN  2
#define PORCH_LED   5
#define HALLWAY_LED 3

void setupPins() {
  pinMode(PORCH_LED,   OUTPUT);
  pinMode(HALLWAY_LED, OUTPUT);
  pinMode(BUTTON_PIN,  INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}

void activatePorchLight() {
  digitalWrite(PORCH_LED, HIGH);
  delay(30000);
  digitalWrite(PORCH_LED, LOW);
}

void activateHallwayLight() {
  digitalWrite(HALLWAY_LED, HIGH);
  delay(60000);
  digitalWrite(HALLWAY_LED, LOW);
}

void setup() {
  setupPins();
}

void loop() {
  if (isButtonPressed()) {
    delay(50);
    activatePorchLight();
    activateHallwayLight();
  }
}
