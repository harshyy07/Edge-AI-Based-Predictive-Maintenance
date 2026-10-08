"""Modern Industrial IoT Streamlit Dashboard for Edge-AI Predictive Maintenance.
Run: python -m streamlit run dashboard.py
"""
import json, time
import numpy as np, pandas as pd, joblib, streamlit as st
import plotly.graph_objects as go
from data_gen import gen, CLASSES, FS, N
from features import features, spectrum
from edge_vs_cloud import NetSim, EDGE_MS_ESP32

st.set_page_config(
    page_title="EdgeAI Motor Sense | Predictive Maintenance",
    page_icon="⚡",
    layout="wide",
    initial_sidebar_state="expanded"
)

# Custom Industrial Dark Theme CSS with Glassmorphism & Micro-animations
st.markdown("""
<style>
@import url('https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:wght@400;500;600;700;800&family=JetBrains+Mono:wght@400;600&display=swap');

html, body, [class*="css"] {
    font-family: 'Plus Jakarta Sans', sans-serif;
}

code, pre, .stCode {
    font-family: 'JetBrains Mono', monospace !important;
}

/* Background gradient styling */
.stApp {
    background: radial-gradient(circle at 15% 15%, rgba(17, 24, 39, 0.95), rgba(10, 15, 29, 1));
}

/* Glassmorphism Header */
.header-container {
    background: linear-gradient(135deg, rgba(30, 41, 59, 0.7), rgba(15, 23, 42, 0.8));
    border: 1px solid rgba(255, 255, 255, 0.08);
    backdrop-filter: blur(16px);
    border-radius: 16px;
    padding: 1.25rem 2rem;
    margin-bottom: 1.5rem;
    box-shadow: 0 10px 30px -10px rgba(0, 0, 0, 0.5);
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.header-title {
    font-size: 1.6rem;
    font-weight: 800;
    letter-spacing: -0.5px;
    background: linear-gradient(90deg, #38bdf8, #818cf8, #c084fc);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    margin: 0;
}

.header-badge {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    font-size: 0.82rem;
    font-weight: 600;
    color: #94a3b8;
    background: rgba(15, 23, 42, 0.6);
    padding: 6px 14px;
    border-radius: 9999px;
    border: 1px solid rgba(255, 255, 255, 0.08);
}

.pulsing-dot {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background-color: #10b981;
    box-shadow: 0 0 12px #10b981;
    animation: pulse 1.8s infinite;
}

@keyframes pulse {
    0% { transform: scale(0.95); opacity: 0.7; }
    50% { transform: scale(1.3); opacity: 1; }
    100% { transform: scale(0.95); opacity: 0.7; }
}

/* Custom Card Elements */
.kpi-card {
    background: linear-gradient(145deg, rgba(30, 41, 59, 0.6), rgba(15, 23, 42, 0.75));
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 14px;
    padding: 1.2rem;
    transition: transform 0.2s ease, border-color 0.2s ease;
    box-shadow: 0 8px 24px -8px rgba(0,0,0,0.4);
    min-height: 120px;
    position: relative;
    overflow: hidden;
}

.kpi-card:hover {
    transform: translateY(-2px);
    border-color: rgba(99, 102, 241, 0.4);
}

.kpi-label {
    font-size: 0.78rem;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.8px;
    color: #94a3b8;
    margin-bottom: 0.4rem;
}

.kpi-val {
    font-size: 1.65rem;
    font-weight: 700;
    color: #f8fafc;
    line-height: 1.2;
}

.kpi-sub {
    font-size: 0.78rem;
    color: #64748b;
    margin-top: 0.4rem;
}

/* Status Pill */
.status-pill-ok {
    display: inline-block;
    padding: 4px 12px;
    border-radius: 8px;
    font-size: 1rem;
    font-weight: 700;
    background: rgba(16, 185, 129, 0.15);
    color: #34d399;
    border: 1px solid rgba(52, 211, 153, 0.35);
}

.status-pill-alert {
    display: inline-block;
    padding: 4px 12px;
    border-radius: 8px;
    font-size: 1rem;
    font-weight: 700;
    background: rgba(239, 68, 68, 0.18);
    color: #f87171;
    border: 1px solid rgba(248, 113, 113, 0.35);
}

.log-box {
    background: rgba(10, 15, 29, 0.7);
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 12px;
    padding: 12px;
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.82rem;
    color: #cbd5e1;
    max-height: 220px;
    overflow-y: auto;
}

.log-entry {
    padding: 4px 8px;
    border-bottom: 1px solid rgba(255, 255, 255, 0.04);
}
.log-entry:last-child { border-bottom: none; }
.log-ok { color: #34d399; }
.log-alert { color: #f87171; font-weight: 600; }
</style>
""", unsafe_allow_html=True)


