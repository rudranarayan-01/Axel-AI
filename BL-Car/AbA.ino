// Only stops when object detected
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
byte command;
char data;

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 

// Motor B
int motor2Pin1 = 32;
int motor2Pin2 = 33;

int clight = 2; // Constant red light
int rlight = 4; // Brake red light

// Ultrasonic Sensor 
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

// Indicators
int forwardled = 19;
int leftled = 21;
int rightled = 22;

// Sound speed constant
#define SOUND_SPEED 0.034

// For Ultrasonic HC-SR04
long duration;
float distance;
const int obstacleThreshold = 20; // cm

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  pinMode(clight, OUTPUT);
  pinMode(rlight, OUTPUT);

  pinMode(forwardled, OUTPUT);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Bluetooth Started! Ready to pair...");
}

void loop() {
  if (SerialBT.available()) {
    command = SerialBT.read();
    Serial.print("\n\nBluetooth Serial : ");
    Serial.println(command);
  }

  if (Serial.available()) {
    data = Serial.read();
    Serial.print("\n\nRaspi command: ");
    Serial.println(data);
  }

  // Measure distance using ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * SOUND_SPEED / 2;

  digitalWrite(clight, HIGH); // constant light always on

  char effectiveCommand = (command != 0) ? command : data;

  if (effectiveCommand == '1') { // Forward
    if (distance > 0 && distance <= obstacleThreshold) {
      stopMotors();
      Serial.println("🚧 Obstacle detected! Forward blocked.");
      digitalWrite(rlight, HIGH); // Brake light on
    } else {
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);

      digitalWrite(rlight, LOW);      // brake off
      digitalWrite(forwardled, HIGH); // forward light
      digitalWrite(leftled, LOW);
      digitalWrite(rightled, LOW);
    }
  } 
  else if (effectiveCommand == '2') { // Backward
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);

    digitalWrite(rlight, HIGH);
    digitalWrite(forwardled, LOW);
    digitalWrite(leftled, LOW);
    digitalWrite(rightled, LOW);
  } 
  else if (effectiveCommand == '3') { // Left
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);

    digitalWrite(rlight, LOW);
    digitalWrite(forwardled, LOW);
    digitalWrite(leftled, HIGH);
    digitalWrite(rightled, LOW);
  } 
  else if (effectiveCommand == '4') { // Right
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);

    digitalWrite(rlight, LOW);
    digitalWrite(forwardled, LOW);
    digitalWrite(leftled, LOW);
    digitalWrite(rightled, HIGH);
  } 
  else if (effectiveCommand == '0') { // Stop
    stopMotors();
    digitalWrite(rlight, LOW);
  }

  delay(20);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);

  digitalWrite(forwardled, LOW);
  digitalWrite(leftled, LOW);
  digitalWrite(rightled, LOW);
}
