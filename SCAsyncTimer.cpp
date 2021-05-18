#include "Arduino.h"
#include "SCAsyncTimer.h"

SCAsyncTimer::SCAsyncTimer(unsigned long intervall, TimeUnit unit)
{
  setIntervall(intervall, unit);
  _active = false;
  timeUnit = unit;
}

void SCAsyncTimer::setIntervall(unsigned long intervall, TimeUnit unit)
{
  if (unit == UNIT_MILLISECONDS)
  {
    _intervall = intervall;
  }
  if (unit == UNIT_SECONDS)
  {
    _intervall = intervall * 1000;
  }
  if (unit == UNIT_MINUTES)
  {
    _intervall = intervall * 60 * 1000;
  }
  if (unit == UNIT_HOURS)
  {
    _intervall = intervall * 60 * 60 * 1000;
  }
  _lastEvent = millis();
  _active = false;
}

bool SCAsyncTimer::isTriggered()
{
  if (_active)
  {
    if (millis() - _lastEvent >= _intervall)
    {
      _lastEvent = millis();
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void SCAsyncTimer::isActive(bool active)
{
  _lastEvent = millis();
  _active = active;
}
void SCAsyncTimer::start()
{
  _active = true;
  _lastEvent = millis();
}
void SCAsyncTimer::stop_timer()
{
  _active = false;
  _lastEvent = millis();
}
bool SCAsyncTimer::is_on()
{
  return _active ;
}

unsigned long SCAsyncTimer::getTimeLeft()
{
  unsigned long val = millis() - _lastEvent - _intervall;
  return val;
}
