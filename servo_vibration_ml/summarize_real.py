"""Extract intact summary statistics from the pasted real servo experiment log.

These aggregate RMS/peak observations are retained as real measurements but do
not contain the raw time series needed to calculate the 12 training features.
"""
from __future__ import annotations
import argparse, re
from pathlib import Path
import pandas as pd

LABELS = ("DETACHED", "STATIONARY_90", "STATIONARY_45", "STATIONARY_135", "MOVING")
LINE = re.compile(r"(?P<label>DETACHED|STATIONARY_90|STATIONARY_45|STATIONARY_135|MOVING)\s*\|\s*RMS\s*:\s*(?P<rms>\d+(?:\.\d+)?)\s*g?(?:\s*\|\s*Peak\s*:\s*(?P<peak>\d+(?:\.\d+)?)\s*g?)?", re.I)


def extract(path: Path) -> pd.DataFrame:
    text = path.read_text(encoding="utf-8", errors="replace")
    out=[]
    for i,line in enumerate(text.splitlines()):
        m=LINE.search(line)
        if m:
            out.append({"source_line": i+1, "condition":m.group("label").upper(), "rms_g":float(m.group("rms")), "peak_g":float(m.group("peak")) if m.group("peak") else None, "provenance":"real_servo_summary"})
    return pd.DataFrame(out)


def main():
    p=argparse.ArgumentParser(); p.add_argument("input",type=Path); p.add_argument("--out",type=Path,default=Path(__file__).resolve().parent/"results"/"real_summary_extracted.csv"); a=p.parse_args()
    df=extract(a.input); a.out.parent.mkdir(parents=True,exist_ok=True); df.to_csv(a.out,index=False)
    print(df.groupby("condition").agg(observations=("rms_g","size"),rms_mean_g=("rms_g","mean"),rms_min_g=("rms_g","min"),rms_max_g=("rms_g","max"),usable_peak_values=("peak_g","count")).round(6).to_string())
    print(f"Extracted {len(df)} intact RMS entries to {a.out}")

if __name__=="__main__": main()
