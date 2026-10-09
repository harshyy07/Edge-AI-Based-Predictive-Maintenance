"""Download selected CWRU bearing dataset recordings (.mat)."""
import os
import urllib.request

CWRU_FILES = {
    "normal": [
        ("97.mat", "https://engineering.case.edu/sites/default/files/97.mat"),
        ("98.mat", "https://engineering.case.edu/sites/default/files/98.mat"),
        ("99.mat", "https://engineering.case.edu/sites/default/files/99.mat"),
        ("100.mat", "https://engineering.case.edu/sites/default/files/100.mat"),
    ],
    "bearing_ball": [
        ("118.mat", "https://engineering.case.edu/sites/default/files/118.mat"),
        ("119.mat", "https://engineering.case.edu/sites/default/files/119.mat"),
        ("120.mat", "https://engineering.case.edu/sites/default/files/120.mat"),
        ("121.mat", "https://engineering.case.edu/sites/default/files/121.mat"),
    ],
    "bearing_outer": [
        ("130.mat", "https://engineering.case.edu/sites/default/files/130.mat"),
        ("131.mat", "https://engineering.case.edu/sites/default/files/131.mat"),
        ("132.mat", "https://engineering.case.edu/sites/default/files/132.mat"),
        ("133.mat", "https://engineering.case.edu/sites/default/files/133.mat"),
    ],
}

os.makedirs("data_cwru", exist_ok=True)

print("Starting download of CWRU benchmark files...")
for label, files in CWRU_FILES.items():
    label_dir = os.path.join("data_cwru", label)
    os.makedirs(label_dir, exist_ok=True)
    for fname, url in files:
        dest = os.path.join(label_dir, fname)
        if os.path.exists(dest) and os.path.getsize(dest) > 1000:
            print(f" -> [{label}] {fname} already downloaded.")
            continue
        print(f" -> Downloading [{label}] {fname} from {url} ...")
        req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
        with urllib.request.urlopen(req) as resp, open(dest, "wb") as out:
            out.write(resp.read())
        print(f"    Saved {fname} ({os.path.getsize(dest) / 1024:.1f} KB)")

print("\nAll CWRU files downloaded successfully into data_cwru/!")
