# Servo Vibration Monitoring with Edge AI

This project now uses the servo MPU6050 capture workflow as its primary run path. It samples at 200 Hz, records one-second windows, trains a separate servo classifier, and exports an ESP32-oriented C model header. The earlier industrial motor trainer, its datasets, firmware, and model headers remain available as a legacy workflow and are not replaced by servo training.

## Quick start

Install the Python dependencies, including `pyserial` for capture:

```powershell
python -m pip install -r requirements.txt
```

Connect the MPU6050 to the ESP32 collector, then capture labeled samples:

```powershell
python tools\capture_servo_data.py --port COM3
```

At `label>`, enter one of `NORMAL`, `HIGH_FRICTION`, `MOUNTING_LOOSENESS`, `ABNORMAL_VIBRATION`, `IRREGULAR_MOVEMENT`, or `STOP`. The default capture path is under `data\`. Record independent trials in separate files when possible, and document the physical condition, servo angle, load, motion state, mounting, and supply.

Train and evaluate:

```powershell
python train.py
```

The entry point discovers `data\servo_phase1_*.csv` automatically and combines those raw samples with randomized synthetic windows. Explicit input files can be supplied with `python train.py --csv path1.csv path2.csv`. Group splitting holds out complete capture files or explicit trial IDs; adjacent windows from one capture do not cross the split. Outputs are isolated under `servo_vibration_ml\results\`, and the generated servo model is `servo_vibration_ml\model.h`.

The synthetic generator uses 200 Hz, 200-sample windows, and components below 100 Hz. Its 12 features are per-axis mean, standard deviation, RMS, and absolute peak. See [servo_vibration_ml/README.md](servo_vibration_ml/README.md) for the detailed feature contract and CSV format.

## Real measurements currently available

The provided pasted experiment log is real servo data, with aggregate RMS and peak measurements for DETACHED, STATIONARY_90, STATIONARY_45, STATIONARY_135, and MOVING. The readable aggregate values are extracted to `servo_vibration_ml\results\real_summary_extracted.csv`. The text does not contain raw three-axis samples or NORMAL/fault labels, so it is not used to train or evaluate the raw-window classifier. Current synthetic scores must not be presented as real fault validation.

## ESP32 and validation status

`sketch\sketch.ino` remains the labeled data collector. The separate runtime sketch in `servo_vibration_ml\esp32\` drives a servo, reads the MPU6050, extracts features, classifies windows, and turns on an LED and active buzzer after two consecutive non-NORMAL predictions. It uses the generated `servo_vibration_ml\esp32\model.h`; training refreshes this copy without touching the industrial model. Compile with the ESP32 Arduino core 3.x. The generated model is synthetic-heavy until sufficient independently recorded real trials are available. No firmware is uploaded by the training command.

## Legacy industrial motor workflow

The original industrial project is preserved. Run `python train_hardware.py` for its recording-held-out motor model training. MaFaulDA and CWRU files remain in their existing directories, and the existing root and sketch model headers, Wokwi project, dashboard, and reports are not changed by servo training. The prior root `README.md`, `train.py`, and `requirements.txt` are backed up under `servo_vibration_ml\backups\`.
