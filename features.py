"""12 lightweight features (identical formulas are implemented in esp32_wokwi/sketch.ino)."""
import numpy as np
from scipy.stats import kurtosis, skew
from data_gen import FS

FEATURE_NAMES = ["rms", "peak", "crest", "kurtosis", "skew", "std", "dom_freq",
                 "band_0_40", "band_40_80", "band_80_150", "band_150_500", "band_500_1000"]


def spectrum(x):
    F = np.abs(np.fft.rfft(x * np.hanning(len(x)))) / len(x)
    fr = np.fft.rfftfreq(len(x), 1 / FS)
    return fr, F


def features(x):
    rms = np.sqrt(np.mean(x ** 2))
    pk = np.max(np.abs(x))
    fr, F = spectrum(x)
    band = lambda lo, hi: F[(fr >= lo) & (fr < hi)].sum()
    return [rms, pk, pk / rms, kurtosis(x), skew(x), x.std(), fr[np.argmax(F)],
            band(0, 40), band(40, 80), band(80, 150), band(150, 500), band(500, 1000)]
