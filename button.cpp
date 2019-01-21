#include "button.h"
#include "Arduino.h"

bool Button::transitionOccured() {
  return buttonState != prevButtonState;
}

void Button::updateButtonState(bool pinRead) {
  prevButtonState = buttonState;
  if(pinRead == LOW) {
    counter++;
    if(counter > counterUpperLimit) {
      buttonState = LOW;
      counter = 0;  
    }
  } else {
    counter--;
    if(counter <= counterLowerLimit) {
      buttonState = HIGH;
      counter = 0;
    }
  }
}

bool Button::getButtonState() {
  return buttonState;
}

bool Button::isPushed() {
  return transitionOccured() && buttonState == LOW;
}

bool Button::isReleased() {
  return transitionOccured() && buttonState == HIGH;
}

Button::Button(unsigned int buttonPin, int counterLimit) {
  buttonState = HIGH;
  prevButtonState = HIGH;
  counter = 0;
  counterUpperLimit = counterLimit;
  counterLowerLimit = -counterLimit;
}
