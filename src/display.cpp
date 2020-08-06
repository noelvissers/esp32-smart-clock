#include <Arduino.h>
#include "display.h"
#include "config.h"
#include "weather.h"
#include "rtc.h"
#include "ldr.h"

CRtc RtcDisplay;
CLdr Ldr;

unsigned int _state = 0;
unsigned int brightness = 0; //0..15

void updateBrightness()
{
  if (_autoBrightness)
  {
    brightness = Ldr.read();
  }
  //Display.setbrightness(brightness);
}

void CDisplay::brightnessUp()
{
  Serial.println("[Display] Turning up brightness...");
  if (_autoBrightness)
  {
    Serial.println("[Display] Turning off auto brightness...");
  _autoBrightness = false;
  }
  if (brightness < 15)
  {
    brightness++;
    updateBrightness();
  }
}

void CDisplay::brightnessDown()
{
  Serial.println("[Display] Turning down brightness...");
  if (_autoBrightness)
  {
    Serial.println("[Display] Turning off auto brightness...");
    _autoBrightness = false;
  }
  if (brightness > 0)
  {
    brightness--;
    updateBrightness();
  }
}

void CDisplay::showTime()
{
  //48 easter egg
  printf("[Display] Showing time... %u:%02u\n", _timeHour, _timeMinute);
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showDate()
{
  printf("[Display] Showing date (dd/mm: %02u/%02u)...\n", _timeDay, _timeMonth);
  printf("[Display] Showing date (mm/dd: %02u/%02u)...\n", _timeMonth, _timeDay);
  //if not showing brightness
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showTemperature()
{
  printf("[Display] Showing temperature (%i °C)...\n", int((_temperature - 273.15) + 0.5));
  printf("[Display] Showing temperature (%i °F)...\n", int(((_temperature - 273.15) * 1.8) + 32.5));
  //if not showing brightness
  updateBrightness();
  //Check for error (temp < 0)
}

void CDisplay::showHumidity()
{
  printf("[Display] Showing humidity (%u %%)...\n", _humidity);
  //if not showing brightness
  updateBrightness();
  //check for error (humidity < 0)
}

void CDisplay::showTimeBin()
{
  printf("[Display] Showing binairy time... %u:%02u\n", _timeHour, _timeMinute);
  //if not showing brightness
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showBrightness()
{
  printf("[Display] Showing brightness... %u:%02u\n", _timeHour, _timeMinute);
  //show for 3 sec
  //Set timer when it started
  updateBrightness();
}

void CDisplay::showBrightnessAuto()
{
  printf("[Display] Showing auto brightness setting... %u:%02u\n", _timeHour, _timeMinute);
  //show for 3 sec
  //Set timer when it started
  updateBrightness();
}