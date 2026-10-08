# Edge-AI Based Predictive Maintenance and Motor Fault Detection

[![Python 3.10+](https://img.shields.io/badge/python-3.10+-blue.svg)](https://www.python.org/downloads/)
[![Streamlit](https://img.shields.io/badge/Streamlit-1.37+-FF4B4B.svg)](https://streamlit.io/)
[![TinyML](https://img.shields.io/badge/TinyML-ESP32-green.svg)](https://wokwi.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An end-to-end TinyML and IoT system for real-time condition-based monitoring (CBM) of industrial motors using vibration sensing.

Instead of streaming bandwidth-heavy raw vibration data to a cloud server, this project implements an **on-device Machine Learning pipeline on an ESP32 microcontroller**. The ESP32 extracts time and spectral features locally, executes a compiled Random Forest classifier, and transmits only lightweight diagnostic alerts over MQTT.

---

## ⚡ Key Highlights & Results

- **>95% Bandwidth Reduction**: Transmits only ~30-byte JSON diagnostic telemetry (`{"s": "bearing", "c": 0.96}`) instead of streaming 2+ KB raw sensor windows every cycle.
- **Ultra-low Decision Latency**: Local inference latency of ~20 ms on ESP32 vs ~125–170 ms round-trip time in cloud setups.
- **Zero-Dependency TinyML Export**: The Random Forest model is exported directly into pure C header code (`model.h`), fitting within only ~5.1 KB of flash memory.
- **Modern Live Dashboard**: Interactive dark-themed Streamlit UI with real-time waveform inspection, FFT harmonics, fault injection simulator, and MQTT stream listener.

---

## 🏗️ System Architecture

```text
[Vibration Sensor (MPU6050 / Sim)]
               │
               ▼
   [Windowing (1024 samples @ 2 kHz)]
               │
               ▼
[Feature Extraction (12 Time & Spectral Features)]
               │
               ▼
 [TinyML Random Forest (Compiled in model.h)]
               │
               ▼  (Inference Result: ~30 bytes)
        [MQTT Broker]
               │
               ▼
[Streamlit Live Monitoring Dashboard]
```

### 5 Detectable Motor Conditions:
1. **Normal Baseline**: Low background vibration and weak 1X shaft harmonic.
2. **Mass Imbalance**: Strong fundamental harmonic (1X, 25 Hz).
3. **Shaft Misalignment**: Elevated 2X and 3X shaft harmonics (50 Hz, 75 Hz).
4. **Bearing Outer Race Fault**: High-frequency periodic impulses exciting resonance (~450 Hz).
5. **Mechanical Looseness**: 0.5X sub-harmonic, fractional harmonics, and higher random noise.

---

## 📁 Repository Structure

```text
├── dashboard.py           # Streamlit real-time monitoring dashboard
├── train.py               # ML training, cross-validation, and C code exporter
├── features.py            # Time/frequency domain feature extraction
├── data_gen.py            # Physics-based synthetic vibration generator
├── edge_vs_cloud.py       # Latency and bandwidth benchmark simulation
├── edge_features.h        # C implementation of feature extraction for ESP32
├── model.h                # Exported C code of trained Random Forest
├── sketch.ino             # ESP32 firmware for Wokwi simulation / hardware
├── diagram.json           # Wokwi simulation hardware wiring
├── libraries.txt          # Arduino / Wokwi libraries
├── requirements.txt       # Python dependencies
└── results/               # Confusion matrix, feature importance & benchmark plots
```

---

## 🚀 Getting Started

### 1. Prerequisites & Installation

Clone the repository and install the required dependencies:

```bash
git clone https://github.com/harshyy07/Edge-AI-Based-Predictive-Maintenance.git
cd Edge-AI-Based-Predictive-Maintenance
pip install -r requirements.txt
```

---

### 2. Step-by-Step Execution

#### Step 1: Train Model & Export TinyML C Header
Generates synthetic dataset, trains the Random Forest classifier, produces validation plots in `results/`, and exports `model.h`:
```bash
python train.py
```

#### Step 2: Run Edge vs Cloud Latency & Bandwidth Benchmark
Simulates network jitter, payload sizes, and generates comparative metrics:
```bash
python edge_vs_cloud.py
```

#### Step 3: Launch the Interactive Dashboard
Start the real-time condition monitoring UI:
```bash
python -m streamlit run dashboard.py
```
Open **`http://localhost:8501`** in your browser.
- **Simulator Mode**: Use the sidebar to inject faults (Normal, Imbalance, Bearing, etc.) and observe live waveforms, FFT spectra, and alert triggers.
- **MQTT Mode**: Connects to a live MQTT broker to receive real-time packets from your ESP32.

---

### 3. Running Firmware Simulation in Wokwi (Optional)

1. Open [Wokwi ESP32 Simulator](https://wokwi.com/).
2. Create a new **ESP32** project.
3. Copy and paste the contents of:
   - `sketch.ino` into the main sketch tab.
   - `diagram.json` into the diagram tab.
   - `libraries.txt` into the library manager.
   - `edge_features.h` and `model.h` as additional project files.
4. Click **Start Simulation** to observe serial diagnostics and MQTT publishing.

---

## 📊 Features Extracted on Edge (12 Total)
- **Time-Domain**: RMS, Peak value, Crest Factor, Kurtosis, Skewness, Standard Deviation.
- **Frequency-Domain**: Dominant Frequency, Band Energy across 5 spectrum bands (0–40 Hz, 40–80 Hz, 80–150 Hz, 150–500 Hz, 500–1000 Hz).

---

## 📜 License
This project is open-source under the [MIT License](LICENSE).
