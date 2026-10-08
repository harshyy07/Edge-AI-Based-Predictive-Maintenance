"""Synthetic vibration generator for 5 motor conditions."""
import numpy as np

FS, N, F0 = 2000, 1024, 25.0          # sample rate, window length, shaft freq (Hz)
CLASSES = ["normal", "imbalance", "misalignment", "bearing", "looseness"]
t = np.arange(N) / FS


def gen(cls, rng):
    f0 = F0 * rng.uniform(0.985, 1.015)        # slight speed variation
    a = rng.uniform(0.85, 1.15)                # amplitude variation
    sig = rng.uniform(0.1, 0.16)               # noise level
    n = rng.normal(0, sig, N)
    s = lambda k, amp: a * amp * np.sin(2 * np.pi * k * f0 * t + rng.uniform(0, 2 * np.pi))
    if cls == 0:
        x = s(1, 0.5) + n
    elif cls == 1:
        x = s(1, 1.6) + n
    elif cls == 2:
        x = s(1, 0.6) + s(2, 0.9) + s(3, 0.4) + n
    elif cls == 3:
        x = s(1, 0.5) + n
        bpfo = 3.58 * f0
        for k in np.arange(rng.uniform(0, 1 / bpfo), N / FS, 1 / bpfo):
            i = int(k * FS)
            L = min(60, N - i)
            tau = np.arange(L) / FS
            x[i:i + L] += a * 1.5 * np.exp(-tau * 600) * np.sin(2 * np.pi * 450 * tau)
    else:
        x = s(0.5, 0.5) + s(1, 0.7) + s(2, 0.5) + s(3, 0.4) + 2 * n
    return x


def make_dataset(per_class=400, seed=0):
    from features import features
    rng = np.random.default_rng(seed)
    X, y = [], []
    for c in range(len(CLASSES)):
        for _ in range(per_class):
            X.append(features(gen(c, rng)))
            y.append(c)
    return np.array(X, dtype=np.float32), np.array(y)
