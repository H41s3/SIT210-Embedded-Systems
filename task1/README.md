# Task 1.1P: Switching ON Lights — Modular Programming Approach

## System Description

This project implements a light control system for Linda, a resident of a smart elderly care home. When Linda arrives home at night, she presses a push button which triggers two lights:

- **Porch light** — turns ON for **30 seconds**
- **Hallway light** — turns ON for **60 seconds**

Both lights turn off automatically after their respective durations. The system is built using an **Arduino Nano 33 IoT** and follows a **modular programming approach**.

---

## Hardware Used

- Arduino Nano 33 IoT
- 2x LEDs (Porch light, Hallway light)
- 2x 220Ω Resistors
- 1x Push button
- Breadboard and jumper wires
- Micro-USB to USB-A cable

## Wiring

| Component       | Arduino Pin |
|----------------|-------------|
| Porch LED (+)  | D2          |
| Hallway LED (+)| D3          |
| Push Button    | D4          |
| LED (-) legs   | GND (via 220Ω resistor) |
| Button other leg | GND       |

---

## Code Structure & Function Explanations

### `setupPins()`
Initialises all digital pins. Sets the LED pins as OUTPUT and the button pin as INPUT with internal pull-up resistor enabled. This means the button reads LOW when pressed and HIGH when released.

### `isButtonPressed()`
Reads the digital state of the button pin and returns `true` if the button is pressed (reads LOW due to INPUT_PULLUP). This isolates all input-reading logic into one reusable function.

### `activatePorchLight()`
Turns the porch LED on by writing HIGH to pin D2, waits 30 seconds using `delay(30000)`, then turns it off. All porch light logic is contained here.

### `activateHallwayLight()`
Turns the hallway LED on by writing HIGH to pin D3, waits 60 seconds using `delay(60000)`, then turns it off. All hallway light logic is contained here.

### `setup()`
Arduino's built-in initialisation function. Calls `setupPins()` and starts Serial communication for debugging.

### `loop()`
Arduino's main loop. Continuously checks if the button is pressed using `isButtonPressed()`, and if so, calls `activatePorchLight()` followed by `activateHallwayLight()`.

---

## Software

- Arduino IDE 2 / Arduino Web IDE
- Language: C/C++ (Arduino framework)
