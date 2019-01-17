#ifndef TIME_H
#define TIME_H

#include "String.h"

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
#define MILLIS_PER_SECOND 1000
#define SECONDS_PER_DAY 86400
#define MILLIS_PER_DAY 86400000

#define DEFAULT_ALARM_TIME_MILLIS 25200000
class Time
{
private:
    unsigned long timeInMillis;
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int prevTickCount;
 
public:
    Time(unsigned long timeInMillis);

    void setMillis(unsigned long millis);
    void setHours(unsigned int hours);
    void setMinutes(unsigned int minutes);
 
    int getHours() { return hours; }
    int getMinutes() {return minutes; }
    int getSeconds() { return seconds; }
    

    void updateTime(unsigned long millis);
};

#endif
