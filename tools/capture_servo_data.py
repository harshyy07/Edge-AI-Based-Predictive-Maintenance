"""Capture labelled MPU6050 samples from the Phase 1 ESP32 collector."""
from __future__ import annotations

import argparse
import csv
import queue
import sys
import threading
from datetime import datetime
from pathlib import Path

import serial

LABELS = (
    "NORMAL",
    "HIGH_FRICTION",
    "MOUNTING_LOOSENESS",
    "ABNORMAL_VIBRATION",
    "IRREGULAR_MOVEMENT",
    "STOP",
)
HEADER = ("window_id", "sample_index", "timestamp_us", "label", "ax_g", "ay_g", "az_g")


def read_commands(commands: queue.Queue[str]) -> None:
    """Read operator labels in a background thread while serial capture continues."""
    while True:
        try:
            commands.put(input("label> ").strip().upper())
        except EOFError:
            commands.put("Q")
            return


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", default="COM3", help="ESP32 serial port (default: COM3)")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--output", type=Path, help="CSV output path (default: data/servo_phase1_TIMESTAMP.csv)")
    args = parser.parse_args()
    output = args.output or Path("data") / f"servo_phase1_{datetime.now():%Y%m%d_%H%M%S}.csv"
    output.parent.mkdir(parents=True, exist_ok=True)
    commands: queue.Queue[str] = queue.Queue()
    threading.Thread(target=read_commands, args=(commands,), daemon=True).start()

    try:
        with serial.Serial(args.port, args.baud, timeout=0.2, write_timeout=2) as port, output.open(
            "w", newline="", encoding="utf-8"
        ) as csv_file:
            writer = csv.writer(csv_file)
            writer.writerow((*HEADER, "host_received_at"))
            print(f"Connected to {args.port} at {args.baud}; saving {output}")
            print("Enter a label to apply it to subsequent samples, or q to quit.")
            print("Labels: " + ", ".join(LABELS))
            while True:
                try:
                    command = commands.get_nowait()
                except queue.Empty:
                    command = ""
                if command in ("Q", "QUIT", "EXIT"):
                    break
                if command:
                    if command not in LABELS:
                        print("Unknown label. Choose one of the listed labels.")
                    else:
                        port.write(f"LABEL,{command}\n".encode("ascii"))
                        port.flush()
                        print(f"Requested label {command}")
                raw = port.readline()
                if not raw:
                    continue
                line = raw.decode("utf-8", errors="replace").strip()
                if line.startswith("#"):
                    print(line)
                    continue
                fields = next(csv.reader([line]))
                if len(fields) != len(HEADER):
                    print(f"Ignoring non-data serial line: {line}")
                    continue
                try:
                    int(fields[0])
                    int(fields[1])
                    int(fields[2])
                    float(fields[4])
                    float(fields[5])
                    float(fields[6])
                except ValueError:
                    print(f"Ignoring malformed sample: {line}")
                    continue
                writer.writerow((*fields, datetime.now().isoformat(timespec="milliseconds")))
                csv_file.flush()
    except (serial.SerialException, OSError) as exc:
        print(f"Capture error: {exc}", file=sys.stderr)
        return 2
    except KeyboardInterrupt:
        print("\nCapture stopped.")
    print(f"Saved samples to {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
