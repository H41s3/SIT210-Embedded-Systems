  // ── Pin Definitions ──────────────────────────────────
const int PIR_PIN    = 2;   // PIR sensor  — D2, RISING interrupt
const int SWITCH_PIN = 3;   // Slider switch — D3, CHANGE interrupt
const int LED1_PIN   = 4;   // LED 1 output
const int LED2_PIN   = 5;   // LED 2 output
const int LDR_PIN    = A0;  // LDR module — analog input

// ── LDR Configuration ────────────────────────────────
// Higher analogRead() = darker. Tune by reading Serial in your space.
const int DARK_THRESHOLD = 600;

// ── State Variables ──────────────────────────────────
// volatile: prevents compiler caching — ISR and loop() share these.
volatile bool pirTriggered    = false;
volatile bool switchTriggered = false;
bool ledsOn = false;

// ── Interrupt Service Routines ───────────────────────
// ISRs only set a flag — no analogRead or Serial here.
void onMotionDetected() {
  pirTriggered = true;
}

void onSwitchToggled() {
  switchTriggered = true;
}

// ── Read Ambient Light ───────────────────────────────
int readLightLevel() {
  return analogRead(LDR_PIN);
}

// ── LED Control ──────────────────────────────────────
void setLEDs(bool state) {
  digitalWrite(LED1_PIN, state ? HIGH : LOW);
  digitalWrite(LED2_PIN, state ? HIGH : LOW);
  ledsOn = state;
}

// ── PIR Event Handler ────────────────────────────────
void handlePIR() {
  int lightLevel = readLightLevel();
  Serial.print("[PIR] Motion detected. LDR: ");
  Serial.println(lightLevel);
  if (lightLevel > DARK_THRESHOLD) {
    setLEDs(true);
    Serial.println("[PIR] Dark — LEDs switched ON.");
  } else {
    Serial.println("[PIR] Bright — LEDs remain OFF.");
  }
}

// ── Switch Event Handler ─────────────────────────────
void handleSwitch() {
  setLEDs(!ledsOn);
  Serial.print("[SWITCH] Slider activated — LEDs now: ");
  Serial.println(ledsOn ? "ON" : "OFF");
}

// ── Setup ────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN,    INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP); // prevents false triggers
  pinMode(LED1_PIN,   OUTPUT);
  pinMode(LED2_PIN,   OUTPUT);

  setLEDs(false);

  attachInterrupt(digitalPinToInterrupt(PIR_PIN),    onMotionDetected, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), onSwitchToggled,  CHANGE);

  Serial.println("=== Task 4.1P: Interrupt-Based Lighting System ===");
  Serial.println("Waiting for motion or switch input...");
}

// ── Loop ─────────────────────────────────────────────
// Checks ISR flags — all real work handled here, not in ISRs.
void loop() {
  if (pirTriggered) {
    pirTriggered = false;
    handlePIR();
  }

  if (switchTriggered) {
    switchTriggered = false;
    handleSwitch();
  }
}


