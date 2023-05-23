#include <iostream>
#include <ShiftRegister74HC595.h>

using namespace std;

const int buttonPins[9] = {34, 35, 32, 33, 25, 26, 27, 14, 12};
bool buttonStates[9];

const int difficultySwithPin = 13;
bool difficultySwithState;

const ShiftRegister74HC595<1> redLeds(23, 19, 21);
const ShiftRegister74HC595<1> greenLeds(5, 15, 21);

const int redLed8Pin = 4;
const int greenLed8Pin = 2;

void setup() {
  for(int pin : buttonPins) {
    pinMode(pin, INPUT);
  }

  pinMode(difficultySwithPin, INPUT);

  pinMode(redLed8Pin, OUTPUT);
  pinMode(greenLed8Pin, OUTPUT);
}

void loop() {
  for(int i = 0; i < 9; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);
  }

  difficultySwithState = digitalRead(difficultySwithPin);
  
  delay(500);
}
