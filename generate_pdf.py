import os
from reportlab.lib.pagesizes import letter
from reportlab.lib import colors
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, Image, KeepTogether, HRFlowable
)
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch

pdf_path = "Edge_AI_Predictive_Maintenance_Guide.pdf"
doc = SimpleDocTemplate(
    pdf_path,
    pagesize=letter,
    leftMargin=36,
    rightMargin=36,
    topMargin=36,
    bottomMargin=36
)

styles = getSampleStyleSheet()

# Custom styles
primary_color = colors.HexColor("#1A365D")   # Deep navy blue
accent_color = colors.HexColor("#2B6CB0")    # Classic steel blue
highlight_bg = colors.HexColor("#EDF2F7")    # Subtle gray/blue tint
text_dark = colors.HexColor("#2D3748")

title_style = ParagraphStyle(
    'DocTitle',
    parent=styles['Normal'],
    fontName='Helvetica-Bold',
    fontSize=22,
    leading=26,
    textColor=primary_color,
    alignment=1, # Center
    spaceAfter=6
)

subtitle_style = ParagraphStyle(
    'DocSubtitle',
    parent=styles['Normal'],
    fontName='Helvetica',
    fontSize=11,
    leading=14,
    textColor=accent_color,
    alignment=1,
    spaceAfter=15
)

h1_style = ParagraphStyle(
    'Heading1_Custom',
    parent=styles['Normal'],
    fontName='Helvetica-Bold',
    fontSize=13,
    leading=17,
    textColor=primary_color,
    spaceBefore=12,
    spaceAfter=6
)

body_style = ParagraphStyle(
    'Body_Custom',
    parent=styles['Normal'],
    fontName='Helvetica',
    fontSize=9.5,
    leading=13.5,
    textColor=text_dark,
    spaceAfter=6
)

body_bold = ParagraphStyle(
    'Body_Bold',
    parent=body_style,
    fontName='Helvetica-Bold'
)

bullet_style = ParagraphStyle(
    'Bullet_Custom',
    parent=body_style,
    leftIndent=14,
    firstLineIndent=-10,
    spaceAfter=3
)

code_snippet_style = ParagraphStyle(
    'Code_Custom',
    parent=styles['Normal'],
    fontName='Courier',
    fontSize=8.5,
    leading=11,
    textColor=colors.HexColor("#2C5282")
)

story = []

# Title & Subtitle
story.append(Paragraph("Edge-AI Predictive Maintenance & Motor Fault Detection", title_style))
story.append(Paragraph("System Architecture, Hardware Integration, & Benchmark Report", subtitle_style))
story.append(HRFlowable(width="100%", thickness=1.5, color=accent_color, spaceAfter=12))

# Executive Overview
story.append(Paragraph("1. Executive Overview & Key Results", h1_style))
overview_text = (
    "This end-to-end TinyML and IoT system performs on-device Condition-Based Monitoring (CBM) for "
    "industrial electric motors. Instead of streaming raw vibration waveforms over the network to a cloud server, "
    "an <b>ESP32 microcontroller</b> samples vibration via an MPU6050 accelerometer, extracts 12 statistical time "
    "and frequency-domain features on-chip, runs an embedded Random Forest classifier (pure C code in <code>model.h</code>), "
    "and transmits only lightweight JSON alerts (~30 bytes) over MQTT."
)
story.append(Paragraph(overview_text, body_style))

# Benchmark Table
bench_data = [
    [Paragraph("<b>Metric</b>", body_style), Paragraph("<b>Cloud Setup</b>", body_style), Paragraph("<b>Edge (ESP32)</b>", body_style), Paragraph("<b>Improvement</b>", body_style)],
    [Paragraph("Mean Decision Latency", body_style), Paragraph("125.85 ms", body_style), Paragraph("20.00 ms", body_style), Paragraph("<b>84.1% Faster</b>", body_style)],
    [Paragraph("P95 Tail Latency", body_style), Paragraph("173.15 ms", body_style), Paragraph("20.00 ms", body_style), Paragraph("<b>88.4% Faster</b>", body_style)],
    [Paragraph("Data Transmitted / min", body_style), Paragraph("123.52 KB", body_style), Paragraph("4.97 KB", body_style), Paragraph("<b>96.0% Less Bandwidth</b>", body_style)],
    [Paragraph("Model Test Accuracy", body_style), Paragraph("99.3%", body_style), Paragraph("99.3%", body_style), Paragraph("<b>5-Fold CV: 96.9%</b>", body_style)]
]
bench_table = Table(bench_data, colWidths=[1.8*inch, 1.4*inch, 1.4*inch, 1.6*inch])
bench_table.setStyle(TableStyle([
    ('BACKGROUND', (0, 0), (-1, 0), highlight_bg),
    ('TEXTCOLOR', (0, 0), (-1, 0), primary_color),
    ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
    ('BOTTOMPADDING', (0, 0), (-1, -1), 4),
    ('TOPPADDING', (0, 0), (-1, -1), 4),
    ('GRID', (0, 0), (-1, -1), 0.5, colors.HexColor("#CBD5E0")),
]))
story.append(bench_table)
story.append(Spacer(1, 10))

# Hardware Wiring Section
story.append(Paragraph("2. Physical Hardware Wiring & Pin Mapping", h1_style))
story.append(Paragraph(
    "Connect the MPU6050 6-DOF IMU and status indicator LEDs to the ESP32 development board as shown below:",
    body_style
))

