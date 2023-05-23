#include <iostream>
#include <ShiftRegister74HC595.h>

using namespace std;

// Defining pins and state variables
const int buttonPins[9] = {34, 35, 32, 33, 25, 26, 27, 14, 12};
bool buttonStates[9];

const int difficultySwitchPin = 13;
bool difficultySwitchState;

ShiftRegister74HC595<1> redLeds(23, 19, 21);
ShiftRegister74HC595<1> greenLeds(5, 15, 21);

const int redLed8Pin = 4;
const int greenLed8Pin = 2;

bool xFields[9] = {false, false, false, false, false, false, false, false, false};
bool oFields[9] = {false, false, false, false, false, false, false, false, false};
char board[3][3] = {{'X', ' ', ' '}, {' ', '!', ' '}, {' ', 'O', ' '}};

bool playerMove = true;


void lightLeds() {
  for(int i=0; i<9; i++) {
    xFields[i] = false;
    oFields[i] = false;

    int row = i/3;
    int col = i%3;

    if(board[row][col] == 'X') {
      xFields[i] = true;
    } else if(board[row][col] == 'O') {
      oFields[i] = true;
    } else if(board[row][col] == '!') {
      xFields[i] = true;
      oFields[i] = true;
    }
    
    if(i == 8) {
      break;
    }

    if(xFields[i]) {
      greenLeds.set(i, HIGH);
    } else {
      greenLeds.set(i, LOW);
    }

    if(oFields[i]) {
      redLeds.set(i, HIGH);
    } else {
      redLeds.set(i, LOW);
    }
  }

  if(xFields[8]) {
    digitalWrite(greenLed8Pin, HIGH);
  } else {
    digitalWrite(greenLed8Pin, LOW);
  }

  if(oFields[8]) {
    digitalWrite(redLed8Pin, HIGH);
  } else {
    digitalWrite(redLed8Pin, LOW);
  }
}



void setup() {
  for(int pin : buttonPins) {
    pinMode(pin, INPUT);
  }

  pinMode(difficultySwitchPin, INPUT);

  pinMode(redLed8Pin, OUTPUT);
  pinMode(greenLed8Pin, OUTPUT);
}



void loop() {
  // Updating input state variables
  for(int i = 0; i < 9; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);

    if(digitalRead(buttonStates[i]) && playerMove) {
      int row = i/3;
      int col = i%3;

      if(board[row][col] == ' ') {
        playerMove = false;
        board[row][col] = 'X';
      }
    }
  }

  difficultySwitchState = digitalRead(difficultySwitchPin);
  
  lightLeds();

  if(difficultySwitchState) {
  
  } else {
    playerMove = true;
  }

  delay(10);
}
