#pragma once

#include <stdint.h>

extern uint16_t _timeYear;
extern uint8_t _timeMonth;
extern uint8_t _timeDay;
extern uint8_t _timeHour;
extern uint8_t _timeMinute; 
extern uint8_t _timeSecond;

extern uint8_t _timeDayOfWeek;

class CRtc
{
public:
  bool init();
  bool update();
  void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
  bool checkRtc();
};