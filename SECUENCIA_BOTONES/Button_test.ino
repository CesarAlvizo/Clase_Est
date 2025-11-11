// constants won't change
const int ledPin = 13;       // the number of the LED pin (built-in)

// the numbers of the pushbutton pins
const int buttonPin1 = 44;   // SOS
const int buttonPin2 = 46;   // Blink
const int buttonPin3 = 47;   // Pulse
const int buttonPin4 = 45;   // Not used right now

// --- Setup ---
// This runs once when you power on or reset the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // initialize all pushbutton pins as inputs with pull-ups:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP); // Good to set it up anyway
}

// --- Main Loop ---
// This code runs over and over again
void loop() {
  
  // Check buttons in order of priority.
  // We check for LOW because we're using INPUT_PULLUP.
  
  if (digitalRead(buttonPin1) == LOW) {
    // Button 1 is pressed: run SOS
    runSOS();
  } 
  else if (digitalRead(buttonPin2) == LOW) {
    for (int i = 0; i < 7; i++)
    {
      runBlink();
    }
  } 
  else if (digitalRead(buttonPin3) == LOW) {
    // Button 3 is pressed: run Pulse
    runPulse();
  } 
  else {
    // No buttons are pressed: turn the LED off.
    digitalWrite(ledPin, LOW);
  }
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
// Simple on-off blink
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
  // We use analogWrite, which sends a PWM signal (0-255)
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
