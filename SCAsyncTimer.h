#ifndef SCAsyncTimer_h
#define SCAsyncTimer_h

#include "Arduino.h"

enum TimeUnit {
  UNIT_MILLISECONDS,
  UNIT_SECONDS,
  UNIT_MINUTES,
  UNIT_HOURS
};

class SCAsyncTimer
{
  public:
    SCAsyncTimer(unsigned long intervall, TimeUnit unit);
    bool isTriggered();
    void isActive(bool active);
    void setIntervall(unsigned long intervall, TimeUnit unit);
    unsigned long getTimeLeft();
    void start();
    void stop_timer();
    bool is_on();
    TimeUnit timeUnit;

  private:
    unsigned long _intervall;
    unsigned long _lastEvent;
    bool _active;
};

#endif