@st.cache_resource
def load_model():
    return joblib.load("results/model.joblib")


@st.cache_resource
def mqtt_store(broker, topic):
    import paho.mqtt.client as mqtt
    store = {"msg": None, "ts": 0.0, "count": 0}
    try:
        c = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    except AttributeError:
        c = mqtt.Client()

    def on_connect(cl, *a):
        cl.subscribe(topic)

    def on_message(cl, ud, m):
        try:
            store["msg"] = json.loads(m.payload)
            store["ts"] = time.time()
            store["count"] += 1
        except Exception:
            pass

    c.on_connect, c.on_message = on_connect, on_message
    try:
        c.connect(broker, 1883, 60)
        c.loop_start()
    except Exception as e:
        print(f"MQTT Connect failed: {e}")
    return store


# Session state initialization
S = st.session_state
S.setdefault("log", [])
S.setdefault("hist", [])
S.setdefault("edge_B", 0)
S.setdefault("cloud_B", 0)
S.setdefault("sim", NetSim(0))
S.setdefault("rng", np.random.default_rng(7))
S.setdefault("last_state", None)

# Sidebar Design
with st.sidebar:
    st.markdown("### ⚙️ Telemetry Configuration")
    source = st.radio("📡 Data Source", ["Simulator", "MQTT (Wokwi / Hardware)"], index=0)

    if source == "Simulator":
        st.markdown("---")
        st.markdown("#### 🧪 Fault Injection")
        fault = st.selectbox(
            "Target Condition",
            CLASSES,
            index=0,
            format_func=lambda s: {
                "normal": "🟢 Normal Baseline",
                "imbalance": "🟠 Mass Imbalance",
                "misalignment": "🟡 Shaft Misalignment",
                "bearing": "🔴 Bearing Outer Race",
                "looseness": "🟣 Mechanical Looseness"
            }.get(s, s.capitalize())
        )
        st.info("💡 Adjust the condition to observe real-time model re-classification & spectral harmonic changes.")
    else:
        st.markdown("---")
        st.markdown("#### 🌐 MQTT Broker Settings")
        broker = st.text_input("Broker Host", "broker.hivemq.com")
        topic = st.text_input("Vibration Topic", "yourname_motor_edge/status")
        st.caption("Firmware publishes JSON payloads like `{\"s\":\"normal\", \"c\":0.98}`.")

    st.markdown("---")
    if st.button("🧹 Reset Counters & Logs", use_container_width=True):
        S.log, S.hist, S.edge_B, S.cloud_B = [], [], 0, 0
        st.rerun()

    st.markdown("---")
    st.markdown(
        "<div style='font-size:0.75rem; color:#64748b; line-height:1.4;'>"
        "<b>Edge AI Motor Guardian v2.1</b><br>"
        "TinyML Edge Random Forest (10 Trees) deployed to ESP32.<br>"
        "Vibration sampling @ 2.0 kHz.</div>",
        unsafe_allow_html=True
    )


# Header Bar
st.markdown("""
<div class="header-container">
    <div>
        <h2 class="header-title">⚡ Edge-AI Predictive Maintenance</h2>
        <div style="font-size: 0.88rem; color: #94a3b8; margin-top: 3px;">
            Real-time Condition-Based Monitoring (CBM) • TinyML On-Device Vibration Diagnostics
        </div>
    </div>
    <div class="header-badge">
        <span class="pulsing-dot"></span>
        <span>EDGE TELEMETRY LIVE</span>
    </div>
</div>
""", unsafe_allow_html=True)


