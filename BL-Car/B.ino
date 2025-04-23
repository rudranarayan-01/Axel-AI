// Stops all the motors when detected 
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
byte command;
char data;

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 

// Motor B
int motor2Pin1=32;
int motor2Pin2=33;

int clight = 2; // Constant red light
int rlight = 4; // brake red light
int flight = 15; // Front light

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
const int obstacleThreshold = 35; // cm

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

void setup()
{
  // Motor pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Lights
  pinMode(clight, OUTPUT); // Constant red
  pinMode(rlight, OUTPUT); // Brake
  pinMode(flight, OUTPUT); // Front light

  // Indicators
  pinMode(forwardled, OUTPUT);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Bluetooth Started! Ready to pair...");
}

void loop()
{
  // Read Bluetooth command
  if (SerialBT.available()) {
    command = SerialBT.read();
    Serial.print("\n\nBluetooth Serial : ");
    Serial.println(command);
  }

  // Read Serial (from RasPi if needed)
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

  // Fixed lights always on
  digitalWrite(clight, HIGH);
  digitalWrite(flight, HIGH);

  // 🚫 Obstacle detected? Stop everything
  if (distance > 0 && distance <= obstacleThreshold) {
    stopMotors();
    Serial.println("🚧 Obstacle detected! Car stopped.");
    digitalWrite(rlight, HIGH); // brake light
    return; // skip rest of loop
  }

  // Movement logic
  if (command == '1' || data == '1') { // Forward
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(forwardled, HIGH);
    digitalWrite(rlight, LOW);
  } 
  else if (command == '2' || data == '2') { // Backward
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(rlight, HIGH);
  } 
  else if (command == '3' || data == '3') { // Left
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(leftled, HIGH);
    digitalWrite(rlight, LOW);
  } 
  else if (command == '4' || data == '4') { // Right
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(rightled, HIGH);
    digitalWrite(rlight, LOW);
  } 
  else if (command == '0' || data == '0') { // Stop
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
