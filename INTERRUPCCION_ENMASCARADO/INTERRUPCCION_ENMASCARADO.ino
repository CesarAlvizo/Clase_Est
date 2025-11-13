// === CONFIGURACIÓN DE PINES ===
const int ledPin = 13;       // LED integrado del Arduino
const int buttonPin1 = 44;   // Botón 1 - SOS
const int buttonPin2 = 46;   // Botón 2 - Blink
const int buttonPin3 = 47;   // Botón 3 - Pulse
const int buttonPin4 = 45;   // Botón 4 - Interrupción / Pausa


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
  // El botón 45 interrumpe las secuencias desde dentro de las funciones
  if (digitalRead(buttonPin1) == LOW) {
    runSOS();
  } 
  else if (digitalRead(buttonPin2) == LOW) {
    for (int i = 0; i < 7; i++) {
      if (!runBlink()) break;  // Termina si se interrumpe
    }
  } 
  else if (digitalRead(buttonPin3) == LOW) {
    runPulse();
  } 
  else {
    // Si no hay botón activo, apaga el LED
    digitalWrite(ledPin, LOW);
  }
}


// === FUNCIÓN DE PAUSA INTERRUPTIBLE ===
bool delayInterruptible(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (digitalRead(buttonPin4) == LOW) {
      return true; // Se interrumpió
    }
    delay(1); // Espera mínima
  }
  return false; // Tiempo completado sin interrupción
}


// === 1. SECUENCIA SOS ===
void runSOS() {
  int dot = 200;  // punto corto
  int dash = 600; // raya larga
  int pause = 200; // pausa

  // S (...)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 2)) return;

  // O (---)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 2)) return;

  // S (...)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 4)) return;
}


// === 2. PARPADEO (BLINK) ===
bool runBlink() {
  digitalWrite(ledPin, HIGH);
  if (delayInterruptible(500)) return false;

  digitalWrite(ledPin, LOW);
  if (delayInterruptible(500)) return false;

  return true; // Completado
}


// === 3. EFECTO PULSO (FADE) ===
void runPulse() {
  // Aumento de brillo
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    if (delayInterruptible(30)) return;
  }
  // Disminución de brillo
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    if (delayInterruptible(30)) return;
  }
}

