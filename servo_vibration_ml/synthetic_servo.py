"""Synthetic MPU6050 servo vibration generator; these are not measured faults."""
from __future__ import annotations
import numpy as np
import pandas as pd

LABELS = ("NORMAL", "HIGH_FRICTION", "MOUNTING_LOOSENESS", "ABNORMAL_VIBRATION", "IRREGULAR_MOVEMENT", "STOP")
FS_HZ = 200
WINDOW_SAMPLES = 200


def generate_windows(windows_per_class: int = 50, seed: int = 42) -> pd.DataFrame:
    rng = np.random.default_rng(seed)
    t = np.arange(WINDOW_SAMPLES) / FS_HZ
    rows = []
    for label in LABELS:
        for w in range(windows_per_class):
            # Randomize nuisance factors per independent trial to support grouped evaluation.
            group = f"syn_{label.lower()}_{w:04d}"
            phase, speed = rng.uniform(0, 2*np.pi), rng.uniform(1.5, 4.0)
            amp = rng.uniform(.015, .045)
            axes = np.column_stack([
                amp*np.sin(2*np.pi*speed*t+phase),
                .7*amp*np.sin(2*np.pi*(speed*1.3)*t+phase/2),
                1.0 + .5*amp*np.sin(2*np.pi*(speed*.8)*t+phase*1.4),
            ])
            if label == "HIGH_FRICTION":
                axes += np.column_stack([.035*np.sin(2*np.pi*18*t), .025*np.sin(2*np.pi*23*t), np.zeros_like(t)])
            elif label == "MOUNTING_LOOSENESS":
                pulse = np.maximum(0, np.sin(2*np.pi*7*t+phase))**5
                axes[:, 0] += .15*pulse; axes[:, 1] -= .08*pulse
            elif label == "ABNORMAL_VIBRATION":
                f = rng.uniform(35, 80)
                axes += np.column_stack([.11*np.sin(2*np.pi*f*t), .08*np.sin(2*np.pi*(f*.83)*t), .06*np.sin(2*np.pi*(f*.67)*t)])
            elif label == "IRREGULAR_MOVEMENT":
                mod = 1 + .75*np.sin(2*np.pi*.7*t+rng.uniform(0, 2*np.pi))
                axes[:, :2] *= mod[:, None]
                axes += rng.normal(0, .025, axes.shape)
            elif label == "STOP":
                axes[:, :2] *= .06
                axes[:, 2] = 1.0 + rng.normal(0, .004, len(t))
            axes += rng.normal(0, rng.uniform(.003, .012), axes.shape)
            for i, (x, y, z) in enumerate(axes):
                rows.append((group, i, x, y, z, label, "synthetic"))
    return pd.DataFrame(rows, columns=["group_id", "sample_index", "ax_g", "ay_g", "az_g", "label", "provenance"])
