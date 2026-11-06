# Edge-AI Based Predictive Maintenance and Motor Fault Detection

[![Python 3.10+](https://img.shields.io/badge/python-3.10+-blue.svg)](https://www.python.org/downloads/)
[![Streamlit](https://img.shields.io/badge/Streamlit-1.37+-FF4B4B.svg)](https://streamlit.io/)
[![TinyML](https://img.shields.io/badge/TinyML-ESP32-green.svg)](https://wokwi.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An end-to-end TinyML and IoT system for real-time condition-based monitoring (CBM) of industrial motors using vibration sensing.

Instead of streaming bandwidth-heavy raw vibration data to a cloud server, this project implements an **on-device Machine Learning pipeline on an ESP32 microcontroller**. The ESP32 extracts time and spectral features locally, executes a compiled tree-ensemble classifier, and transmits only lightweight diagnostic alerts over MQTT.

---

## ⚡ Key Highlights & Results

- **>95% Bandwidth Reduction**: Transmits compact JSON diagnostic telemetry with predicted class, confidence, and alert state instead of streaming raw sensor windows.
- **Ultra-low Decision Latency**: Local inference latency of ~20 ms on ESP32 vs ~125–170 ms round-trip time in cloud setups.
- **Zero-Dependency TinyML Export**: The Extra Trees model is exported directly into pure C header code (`sketch/model.h`).
- **Modern Live Dashboard**: Interactive dark-themed Streamlit UI with real-time waveform inspection, FFT harmonics, fault injection simulator, and MQTT stream listener.

---

## 🏗️ System Architecture

```text
[Vibration Sensor (MPU6050 / Sim)]
               │
               ▼
   [Windowing (1024 samples @ 1 kHz)]
               │
               ▼
[Feature Extraction (12 Time & Spectral Features)]
               │
               ▼
 [TinyML Extra Trees (compiled in sketch/model.h)]
               │
               ▼  (Inference Result: ~30 bytes)
        [MQTT Broker]
               │
               ▼
[Streamlit Live Monitoring Dashboard]
```

### 5 Model Classes:
1. **Normal**
2. **Mass imbalance**
3. **Shaft misalignment**
4. **Bearing outer race fault**
5. **Bearing ball fault**

The bundled recordings do not include a labeled mechanical-looseness class, so the firmware does not claim to detect it.

---

## 📁 Repository Structure

```text
├── dashboard.py           # Streamlit real-time monitoring dashboard
├── train.py               # Recording-level model training entry point
├── train_hardware.py      # Real-data loading, grouped evaluation, and C export
├── features.py            # Time/frequency domain feature extraction
├── data_gen.py            # MaFaulDA vibration playback for dashboard simulation
├── edge_vs_cloud.py       # Latency and bandwidth benchmark simulation
├── sketch/edge_features.h # C implementation of feature extraction for ESP32
├── sketch/model.h         # Exported C model used by the firmware
├── model_mafaulda.h       # Exported C code trained on MaFaulDA dataset
├── model_combined.h       # Exported C code trained on combined MaFaulDA + CWRU (97.9% Acc)
├── train_mafaulda.py      # Legacy MaFaulDA trainer
├── train_combined.py      # Legacy combined-dataset trainer
├── download_sample.py     # Script to pull targeted MaFaulDA files
├── download_cwru.py       # Script to pull CWRU .mat benchmark files
├── sketch/sketch.ino     # ESP32 hardware firmware
├── esp32_wokwi/           # Wokwi simulation files
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
Loads MaFaulDA and CWRU recordings, trains and evaluates compact tree-ensemble candidates by holding out complete recordings, produces validation plots in `results/`, and exports `model.h`:
```bash
python train.py
```

The current trainer uses MaFaulDA and CWRU recordings, resamples them to the MPU6050's 1 kHz accelerometer rate, and holds out complete recordings during evaluation. The latest held-out run scored 82.5% accuracy and 0.820 macro-F1 on 40 windows from five held-out recordings. Recall was 62.5% for imbalance and 50.0% for misalignment. This small dataset-level evaluation is preliminary; validate with labeled recordings from the assembled motor before relying on alerts.

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
- **Frequency-Domain**: Dominant Frequency and relative band energy (0–20 Hz, 20–40 Hz, 40–80 Hz, 80–200 Hz, 200–500 Hz).

---

## 📜 License
This project is open-source under the [MIT License](LICENSE).
