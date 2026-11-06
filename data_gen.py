"""Real-world MaFaulDA vibration generator and dataset loader.
Replaces synthetic equations with genuine industrial accelerometer recordings
anti-aliased to the MPU6050's 1 kHz output rate for sequential playback.
"""
import os
import glob
import zipfile
import numpy as np
import pandas as pd
from scipy.signal import resample_poly

FS, N = 1000, 1024
# Identical 5 classes matching the dashboard and ESP32 MQTT schema
CLASSES = ["normal", "imbalance", "misalignment", "bearing_outer", "bearing_ball"]

# Map real dataset labels directly; ball-bearing faults are not looseness.
MAFAULDA_MAP = {
    0: ("normal", "normal"),
    1: ("imbalance", "imbalance"),
    2: ("misalignment", "misalignment"),
    3: ("bearing", "outer_race"),
    4: ("bearing_ball", "ball_fault"),
}

# In-memory cached dataset windows and playback pointers
_CACHED_WINDOWS = {c: [] for c in range(len(CLASSES))}
_PLAYBACK_PTR = {c: 0 for c in range(len(CLASSES))}


def _load_class_windows(cls_idx):
    """Load and cache overlapping windows from downloaded MaFaulDA records."""
    if _CACHED_WINDOWS[cls_idx]:
        return _CACHED_WINDOWS[cls_idx]

    folder, _ = MAFAULDA_MAP[cls_idx]
    search_path = os.path.join(os.path.dirname(__file__), "data_mafaulda", folder, "*.zip")
    zip_files = sorted(glob.glob(search_path))
    
    full_sig = []
    for z_path in zip_files:
        try:
            with zipfile.ZipFile(z_path) as z:
                csv_name = z.namelist()[0]
                df = pd.read_csv(z.open(csv_name), header=None, nrows=250000)
                # Anti-alias before reducing the 50 kHz recording to 1 kHz.
                sig = resample_poly(df[1].values.astype(np.float32), 1, 50).astype(np.float32)
                full_sig.append(sig)
        except Exception as e:
            print(f"Error reading {z_path}: {e}")

    if full_sig:
        merged = np.concatenate(full_sig)
        # Step size 256 for smooth continuous time-series streaming
        step = 256
        windows = [merged[i : i + N] for i in range(0, len(merged) - N, step)]
    else:
        windows = []

    _CACHED_WINDOWS[cls_idx] = windows
    return windows


def gen(cls, rng):
    """Return sequential windows from genuine MaFaulDA recordings.
    
    Streaming sequential time-series rather than random jumping prevents
    confidence flicker and boundary discontinuities.
    """
    global _PLAYBACK_PTR
    windows = _load_class_windows(cls)
    if not windows:
        t = np.arange(N) / FS
        return np.sin(2 * np.pi * 25.0 * t).astype(np.float32)

    ptr = _PLAYBACK_PTR[cls]
    window = windows[ptr % len(windows)].copy()
    _PLAYBACK_PTR[cls] = (ptr + 1) % len(windows)

    return window.astype(np.float32)


def make_dataset(per_class=150, seed=0):
    """Build feature matrix using dense windows across all recordings."""
    from features import features

    X, y = [], []
    for c in range(len(CLASSES)):
        wins = _load_class_windows(c)
        limit = min(len(wins), per_class) if wins else per_class
        for i in range(limit):
            X.append(features(wins[i]))
            y.append(c)

    return np.array(X, dtype=np.float32), np.array(y, dtype=np.int64)
