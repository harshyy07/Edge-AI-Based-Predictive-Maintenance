"""Train TinyML Random Forest on combined MaFaulDA + CWRU vibration datasets."""
import os, glob, zipfile
import numpy as np, pandas as pd
import scipy.io
from scipy.signal import resample
from scipy.stats import kurtosis, skew
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score, precision_recall_fscore_support
import joblib

try:
    import matplotlib.pyplot as plt
    from sklearn.metrics import ConfusionMatrixDisplay
    HAS_MATPLOTLIB = True
except ImportError:
    HAS_MATPLOTLIB = False


CLASSES = ["normal", "imbalance", "misalignment", "bearing_outer", "bearing_ball"]
FS = 50000  # Target Edge-AI sampling frequency (50 kHz)
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


def load_mafaulda(max_windows_per_file=150):
    category_map = {
        "normal": "normal",
        "imbalance": "imbalance",
        "misalignment": "misalignment",
        "bearing": "bearing_outer",
        "bearing_ball": "bearing_ball",
    }
    X, y = [], []
    print("Loading MaFaulDA dataset (50 kHz)...")
    for folder, label in category_map.items():
        class_idx = CLASSES.index(label)
        zip_files = glob.glob(f"data_mafaulda/{folder}/*.zip")
        for z_path in zip_files:
            with zipfile.ZipFile(z_path) as z:
                csv_name = z.namelist()[0]
                df = pd.read_csv(z.open(csv_name), header=None, nrows=200000)
                sig = df[1].values.astype(np.float32)
                num_windows = min(len(sig) // N, max_windows_per_file)
                for w in range(num_windows):
                    feat = extract_features(sig[w * N : (w + 1) * N])
                    X.append(feat)
                    y.append(class_idx)
    return X, y


def load_cwru(max_windows_per_file=150):
    """Load CWRU dataset (12 kHz Drive-End accelerometer) and resample to 50 kHz."""
    mapping = {
        "normal": "normal",
        "bearing_ball": "bearing_ball",
        "bearing_outer": "bearing_outer",
    }
    X, y = [], []
    print("Loading CWRU dataset (resampling 12 kHz -> 50 kHz)...")
    for folder, label in mapping.items():
        class_idx = CLASSES.index(label)
        mat_files = glob.glob(f"data_cwru/{folder}/*.mat")
        for m_path in mat_files:
            mat = scipy.io.loadmat(m_path)
            # Find Drive End (DE) acceleration time series
            de_key = None
            for k in mat.keys():
                if "DE_time" in k:
                    de_key = k
                    break
            if not de_key:
                continue

            raw_sig = mat[de_key].flatten().astype(np.float32)
            
            # Resample from 12 kHz to 50 kHz to match edge sensor feature space
            # Ratio: 50 / 12
            resampled_len = int(len(raw_sig) * (FS / 12000.0))
            # Slice first 200,000 points to keep memory and speed optimal
            chunk_len = min(len(raw_sig), 48000)
            resampled_sig = resample(raw_sig[:chunk_len], int(chunk_len * (FS / 12000.0))).astype(np.float32)

            num_windows = min(len(resampled_sig) // N, max_windows_per_file)
            for w in range(num_windows):
                feat = extract_features(resampled_sig[w * N : (w + 1) * N])
                X.append(feat)
                y.append(class_idx)
    return X, y


def export_rf_to_c(clf, path):
    nc = len(clf.classes_)
    nt = len(clf.estimators_)
    out = [
        "// Auto-generated TinyML Random Forest from Combined MaFaulDA + CWRU dataset",
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

    X_maf, y_maf = load_mafaulda(max_windows_per_file=150)
    print(f" -> MaFaulDA samples: {len(X_maf)}")

    X_cwru, y_cwru = load_cwru(max_windows_per_file=150)
    print(f" -> CWRU samples: {len(X_cwru)}")

    X = np.array(X_maf + X_cwru, dtype=np.float32)
    y = np.array(y_maf + y_cwru, dtype=np.int64)

    print(f"\nTotal Combined Dataset: {X.shape[0]} feature windows across {len(CLASSES)} classes.")
    for idx, c in enumerate(CLASSES):
        print(f"   Class {c} ({idx}): {np.sum(y == idx)} samples")

    Xtr, Xte, ytr, yte = train_test_split(X, y, test_size=0.25, stratify=y, random_state=42)
    print(f"\nTrain set: {Xtr.shape[0]} samples | Test set: {Xte.shape[0]} samples")

    clf = RandomForestClassifier(n_estimators=10, max_depth=8, random_state=42)
    clf.fit(Xtr, ytr)

    pred = clf.predict(Xte)

    # Detailed metrics
    acc = accuracy_score(yte, pred)
    prec_macro, rec_macro, f1_macro, _ = precision_recall_fscore_support(yte, pred, average="macro")
    prec_wt, rec_wt, f1_wt, _ = precision_recall_fscore_support(yte, pred, average="weighted")

    print("\n================ MODEL EVALUATION METRICS ================")
    print(f"Overall Accuracy:  {acc * 100:.2f}%")
    print(f"Macro Precision:   {prec_macro * 100:.2f}%")
    print(f"Macro Recall:      {rec_macro * 100:.2f}%")
    print(f"Macro F1-Score:    {f1_macro * 100:.2f}%")
    print(f"Weighted F1-Score: {f1_wt * 100:.2f}%")
    print("----------------------------------------------------------")
    print("\nPer-Class Classification Report:")
    print(classification_report(yte, pred, target_names=CLASSES, digits=4))

    cv = cross_val_score(RandomForestClassifier(n_estimators=10, max_depth=8, random_state=42), X, y, cv=5)
    print(f"5-Fold Cross-Validation Accuracy: {cv.mean() * 100:.2f}% (+/- {cv.std() * 100:.2f}%)")

    # Plot Confusion Matrix
    disp = ConfusionMatrixDisplay(confusion_matrix(yte, pred), display_labels=CLASSES)
    disp.plot(xticks_rotation=30, cmap="Blues")
    plt.title("Combined MaFaulDA + CWRU Vibration Test Set")
    plt.tight_layout()
    plt.savefig("results/combined_confusion_matrix.png", dpi=150)
    plt.close()

    # Save models & export C header
    joblib.dump(clf, "results/model_combined.joblib")
    export_rf_to_c(clf, "model_combined.h")
    print(f"\nTinyML C header exported: model_combined.h ({os.path.getsize('model_combined.h') / 1024:.1f} KB)")
