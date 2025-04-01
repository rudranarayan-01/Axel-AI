import cv2
import numpy as np

# Load the image
image = cv2.imread('lane_image.jpeg')

# Step 1: Convert to Grayscale
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
cv2.imshow('Grayscale Image', gray)

# Step 2: Apply Gaussian Blur
blurred = cv2.GaussianBlur(gray, (5, 5), 0)
cv2.imshow('Blurred Image', blurred)

# Step 3: Perform Canny Edge Detection
edges = cv2.Canny(blurred, 50, 150)
cv2.imshow('Canny Edge Detection', edges)

# Step 4: Draw Lane Lines using Hough Transform
lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 50, minLineLength=100, maxLineGap=50)
line_image = np.zeros_like(image)

if lines is not None:
    for line in lines:
        x1, y1, x2, y2 = line[0]
        cv2.line(line_image, (x1, y1), (x2, y2), (0, 255, 0), 5)

# Combine with original image
lane_image = cv2.addWeighted(image, 0.8, line_image, 1, 0)
cv2.imshow('Lane Detection', lane_image)

# Print all images
cv2.waitKey(0)
cv2.destroyAllWindows()