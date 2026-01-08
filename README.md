# 🚗 Autonomous Car Prototype using Deep learning and Computer Vision

This project demonstrates a mini self-driving car system built using Raspberry Pi 4, ESP32, and multiple AI-based modules for real-time perception, control, and monitoring. The system integrates Computer Vision, IoT, and ML to simulate real-world autonomous driving features such as lane detection, traffic sign recognition, obstacle avoidance, and more.

## 🧠 Features

🔹 1. Traffic Sign Recognition (TSR)

Detects and classifies traffic signs (Stop, Left, Right, etc.) in real-time.

Model trained on GTSRB dataset using YOLOv5s.

Detection results are used to trigger actions via ESP32-controlled actuators.

## 2. Lane Detection

Uses OpenCV to detect road lanes in real time.

Provides direction guidance for path alignment.

Optimized for Raspberry Pi 4 using efficient edge detection and masking.

## 3. Drowsiness and Alcohol Detection

Detects driver drowsiness using facial landmarks and eye aspect ratio.

Alcohol detection integrated using a MQ3 sensor connected to ESP32.

Triggers safety alerts and stops the vehicle when needed.

## 4. Cruise Control and Object Detection

Maintains constant vehicle speed using motor feedback.

Uses ultrasonic sensors for obstacle detection and automatic braking.

🔹 5. IoT Cloud Monitoring

Real-time data (speed, sensor readings, detection results) sent to ThingSpeak Cloud.
