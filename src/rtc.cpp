#include "rtc.h"
#include "RTClib.h"

RTC_DS3231 Time;

uint16_t _timeYear;
uint8_t _timeMonth;
uint8_t _timeDay;
uint8_t _timeHour;
uint8_t _timeMinute;
uint8_t _timeSecond;

uint8_t _timeDayOfWeek;
bool _timeDst = 0;

bool checkDST()
{
  return true;
}

void CRtc::update() //Copy RTC values to time data
{
  DateTime now = Time.now();

  _timeYear = now.year();
  _timeMonth = now.month();
  _timeDay = now.day();
  _timeDayOfWeek = now.dayOfTheWeek();
  _timeHour = now.hour();
  _timeMinute = now.minute();
  _timeSecond = now.second();

  if (checkDST())
  {
    //Check with online
  }
}

void CRtc::setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
  Time.adjust(DateTime(year, month, day, hour, minute, second));
}

bool CRtc::init()
{
  if (!Time.begin())
  {
    if (Time.lostPower())
    {
      Serial.println("[I][Rtc] RTC lost power. Setting time with online time on next check.");
    }
    return true;
  }
  else
  {
    Serial.println("[E][Rtc] Could not find RTC.");
  }
  return false;
}
