// --- Pin Constants ---
const int ledPin = 13;       // Built-in LED
const int buttonPin1 = 44;   // SOS
const int buttonPin2 = 46;   // Blink
const int buttonPin3 = 47;   // Pulse
const int buttonPin4 = 45;   // INTERRUPT (PAUSE) button

// --- State Machine Variables ---
int currentPattern = 0;  // 0=Off, 1=SOS, 2=Blink, 3=Pulse
bool isPaused = false;     // True if Button 4 was pressed

// --- General Timer ---
unsigned long previousMillis = 0;

// --- "Memory" (State Variables) for Each Pattern ---

// For SOS (Pattern 1)
int sosStep = 0; // Stores which step of the SOS we are on
const int sosNumSteps = 18;
int sosIntervals[sosNumSteps] = { 200, 200, 200, 200, 200, 400,  /* S ... */
                                  600, 200, 600, 200, 600, 400,  /* O --- */
                                  200, 200, 200, 200, 200, 1000 };/* S ... (long pause) */
bool sosStates[sosNumSteps] = { HIGH, LOW, HIGH, LOW, HIGH, LOW,
                                HIGH, LOW, HIGH, LOW, HIGH, LOW,
                                HIGH, LOW, HIGH, LOW, HIGH, LOW };

// For Blink (Pattern 2)
int blinkCount = 0; // Stores how many of the 7 blinks (14 steps) are done
bool blinkLedState = LOW; // Stores if LED is currently on or off

// For Pulse (Pattern 3)
int fadeValue = 0;     // Stores the current brightness (0-255)
int fadeDirection = 5; // Stores if we are fading up (+5) or down (-5)

// --- Debounce Variables ---
unsigned long lastButtonPressTime = 0;
long debounceDelay = 200; // 200ms delay to prevent fast double-presses

// --- SETUP ---
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  
  digitalWrite(ledPin, LOW); // Start with LED off
}

// --- MAIN LOOP ---
void loop() {
  unsigned long currentMillis = millis();

  // --- 1. Check for Button Presses (with Debounce) ---
  if (currentMillis - lastButtonPressTime > debounceDelay) {
    
    // Check for INTERRUPT/PAUSE button (Pin 45)
    if (digitalRead(buttonPin4) == LOW) {
      if (!isPaused && currentPattern != 0) {
        isPaused = true;
        digitalWrite(ledPin, LOW); // Turn off LED when paused
        analogWrite(ledPin, 0);    // (for the pulse)
        lastButtonPressTime = currentMillis;
      }
    }
    // Check for Pattern 1 (SOS)
    else if (digitalRead(buttonPin1) == LOW) {
      handlePatternButton(1);
      lastButtonPressTime = currentMillis;
    }
    // Check for Pattern 2 (Blink)
    else if (digitalRead(buttonPin2) == LOW) {
      handlePatternButton(2);
      lastButtonPressTime = currentMillis;
    }
    // Check for Pattern 3 (Pulse)
    else if (digitalRead(buttonPin3) == LOW) {
      handlePatternButton(3);
      lastButtonPressTime = currentMillis;
    }
  }

  // --- 2. Run the Active Pattern (if not paused) ---
  if (!isPaused) {
    switch (currentPattern) {
      case 1:
        handleSOS(currentMillis);
        break;
      case 2:
        handleBlink(currentMillis);
        break;
      case 3:
        handlePulse(currentMillis);
        break;
      case 0: // Off
        digitalWrite(ledPin, LOW);
        analogWrite(ledPin, 0);
        break;
    }
  }
  // If (isPaused), do nothing. The LED stays off and variables are frozen.
}

// --- Helper Functions ---

/**
 * @brief Handles the logic for starting, resuming, or switching a pattern.
 */
void handlePatternButton(int patternNum) {
  if (currentPattern != patternNum) {
    // --- This is a NEW pattern ---
    // Start it from the beginning
    currentPattern = patternNum;
    isPaused = false;
    resetPatternStates(); // Reset all "memory" variables
  } 
  else if (currentPattern == patternNum && isPaused) {
    // --- This is to RESUME a paused pattern ---
    isPaused = false;
    // We DO NOT reset variables, so it resumes where it left off
  }
  // (If pattern is already running and not paused, pressing its button does nothing)
}

/**
 * @brief Resets all "memory" state variables to their starting values.
 */
void resetPatternStates() {
  // Reset SOS
  sosStep = 0;
  
  // Reset Blink
  blinkCount = 0;
  blinkLedState = LOW;
  
  // Reset Pulse
  fadeValue = 0;
  fadeDirection = 5;

  // IMPORTANT: Reset the master timer for the new pattern
  previousMillis = millis(); 
}

// --- Pattern Handlers (Non-Blocking) ---

// 1. SOS Handler
void handleSOS(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= sosIntervals[sosStep]) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, sosStates[sosStep]);
    sosStep++;
    if (sosStep >= sosNumSteps) {
      sosStep = 0; // Loop pattern
    }
  }
}

// 2. Blink Handler (7 times)
void handleBlink(unsigned long currentMillis) {
  // 7 blinks = 14 steps (on, off, on, off...)
  if (blinkCount >= 14) {
    digitalWrite(ledPin, LOW); // Stay off when done
    return; // Finished
  }
  
  if (currentMillis - previousMillis >= 500) { // 0.5 second interval
    previousMillis = currentMillis;
    blinkLedState = !blinkLedState; // Invert state
    digitalWrite(ledPin, blinkLedState);
    blinkCount++; // Increment "memory"
  }
}

// 3. Pulse Handler
void handlePulse(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= 30) { // 30ms for smooth fade
    previousMillis = currentMillis;
    
    fadeValue = fadeValue + fadeDirection; // Update "memory"
    
    if (fadeValue >= 255) {
      fadeValue = 255;
      fadeDirection = -5; // Change "memory"
    }
    if (fadeValue <= 0) {
      fadeValue = 0;
      fadeDirection = 5; // Change "memory"
    }
    
    analogWrite(ledPin, fadeValue);
  }
}
