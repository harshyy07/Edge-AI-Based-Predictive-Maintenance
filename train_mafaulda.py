"""Train TinyML Random Forest on real-world MaFaulDA vibration dataset."""
import os, glob, zipfile, time
import numpy as np, pandas as pd
import joblib
from scipy.stats import kurtosis, skew
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import classification_report, confusion_matrix, ConfusionMatrixDisplay
import matplotlib.pyplot as plt

CLASSES = ["normal", "imbalance", "misalignment", "bearing_outer", "bearing_ball"]
FS = 50000  # MaFaulDA native sampling frequency (50 kHz)
N = 1024    # Window size per edge frame


def extract_features(sig):
    """Extract identical 12 Edge-AI features matching edge_features.h."""
    rms = np.sqrt(np.mean(sig ** 2))
    peak = np.max(np.abs(sig))
    crest = peak / (rms + 1e-9)
    kurt = float(kurtosis(sig))
    skw = float(skew(sig))
    std = float(np.std(sig))

    # Spectral features via FFT
    F = np.abs(np.fft.rfft(sig))
    freqs = np.fft.rfftfreq(len(sig), 1.0 / FS)
    dom_freq = float(freqs[np.argmax(F[1:]) + 1]) if len(F) > 1 else 0.0

    bands = [(0, 500), (500, 1500), (1500, 3000), (3000, 6000), (6000, 12000)]
    b_eng = []
    for low, high in bands:
        mask = (freqs >= low) & (freqs < high)
        b_eng.append(float(np.sum(F[mask] ** 2)) if np.any(mask) else 0.0)

    return [rms, peak, crest, kurt, skw, std, dom_freq] + b_eng


def load_mafaulda_dataset(max_windows_per_file=150):
    category_map = {
        "normal": "normal",
        "imbalance": "imbalance",
        "misalignment": "misalignment",
        "bearing": "bearing_outer",
        "bearing_ball": "bearing_ball",
    }
    
    X, y = [], []
    print("Loading and windowing MaFaulDA vibration records...")
    
    for folder, label in category_map.items():
        class_idx = CLASSES.index(label)
        zip_files = glob.glob(f"data_mafaulda/{folder}/*.zip")
        print(f" -> Processing {label} ({len(zip_files)} recordings)...")
        
        for z_path in zip_files:
            with zipfile.ZipFile(z_path) as z:
                csv_name = z.namelist()[0]
                df = pd.read_csv(z.open(csv_name), header=None, nrows=200000)
                # Channel 1 (underhang accelerometer) or Channel 0
                sig = df[1].values.astype(np.float32)
                
                # Split into non-overlapping windows of length N
                num_windows = min(len(sig) // N, max_windows_per_file)
                for w in range(num_windows):
                    window = sig[w * N : (w + 1) * N]
                    feat = extract_features(window)
                    X.append(feat)
                    y.append(class_idx)

    return np.array(X, dtype=np.float32), np.array(y, dtype=np.int64)


def export_rf_to_c(clf, path):
    nc = len(clf.classes_)
    nt = len(clf.estimators_)
    out = [
        "// Auto-generated TinyML Random Forest from MaFaulDA dataset",
        f"#define RF_N_TREES {nt}",
        f"#define RF_N_CLASSES {nc}",
        f"#define RF_N_FEATURES 12",
        "",
    ]
    for i, est in enumerate(clf.estimators_):
        t = est.tree_
        lines = [f"static void rf_tree_{i}(const float *x, float *proba) {{"]
        for node in range(t.node_count):
            if t.children_left[node] == -1:
                vals = t.value[node][0]
                norm = vals / vals.sum()
                code = " ".join([f"proba[{c}] += {norm[c]:.4f}f;" for c in range(nc)])
                lines.append(f"  node_{node}: {{ {code} return; }}")
            else:
                f_idx = t.feature[node]
                th = t.threshold[node]
                lines.append(
                    f"  node_{node}: if (x[{f_idx}] <= {th:.6f}f) goto node_{t.children_left[node]}; else goto node_{t.children_right[node]};"
                )
        lines.append("}\n")
        out.extend(lines)

    out.append("static int rf_predict(const float *x, float *proba) {")
    out.append(f"  for (int c = 0; c < {nc}; c++) proba[c] = 0;")
    out += [f"  rf_tree_{i}(x, proba);" for i in range(nt)]
    out.append(f"  int best = 0;\n  for (int c = 0; c < {nc}; c++) {{ proba[c] /= {nt}.0f; if (proba[c] > proba[best]) best = c; }}")
    out.append("  return best;\n}")

    if os.path.dirname(path):
        os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write("\n".join(out))


if __name__ == "__main__":
    os.makedirs("results", exist_ok=True)
    X, y = load_mafaulda_dataset(max_windows_per_file=150)
    print(f"Dataset extracted: {X.shape[0]} windows across {len(CLASSES)} classes.")

    Xtr, Xte, ytr, yte = train_test_split(X, y, test_size=0.25, stratify=y, random_state=42)
    clf = RandomForestClassifier(n_estimators=10, max_depth=6, random_state=42).fit(Xtr, ytr)

    pred = clf.predict(Xte)
    print("\n--- Model Performance on MaFaulDA Test Set ---")
    print(classification_report(yte, pred, target_names=CLASSES, digits=3))

    cv = cross_val_score(RandomForestClassifier(n_estimators=10, max_depth=6, random_state=42), X, y, cv=5)
    print(f"5-Fold Cross-Validation Accuracy: {cv.mean():.3f} +/- {cv.std():.3f}")

    # Plot Confusion Matrix
    disp = ConfusionMatrixDisplay(confusion_matrix(yte, pred), display_labels=CLASSES)
    disp.plot(xticks_rotation=30, cmap="Blues")
    plt.title("MaFaulDA Real Vibration Test Set")
    plt.tight_layout()
    plt.savefig("results/mafaulda_confusion_matrix.png", dpi=150)
    plt.close()

    # Save models & export C header
    joblib.dump(clf, "results/model_mafaulda.joblib")
    export_rf_to_c(clf, "model_mafaulda.h")
    print(f"\nTinyML C header exported: model_mafaulda.h ({os.path.getsize('model_mafaulda.h') / 1024:.1f} KB)")
