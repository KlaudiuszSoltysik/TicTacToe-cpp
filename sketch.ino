#include <iostream>
#include <random>
#include <vector>
#include <ShiftRegister74HC595.h>


using namespace std;


const int buttonPins[9] = {34, 35, 32, 33, 25, 26, 27, 14, 12};
bool buttonStates[9];

const int difficultySwitchPin = 13;
bool difficultySwitchState;

ShiftRegister74HC595 < 1 > redLeds(23, 19, 21);
ShiftRegister74HC595 < 1 > greenLeds(5, 15, 21);

const int redLed8Pin = 4;
const int greenLed8Pin = 2;

bool xFields[9] = {false, false, false, false, false, false, false, false, false};
bool oFields[9] = {false, false, false, false, false, false, false, false, false};
char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

bool playerMove = true;


void lightLeds();
pair < int, int > calculateBestMove();
int minimax(char board[][3], bool isMaximizing);
bool checkWinner(char board[][3], char player);
vector < pair < int, int >> checkWinningCoordinates();
bool isBoardFull(char board[][3]);
void flashLeds(pair < int, int > coordinates[]);


void lightLeds() {
  for (int i = 0; i < 9; i++) {
    xFields[i] = false;
    oFields[i] = false;

    int row = i / 3;
    int col = i % 3;

    if (board[row][col] == 'X') {
      xFields[i] = true;
    } else if (board[row][col] == 'O') {
      oFields[i] = true;
    } else if (board[row][col] == '!') {
      xFields[i] = true;
      oFields[i] = true;
    }

    if (i == 8) {
      break;
    }

    if (xFields[i]) {
      greenLeds.set(i, HIGH);
    } else {
      greenLeds.set(i, LOW);
    }

    if (oFields[i]) {
      redLeds.set(i, HIGH);
    } else {
      redLeds.set(i, LOW);
    }
  }

  if (xFields[8]) {
    digitalWrite(greenLed8Pin, HIGH);
  } else {
    digitalWrite(greenLed8Pin, LOW);
  }

  if (oFields[8]) {
    digitalWrite(redLed8Pin, HIGH);
  } else {
    digitalWrite(redLed8Pin, LOW);
  }
}


pair < int, int > calculateBestMove() {
  int bestScore = -1000;
  pair < int, int > bestMove;

  for (int i = 0; i < 9; i++) {
    int row = i / 3;
    int col = i % 3;

    if (board[row][col] == ' ') {
      board[row][col] = 'O';

      int score = minimax(board, false);

      board[row][col] = ' ';

      if (score > bestScore) {
        bestScore = score;
        bestMove = make_pair(row, col);
      }
    }
  }
  return bestMove;
}


int minimax(char board[][3], bool isMaximizing) {
  if (checkWinner(board, 'X')) {
    return -1;
  } else if (checkWinner(board, 'O')) {
    return 1;
  } else if (isBoardFull(board)) {
    return 0;
  }
  
  int bestScore;
  char player;

  if (isMaximizing) {
    bestScore = -1000;
    player = 'O';
  } else {
    bestScore = 1000;
    player = 'X';
  }
  
  for (int i = 0; i < 9; i++) {
    int row = i / 3;
    int col = i % 3;

    if (board[row][col] == ' ') {
      board[row][col] = player;

      int score = minimax(board, !isMaximizing);

      board[row][col] = ' ';

      if (isMaximizing) {
        bestScore = max(bestScore, score);
      } else {
        bestScore = min(bestScore, score);
      }
    }
  }
  return bestScore;
}


bool checkWinner(char board[][3], char player) {
  for (int row = 0; row < 3; row++) {
    if (board[row][0] == board[row][1] && board[row][0] == board[row][2] && board[row][0] == player) {
      return true;
    }
  }

  for (int col = 0; col < 3; col++) {
    if (board[0][col] == board[1][col] && board[0][col] == board[2][col] && board[0][col] == player) {
      return true;
    }
  }

  if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == player) {
    return true;
  }

  if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == player) {
    return true;
  }

  return false;
}


vector < pair < int, int >> checkWinningCoordinates() {
  vector < pair < int, int >> coordinates;

  for (int row = 0; row < 3; row++) {
    if (board[row][0] == board[row][1] && board[row][0] == board[row][2] && board[row][0] != ' '){
      for (int i = 0; i < 3; i++) {
        coordinates.push_back(make_pair(row, i));
      }
      return coordinates;
    }
  }

  for (int col = 0; col < 3; col++) {
    if (board[0][col] == board[1][col] && board[0][col] == board[2][col] && board[0][col] != ' ') {
      for (int i = 0; i < 3; i++) {
        coordinates.push_back(make_pair(i, col));
      }
      return coordinates;
    }
  }

  if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != ' ') {
    for (int i = 0; i < 3; i++) {
      coordinates.push_back(make_pair(i, i));
    }
    return coordinates;
  }

  if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != ' ') {
    for (int i = 0; i < 3; i++) {
      coordinates.push_back(make_pair(i, 2 - i));
    }
    return coordinates;
  }

  return coordinates;
}


bool isBoardFull(char board[][3]) {
  for (int i = 0; i < 9; i++) {
    int row = i / 3;
    int col = i % 3;

    if (board[row][col] == ' ') {
      return false;
    }
  }
  return true;
}


void flashLeds(vector < pair < int, int >> coordinates) {
  char signs[3];

  for (int i = 0; i < coordinates.size(); i++) {
    signs[i] = board[coordinates[i].first][coordinates[i].second];

    board[coordinates[i].first][coordinates[i].second] = '!';
  }

  lightLeds();
  delay(250);

  for (int i = 0; i < coordinates.size(); i++) {
    board[coordinates[i].first][coordinates[i].second] = signs[i];
  }

  lightLeds();
  delay(250);
}



void setup() {
  for (int pin: buttonPins) {
    pinMode(pin, INPUT);
  }

  pinMode(difficultySwitchPin, INPUT);

  pinMode(redLed8Pin, OUTPUT);
  pinMode(greenLed8Pin, OUTPUT);

  difficultySwitchState = digitalRead(difficultySwitchPin);

  if (difficultySwitchState) {
    int v[4] = {
      1,
      3,
      5,
      7
    };

    srand(static_cast < unsigned int > (time(nullptr)));

    int r = v[rand() % 4];

    board[r / 3][r % 3] = 'O';
  }
}


void loop() {
  for (int i = 0; i < 9; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (buttonStates[i] && playerMove) {
      int row = i / 3;
      int col = i % 3;

      if (board[row][col] == ' ') {
        playerMove = false;
        board[row][col] = 'X';
      }
    }
  }

  lightLeds();

  if (!playerMove && !isBoardFull(board)) {
    pair < int, int > bestMove = calculateBestMove();

    board[bestMove.first][bestMove.second] = 'O';

    playerMove = true;

    lightLeds();
  }
  if (checkWinner(board, 'O')) {
    flashLeds(checkWinningCoordinates());
  }
  
  delay(1);
}
