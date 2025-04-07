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
int rlight = 4; // fixed red light
int flight = 15; // Front light

/* Check if Bluetooth configurations are enabled in the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void setup()
{
  //Motor1 Pins
  pinMode(motor1Pin1,OUTPUT);
  pinMode(motor1Pin2,OUTPUT);

  //Motor2 Pins
  pinMode(motor2Pin1,OUTPUT);
  pinMode(motor2Pin2,OUTPUT);

  pinMode(clight, OUTPUT);
  pinMode(rlight, OUTPUT);
  pinMode(flight, OUTPUT);
  
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("Bluetooth Started! Ready to pair...");
  Serial.print("Testing DC Motor...");
}

void loop()
{

  // Read data from bluetooth
  if(SerialBT.available())
  {
    command = SerialBT.read();
    Serial.write("\n\nBluetooth Serial :",command);
  }

  // Read data from raspi
  if (Serial.available()) {
    data = Serial.read();
    Serial.write("\n\nRaspi command:",data);
  }

  digitalWrite(clight, HIGH);
  digitalWrite(flight, HIGH);
// Going Forward
  if(command=='1' || data == '1')
  {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(rlight,LOW);
  }
//Going Backward
  if(command=='2' || data == '2')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(rlight,HIGH);
  }
//Turning Left
  if(command=='3' || data == '3')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(rlight,LOW);
  }
//Turning Left
  if(command=='4' || data == '4')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(rlight,LOW);
  }
//Stopping
  if(command=='0' || data == '0')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(rlight,LOW);

  }

  delay(20);
  
}