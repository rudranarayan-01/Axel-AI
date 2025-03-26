void setup() {
    Serial.begin(9600);  
    pinMode(9, OUTPUT);  // Red light
    pinMode(10, OUTPUT); // Yellow light
    pinMode(11, OUTPUT); // Green light
  }
  
  void loop() {
    if (Serial.available()) {
      char data = Serial.read();
      if (data == 'R') {
        digitalWrite(9, HIGH); digitalWrite(10, LOW); digitalWrite(11, LOW);
      } else if (data == 'Y') {
        digitalWrite(9, LOW); digitalWrite(10, HIGH); digitalWrite(11, LOW);
      } else if (data == 'G') {
        digitalWrite(9, LOW); digitalWrite(10, LOW); digitalWrite(11, HIGH);
      }
    }
  }
  