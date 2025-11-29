// --- Constants & Pin Definitions ---
const int NUM_ITEMS = 6;
// Note: Pin 58 is Analog Pin A4, but works as digital 58 on Mega.
const int buttonPins[NUM_ITEMS] = { 52, 53, 50, 51, 58, 49 }; 
const int ledPins[NUM_ITEMS]    = { 27, 26, 25, 24, 23, 22 };

// --- Game Settings ---
const int MAX_GUESSES = 3;

// --- State Machine ---
enum GameState {
  STATE_PLAYING,
  STATE_WIN_ANIM,
  STATE_LOSE_ANIM,
  STATE_RESET
};

GameState currentState = STATE_RESET;

// --- Game Variables ---
int minePosition = -1;      // 0 to 5
int guessesUsed = 0;        
bool ledStates[NUM_ITEMS];  // Tracks which LEDs are currently ON (Safe zones)

// --- Timer Variables ---
unsigned long previousMillis = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

// --- Animation Variables ---
int animStep = 0;
bool animState = LOW;

// --- SETUP ---
void setup() {
  // Initialize Pins
  for (int i = 0; i < NUM_ITEMS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // All off initially
  }

  // Seed random generator (reads noise from an unconnected pin)
  randomSeed(analogRead(A0));
  
  // Start the game
  currentState = STATE_RESET;
}

// --- MAIN LOOP ---
void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {
    
    // 1. RESET GAME STATE
    case STATE_RESET:
      resetGame();
      currentState = STATE_PLAYING;
      break;

    // 2. PLAYING STATE
    case STATE_PLAYING:
      checkButtons(currentMillis);
      break;

    // 3. WIN ANIMATION (Blink 2 times)
    case STATE_WIN_ANIM:
      runWinAnimation(currentMillis);
      break;

    // 4. LOSE ANIMATION (Slow Pulse / Fade simulation)
    case STATE_LOSE_ANIM:
      runLoseAnimation(currentMillis);
      break;
  }
}

// --- Helper Functions ---

void resetGame() {
  // Turn all LEDs off
  for (int i = 0; i < NUM_ITEMS; i++) {
    digitalWrite(ledPins[i], LOW);
    ledStates[i] = false;
  }
  
  guessesUsed = 0;
  minePosition = random(0, NUM_ITEMS); // Pick a random number 0-5
  
  // Reset animation vars
  animStep = 0;
  previousMillis = millis();
}

void checkButtons(unsigned long currentMillis) {
  // Simple Debounce check
  if (currentMillis - lastDebounceTime < debounceDelay) return;

  for (int i = 0; i < NUM_ITEMS; i++) {
    // Check if button pressed (LOW)
    if (digitalRead(buttonPins[i]) == LOW) {
      
      lastDebounceTime = currentMillis; // Update debounce
      
      // If this LED is already on, ignore the press (don't waste a guess)
      if (ledStates[i] == true) return; 

      // LOGIC: Did they find the mine?
      if (i == minePosition) {
        // WINNER!
        currentState = STATE_WIN_ANIM;
        previousMillis = currentMillis;
        animStep = 0;
      } else {
        // SAFE SPOT (Not the mine)
        ledStates[i] = true;          // Mark as revealed
        digitalWrite(ledPins[i], HIGH); // Turn LED ON and keep it ON
        guessesUsed++;
        
        // CHECK: Did they run out of guesses?
        if (guessesUsed >= MAX_GUESSES) {
          currentState = STATE_LOSE_ANIM;
          previousMillis = currentMillis;
          animStep = 0;
        }
      }
    }
  }
}

// --- Animations ---

void runWinAnimation(unsigned long currentMillis) {
  // Goal: Blink all LEDs 2 times.
  // 2 times = On, Off, On, Off (4 steps)
  
  if (animStep >= 4) {
    // Animation done, restart game after short pause
    if (currentMillis - previousMillis > 1000) {
      currentState = STATE_RESET;
    }
    return;
  }

  // Blink speed: 300ms
  if (currentMillis - previousMillis >= 300) {
    previousMillis = currentMillis;
    
    // Toggle state
    animState = !animState; 
    
    // Apply to ALL LEDs
    for (int i = 0; i < NUM_ITEMS; i++) {
      digitalWrite(ledPins[i], animState);
    }
    animStep++;
  }
}

void runLoseAnimation(unsigned long currentMillis) {
  // Goal: Fade up/down simulation (Slow Pulse)
  // Since pins 22-27 aren't PWM, we just toggle them slowly 3 times
  
  if (animStep >= 6) { // 3 pulses (On/Off * 3 = 6 steps)
     // Animation done, restart game
    if (currentMillis - previousMillis > 1000) {
      currentState = STATE_RESET;
    }
    return;
  }

  // Pulse speed: 600ms (Slower than win blink)
  if (currentMillis - previousMillis >= 600) {
    previousMillis = currentMillis;
    
    animState = !animState;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
      digitalWrite(ledPins[i], animState);
    }
    animStep++;
  }
}
