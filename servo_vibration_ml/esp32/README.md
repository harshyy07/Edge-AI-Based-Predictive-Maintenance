# ESP32 servo runtime

This is a separate experimental inference sketch. It drives a servo between 45° and 135° at 2° per 20 ms, reads the MPU6050 at 200 Hz, extracts the same 12 features as Python, and runs the generated Random Forest. A non-`NORMAL` label for two consecutive windows turns on GPIO 2 LED and GPIO 25 active buzzer. Two consecutive `NORMAL` windows clear the alert. `STOP` also alerts because it is non-NORMAL.

## Wiring and dependencies

- MPU6050 SDA GPIO 21, SCL GPIO 22, common ground.
- Servo signal GPIO 18. Power the servo from a suitable external supply and connect grounds; do not power it from the ESP32 3.3 V rail.
- LED GPIO 2 (or adjust `LED_PIN` for the board and external resistor wiring).
- Active buzzer module signal GPIO 25; adjust for the module as needed.
- Use the ESP32 Arduino core 3.x (the sketch uses its built-in LEDC PWM API; no external servo library is needed).

The model header is generated into this folder by running `python train.py` from the project root. Open this folder as the Arduino sketch directory and compile without uploading to the board. Use the data collector sketch under `sketch/` separately to capture labeled samples.

## Important validation limit

The current model is synthetic-heavy and has not been validated on real servo faults. Its scores do not establish hardware alert accuracy. Keep the first hardware runs supervised, compare predictions with known conditions, and collect independent real trials before depending on alerts. This sketch was not flashed to the ESP32.
