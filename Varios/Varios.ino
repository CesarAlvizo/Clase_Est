/*
 * Fecha: 10/10/2025
 * Descripción: 
 * Envía un mensaje en código Morse con la señal SOS, luego hace parpadear el LED 7 veces,  
 * repite el SOS, y finalmente realiza un efecto de fade (encendido y apagado gradual) también 7 veces.
 */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // 1. Prende el led haciendo mensaje: SOS
  sendSOS();

  // 2. Blink 7 veces
  for (int i = 0; i < 7; i++) {
    digitalWrite(LED_BUILTIN, HIGH); // Prender LED
    delay(500);
    digitalWrite(LED_BUILTIN, LOW); // Apagar LED
    delay(500);
  }

  // 3. Prende el led haciendo mensaje: SOS
  sendSOS();

  // 4. Pulse (fade) 7 veces
  for (int i = 0; i < 7; i++) { // Loop de 0 a 6 (7 veces)
    fadeOnce(); // Llamada a la funcion fade
  }
  delay(2000);
}

// Funcion para punto en Morse
void dot() {
  digitalWrite(LED_BUILTIN, HIGH); // Prender Led
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);// Apagar LED
  delay(200);
}

// Funcion para linea en Morse
void dash() {
  digitalWrite(LED_BUILTIN, HIGH); // Prender Led
  delay(600);
  digitalWrite(LED_BUILTIN, LOW); // Apagar LED
  delay(200);
}

// Funcion que manda S utilizando las funciones dot() y dash()
void sendS() {
  dot(); dot(); dot();
}

// Funcion que manda O utilizando las funciones dot() y dash()
void sendO() {
  dash(); dash(); dash();
}

// Funcion que manda SOS utilizando las funciones dot() y dash()
void sendSOS() {
  sendS(); 
  delay(500); 
  sendO();
  delay(500); 
  sendS();
  delay(1000);
}

// === Fade ===
void fadeOnce() {
  // Fade in
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LED_BUILTIN, brightness); // hace fade de apagado hasta prendido
    delay(2); // 0–255 
  }
  // Fade out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LED_BUILTIN, brightness); // hace fade de prendido a apagado
    delay(2); 
  }
}



