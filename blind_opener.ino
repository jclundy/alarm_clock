// Arduino Libraries:
#include <LiquidCrystal.h>

// custom definitions
#include "Time.h"
typedef enum {
  DEFAULT_MODE,
  SET_TIME_MODE,
  SET_ALARM_MODE,
  ALARM_MODE
} clock_mode_t;

// LCD interface pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Clock button pins
const int SET_CLK_PIN     = 22,
          SET_ALARM_PIN   = 23,
          HR_INC_PIN      = 24,
          HR_DEC_PIN      = 25,
          MIN_INC_PIN     = 26,
          MIN_DEC_PIN     = 27;

// Stepper motor pins
const int MOTOR_STEP_PIN            = 30,
          MOTOR_DIR_PIN             = 31,
          MOTOR_LIMIT_SWITCH_PIN_1  = 32,
          MOTOR_LIMIT_SWITCH_PIN_2  = 33;

clock_mode_t current_mode = DEFAULT_MODE;
clock_mode_t previous_mode = DEFAULT_MODE;

Time time = Time(0);
Time alarm_time = Time(DEFAULT_ALARM_TIME_MILLIS);
bool setTimePinState = LOW;
bool prevSetTimePinState = LOW;
bool setTimeTransitionOccured = false;
int setTimeButtonStateCounter = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("starting up");
  pinMode(SET_CLK_PIN, INPUT);
}

String formatTimeValue(unsigned int timeValue) {
  String numberString = String(timeValue);
  if(timeValue < 10) {
    return String("0") + numberString;
  } else {
    return numberString;
  }
}

String getTimeString(Time time) {
  
  return formatTimeValue(time.getHours()) + String(":") + formatTimeValue(time.getMinutes()) + ":" + formatTimeValue(time.getSeconds());
}

void loop() {
  time.updateTime(millis());

  if(digitalRead(SET_CLK_PIN) == LOW) {
    setTimeButtonStateCounter ++;
    if(setTimeButtonStateCounter > 10) {
      prevSetTimePinState = setTimePinState;
      setTimePinState = LOW;
      setTimeButtonStateCounter = 0;  
    }
  } else {
    setTimeButtonStateCounter--;
    if(setTimeButtonStateCounter <= 0) {
      prevSetTimePinState = setTimePinState;
      setTimePinState = HIGH;
      setTimeButtonStateCounter = 0;
    }
  }
  if(setTimePinState == LOW) {
    current_mode = SET_TIME_MODE; 
  } else {
    current_mode = DEFAULT_MODE;
  }

  if(current_mode != previous_mode) {
    lcd.clear();
  }
  if (current_mode == DEFAULT_MODE) {
      String timeString = String("Time: ") + getTimeString(time);
      // print the number of seconds since reset:
      lcd.setCursor(0, 0);
      lcd.print(timeString);
    
      String alarmString = String("Alarm: ") + getTimeString(alarm_time);
      lcd.setCursor(0, 1);
      lcd.print(alarmString);

  } else if(current_mode == SET_TIME_MODE) {
      String textString = String("Set Time");
      lcd.setCursor(0, 0);
      lcd.print(textString);
      String timeString = String("Time: ") + getTimeString(time);
      // print the number of seconds since reset:
      lcd.setCursor(0, 1);
      lcd.print(timeString);
  }
  
   previous_mode = current_mode;
  
  delay(10);
}