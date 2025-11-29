const int NUM_ITEMS = 6;
const int buttonPins[NUM_ITEMS] = { 52, 53, 50, 51, 58, 49 };
const int ledPins[NUM_ITEMS]    = { 27, 26, 25, 24, 23, 22 };

const int MAX_GUESSES = 3;

enum GameState {
  STATE_PLAYING,
  STATE_WIN_ANIM,
  STATE_LOSE_ANIM,
  STATE_RESET
};

GameState currentState = STATE_RESET;

int minePosition = -1;      
int guessesUsed = 0;        
bool ledStates[NUM_ITEMS];  

unsigned long previousMillis = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

int animStep = 0;
bool animState = LOW;

void setup() {

  // Configurar botones y LEDs
  for (int i = 0; i < NUM_ITEMS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  randomSeed(analogRead(A0));  // Para la mina aleatoria
  currentState = STATE_RESET;
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {
    case STATE_RESET:
      resetGame();
      currentState = STATE_PLAYING;
      break;

    case STATE_PLAYING:
      checkButtons(currentMillis);
      break;

    case STATE_WIN_ANIM:
      runWinAnimation(currentMillis);
      break;

    case STATE_LOSE_ANIM:
      runLoseAnimation(currentMillis);
      break;
  }
}

void resetGame() {
  // Apagar LEDs y reiniciar variables
  for (int i = 0; i < NUM_ITEMS; i++) {
    digitalWrite(ledPins[i], LOW);
    ledStates[i] = false;
  }

  guessesUsed = 0;
  minePosition = random(0, NUM_ITEMS);   // Elegir nueva mina
  animStep = 0;
  previousMillis = millis();
}

void checkButtons(unsigned long currentMillis) {

  // Debounce básico
  if (currentMillis - lastDebounceTime < debounceDelay) return;

  for (int i = 0; i < NUM_ITEMS; i++) {

    // Botón presionado
    if (digitalRead(buttonPins[i]) == LOW) {

      lastDebounceTime = currentMillis;

      // Si ese LED ya está revelado, no cuenta
      if (ledStates[i]) return;

      // ¿Presionó la mina?
      if (i == minePosition) {
        currentState = STATE_WIN_ANIM;
        previousMillis = currentMillis;
        animStep = 0;
      } 
      else {
        // Zona segura
        ledStates[i] = true;
        digitalWrite(ledPins[i], HIGH);
        guessesUsed++;

        // Si gastó todos los intentos → perder
        if (guessesUsed >= MAX_GUESSES) {
          currentState = STATE_LOSE_ANIM;
          previousMillis = currentMillis;
          animStep = 0;
        }
      }
    }
  }
}

void runWinAnimation(unsigned long currentMillis) {

  // Termina la animación
  if (animStep >= 4) {
    if (currentMillis - previousMillis > 1000) {
      currentState = STATE_RESET;
    }
    return;
  }

  // Parpadeo rápido
  if (currentMillis - previousMillis >= 300) {
    previousMillis = currentMillis;

    animState = !animState;

    for (int i = 0; i < NUM_ITEMS; i++) {
      digitalWrite(ledPins[i], animState);
    }

    animStep++;
  }
}

void runLoseAnimation(unsigned long currentMillis) {

  // Termina la animación
  if (animStep >= 6) {
    if (currentMillis - previousMillis > 1000) {
      currentState = STATE_RESET;
    }
    return;
  }

  // Parpadeo lento
  if (currentMillis - previousMillis >= 600) {
    previousMillis = currentMillis;

    animState = !animState;

    for (int i = 0; i < NUM_ITEMS; i++) {
      digitalWrite(ledPins[i], animState);
    }

    animStep++;
  }
}

