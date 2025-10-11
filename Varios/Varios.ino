void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // 1. SOS
  sendSOS();

  // 2. Blink 7 
  for (int i = 0; i < 7; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  // 3. SOS
  sendSOS();

  // 4. Pulse (fade) 7
  for (int i = 0; i < 7; i++) {
    fadeOnce();  // fade in and out once (~1 sec)
  }

  delay(2000);
}

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

void sendS() {
  dot(); dot(); dot();
}

void sendO() {
  dash(); dash(); dash();
}

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
    analogWrite(LED_BUILTIN, brightness);
    delay(2); // 0–255 
  }
  // Fade out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LED_BUILTIN, brightness);
    delay(2); // 
  }
}
