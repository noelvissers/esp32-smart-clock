#pragma once

#include <stdint.h>

extern uint16_t _onlineTimeYear;
extern uint8_t _onlineTimeMonth;
extern uint8_t _onlineTimeDay;
extern uint8_t _onlineTimeHour;
extern uint8_t _onlineTimeMinute;
extern uint8_t _onlineTimeSecond;

extern bool _onlineSync;

extern long _onlineTimeUnix;
extern int _onlineDayOfWeek;
extern bool _onlineDst;

class CTime
{
public:
  bool update();
};