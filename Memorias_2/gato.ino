#include "LedControl.h"
#include "Keypad.h"

// --- PINS ---
#define DIN_PIN   28  
#define CLK_PIN   26  
#define CS_PIN_L  27  // Left Matrix (Game)
#define CS_PIN_R  29  // Right Matrix (Score)

LedControl lcBoard = LedControl(DIN_PIN, CLK_PIN, CS_PIN_L, 1);
LedControl lcScore = LedControl(DIN_PIN, CLK_PIN, CS_PIN_R, 1);

// --- KEYPAD ---
byte rowPins[4] = {4, 5, 6, 7}; 
byte colPins[4] = {8, 9, 10, 11}; 
char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// --- GAME VARIABLES ---
int board[3][3]; 
int turn = 1; 
boolean gameOver = false;
int playerScore = 0;

void setup() {
  randomSeed(analogRead(0)); 
  
  lcBoard.shutdown(0, false); lcBoard.setIntensity(0, 8); lcBoard.clearDisplay(0);
  lcScore.shutdown(0, false); lcScore.setIntensity(0, 8); lcScore.clearDisplay(0);

  resetGame();
}

void loop() {
  if (gameOver) {
    char key = keypad.getKey();
    if (key == '#') resetGame(); 
    return;
  }

  if (turn == 1) humanMove();
  else {
    delay(500);
    aiMove();
  }
}

// --- LOGIC ---
void humanMove() {
  char key = keypad.getKey();
  if (key) {
    int r = -1, c = -1;
    switch (key) {
      case '1': r=0; c=0; break; case '2': r=0; c=1; break; case '3': r=0; c=2; break;
      case '4': r=1; c=0; break; case '5': r=1; c=1; break; case '6': r=1; c=2; break;
      case '7': r=2; c=0; break; case '8': r=2; c=1; break; case '9': r=2; c=2; break;
    }
    if (r != -1 && board[r][c] == 0) {
      board[r][c] = 1; 
      updateDisplay();
      checkWin();
      if (!gameOver) turn = 2;
    }
  }
}

void aiMove() {
  int r, c;
  boolean found = false;
  for(int i=0; i<50; i++) {
    r = random(0, 3); c = random(0, 3);
    if(board[r][c] == 0) { board[r][c] = 2; found = true; break; }
  }
  if (!found) {
     for(int i=0; i<3; i++) for(int j=0; j<3; j++)
       if(board[i][j] == 0) { board[i][j] = 2; goto done; }
  }
  done:
  updateDisplay();
  checkWin();
  if (!gameOver) turn = 1;
}

// --- VISUALS ---
void updateDisplay() {
  lcBoard.clearDisplay(0);
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      if (board[r][c] == 0) continue;
      // Rotation Logic (Matches your hardware)
      int ledRow = c * 3; 
      int ledCol = (2 - r) * 3; 

      if (board[r][c] == 1) { 
        lcBoard.setLed(0, ledRow, ledCol + 1, true); 
      } 
      else if (board[r][c] == 2) { 
        lcBoard.setLed(0, ledRow, ledCol, true);
        lcBoard.setLed(0, ledRow + 1, ledCol, true);
        lcBoard.setLed(0, ledRow, ledCol + 1, true);
        lcBoard.setLed(0, ledRow + 1, ledCol + 1, true);
      }
    }
  }
}

// --- NEW WIN CHECKER ---
void checkWin() {
  int winner = 0;
  
  // 1. Check Rows
  for (int r=0; r<3; r++) {
    if(board[r][0]!=0 && board[r][0]==board[r][1] && board[r][1]==board[r][2]) {
      winner = board[r][0];
      gameOver = true;
      if(winner==1) updateScore();
      blinkLine(r,0, r,1, r,2); // Blink this specific row
      return;
    }
  }

  // 2. Check Cols
  for (int c=0; c<3; c++) {
    if(board[0][c]!=0 && board[0][c]==board[1][c] && board[1][c]==board[2][c]) {
      winner = board[0][c];
      gameOver = true;
      if(winner==1) updateScore();
      blinkLine(0,c, 1,c, 2,c); // Blink this specific column
      return;
    }
  }

  // 3. Check Diagonals
  if(board[0][0]!=0 && board[0][0]==board[1][1] && board[1][1]==board[2][2]) {
      winner = board[0][0];
      gameOver = true;
      if(winner==1) updateScore();
      blinkLine(0,0, 1,1, 2,2);
      return;
  }
  if(board[0][2]!=0 && board[0][2]==board[1][1] && board[1][1]==board[2][0]) {
      winner = board[0][2];
      gameOver = true;
      if(winner==1) updateScore();
      blinkLine(0,2, 1,1, 2,0);
      return;
  }

  // 4. Check Draw
  boolean full = true;
  for(int r=0; r<3; r++) for(int c=0; c<3; c++) if(board[r][c]==0) full = false;

  if (full) {
    gameOver = true;
    blinkAll(); // If it's a draw, blink everything briefly
  }
}

// --- NEW ANIMATION LOGIC ---
// Takes the 3 coordinates of the winning line
void blinkLine(int r1, int c1, int r2, int c2, int r3, int c3) {
  int winnerVal = board[r1][c1]; // Remember who won (1 or 2)
  
  for(int i=0; i<5; i++) {
    // Turn winning cells OFF (set to 0 temporarily)
    board[r1][c1] = 0;
    board[r2][c2] = 0;
    board[r3][c3] = 0;
    updateDisplay(); 
    delay(250);
    
    // Turn winning cells ON
    board[r1][c1] = winnerVal;
    board[r2][c2] = winnerVal;
    board[r3][c3] = winnerVal;
    updateDisplay();
    delay(250);
  }
}

void blinkAll() {
  for(int i=0; i<3; i++) {
    lcBoard.clearDisplay(0);
    delay(200);
    updateDisplay();
    delay(200);
  }
}

void updateScore() {
  playerScore++;
  updateScoreboard();
}

void updateScoreboard() {
  lcScore.clearDisplay(0);
  for(int i=0; i<playerScore; i++) {
    if (i < 64) lcScore.setLed(0, i/8, i%8, true);
  }
}

void resetGame() {
  for (int r=0; r<3; r++) for (int c=0; c<3; c++) board[r][c] = 0;
  turn = 1;
  gameOver = false;
  updateDisplay();
}
