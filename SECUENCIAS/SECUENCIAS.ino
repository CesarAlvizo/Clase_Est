const int ledPin = 13; // Pin del LED integrado en el Arduino Mega

void setup() {
  pinMode(ledPin, OUTPUT); // Configura el pin como salida
}

void loop() {
  runSOS();         // Reproduce la señal SOS (...---...)
  
  for (int i = 0; i < 7; i++) {
    runBlink();     // Parpadea 7 veces
  }
  
  delay(1000); 
  
  runPulse();       // Efecto de brillo gradual
  delay(1000); 
}

// Señal SOS (...---...)
void runSOS() {
  int dot = 200;
  int dash = 600;
  int pause = 200;
  
  // S (...)
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause * 2);

  // O (---)
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause * 2);

  // S (...)
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause * 4);
}

// Parpadeo simple
void runBlink() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}

// Brillo gradual (fade)
void runPulse() {
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }

  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
}
