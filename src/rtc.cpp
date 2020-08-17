#include "rtc.h"
#include "RTClib.h"

RTC_DS3231 RtcTime;

uint16_t _timeYear = 2020;
uint8_t _timeMonth = 1;
uint8_t _timeDay = 1;
uint8_t _timeHour = 0;
uint8_t _timeMinute = 0;
uint8_t _timeSecond = 0;

uint8_t _timeDayOfWeek = -1; //0 = sunday, 6 = saturday
bool _timeDst = false;

bool checkDST()
{
  //Check all possible DST conditions
  //Check this for < 3 seconds so it doesnt loop for an hour
  return true;
}

bool CRtc::update() //Copy RTC values to time data
{
  DateTime now = RtcTime.now();

  if ((now.month() > 12) || (now.day() > 31) || (now.hour() > 23) || (now.minute() > 59) || (now.second() > 59))
  {
    Serial.println("[E][Rtc] RTC returned invalid data:");
    printf("%04u/%02u/%02u - %02u:%02u:%02u\n", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    RtcTime.adjust(DateTime(_onlineTimeYear, _onlineTimeMonth, _onlineTimeDay, _onlineTimeHour, _onlineTimeMinute, _onlineTimeSecond));
    return false;
  }
  else
  {
    _timeYear = now.year();
    _timeMonth = now.month();
    _timeDay = now.day();
    _timeDayOfWeek = now.dayOfTheWeek();
    _timeHour = now.hour();
    _timeMinute = now.minute();
    _timeSecond = now.second();

    if (checkDST())
    {
      //Check if online time has different DST
      //Sync time
      //If local DST and online DST is same do nothing.
    }
    return true;
  }
  return false;
}

void CRtc::setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
  RtcTime.adjust(DateTime(year, month, day, hour, minute, second));
}

bool CRtc::init()
{
  if (RtcTime.begin())
  {
    if (RtcTime.lostPower())
    {
      Serial.println("[I][Rtc] RTC lost power. Setting time with online time on next check.");
    }
    return true;
  }
  else
    Serial.println("[E][Rtc] Could not find RTC.");
  return false;
}

bool CRtc::checkRtc()
{
  return RtcTime.begin();
}