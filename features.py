"""12 lightweight features (identical formulas are implemented in esp32_wokwi/sketch.ino)."""
import numpy as np
from scipy.stats import kurtosis, skew
from data_gen import FS

FEATURE_NAMES = ["rms", "peak", "crest", "kurtosis", "skew", "std", "dom_freq",
                 "band_0_20", "band_20_40", "band_40_80", "band_80_200", "band_200_500"]


def spectrum(x):
    x = np.asarray(x, dtype=np.float32)
    x = x - np.mean(x)
    F = np.abs(np.fft.rfft(x * np.hanning(len(x)))) / len(x)
    fr = np.fft.rfftfreq(len(x), 1 / FS)
    return fr, F


def features(x):
    x = np.asarray(x, dtype=np.float32)
    x = x - np.mean(x)
    rms = np.sqrt(np.mean(x ** 2))
    pk = np.max(np.abs(x))
    fr, F = spectrum(x)
    bands = np.asarray([F[(fr >= lo) & (fr < hi)].sum()
                        for lo, hi in [(0, 20), (20, 40), (40, 80), (80, 200), (200, 500)]],
                       dtype=np.float32)
    bands /= bands.sum() + 1e-12
    crest = pk / rms if rms > 1e-9 else 0.0
    return [rms, pk, crest, kurtosis(x), skew(x), x.std(), fr[np.argmax(F)], *bands]
