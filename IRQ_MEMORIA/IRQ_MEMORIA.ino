// --- Pines ---
const int ledPin = 13;       
const int buttonPin1 = 44;   // SOS
const int buttonPin2 = 46;   // Blink
const int buttonPin3 = 47;   // Pulse
const int buttonPin4 = 45;   // Pausa

// --- Variables de estado ---
int currentPattern = 0;  
bool isPaused = false;     

unsigned long previousMillis = 0;

// --- SOS ---
int sosStep = 0; 
const int sosNumSteps = 18;
int sosIntervals[sosNumSteps] = {200,200,200,200,200,400,
                                 600,200,600,200,600,400,
                                 200,200,200,200,200,1000};
bool sosStates[sosNumSteps] = {HIGH,LOW,HIGH,LOW,HIGH,LOW,
                               HIGH,LOW,HIGH,LOW,HIGH,LOW,
                               HIGH,LOW,HIGH,LOW,HIGH,LOW};

// --- Blink ---
int blinkCount = 0; 
bool blinkLedState = LOW; 

// --- Pulse ---
int fadeValue = 0;     
int fadeDirection = 5; 

// --- Antirrebote ---
unsigned long lastButtonPressTime = 0;
long debounceDelay = 200; 

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // --- Revisión de botones ---
  if (currentMillis - lastButtonPressTime > debounceDelay) {
    if (digitalRead(buttonPin4) == LOW) {           // Pausa
      if (!isPaused && currentPattern != 0) {
        isPaused = true;
        digitalWrite(ledPin, LOW);
        analogWrite(ledPin, 0);
        lastButtonPressTime = currentMillis;
      }
    }
    else if (digitalRead(buttonPin1) == LOW) {      // SOS
      handlePatternButton(1);
      lastButtonPressTime = currentMillis;
    }
    else if (digitalRead(buttonPin2) == LOW) {      // Blink
      handlePatternButton(2);
      lastButtonPressTime = currentMillis;
    }
    else if (digitalRead(buttonPin3) == LOW) {      // Pulse
      handlePatternButton(3);
      lastButtonPressTime = currentMillis;
    }
  }

  // --- Ejecución del patrón activo ---
  if (!isPaused) {
    switch (currentPattern) {
      case 1: handleSOS(currentMillis); break;
      case 2: handleBlink(currentMillis); break;
      case 3: handlePulse(currentMillis); break;
      case 0: 
        digitalWrite(ledPin, LOW);
        analogWrite(ledPin, 0);
        break;
    }
  }
}

void handlePatternButton(int patternNum) {
  if (currentPattern != patternNum) {
    currentPattern = patternNum;
    isPaused = false;
    resetPatternStates(); 
  } 
  else if (currentPattern == patternNum && isPaused) {
    isPaused = false; // reanuda sin reiniciar
  }
}

void resetPatternStates() {
  sosStep = 0;
  blinkCount = 0;
  blinkLedState = LOW;
  fadeValue = 0;
  fadeDirection = 5;
  previousMillis = millis(); 
}

// --- Patrones ---

void handleSOS(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= sosIntervals[sosStep]) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, sosStates[sosStep]);
    sosStep++;
    if (sosStep >= sosNumSteps) sosStep = 0;
  }
}

void handleBlink(unsigned long currentMillis) {
  if (blinkCount >= 14) {
    digitalWrite(ledPin, LOW);
    return;
  }
  
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    blinkLedState = !blinkLedState;
    digitalWrite(ledPin, blinkLedState);
    blinkCount++;
  }
}

void handlePulse(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= 30) {
    previousMillis = currentMillis;
    fadeValue += fadeDirection;
    
    if (fadeValue >= 255) { fadeValue = 255; fadeDirection = -5; }
    if (fadeValue <= 0)   { fadeValue = 0;   fadeDirection = 5; }

    analogWrite(ledPin, fadeValue);
  }
}
