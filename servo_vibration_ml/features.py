"""Twelve axis-wise time-domain features, matched to the C exporter contract."""
from __future__ import annotations
import numpy as np
import pandas as pd

FEATURE_NAMES = [f"{axis}_{name}" for axis in ("ax", "ay", "az") for name in ("mean", "std", "rms", "peak")]


def extract_window_features(window: pd.DataFrame | np.ndarray) -> np.ndarray:
    values = window[["ax_g", "ay_g", "az_g"]].to_numpy(float) if isinstance(window, pd.DataFrame) else np.asarray(window, dtype=float)
    if values.ndim != 2 or values.shape[1] != 3 or len(values) == 0:
        raise ValueError("Each window must be a non-empty N x 3 acceleration array")
    return np.asarray([v for col in values.T for v in (np.mean(col), np.std(col), np.sqrt(np.mean(col*col)), np.max(np.abs(col)))], dtype=np.float32)


def window_table(samples: pd.DataFrame, window_samples: int = 200) -> pd.DataFrame:
    rows = []
    if "window_id" in samples.columns:
        windows = samples.groupby(["group_id", "window_id"], sort=False)
    else:
        windows = ((group, part.iloc[offset:offset+window_samples])
                   for group, part in samples.groupby("group_id", sort=False)
                   for offset in range(0, len(part)-window_samples+1, window_samples))
    for key, window in windows:
        w = window.sort_values("sample_index")
        if len(w) != window_samples:
            continue
        labels = w.label.str.upper().unique()
        if len(labels) != 1:  # never train across operator label transitions
            continue
        row = dict(zip(FEATURE_NAMES, extract_window_features(w)))
        row.update(label=labels[0], group_id=str(key[0] if isinstance(key, tuple) else key), provenance=w.provenance.iloc[0])
        rows.append(row)
    return pd.DataFrame(rows)
