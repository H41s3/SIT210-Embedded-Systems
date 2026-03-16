#define BUTTON_PIN 2   // button on pin D2 (same as their diagram)
#define PORCH_LED  5   // porch LED on pin D5
#define HALLWAY_LED 3  // hallway LED on pin D3 (add second LED here)

void setup() {
  pinMode(PORCH_LED,   OUTPUT);
  pinMode(HALLWAY_LED, OUTPUT);
  pinMode(BUTTON_PIN,  INPUT_PULLUP);
}

void loop() {
  int button_state = digitalRead(BUTTON_PIN);

  if (button_state == LOW) {   // button pressed once
    delay(50);                 // debounce

    // Porch light ON for 30 seconds
    digitalWrite(PORCH_LED, HIGH);
    delay(30000);
    digitalWrite(PORCH_LED, LOW);

    // Hallway light ON for 60 seconds
    digitalWrite(HALLWAY_LED, HIGH);
    delay(60000);
    digitalWrite(HALLWAY_LED, LOW);
  }
}