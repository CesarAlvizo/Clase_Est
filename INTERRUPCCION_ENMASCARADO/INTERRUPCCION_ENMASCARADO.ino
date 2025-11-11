// --- Constantes de Pines ---
const int ledPin = 13;       // el pin del LED
const int buttonPin1 = 44;   // SOS
const int buttonPin2 = 46;   // Blink
const int buttonPin3 = 47;   // Pulse
const int buttonPin4 = 45;   // Botón de INTERRUPCIÓN

// --- Setup ---
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP); // El botón de interrupción
}

// --- Loop Principal ---
void loop() {
  // El botón 45 (interrupción) se revisa DENTRO de las funciones de patrones.
  
  if (digitalRead(buttonPin1) == LOW) {
    runSOS();
  } 
  else if (digitalRead(buttonPin2) == LOW) {
    for (int i = 0; i < 7; i++) {
      // runBlink() ahora regresa 'false' si fue interrumpido
      if (!runBlink()) {
        break; // Salir del 'for loop' si se interrumpió
      }
    }
  } 
  else if (digitalRead(buttonPin3) == LOW) {
    runPulse();
  } 
  else {
    // Si no hay ningún botón de patrón presionado, apagar el LED.
    // Si presionas el botón 45, se interrumpirá la secuencia
    // y el código caerá aquí, apagando el LED.
    digitalWrite(ledPin, LOW);
  }
}

// --- Nueva Función de Delay ---

/**
 * @brief Hace una pausa similar a delay(), pero revisa el botón de interrupción.
 * @param ms El tiempo en milisegundos para la pausa.
 * @return 'true' si fue interrumpido (botón 45 presionado), 'false' si el tiempo terminó.
 */
bool delayInterruptible(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    // Revisar constantemente el botón de interrupción
    if (digitalRead(buttonPin4) == LOW) {
      return true; // ¡Interrumpido!
    }
    // Pequeña pausa para que el procesador respire
    delay(1); 
  }
  return false; // Pausa completada sin interrupción
}


// --- Funciones de Patrones Modificadas ---

// 1. runSOS()
void runSOS() {
  int dot = 200; int dash = 600; int pause = 200;
  
  // Cada 'delay()' se reemplaza con 'if (delayInterruptible(...)) return;'
  
  // S (...)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 2)) return; // Pausa letra

  // O (---)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dash)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 2)) return; // Pausa letra

  // S (...)
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause)) return;
  digitalWrite(ledPin, HIGH); if (delayInterruptible(dot)) return;
  digitalWrite(ledPin, LOW);  if (delayInterruptible(pause * 4)) return; // Pausa larga
}


// 2. runBlink()
// Ahora regresa 'bool': 'true' si completó, 'false' si fue interrumpido.
bool runBlink() {
  digitalWrite(ledPin, HIGH);
  if (delayInterruptible(500)) return false; // Interrumpido
  
  digitalWrite(ledPin, LOW);
  if (delayInterruptible(500)) return false; // Interrumpido
  
  return true; // Blink completado
}


// 3. runPulse()
void runPulse() {
  // Fade in
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    if (delayInterruptible(30)) return; // Revisar en cada paso del fade
  }

  // Fade out
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    if (delayInterruptible(30)) return; // Revisar en cada paso del fade
  }
}
