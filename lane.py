import cv2
import numpy as np
import time

# Defining Region of Interest (ROI)
def region_of_interest(image, vertices):
    
    # mask: A black image of the same size as the input.(blank matrix)
    mask = np.zeros_like(image)
    
    # cv2.fillPoly: Draws a polygon (like a triangle or trapezoid) to cover the area where lanes are expected.
    cv2.fillPoly(mask, vertices, 255) 
    
    #cv2.bitwise_and: Extracts the region of interest by combining the mask and image.
    masked_image = cv2.bitwise_and(image, mask)
    return masked_image

def draw_lines(image, lines):
    if lines is None:
        return
    for line in lines:
        x1, y1, x2, y2 = line[0]
        cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 5)

def detect_lanes(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # Apply Gaussian Blur: Reduces noise for smoother edge detection.
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # Canny : Detects sharp changes in pixel values to find lane lines.
    edges = cv2.Canny(blur, 50, 150)

	# Detect the half of the frames ignoring sky and buildings 
    height, width = frame.shape[:2]
    vertices = np.array([[(0, height), (width // 2 - 50, height // 2 + 50), (width // 2 + 50, height // 2 + 50), (width, height)]], dtype=np.int32)
    roi = region_of_interest(edges, vertices)

    lines = cv2.HoughLinesP(roi, 1, np.pi / 180, 50, minLineLength=100, maxLineGap=50)
    draw_lines(frame, lines)

    return frame, lines

def calculate_lane_width(lines, frame_width):
    if lines is None or len(lines) < 2:
        return None
    left_lines = [line for line in lines if line[0][0] < frame_width // 2]
    right_lines = [line for line in lines if line[0][0] > frame_width // 2]

    if not left_lines or not right_lines:
        return None

    left_x = np.mean([line[0][0] for line in left_lines])
    right_x = np.mean([line[0][0] for line in right_lines])

    lane_width_px = abs(right_x - left_x)
    lane_width_meters = (lane_width_px / frame_width) * 3.7  # Assuming 3.7m lane width
    return lane_width_meters

def main():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open video stream.")
        return

    prev_time = time.time()

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        frame, lines = detect_lanes(frame)
        lane_width = calculate_lane_width(lines, frame.shape[1])

		# Calculate FPS
        curr_time = time.time()
        fps = 1 / (curr_time - prev_time)
        prev_time = curr_time
		
  		# Putting the text into frame 
        cv2.putText(frame, f"FPS: {fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        if lane_width:
            cv2.putText(frame, f"Lane Width: {lane_width:.2f} m", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

        cv2.imshow('Lane Detection', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()