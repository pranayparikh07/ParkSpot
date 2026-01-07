from flask import Flask, request
from datetime import datetime
import csv
import os

app = Flask(__name__)
filename = "parking_dataset.csv"

if not os.path.exists(filename):
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["timestamp", "ir_number", "status", "occupied"])

@app.route("/update", methods=["POST"])
def update():
    event = request.form.get("event")
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    ir_number = None
    status = "free"
    occupied = 0

    if event:
        parts = event.split(",")
        if len(parts) == 2:
            ir_number = int(parts[0].strip())
            if parts[1].strip().upper() == "OCCUPIED":
                status = "occupied"
                occupied = 1

    with open(filename, "a", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([timestamp, ir_number, status, occupied])

    return "OK"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
