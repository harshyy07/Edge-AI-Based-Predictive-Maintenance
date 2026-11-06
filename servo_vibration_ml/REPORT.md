# Servo vibration pipeline report

## Scope

This is a separate pipeline for servo-mounted MPU6050 data. The project-root `train.py` now runs this workflow; the original industrial motor model and datasets remain untouched. The configured acquisition baseline is 200 Hz with 200 samples per one-second window. Synthetic signal components are kept below the 100 Hz Nyquist limit.

## Inputs found during implementation

No servo NORMAL CSV or synthetic servo CSV was found in the target project data folders or the inspected Downloads tree. The project contains CWRU `.mat` recordings and zipped MaFaulDA CSVs, but neither is used in this servo model. A pasted real servo experiment log was subsequently provided. It contains aggregate RMS/peak measurements labeled `DETACHED`, `STATIONARY_90`, `STATIONARY_45`, `STATIONARY_135`, and `MOVING`; readable entries are extracted to `results/real_summary_extracted.csv`. Some text is corrupted, so incomplete values are omitted. The log has no synchronized three-axis raw samples and no `NORMAL`/fault diagnosis labels, so it cannot train or validate this raw-window classifier.

## Method

The generator creates six labeled conditions with randomized phase, speed, amplitude, and noise for each independent synthetic trial. The 12 features are per-axis mean, standard deviation, RMS, and absolute peak. A Random Forest is evaluated with a held-out group split so that samples from a trial do not cross between training and evaluation. Metrics, a confusion matrix, and an ESP32-oriented C header are written under this directory.

## Results

With the default 50 generated windows per class and seed 42, the grouped split used 225 training and 75 held-out windows from 225 and 75 independent synthetic groups. Accuracy was 0.960 and macro-F1 was 0.9632. These are synthetic evaluation scores only and must not be described as real-fault validation. The real summary parser extracted 94 intact RMS values: 23 DETACHED, 15 STATIONARY_90, 21 STATIONARY_45, 16 STATIONARY_135, and 19 MOVING. Peak values were partially unreadable in the supplied text.

## Validation limits

The separate ESP32 sketch in `esp32/` drives the servo, extracts matching features, classifies each one-second window, and lights an LED and active buzzer after two consecutive non-NORMAL predictions. It compiled successfully with the installed ESP32 Arduino core 3.x. This was a compile check only; firmware was not uploaded or hardware-tested. Synthetic patterns are illustrative signal constructions, not measured physical faults. They cannot establish that friction, looseness, irregular movement, abnormal vibration, or stopped operation can be distinguished on a real actuator. Validate with independently captured, physically checked servo trials and representative mounting, load, supply, and motion conditions before using the classifier operationally.
