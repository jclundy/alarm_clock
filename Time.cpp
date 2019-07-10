#include "Time.h"

Time::Time(long millis)
{
	prevTickCount = 0;
	setMillis(millis);
}

void Time::updateMillis() {
  timeInMillis = seconds * 1000 + minutes * 1000 * 60 + hours * 1000 * 60 * 60;
}

void Time::setMillis(unsigned long millis) {
  timeInMillis = millis;
	timeInMillis %= MILLIS_PER_DAY;
  unsigned long total_seconds = timeInMillis / 1000;
  unsigned long total_minutes = total_seconds / 60;
  unsigned long total_hours = total_minutes / 60;
  hours = total_hours % 24;
  minutes = total_minutes % 60;
  seconds = total_seconds % 60;
}

void Time::incrementMillis(unsigned long millis) {
  milliSeconds += millis;
  if(milliSeconds >= 1000) {
    seconds++;
  }
  if(seconds > 59) {
    minutes ++;
  }
  if(minutes > 59) {
    hours ++;
  }
  milliSeconds %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
}

void Time::setHours(unsigned int new_hour) {
	hours = new_hour % 24;
}

void Time::setSeconds(unsigned int new_seconds) {
  seconds = new_seconds % 60;
  updateMillis();
}

void Time::setMinutes(unsigned int new_minutes) {
  minutes = new_minutes % 60;
  updateMillis();
}

void Time::addHour() {
  hours += 1;
  hours %=24;
  updateMillis();
}

void Time::subtractHour() {
  hours -= 1;
  if(hours < 0) {
    hours += 24;
  }
  hours %=24;
  updateMillis();
}

void Time::addMinute() {
  minutes += 1;
  minutes %=60;
  updateMillis();
}

void Time::subtractMinute() {
  minutes -= 1;
  if(minutes < 0) {
    minutes += 60;
  }
  minutes %=60;
  updateMillis();
}

void Time::addSecond() {
  seconds += 1;
  seconds %=60;
  updateMillis();
}

void Time::subtractSecond() {
  seconds -= 1;
  if(seconds < 0) {
    seconds += 60;
  }
  seconds %=60;
  updateMillis();
}

void Time::updateTime(unsigned long newTickCount) {
	long ticksElapsed = newTickCount - prevTickCount;
  if(ticksElapsed < 0) {
    ticksElapsed *= -1;
  }
	incrementMillis(ticksElapsed);
  prevTickCount = newTickCount;
}

long getDifferenceInSeconds(Time time1, Time time2) {
  long time1_in_seconds = time1.getSeconds() + time1.getMinutes() * 60 + time1.getHours() * 3600;
  long time2_in_seconds = time2.getSeconds() + time2.getMinutes() * 60 + time2.getHours() * 3600;
  
  return time1_in_seconds - time2_in_seconds;
}

