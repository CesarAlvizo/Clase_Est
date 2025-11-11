// --- Constants ---
const int ledPin = 13; // The number of the LED pin (built-in)

// --- Setup ---
// Runs once at the start
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

// --- Main Loop ---
// Runs over and over, one pattern after the other
void loop() {
  
  // 1. Play the SOS pattern once
  runSOS();
  
  // (The SOS function already includes a long pause at the end)

  // 2. Play the Blink pattern 7 times
  for (int i = 0; i < 7; i++) {
    runBlink();
  }
  
  // Pause for 1 second before starting the next pattern
  delay(1000); 
  
  // 3. Play the Pulse pattern once
  runPulse();
  
  // (The Pulse function ends with the LED off)
  
  // Pause for 1 second before the whole loop repeats
  delay(1000); 
}

// --- Helper Functions for Patterns ---

// 1. runSOS()
// Plays the ...---... pattern
void runSOS() {
  int dot = 200; // duration for a "dot"
  int dash = 600; // duration for a "dash"
  int pause = 200; // pause between dots/dashes
  
  // S (...)
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause * 2); // pause between letters

  // O (---)
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dash); digitalWrite(ledPin, LOW); delay(pause * 2); // pause between letters

  // S (...)
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause);
  digitalWrite(ledPin, HIGH); delay(dot); digitalWrite(ledPin, LOW); delay(pause * 4); // longer pause at end
}


// 2. runBlink()
// Simple on-off blink (one full cycle)
void runBlink() {
  digitalWrite(ledPin, HIGH);
  delay(500); // On for half a second
  digitalWrite(ledPin, LOW);
  delay(500); // Off for half a second
}


// 3. runPulse()
// Fades the LED in and out using PWM
void runPulse() {
  // Fade in
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(30); // small delay to make the fade smooth
  }

  // Fade out
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
}
