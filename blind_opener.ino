// Arduino Libraries:
#include <LiquidCrystal.h>
// Hardware Libraries
#include "DS1307.h"
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

#define select_hours_cursor_position 8
#define select_minutes_cursor_position 11
#define select_seconds_cursor_position 14
// LCD interface pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Clock button pins
const int SET_MODE_PIN     = 22,
          SELECT_INCREMENT_PIN   = 23,
          INCREMENT_PIN      = 24,
          DECREMENT_PIN      = 25;

// Stepper motor pins
const int MOTOR_STEP_PIN            = 30,
          MOTOR_DIR_PIN             = 31,
          MOTOR_LIMIT_SWITCH_PIN_1  = 32,
          MOTOR_LIMIT_SWITCH_PIN_2  = 33;

clock_mode_t current_mode = DEFAULT_MODE;
clock_mode_t previous_mode = DEFAULT_MODE;
select_increment_t increment_selection = SELECT_HOURS;
unsigned int cursor_position = select_hours_cursor_position;
// Global Variables
Time time = Time(0);
int rtc[7];
Time alarm_time = Time(DEFAULT_ALARM_TIME_MILLIS);

#define DEBOUNCE_COUNT 4
#define USE_RTC

Button setModeButton = Button(SET_MODE_PIN, DEBOUNCE_COUNT);
Button selectIncrementButton = Button(SELECT_INCREMENT_PIN, DEBOUNCE_COUNT);
Button incrementButton = Button(INCREMENT_PIN, DEBOUNCE_COUNT);
Button decrementButton = Button(DECREMENT_PIN, DEBOUNCE_COUNT);
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  pinMode(SET_MODE_PIN, INPUT);
  pinMode(SELECT_INCREMENT_PIN, INPUT);
  pinMode(INCREMENT_PIN, INPUT);
  pinMode(DECREMENT_PIN, INPUT);

#ifdef USE_RTC
  // RTC code
  DDRC|=_BV(2) |_BV(3);  // POWER:Vcc Gnd
  PORTC |=_BV(3);  // VCC PINC3
  RTC.SetOutput(DS1307_SQW32KHZ);
  RTC.start();
#endif
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
  if(!((current_mode == SET_TIME_MODE) && (increment_selection == SELECT_SECONDS))) {
    #ifdef USE_RTC
      RTC.get(rtc,true);
      int seconds = rtc[0];
      int minutes = rtc[1];
      int hours = rtc[2];

      time.setSeconds(seconds);
      time.setMinutes(minutes);
      time.setHours(hours);
    #else
      time.updateTime(millis());
    #endif
  }

  setModeButton.updateButtonState(digitalRead(SET_MODE_PIN));
  selectIncrementButton.updateButtonState(digitalRead(SELECT_INCREMENT_PIN));
  incrementButton.updateButtonState(digitalRead(INCREMENT_PIN));
  decrementButton.updateButtonState(digitalRead(DECREMENT_PIN));

  if(selectIncrementButton.isPushed()) {
    if(increment_selection == SELECT_HOURS) {
      increment_selection = SELECT_MINUTES;
      cursor_position = select_minutes_cursor_position;
    } else if(increment_selection == SELECT_MINUTES) {
      increment_selection = SELECT_SECONDS;
      cursor_position = select_seconds_cursor_position;
    } else if (increment_selection == SELECT_SECONDS) {
      increment_selection = SELECT_HOURS;
      cursor_position = select_hours_cursor_position;
    }
  }

  if(setModeButton.isPushed()) {
    if(current_mode == DEFAULT_MODE) {
      current_mode = SET_TIME_MODE;
      increment_selection = SELECT_HOURS;
      cursor_position = select_hours_cursor_position;
      #ifdef USE_RTC
      RTC.stop();
      #endif
    } else if(current_mode == SET_TIME_MODE) {
      current_mode = SET_ALARM_MODE;
      increment_selection = SELECT_HOURS;
      cursor_position = select_hours_cursor_position;
      #ifdef USE_RTC
      RTC.set(DS1307_SEC,time.getSeconds());
      RTC.set(DS1307_MIN,time.getMinutes());
      RTC.set(DS1307_HR,time.getHours());
      RTC.start();
      #endif
    } else if (current_mode == SET_ALARM_MODE) {
      current_mode = DEFAULT_MODE;
      lcd.noBlink();
    }
  }

  if(current_mode != previous_mode) {
    previous_mode = current_mode;
    lcd.clear();
  }
  if (current_mode == DEFAULT_MODE) {
      String timeString = String("Time : ") + getTimeString(time);
      // print the number of seconds since reset:
      lcd.setCursor(0, 0);
      lcd.print(timeString);
    
      String alarmString = String("Alarm: ") + getTimeString(alarm_time);
      lcd.setCursor(0, 1);
      lcd.print(alarmString);

  } 
  else if(current_mode == SET_ALARM_MODE) {
      if(incrementButton.isPushed()) {
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
      } else if(decrementButton.isPushed()) {
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
      lcd.setCursor(cursor_position,1);
      //lcd.cursor();
      lcd.blink();
  }
  else if(current_mode == SET_TIME_MODE) {
            if(incrementButton.isPushed()) {
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
      } else if(decrementButton.isPushed()) {
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
      String timeString = String("Time : ") + getTimeString(time);
      // print the number of seconds since reset:
      lcd.setCursor(0, 1);
      lcd.print(timeString);
      lcd.setCursor(cursor_position,1);
      //lcd.cursor();
      lcd.blink();
  }  
  delay(10);
}
