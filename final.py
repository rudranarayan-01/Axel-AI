import cv2
import torch
import serial
import numpy as np
from time import sleep
from picamera2 import Picamera2
from ultralytics import YOLO


# Initialize the camera
picam2 = Picamera2()
picam2.preview_configuration.main.size = (640, 480)
#picam2.preview_configuration.main.format = "RGB888"
picam2.configure("preview")
picam2.start()

# Load the trained YOLO model
model_path = 'best (2).pt'  # Update with your model path
model = YOLO(model_path)

# Set up serial communication with ESP32
esp_serial = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)  # Update with your serial port
sleep(2)  # Allow time for the serial connection to establish

def detect_and_send():
    while True:
        # Capture frame
        frame = picam2.capture_array()
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

        # Perform object detection
        results = model(frame)

        # detected_objects = []  # Store detected labels

        for result in results:
            for box in result.boxes:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                conf = float(box.conf[0])
                label = result.names[int(box.cls[0])]  # Get the class label
                
                if conf > 0.5:
                    cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                    cv2.putText(frame, f"{label} {conf:.2f}", (x1, y1 - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                    print(label)
                    if label == "red-lights":
                        esp_serial.write(b"1")
                    elif label =="yellow-lights":
                        esp_serial.write(b"2")
                    elif label == "green-lights":
                        esp_serial.write(b"3")
                    else:
                        esp_serial.write(b"0")
                        
                    #esp_serial.write(label.encode()) 
                    print(f"Sent to ESP32: {label}")
                    
                    # detected_objects.append(label)

        # If any traffic light is detected, send a command to ESP32
         # Send over serial
            

        # Show output
        cv2.imshow("Traffic Light Detection", frame)

        # Exit on pressing 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()
    esp_serial.close()  # Close the serial connection

detect_and_send()
