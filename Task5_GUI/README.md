# Task 5.1P — Making a Graphical User Interface

## Overview
A Python Tkinter GUI application running on a Raspberry Pi 5 that allows a user to control three LEDs representing rooms in a house (Living Room, Bathroom, Closet).

## Hardware Required
- Raspberry Pi 5
- 3 LEDs (any colour)
- 3 x 330Ω resistors
- Jumper wires
- Breadboard

## Wiring
| LED / Room   | GPIO (BCM) | Physical Pin | Resistor | GND Pin |
|--------------|------------|--------------|----------|---------|
| Living Room  | GPIO 17    | Pin 11       | 330Ω     | Pin 9   |
| Bathroom     | GPIO 27    | Pin 13       | 330Ω     | Pin 14  |
| Closet       | GPIO 22    | Pin 15       | 330Ω     | Pin 20  |

## Software Required
- Raspberry Pi OS (64-bit)
- Python 3
- Tkinter (built-in with Python)
- RPi.GPIO (pre-installed on Raspberry Pi OS)

## How to Run
```bash
python3 task5_gui.py
```

## How It Works
- GUI has 3 radio buttons — one per room
- Selecting a radio button calls `control_leds()` which turns on the selected LED and turns off all others
- Exit button calls `on_exit()` which cleans up GPIO and closes the window
