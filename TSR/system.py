import cv2
import torch

import warnings
warnings.filterwarnings("ignore", category=FutureWarning)

# Load trained model
model = torch.hub.load('ultralytics/yolov5', 'custom', path='best.pt', force_reload=True)
model.conf = 0.50  # confidence threshold

# Start webcam
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Inference
    results = model(frame)

    # Parse results
    for *box, conf, cls in results.xyxy[0]:
        label = model.names[int(cls)]
        print(f"Detected: {label} ({conf:.2f})")

        x1, y1, x2, y2 = map(int, box)
        cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)
        cv2.putText(frame, f"{label} {conf:.2f}", (x1, y1 - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)

    # Show frame
    cv2.imshow("YOLOv5 - Traffic Sign Detection", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
