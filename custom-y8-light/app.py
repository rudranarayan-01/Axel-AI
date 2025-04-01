import cv2
import serial
import numpy as np
from ultralytics import YOLO

# Initialize YOLO model
model = YOLO('best (2).pt')  # Path to your trained model

# Initialize serial communication with ESP
# ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)  # Adjust COM port if necessary

def detect_traffic_sign(frame):
    results = model(frame)
    for result in results:        
        for box in result.boxes:  # Iterate through detected boxes
            label = int(box.cls[0])
            confidence = float(box.conf[0])

            if confidence > 0.5:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                class_name = model.names[label]

                # Draw bounding box and label
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, f'{class_name} {confidence:.2f}', (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)
                print(class_name)

                # # Send data to ESP based on traffic sign
                # if class_name.lower() == 'red-lights':
                #     ser.write(b'R')
                # elif class_name.lower() == 'yellow-lights':
                #     ser.write(b'Y')
                # elif class_name.lower() == 'green-lights':
                #     ser.write(b'G')

    return frame

def main():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        frame = detect_traffic_sign(frame)
        cv2.imshow('Traffic Sign Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
