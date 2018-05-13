/*
DailyTimer.h Library
BulldogLowell@gmail.com
April, 2016
** Library provides tools to set daily timers for control of devices such as lamps, appliances, etc. Developed primarilary for houshold presence simulation.
** Allows for setting ON and OFF times, days of the week (i.e. weekends, weekdays, Fridays) and the use of random on/off times using this Constructor:
   DailyTimer myTimer( START_HOUR, START_MINUTE, END_HOUR, END_MINUTE, DAYS_OF_WEEK, RANDOM or FIXED)

** Timers may bridge midnight, simply enter times accordingly:
   DailyTimer myTimer( 18, 30,  1, 45, WEEKENDS, FIXED);  // starts at 6:30pm Saturdays and Sundays and ends at 1:45am the next day.

** Automatically sets correct timer state on powerup, using isActive() in loop(), see example below.
** Random start and/or end times using this member function:
   myTimer.setRandomOffset(30, RANDOM_START);  //  Can be RANDOM_START, RANDOM_END, or both (RANDOM)  default random offfset is 15min

** Random days of week using this member function:
   myTimer.setRandomDays(4); // will select four days of the week, randomly.  Useful to run this member function once a week, for example.

** Select custom days of the week using this member function:
   myTimer.setDaysActive(0b10101010);  // e.g.Sunday, Tuesday, Thursday and Saturday Note: Days in this order:  0bSMTWTFS0 <- LSB is zero

** Set a timed event with just the start time as a trigger:
   myTimer.startTrigger();  // will return true when Start Time is passed if today is an active day. use it in loop()

** you can return the active days using this getter:
   byte myByte = myTimer.getDays();  // returns a byte... Sunday is the MSB, Saturday is the LSB << 1

** Dynamically set your start or end time (i.e. using some type of celestial function or web call to determine Sunrise or Sunset times)
   myTimer.setStartTime(byte hour, byte minute);
   myTimer.setEndTime(byte hour, byte minute);

*/
//*****************************************************************************************************************************************


#ifndef DailyTimerSpark_h
#define DailyTimerSpark_h

#include "application.h"


enum EventDays{SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, WEEKENDS, WEEKDAYS, EVERY_DAY};

enum RandomTime{FIXED, RANDOM, RANDOM_START, RANDOM_END};

struct TimerTime{ // bounded 00:00 to 23:59
  uint8_t hour;
  uint8_t minute;
};

const byte dayTemplate[10] = {
  /*SMTWTFSS*/                    // the mask is set with an extra bit for determining off times for days of the week where off time is earlier than on time (i.e. stradles Midnight)
  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b10000010, // Weekends
  0b01111100, // Weekdays
  0b11111110  // Everyday
};

class DailyTimer{
  public:
    DailyTimer(byte StartHour, byte StartMinute, byte endHour, byte endMinute, EventDays DayOfWeek);
    DailyTimer(byte startHour, byte startMinute, byte endHour, byte endMinute, EventDays DayOfWeek, RandomTime randomTime);
    DailyTimer(byte startHour, byte startMinute, byte endHour, byte endMinute, EventDays DayOfWeek, RandomTime randomTime, uint32_t (*randomCallback)());
    void setRandomOffset(int random_minutes, RandomTime randomSetting = RANDOM);
    void setDaysActive(EventDays days);
    void setDaysActive(byte activeDays);
    byte setRandomDays(byte number_Days);
    bool isActive();
    bool startTrigger();
    bool endTrigger();
    bool begin();
    void setStartTime(byte hour, byte minute);
    void setEndTime(byte hour, byte minute);
    byte getDays();

  private:
    byte _onMask  = dayTemplate[EVERY_DAY];
    byte _offMask = dayTemplate[EVERY_DAY];
    bool _state;
    TimerTime _startTime;
    TimerTime _endTime;
    TimerTime _randomStartTime;
    TimerTime _randomEndTime;
    byte _randomType = FIXED;
    byte _currentDay = 0;
    uint8_t _offset = 15;
    time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss);
    uint32_t (*_randomCallback)();
};

#endif
