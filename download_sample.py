"""Download selected subset of MaFaulDA dataset via Kaggle API."""
import os
import json
import time
from kaggle.api.kaggle_api_extended import KaggleApi

# Load credentials from environment if available
os.environ.setdefault('KAGGLE_USERNAME', os.getenv('KAGGLE_USERNAME', 'harshyy07'))
os.environ.setdefault('KAGGLE_KEY', os.getenv('KAGGLE_KEY', ''))

api = KaggleApi()
api.authenticate()

os.makedirs('data_mafaulda', exist_ok=True)
manifest = json.load(open('selected_manifest.json'))

print(f"Starting download of {len(manifest)} targeted CSV files (~125 MB download total)...")

for idx, item in enumerate(manifest, 1):
    file_name = item['name']
    label = item['label']
    target_dir = os.path.join('data_mafaulda', label)
    os.makedirs(target_dir, exist_ok=True)
    
    base_name = os.path.basename(file_name)
    zip_expected = os.path.join(target_dir, base_name + '.zip')
    csv_expected = os.path.join(target_dir, base_name)
    
    if os.path.exists(zip_expected) or os.path.exists(csv_expected):
        print(f"[{idx}/{len(manifest)}] Already downloaded: {file_name}")
        continue
        
    print(f"[{idx}/{len(manifest)}] Downloading {label}: {file_name} ...")
    try:
        api.dataset_download_file('vuxuancu/mafaulda-full', file_name, path=target_dir)
    except Exception as e:
        print(f"Error downloading {file_name}: {e}")
    time.sleep(0.3)

print("Download complete!")
