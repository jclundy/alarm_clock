#include "Time.h"

Time::Time(unsigned long millis)
{
	prevTickCount = 0;
	setMillis(millis);
}

void Time::updateMillis() {
  timeInMillis = timeInMillis % 1000 + seconds * 1000 + minutes * 1000 * 60 + hours * 1000 * 60 * 24;
}

void Time::setMillis(unsigned long millis) {
	timeInMillis = millis;
	timeInMillis %= MILLIS_PER_DAY;
	int timeInSeconds = timeInMillis / 1000;
	int timeInMinutes = timeInSeconds / 60;
	int timeInHours = timeInMinutes / 60;
	seconds = timeInSeconds % 60;
	minutes = timeInMinutes % 60;
	hours = timeInHours % 24;
}

void Time::setHours(unsigned int new_hour) {
	hours = new_hour % 24;
	updateMillis();
}

void Time::addHour() {
  hours += 1;
  hours %= 24;
  updateMillis();
}

void Time::subtractHour() {
  hours --;
  hours = (hours + 24) % 24;
  updateMillis();
}

void Time::addMinute() {
  hours += 1;
  minutes %= 60;
  updateMillis();
}

void Time::subtractMinute() {
  minutes --;
  minutes = (minutes + 60) % 60;
  updateMillis();
}
void Time::setMinutes(unsigned int new_minutes) {
	minutes = new_minutes % 60;
	updateMillis();
}

void Time::updateTime(unsigned long newTickCount) {
	int ticksElapsed = newTickCount - prevTickCount;
	setMillis(timeInMillis + ticksElapsed);
  prevTickCount = newTickCount;
}