hw_data = [
    [Paragraph("<b>Component Pin</b>", body_style), Paragraph("<b>ESP32 Pin</b>", body_style), Paragraph("<b>Electrical Interface / Purpose</b>", body_style)],
    [Paragraph("MPU6050 VCC", code_snippet_style), Paragraph("3V3 or VIN", code_snippet_style), Paragraph("Regulated 3.3V DC power supply", body_style)],
    [Paragraph("MPU6050 GND", code_snippet_style), Paragraph("GND", code_snippet_style), Paragraph("Common ground reference", body_style)],
    [Paragraph("MPU6050 SDA", code_snippet_style), Paragraph("GPIO 21", code_snippet_style), Paragraph("I2C Serial Data line (400 kHz Fast-Mode)", body_style)],
    [Paragraph("MPU6050 SCL", code_snippet_style), Paragraph("GPIO 22", code_snippet_style), Paragraph("I2C Serial Clock line", body_style)],
    [Paragraph("Green Status LED", code_snippet_style), Paragraph("GPIO 26 (via 220Ω)", code_snippet_style), Paragraph("Active HIGH when machine state is NORMAL", body_style)],
    [Paragraph("Red Warning LED", code_snippet_style), Paragraph("GPIO 27 (via 220Ω)", code_snippet_style), Paragraph("Active HIGH when any FAULT is detected", body_style)]
]
hw_table = Table(hw_data, colWidths=[1.7*inch, 1.6*inch, 2.9*inch])
hw_table.setStyle(TableStyle([
    ('BACKGROUND', (0, 0), (-1, 0), highlight_bg),
    ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
    ('BOTTOMPADDING', (0, 0), (-1, -1), 3.5),
    ('TOPPADDING', (0, 0), (-1, -1), 3.5),
    ('GRID', (0, 0), (-1, -1), 0.5, colors.HexColor("#CBD5E0")),
]))
story.append(hw_table)
story.append(Spacer(1, 10))

# Operational Flow
story.append(Paragraph("3. How the Firmware Executes on Hardware", h1_style))
steps = [
    ("<b>1. Sensor Sampling (2 kHz)</b>: The ESP32 accesses the MPU6050 Z-axis accelerometer registers via I2C at a precise 500 µs interval, filling a 1,024-sample window buffer (~0.512s of physical vibration).", bullet_style),
    ("<b>2. Edge Feature Extraction (edge_features.h)</b>: Calculates 12 time-domain & spectral features locally (RMS, Peak, Crest Factor, Kurtosis, Skewness, Shape Factor, Spectral Centroid, Energy, 1X/2X/3X harmonics, and high-frequency bearing bands).", bullet_style),
    ("<b>3. Zero-Dependency TinyML Inference (model.h)</b>: An optimized pure C Random Forest evaluates the 12 features directly in RAM in ~20 ms, requiring only ~5.1 KB of flash memory.", bullet_style),
    ("<b>4. Five Detectable Motor States</b>: Accurately classifies conditions into <i>Normal, Mass Imbalance, Shaft Misalignment, Bearing Fault,</i> or <i>Mechanical Looseness</i>.", bullet_style),
    ("<b>5. Local Actuation & Cloud Telemetry</b>: Drives local LEDs instantaneously (Green on GPIO 26, Red on GPIO 27) and publishes a ~30-byte JSON payload (e.g. <code>{\"s\":\"bearing\",\"c\":0.97}</code>) over MQTT to HiveMQ broker for live dashboard display.", bullet_style)
]
for step_text, style in steps:
    story.append(Paragraph(f"• {step_text}", style))

story.append(Spacer(1, 10))

# Firmware Configuration steps
story.append(Paragraph("4. Step-by-Step Deployment Guide", h1_style))
config_steps = [
    ("<b>Step 1: Open sketch.ino</b> in Arduino IDE or VS Code PlatformIO.", bullet_style),
    ("<b>Step 2: Enable Physical Sensor</b>: On line 16, switch <code>#define USE_REAL_MPU 0</code> to <code>#define USE_REAL_MPU 1</code>.", bullet_style),
    ("<b>Step 3: Network Credentials</b>: Enter your WiFi SSID, WiFi password, and set your unique MQTT topic (e.g. <code>my_plant_motor/status</code>).", bullet_style),
    ("<b>Step 4: Mount Sensor</b>: Mechanically bolt or attach the MPU6050 to the motor bearing cap. Avoid loose wiring or soft foam mounting.", bullet_style),
    ("<b>Step 5: Flash ESP32 & Launch Dashboard</b>: Upload firmware to the ESP32. In your PC terminal, run <code>streamlit run dashboard.py</code> to view live vibration spectra, health metrics, and automated alerts.", bullet_style)
]
for step_text, style in config_steps:
    story.append(Paragraph(f"• {step_text}", style))

story.append(Spacer(1, 10))

# Benchmark & Results Visuals
story.append(Paragraph("5. Validation Visualizations & Plots", h1_style))
img_path_cm = os.path.abspath("results/confusion_matrix.png")
img_path_bench = os.path.abspath("results/edge_vs_cloud.png")

if os.path.exists(img_path_cm) and os.path.exists(img_path_bench):
    img_table_data = [
        [
            Image(img_path_cm, width=3.0*inch, height=2.25*inch),
            Image(img_path_bench, width=3.0*inch, height=2.25*inch)
        ],
        [
            Paragraph("<b>Figure 1: Confusion Matrix (99.3% Accuracy)</b>", body_style),
            Paragraph("<b>Figure 2: Edge vs Cloud Latency & Bandwidth</b>", body_style)
        ]
    ]
    img_table = Table(img_table_data, colWidths=[3.2*inch, 3.2*inch])
    img_table.setStyle(TableStyle([
        ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
        ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
        ('BOTTOMPADDING', (0, 0), (-1, -1), 2),
        ('TOPPADDING', (0, 0), (-1, -1), 2),
    ]))
    story.append(img_table)

doc.build(story)
print("PDF generation complete.")

