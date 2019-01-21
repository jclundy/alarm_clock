#ifndef TIME_H
#define TIME_H

#include "String.h"

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
#define MILLIS_PER_SECOND 1000
#define MILLIS_PER_MINUTE (SECONDS_PER_MINUTE * MILLIS_PER_SECOND)
#define MILLIS_PER_HOUR (SECONDS_PER_HOUR * MILLIS_PER_SECOND)
#define SECONDS_PER_DAY 86400
#define MILLIS_PER_DAY  86400000

#define DEFAULT_ALARM_TIME_MILLIS 25200000
class Time
{
private:
    long timeInMillis;
    int seconds;
    int minutes;
    int hours;
    unsigned long milliSeconds;
    unsigned long prevTickCount;

    void updateMillis();

 
public:
    Time(long timeInMillis);

    void setMillis(unsigned long millis);
    void setHours(unsigned int hours);
    void setMinutes(unsigned int minutes);

    void incrementMillis(unsigned long millis);
 
    int getHours() { return hours; }
    int getMinutes() {return minutes; }
    int getSeconds() { return seconds; }
    void addHour();
    void subtractHour();
    void addMinute();
    void subtractMinute();
    void addSecond();
    void subtractSecond();
    
    void updateTime(unsigned long millis);
};

#endif
