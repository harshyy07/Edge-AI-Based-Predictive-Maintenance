# Servo Phase 1: raw vibration data collection

This package collects labelled raw acceleration from an MPU6050 mounted on the servo-driven mechanism. It is a data collector only: it has no trained classifier and does not perform ML inference.

## Firmware and sampling

The ESP32 sketch samples acceleration at 200 Hz in windows of 200 samples (about one second per window), using the MPU6050 ±2 g range. It streams rows as:

`window_id,sample_index,timestamp_us,label,ax_g,ay_g,az_g`

The six operator-supplied labels are `NORMAL`, `HIGH_FRICTION`, `MOUNTING_LOOSENESS`, `ABNORMAL_VIBRATION`, `IRREGULAR_MOVEMENT`, and `STOP`. Labels apply to samples captured after the serial command is received. Use the same mechanical protocol consistently across labelled captures and record the physical setup separately in your experiment notes.

## Wiring

- MPU6050 VCC to the sensor board's supported supply, GND to GND, SDA to ESP32 GPIO 21, SCL to GPIO 22.
- Connect and power the servo according to its rated supply; share ground with the ESP32. Do not power a high-current servo from the ESP32 3.3 V pin.
- Verify the sensor address is 0x68 or 0x69. The sketch currently uses 0x68.

## Compile and flash

Compile with Arduino CLI for `esp32:esp32:esp32` and flash using your normal development setup. This project task compiles only; it does not upload firmware.

## Capture on COM3

Install the serial dependency in the Python environment you use:

```powershell
python -m pip install pyserial
python tools\capture_servo_data.py --port COM3
```

Optionally choose an output file:

```powershell
python tools\capture_servo_data.py --port COM3 --output data\servo_session.csv
```

At the `label>` prompt, enter one of the six labels. Enter `q` to stop. The capture tool stores sensor values and a host receive timestamp in CSV under `data/` by default. The serial monitor must not be open on COM3 at the same time.

## Data collection notes

Capture each condition for multiple independent windows and repeat trials. Mark condition changes explicitly with the prompt. Do not assume a class is represented merely because it was selected: validate each trial's physical condition and annotate actuator angle, motion/hold state, supply, and mounting details in the experiment log. `STOP` marks the operator's stop condition; it does not itself stop the firmware.
