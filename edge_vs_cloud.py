"""Edge vs Cloud simulation: decision latency and bytes transmitted."""
import json, time
import numpy as np, joblib
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd
from data_gen import gen, CLASSES, N
from features import features

HEADER_BYTES = 60          # MQTT/TCP/IP overhead assumed per message
UPLINK_BPS = 1_000_000     # 1 Mbps uplink
EDGE_MS_ESP32 = 20.0       # ASSUMED ESP32 feature+inference time; replace with value printed by firmware
SERVER_MS = 2.0            # server-side inference time


class NetSim:
    def __init__(self, seed=0):
        self.rng = np.random.default_rng(seed)

    def rtt(self):
        return self.rng.uniform(50, 150) + self.rng.exponential(10)

    def cloud(self):
        payload = N * 2 + HEADER_BYTES
        lat = payload * 8 / UPLINK_BPS * 1000 + self.rtt() + SERVER_MS
        return lat, payload

    def edge(self, label, conf):
        payload = len(json.dumps({"s": label, "c": round(conf, 2)}, separators=(",", ":"))) + HEADER_BYTES
        return EDGE_MS_ESP32, payload     # decision made locally; result uploaded asynchronously


if __name__ == "__main__":
    clf = joblib.load("results/model.joblib")
    rng, sim = np.random.default_rng(3), NetSim(1)
    rows = []
    for i in range(300):
        c = int(rng.integers(0, 5))
        p = clf.predict_proba([features(gen(c, rng))])[0]
        lab, conf = CLASSES[int(p.argmax())], float(p.max())
        cl, cb = sim.cloud(); el, eb = sim.edge(lab, conf)
        rows.append(dict(cloud_ms=cl, edge_ms=el, cloud_B=cb, edge_B=eb))
    d = pd.DataFrame(rows)
    per_min = 60  # 1 window per second
    summary = pd.DataFrame({
        "Cloud": [d.cloud_ms.mean(), d.cloud_ms.quantile(.95), d.cloud_B.mean() * per_min / 1024],
        "Edge": [d.edge_ms.mean(), d.edge_ms.quantile(.95), d.edge_B.mean() * per_min / 1024]},
        index=["Mean decision latency (ms)", "P95 latency (ms)", "Data sent (KB/min)"])
    summary["Improvement"] = [f"{(1 - e / c) * 100:.1f}%" for e, c in zip(summary.Edge, summary.Cloud)]
    print(summary.round(2).to_string())
    summary.to_csv("results/edge_vs_cloud.csv")
    fig, ax = plt.subplots(1, 2, figsize=(9, 3.6))
    ax[0].bar(["Cloud", "Edge"], [d.cloud_ms.mean(), d.edge_ms.mean()], color=["#d9534f", "#5cb85c"])
    ax[0].set_ylabel("Decision latency (ms)")
    ax[1].bar(["Cloud", "Edge"], [d.cloud_B.mean() * 60 / 1024, d.edge_B.mean() * 60 / 1024], color=["#d9534f", "#5cb85c"])
    ax[1].set_ylabel("Data sent (KB/min)")
    plt.tight_layout(); plt.savefig("results/edge_vs_cloud.png", dpi=150)
