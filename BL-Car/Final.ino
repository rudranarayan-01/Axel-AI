#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
byte command = '0';
char data = '0';

// Motor pins
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int motor2Pin1 = 32;
int motor2Pin2 = 33;

// Lights and indicators
int clight = 2;
int rlight = 4;
int flight = 15;
int forwardled = 19;
int leftled = 21;
int rightled = 22;

// Ultrasonic
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
#define SOUND_SPEED 0.034
long duration;
float distance;
const int obstacleThreshold = 35;

bool obstacleDetected = false;
bool forwardRequested = false;

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  pinMode(clight, OUTPUT);
  pinMode(rlight, OUTPUT);
  pinMode(flight, OUTPUT);
  pinMode(forwardled, OUTPUT);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Bluetooth Started!");
}

void loop() {
  // Fixed lights always on
  digitalWrite(clight, HIGH);
  digitalWrite(flight, HIGH);

  // Read commands
  if (SerialBT.available()) command = SerialBT.read();
  if (Serial.available()) data = Serial.read();

  // Update ultrasonic reading
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * SOUND_SPEED / 2.0;
  obstacleDetected = (distance > 0 && distance <= obstacleThreshold);

  // Check if forward was requested
  bool inputIsForward = (command == '1' || data == '1');
  bool inputIsBackward = (command == '2' || data == '2');
  bool inputIsLeft = (command == '3' || data == '3');
  bool inputIsRight = (command == '4' || data == '4');
  bool inputIsStop = (command == '0' || data == '0');

  // Handle movement
  if (inputIsForward) {
    forwardRequested = true;
    if (!obstacleDetected) {
      moveForward();
    } else {
      stopMotors();
      Serial.println("🚧 Obstacle ahead. Forward blocked.");
    }
  } else if (inputIsBackward) {
    forwardRequested = false;
    moveBackward();
  } else if (inputIsLeft) {
    forwardRequested = false;
    moveLeft();
  } else if (inputIsRight) {
    forwardRequested = false;
    moveRight();
  } else if (inputIsStop) {
    forwardRequested = false;
    stopMotors();
  }

  // Auto-resume forward if obstacle is cleared
  if (forwardRequested && !obstacleDetected && !inputIsForward) {
    moveForward();
  }

  delay(20);
}

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(forwardled, HIGH);
  digitalWrite(leftled, LOW);
  digitalWrite(rightled, LOW);
  digitalWrite(rlight, LOW);
  Serial.println("🚗 Moving Forward");
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(forwardled, LOW);
  digitalWrite(leftled, LOW);
  digitalWrite(rightled, LOW);
  digitalWrite(rlight, HIGH);
  Serial.println("🔙 Moving Backward");
}

void moveLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(leftled, HIGH);
  digitalWrite(rightled, LOW);
  digitalWrite(forwardled, LOW);
  digitalWrite(rlight, LOW);
  Serial.println("⬅️ Turning Left");
}

void moveRight() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(rightled, HIGH);
  digitalWrite(leftled, LOW);
  digitalWrite(forwardled, LOW);
  digitalWrite(rlight, LOW);
  Serial.println("➡️ Turning Right");
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(forwardled, LOW);
  digitalWrite(leftled, LOW);
  digitalWrite(rightled, LOW);
  digitalWrite(rlight, LOW);
  Serial.println("🛑 Motors Stopped");
}
