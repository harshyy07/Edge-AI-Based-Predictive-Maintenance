# PROJECT CONTEXT: Edge-AI Predictive Maintenance & Fault Detection of an Industrial Motor

**Title:** Edge-AI Based Predictive Maintenance and Fault Detection of an Industrial Motor Using Vibration Sensing
**Goal:** Lightweight Edge-AI system on ESP32 + vibration sensor for real-time motor fault detection with reduced latency and communication overhead.
**Current phase:** Firmware, model, and MQTT dashboard software are implemented. The board is currently disconnected; hardware validation will happen after it is reconnected.
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

## 3. Signal model (MPU6050 output rate = 1 kHz, N = 1024 samples/window)
| Class | Signature |
|---|---|
| 0 normal | weak 1X + low noise |
| 1 imbalance | strong 1X |
| 2 misalignment | strong 2X, 3X |
| 3 bearing_outer | Outer-race bearing fault recordings |
| 4 bearing_ball | Ball-bearing fault recordings |

## 4. Features (12, computed on the ESP32)
rms, peak, crest factor, kurtosis, skewness, std, dominant freq, relative band energy in 0-20, 20-40, 40-80, 80-200, 200-500 Hz.
Model: recording-balanced, gain-augmented Extra Trees -> exported to C (`sketch/model.h`). Latest held-out evaluation: 82.5% accuracy and 0.820 macro-F1 across 40 windows from five recordings; this is preliminary and does not establish performance on the assembled motor. The real-data dataset does not include mechanical looseness.

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
sketch/            hardware sketch.ino, edge_features.h, model.h
esp32_wokwi/       Wokwi simulation files and model.h
```

## 7. Run order
```
pip install -r requirements.txt
python train.py            # accuracy, confusion matrix, model.h
python edge_vs_cloud.py    # comparison table + plots
streamlit run dashboard.py # live demo
```
Wokwi: use the simulation files under `esp32_wokwi/`. Hardware firmware is under `sketch/`.

## 8. PPT outline (12-14 slides)
1 Title | 2 Problem/motivation | 3 Objectives | 4 Literature/existing systems | 5 Proposed Edge-AI approach | 6 Architecture | 7 Fault signatures (waveform/FFT) | 8 Data gen + features | 9 ML model + TinyML | 10 Simulation setup (Wokwi, MQTT, dashboard) | 11 Accuracy + confusion matrix | 12 Edge vs cloud results | 13 Hardware integration plan | 14 Conclusion + references

## 9. Demo video script (3-5 min, OBS Studio)
20s problem/idea | 30s architecture | 60s normal state on dashboard | 90s inject faults, show status/FFT/alert | 45s edge vs cloud panel | 30s results + hardware next steps

## 10. Hardware model validation
The firmware reads the MPU6050 at its native 1 kHz accelerometer output rate. The bundled model is trained from MaFaulDA and CWRU records; validate with labeled recordings from the assembled motor before relying on imbalance or unseen-fault alerts.
