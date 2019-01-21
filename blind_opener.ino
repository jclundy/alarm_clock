// Arduino Libraries:
#include <LiquidCrystal.h>

// Custom Libraries
#include "Time.h"
#include "button.h"

typedef enum {
  DEFAULT_MODE,
  SET_TIME_MODE,
  SET_ALARM_MODE,
  ALARM_MODE
} clock_mode_t;

typedef enum {
  SELECT_SECONDS,
  SELECT_MINUTES,
  SELECT_HOURS
} select_increment_t;

// LCD interface pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Clock button pins
const int SET_CLK_PIN     = 22,
          SELECT_INCREMENT_PIN   = 23,
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
select_increment_t increment_selection = SELECT_HOURS;

// Global Variables
Time time = Time(0);
Time alarm_time = Time(DEFAULT_ALARM_TIME_MILLIS);

#define DEBOUNCE_COUNT 10

Button setTimeButton = Button(SET_CLK_PIN, DEBOUNCE_COUNT);
Button selectIncrementButton = Button(SELECT_INCREMENT_PIN, DEBOUNCE_COUNT);
Button hourIncrementButton = Button(HR_INC_PIN, DEBOUNCE_COUNT);
Button hourDecrementButton = Button(HR_DEC_PIN, DEBOUNCE_COUNT);
Button minuteIncrementButton = Button(MIN_INC_PIN, DEBOUNCE_COUNT);
Button minuteDecrementButton = Button(MIN_DEC_PIN, DEBOUNCE_COUNT);
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  pinMode(SET_CLK_PIN, INPUT);
  pinMode(SELECT_INCREMENT_PIN, INPUT);
  pinMode(HR_INC_PIN, INPUT);
  pinMode(HR_DEC_PIN, INPUT);
  pinMode(MIN_INC_PIN, INPUT);
  pinMode(MIN_DEC_PIN, INPUT);
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
  //if(current_mode != SET_TIME_MODE) {
    time.updateTime(millis());  
  //}

  setTimeButton.updateButtonState(digitalRead(SET_CLK_PIN));
  selectIncrementButton.updateButtonState(digitalRead(SELECT_INCREMENT_PIN));
  hourIncrementButton.updateButtonState(digitalRead(HR_INC_PIN));
  hourDecrementButton.updateButtonState(digitalRead(HR_DEC_PIN));

  if(selectIncrementButton.isPushed()) {
    if(increment_selection == SELECT_HOURS) {
      increment_selection = SELECT_MINUTES;
    } else if(increment_selection == SELECT_MINUTES) {
      increment_selection = SELECT_SECONDS;
    } else if (increment_selection == SELECT_SECONDS) {
      increment_selection = SELECT_HOURS;
    }
  }

  if(setTimeButton.isPushed()) {
    if(current_mode == DEFAULT_MODE) {
      current_mode = SET_TIME_MODE;
    } else if(current_mode == SET_TIME_MODE) {
      current_mode = SET_ALARM_MODE;
    } else if (current_mode == SET_ALARM_MODE) {
      current_mode = DEFAULT_MODE;
    }
  }

  if(current_mode != previous_mode) {
    previous_mode = current_mode;
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

  } 
  else if(current_mode == SET_ALARM_MODE) {
      if(hourIncrementButton.isPushed()) {
        switch(increment_selection) {
          case SELECT_SECONDS:
            alarm_time.addSecond();
            break;
          case SELECT_MINUTES:
            alarm_time.addMinute();
            break;
          case SELECT_HOURS:
            alarm_time.addHour();
            break;
          default:
            break;
        }
      } else if(hourDecrementButton.isPushed()) {
        switch(increment_selection) {
          case SELECT_SECONDS:
            alarm_time.subtractSecond();
            break;
          case SELECT_MINUTES:
            alarm_time.subtractMinute();
            break;
          case SELECT_HOURS:
            alarm_time.subtractHour();
            break;
          default:
            break;
        }
      }


      String textString = String("Set Alarm");
      lcd.setCursor(0, 0);
      lcd.print(textString);
      String alarmString = String("Alarm: ") + getTimeString(alarm_time);
      lcd.setCursor(0, 1);
      lcd.print(alarmString);
  }
  else if(current_mode == SET_TIME_MODE) {
            if(hourIncrementButton.isPushed()) {
        switch(increment_selection) {
          case SELECT_SECONDS:
            time.addSecond();
            break;
          case SELECT_MINUTES:
            time.addMinute();
            break;
          case SELECT_HOURS:
            time.addHour();
            break;
          default:
            break;
        }
      } else if(hourDecrementButton.isPushed()) {
        switch(increment_selection) {
          case SELECT_SECONDS:
            time.subtractSecond();
            break;
          case SELECT_MINUTES:
            time.subtractMinute();
            break;
          case SELECT_HOURS:
            time.subtractHour();
            break;
          default:
            break;
        }
      }
      String textString = String("Set Time");
      lcd.setCursor(0, 0);
      lcd.print(textString);
      String timeString = String("Time: ") + getTimeString(time);
      // print the number of seconds since reset:
      lcd.setCursor(0, 1);
      lcd.print(timeString);
  }  
  delay(10);
}
