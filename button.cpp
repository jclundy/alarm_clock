#include "button.h"
#include "Arduino.h"

bool Button::transitionOccured() {
  return buttonState != prevButtonState;
}

void Button::updateButtonState() {
  if(digitalRead(buttonPin) == LOW) {
    counter++;
    if(counter > counterUpperLimit) {
      prevButtonState = buttonState;
      buttonState = LOW;
      counter = 0;  
    }
  } else {
    counter--;
    if(counter <= counterLowerLimit) {
      prevButtonState = buttonState;
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
