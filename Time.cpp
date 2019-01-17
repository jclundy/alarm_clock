#include "Time.h"

Time::Time(unsigned long millis)
{
	prevTickCount = 0;
	setMillis(millis);
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
	hours = new_hour;
	timeInMillis += hours * SECONDS_PER_HOUR * MILLIS_PER_SECOND;
}

void Time::setMinutes(unsigned int new_minutes) {
	minutes = new_minutes;
	timeInMillis += minutes * SECONDS_PER_MINUTE * MILLIS_PER_SECOND;
}

void Time::updateTime(unsigned long newTickCount) {
	int ticksElapsed = newTickCount - prevTickCount;
	setMillis(timeInMillis + ticksElapsed);
  prevTickCount = newTickCount;
}
