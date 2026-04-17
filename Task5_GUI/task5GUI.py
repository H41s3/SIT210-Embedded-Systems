import tkinter as tk
import RPi.GPIO as GPIO

# GPIO setup using BCM pin numbering
GPIO.setmode(GPIO.BCM)
PINS = {"Living Room": 17, "Bathroom": 27, "Closet": 22}
for pin in PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

def control_leds(room):
    """Turn on the selected room LED, turn off all others."""
    for name, pin in PINS.items():
        GPIO.output(pin, GPIO.HIGH if name == room else GPIO.LOW)

def on_exit():
    """Clean up GPIO and close the GUI window."""
    GPIO.cleanup()
    root.destroy()

# GUI setup
root = tk.Tk()
root.title("Linda's Light Controller")
root.geometry("300x200")

tk.Label(root, text="Select Room to Turn On Light",
         font=("Arial", 12)).pack(pady=10)

selected = tk.StringVar(value="Living Room")
for room in PINS:
    tk.Radiobutton(root, text=room, variable=selected,
                   value=room,
                   command=lambda r=room: control_leds(r)).pack(
                       anchor="w", padx=40)

tk.Button(root, text="Exit", command=on_exit,
          bg="red", fg="white").pack(pady=15)

root.mainloop()
