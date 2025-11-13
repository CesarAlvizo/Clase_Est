// === CONFIGURACIÓN DE PINES ===
const int ledPin = 13;       // LED integrado del Arduino
const int buttonPin1 = 44;   // Botón 1 - SOS
const int buttonPin2 = 46;   // Botón 2 - Blink
const int buttonPin3 = 47;   // Botón 3 - Pulse
const int buttonPin4 = 45;   // Botón 4 (no usado)


// === CONFIGURACIÓN INICIAL ===
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
}


// === BUCLE PRINCIPAL ===
void loop() {
  if (digitalRead(buttonPin1) == LOW) {
    runSOS();        // SOS
  } 
  else if (digitalRead(buttonPin2) == LOW) {
    for (int i = 0; i < 7; i++) {
      runBlink();    // Blink
    }
  } 
  else if (digitalRead(buttonPin3) == LOW) {
    runPulse();      // Pulse
  } 
  else {
    digitalWrite(ledPin, LOW);  // Ningún botón presionado
  }
}


// === 1. SECUENCIA SOS ===
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


// === 2. PARPADEO (BLINK) ===
void runBlink() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}


// === 3. EFECTO PULSO (FADE) ===
void runPulse() {
  // Aumenta brillo
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
  // Disminuye brillo
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
}
