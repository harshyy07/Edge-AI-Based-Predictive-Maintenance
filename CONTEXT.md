# PROJECT CONTEXT: Edge-AI Predictive Maintenance & Fault Detection of an Industrial Motor

**Title:** Edge-AI Based Predictive Maintenance and Fault Detection of an Industrial Motor Using Vibration Sensing
**Goal:** Lightweight Edge-AI system on ESP32 + vibration sensor for real-time motor fault detection with reduced latency and communication overhead.
**Current phase:** SOFTWARE SIMULATION ONLY (hardware, e.g. MPU6050/ADXL345, will be connected later).
**Deliverables:** (1) software simulation, (2) PPT report, (3) demo video of the simulation.

---
## 1. Architecture
```
Simulated vibration -> Windowing -> Feature extraction -> TinyML model -> Result only (MQTT) -> Dashboard
 (ESP32 sensor)        (on edge)     (on edge)            (on edge)       (~30 bytes)
```
A **cloud baseline** (stream raw window, classify on server) is simulated to compare against the edge pipeline. That comparison is the core result.

## 2. Tools (all free)
| Purpose | Tool |
|---|---|
| Data simulation + ML | Python, NumPy, SciPy, scikit-learn, matplotlib |
| Model -> ESP32 C code | custom exporter in `train.py` (no extra dependency) |
| ESP32 simulation | Wokwi (ESP32 + potentiometer fault selector; real MPU6050 path in code) |
| Messaging | MQTT (public HiveMQ broker) |
| Dashboard | Streamlit |

## 3. Signal model (fs = 2 kHz, N = 1024 samples/window, shaft f0 = 25 Hz)
| Class | Signature |
|---|---|
| 0 normal | weak 1X + low noise |
| 1 imbalance | strong 1X |
| 2 misalignment | strong 2X, 3X |
| 3 bearing | periodic impulses (~3.58x f0) exciting a damped ~450 Hz resonance |
| 4 looseness | 0.5X sub-harmonic + harmonics + higher noise |

## 4. Features (12, computed on the ESP32)
rms, peak, crest factor, kurtosis, skewness, std, dominant freq, band energy in 0-40, 40-80, 80-150, 150-500, 500-1000 Hz.
Model: Random Forest (10 trees, depth 6) -> exported to C (`model.h`).

## 5. Edge vs Cloud comparison
- Cloud: raw window 1024 x int16 = 2048 B (+ protocol overhead), simulated network RTT 50-150 ms + jitter + server inference.
- Edge: local inference, send ~30 B JSON `{"s":"bearing","c":0.94}`.
- Metrics: bytes per minute, decision latency (ms).
- NOTE: edge compute time on ESP32 is an *assumed* constant until measured in Wokwi/hardware (firmware prints it).

## 6. Files
```
data_gen.py        signal generator (5 classes)
features.py        feature extraction + spectrum
train.py           train, evaluate, export model.h + plots
edge_vs_cloud.py   latency/bandwidth simulation + plots
dashboard.py       Streamlit live dashboard w/ fault injection (simulator or MQTT)
esp32_wokwi/       sketch.ino, diagram.json, libraries.txt, model.h
```

## 7. Run order
```
pip install -r requirements.txt
python train.py            # accuracy, confusion matrix, model.h
python edge_vs_cloud.py    # comparison table + plots
streamlit run dashboard.py # live demo
```
Wokwi: new ESP32 project, paste sketch.ino, diagram.json, libraries.txt, model.h.

## 8. PPT outline (12-14 slides)
1 Title | 2 Problem/motivation | 3 Objectives | 4 Literature/existing systems | 5 Proposed Edge-AI approach | 6 Architecture | 7 Fault signatures (waveform/FFT) | 8 Data gen + features | 9 ML model + TinyML | 10 Simulation setup (Wokwi, MQTT, dashboard) | 11 Accuracy + confusion matrix | 12 Edge vs cloud results | 13 Hardware integration plan | 14 Conclusion + references

## 9. Demo video script (3-5 min, OBS Studio)
20s problem/idea | 30s architecture | 60s normal state on dashboard | 90s inject faults, show status/FFT/alert | 45s edge vs cloud panel | 30s results + hardware next steps

## 10. Hardware-later plan
Replace synthetic generator in `sketch.ino` (`USE_REAL_MPU 1`) with MPU6050/ADXL345 sampling at 2 kHz; retrain on real recorded data; optionally validate on CWRU bearing dataset.