@st.fragment(run_every=1.5)
def live():
    clf = load_model()
    x = None
    all_probas = None

    if source == "Simulator":
        c = CLASSES.index(fault)
        x = gen(c, S.rng)
        p = clf.predict_proba([features(x)])[0]
        all_probas = p
        label, conf = CLASSES[int(p.argmax())], float(p.max())
        cl_ms, cl_b = S.sim.cloud()
        ed_ms, ed_b = S.sim.edge(label, conf)
        S.edge_B += ed_b
        S.cloud_B += cl_b
        S.hist.append((cl_ms, ed_ms))
    else:
        store = mqtt_store(broker, topic)
        if store["msg"] is None:
            st.warning("⏳ Waiting for inbound MQTT packets from ESP32 on topic `" + topic + "`...")
            return
        label, conf = store["msg"].get("s", "unknown"), float(store["msg"].get("c", 0.0))
        if store["count"] != S.get("mq_count"):
            S.mq_count = store["count"]
            S.edge_B += len(json.dumps(store["msg"])) + 60
            S.cloud_B += N * 2 + 60

    healthy = (label == "normal")

    # 1. Top KPI Row
    col1, col2, col3, col4 = st.columns(4)

    with col1:
        status_html = (
            f'<div class="status-pill-ok">● HEALTHY</div>'
            if healthy else
            f'<div class="status-pill-alert">▲ FAULT DETECTED</div>'
        )
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Machine Health Status</div>
            <div style="margin-top: 6px;">{status_html}</div>
            <div class="kpi-sub">Continuous Real-time Evaluation</div>
        </div>
        """, unsafe_allow_html=True)

    with col2:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Predicted Diagnostics</div>
            <div class="kpi-val" style="color: {'#34d399' if healthy else '#f87171'};">{label.capitalize()}</div>
            <div class="kpi-sub">Classification output</div>
        </div>
        """, unsafe_allow_html=True)

    with col3:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Inference Confidence</div>
            <div class="kpi-val">{conf * 100:.1f}%</div>
            <div class="kpi-sub">Random Forest ensemble agreement</div>
        </div>
        """, unsafe_allow_html=True)

    with col4:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Sensor Sampling Window</div>
            <div class="kpi-val">{N} <span style="font-size:1.1rem; color:#94a3b8;">samples</span></div>
            <div class="kpi-sub">{FS} Hz • {(N / FS) * 1000:.0f} ms frame length</div>
        </div>
        """, unsafe_allow_html=True)

    # Event Logging
    if label != S.last_state:
        ts = time.strftime('%H:%M:%S')
        S.log.insert(0, (ts, healthy, label, conf))
        S.last_state = label

    st.markdown("<div style='height: 12px;'></div>", unsafe_allow_html=True)

    # 2. Charts Section
    if x is not None:
        plot_col1, plot_col2 = st.columns([1, 1])

        with plot_col1:
            time_axis_ms = np.arange(len(x[:512])) / FS * 1000
            fig_time = go.Figure()
            fig_time.add_trace(go.Scatter(
                x=time_axis_ms,
                y=x[:512],
                mode='lines',
                line=dict(color='#38bdf8', width=1.7),
                name='Acceleration'
            ))
            fig_time.update_layout(
                title=dict(text="<b>Time-Domain Vibration Waveform</b>", font=dict(color="#f8fafc", size=14)),
                xaxis=dict(title="Time (ms)", gridcolor="rgba(255,255,255,0.06)", zerolinecolor="rgba(255,255,255,0.1)"),
                yaxis=dict(title="Amplitude (g)", gridcolor="rgba(255,255,255,0.06)", zerolinecolor="rgba(255,255,255,0.1)"),
                paper_bgcolor='rgba(15, 23, 42, 0.45)',
                plot_bgcolor='rgba(15, 23, 42, 0.6)',
                font=dict(color="#94a3b8", family="Plus Jakarta Sans"),
                margin=dict(l=40, r=20, t=40, b=40),
                height=280
            )
            st.plotly_chart(fig_time, use_container_width=True, key="fig_time_wave")

        with plot_col2:
            fr, F = spectrum(x)
            mask = fr <= 1000
            fig_fft = go.Figure()
            fig_fft.add_trace(go.Scatter(
                x=fr[mask],
                y=F[mask],
                mode='lines',
                fill='tozeroy',
                fillcolor='rgba(168, 85, 247, 0.15)',
                line=dict(color='#a855f7', width=1.7),
                name='Harmonics'
            ))
            fig_fft.update_layout(
                title=dict(text="<b>Fast Fourier Transform (FFT) Spectrum</b>", font=dict(color="#f8fafc", size=14)),
                xaxis=dict(title="Frequency (Hz)", gridcolor="rgba(255,255,255,0.06)", zerolinecolor="rgba(255,255,255,0.1)"),
                yaxis=dict(title="Magnitude", gridcolor="rgba(255,255,255,0.06)", zerolinecolor="rgba(255,255,255,0.1)"),
                paper_bgcolor='rgba(15, 23, 42, 0.45)',
                plot_bgcolor='rgba(15, 23, 42, 0.6)',
                font=dict(color="#94a3b8", family="Plus Jakarta Sans"),
                margin=dict(l=40, r=20, t=40, b=40),
                height=280
            )
            st.plotly_chart(fig_fft, use_container_width=True, key="fig_fft_spec")

    # 3. Model Class Distribution (Confidence breakdown)
    if all_probas is not None:
        with st.expander("📊 Diagnostic Class Probability Distribution", expanded=False):
            prob_df = pd.DataFrame({
                "Fault Class": [c.capitalize() for c in CLASSES],
                "Probability": all_probas
            })
            fig_bar = go.Figure(go.Bar(
                x=prob_df["Fault Class"],
                y=prob_df["Probability"],
                marker=dict(
                    color=prob_df["Probability"],
                    colorscale=[[0, '#38bdf8'], [1, '#ef4444' if not healthy else '#10b981']],
                    line=dict(width=0)
                ),
                text=[f"{p*100:.1f}%" for p in all_probas],
                textposition='auto'
            ))
            fig_bar.update_layout(
                yaxis=dict(range=[0, 1.05], tickformat=".0%"),
                paper_bgcolor='rgba(0,0,0,0)',
                plot_bgcolor='rgba(0,0,0,0)',
                font=dict(color="#94a3b8"),
                height=180,
                margin=dict(l=20, r=20, t=20, b=20)
            )
            st.plotly_chart(fig_bar, use_container_width=True, key="fig_class_probas")

    # 4. Edge vs Cloud Architecture Comparison
    st.markdown("### 🌐 Architecture Comparison: Edge vs Cloud")
    c_m1, c_m2, c_m3, c_m4 = st.columns(4)

    cloud_latency = 0
    if S.hist:
        h = np.array(S.hist)
        cloud_latency = h[-1, 0]

    with c_m1:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Cloud Decision Latency</div>
            <div class="kpi-val" style="color: #f59e0b;">{cloud_latency:.0f} <span style="font-size:1rem;">ms</span></div>
            <div class="kpi-sub">Network RTT + Cloud Processing</div>
        </div>
        """, unsafe_allow_html=True)

    with c_m2:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Edge Decision Latency</div>
            <div class="kpi-val" style="color: #34d399;">{EDGE_MS_ESP32:.0f} <span style="font-size:1rem;">ms</span></div>
            <div class="kpi-sub">Local on-device inference</div>
        </div>
        """, unsafe_allow_html=True)

    with c_m3:
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Edge Data Transmitted</div>
            <div class="kpi-val">{S.edge_B / 1024:.1f} <span style="font-size:1rem;">KB</span></div>
            <div class="kpi-sub">JSON classification only</div>
        </div>
        """, unsafe_allow_html=True)

    with c_m4:
        saved = ((1 - S.edge_B / S.cloud_B) * 100) if S.cloud_B else 0
        st.markdown(f"""
        <div class="kpi-card">
            <div class="kpi-label">Cloud Bandwidth Saved</div>
            <div class="kpi-val" style="color: #38bdf8;">{saved:.1f}%</div>
            <div class="kpi-sub">Compared to raw streaming ({S.cloud_B / 1024:.1f} KB)</div>
        </div>
        """, unsafe_allow_html=True)

    # 5. Alert History Log
    st.markdown("<div style='height: 12px;'></div>", unsafe_allow_html=True)
    st.markdown("### 📋 Predictive Maintenance Event Feed")
    if not S.log:
        st.markdown("<div class='log-box' style='color:#64748b;'>No abnormal events detected. Machine running smoothly.</div>", unsafe_allow_html=True)
    else:
        entries = []
        for item in S.log[:10]:
            if isinstance(item, tuple) and len(item) == 4:
                ts, is_ok, state, c_val = item
                cls_name = "log-ok" if is_ok else "log-alert"
                badge = "✔ OK" if is_ok else "⚠ FAULT"
                entries.append(
                    f"<div class='log-entry'><span style='color:#64748b; margin-right:12px;'>[{ts}]</span> "
                    f"<span class='{cls_name}'>{badge}: {state.upper()}</span> "
                    f"<span style='color:#94a3b8; margin-left:8px;'>(Confidence: {c_val*100:.1f}%)</span></div>"
                )
            else:
                entries.append(f"<div class='log-entry'><span style='color:#94a3b8;'>{str(item)}</span></div>")
        st.markdown(f"<div class='log-box'>{''.join(entries)}</div>", unsafe_allow_html=True)


live()
