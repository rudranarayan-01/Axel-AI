nvoid setup() {
    Serial.begin(115200);  
    pinMode(13, OUTPUT);  // Red light
    pinMode(12, OUTPUT); // Yellow light
    pinMode(14, OUTPUT); // Green light
  }
  
  void loop() {
    if (Serial.available()) {
      char data = Serial.read();
      if (data == '1') {
        digitalWrite(13, HIGH); digitalWrite(12, LOW); digitalWrite(14, LOW);
      } else if (data == '2') {
        digitalWrite(13, LOW); digitalWrite(12, HIGH); digitalWrite(14, LOW);
      } else if (data == '3') {
        digitalWrite(3, LOW); digitalWrite(12, LOW); digitalWrite(14, HIGH);
      }
    }
  }
  