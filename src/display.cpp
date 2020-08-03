#include <Arduino.h>
#include "display.h"
#include "config.h"
#include "weather.h"
#include "rtc.h"

CRtc RtcDisplay;

int _state = 0;

void CDisplay::updateBrightness()
{
  //If !autoBrightness
  Serial.println("[Display] Updating brightness");
  //read LDR ...
}

void CDisplay::showTime()
{
  //48 easter egg
  RtcDisplay.update();
  printf("[Display] Show time... %u:%02u\n", _timeHour, _timeMinute);
}

void CDisplay::showDate()
{
  RtcDisplay.update();
  printf("[Display] Show date (dd/mm: %02u/%02u)...\n", _timeDay, _timeMonth);
  printf("[Display] Show date (mm/dd: %02u/%02u)...\n", _timeMonth, _timeDay);
}

void CDisplay::showTemperature()
{
  printf("[Display] Show temperature (%i °C)...\n", int((_temperature - 273.15) + 0.5));
  printf("[Display] Show temperature (%i °F)...\n", int(((_temperature - 273.15) * 1.8) + 32.5));
}

void CDisplay::showHumidity()
{
  printf("[Display] Show humidity (%u %%)...\n", _humidity);
}

void CDisplay::showTimeBin()
{
  RtcDisplay.update();
  printf("[Display] Show binairy time... %u:%02u\n", _timeHour, _timeMinute);
}