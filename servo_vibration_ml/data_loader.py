"""Load labeled servo MPU6050 CSV windows without touching legacy datasets."""
from __future__ import annotations

from pathlib import Path
import pandas as pd

AXES = ("ax_g", "ay_g", "az_g")


def load_servo_csv(path: str | Path) -> pd.DataFrame:
    frame = pd.read_csv(path)
    cols = {str(c).strip().lower(): c for c in frame.columns}
    def find(*names: str) -> str | None:
        return next((cols[n] for n in names if n in cols), None)
    axis_cols = [find("ax_g", "ax", "accel_x", "x"), find("ay_g", "ay", "accel_y", "y"), find("az_g", "az", "accel_z", "z")]
    if any(c is None for c in axis_cols):
        raise ValueError(f"{path}: expected acceleration columns ax_g, ay_g, az_g (or x/y/z aliases)")
    label_col = find("label", "class", "condition", "fault")
    if label_col is None:
        raise ValueError(f"{path}: expected a label/class/condition column")
    window_col = find("window_id", "window")
    trial_col = find("trial_id", "recording_id", "run_id")
    out = pd.DataFrame({"ax_g": pd.to_numeric(frame[axis_cols[0]], errors="coerce"),
                        "ay_g": pd.to_numeric(frame[axis_cols[1]], errors="coerce"),
                        "az_g": pd.to_numeric(frame[axis_cols[2]], errors="coerce"),
                        "label": frame[label_col].astype(str).str.strip().str.upper()})
    # Use the recording/trial as the validation group. window_id marks window
    # boundaries only; splitting windows from one capture across train/test
    # would leak session-specific vibration conditions.
    source = Path(path).resolve().stem
    out["group_id"] = (source + ":" + frame[trial_col].astype(str)) if trial_col else source
    if window_col:
        out["window_id"] = frame[window_col].astype(str)
    out["provenance"] = "real_servo_csv"
    out["sample_index"] = frame[find("sample_index", "sample", "index")] if find("sample_index", "sample", "index") else out.groupby("group_id").cumcount()
    out = out.dropna(subset=[*AXES]).reset_index(drop=True)
    return out


def load_inputs(paths: list[str]) -> pd.DataFrame:
    if not paths:
        return pd.DataFrame()
    return pd.concat([load_servo_csv(p) for p in paths], ignore_index=True)
