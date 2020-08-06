#include <Arduino.h>
#include "display.h"
#include "config.h"
#include "weather.h"
#include "rtc.h"

CRtc RtcDisplay;

int _state = 0;
int brightness = 0; //0..15

void updateBrightness()
{
#ifdef DEBUG
  Serial.println("[Display] Updating brightness");
#endif
  if (_autoBrightness)
  {
    //read LDR ...
    //scale to brightness level
    //brightness = x;
  }
  //Display.setbrightness(brightness);
}

void CDisplay::brightnessUp()
{
#ifdef DEBUG
  Serial.println("[Display] Turning up brightness...");
#endif
  if (_autoBrightness)
    _autoBrightness = false;
  if (brightness < 15)
  {
    brightness++;
    updateBrightness();
  }
}

void CDisplay::brightnessDown()
{
#ifdef DEBUG
  Serial.println("[Display] Turning down brightness...");
#endif
  if (_autoBrightness)
    _autoBrightness = false;
  if (brightness > 0)
  {
    brightness--;
    updateBrightness();
  }
}

void CDisplay::showTime()
{
//48 easter egg
#ifdef DEBUG
  printf("[Display] Showing time... %u:%02u\n", _timeHour, _timeMinute);
#endif
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showDate()
{
  //if not showing brightness
#ifdef DEBUG
  printf("[Display] Showing date (dd/mm: %02u/%02u)...\n", _timeDay, _timeMonth);
  printf("[Display] Showing date (mm/dd: %02u/%02u)...\n", _timeMonth, _timeDay);
#endif
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showTemperature()
{
  //if not showing brightness
#ifdef DEBUG
  printf("[Display] Showing temperature (%i °C)...\n", int((_temperature - 273.15) + 0.5));
  printf("[Display] Showing temperature (%i °F)...\n", int(((_temperature - 273.15) * 1.8) + 32.5));
#endif
  updateBrightness();
}

void CDisplay::showHumidity()
{
  //if not showing brightness
#ifdef DEBUG
  printf("[Display] Showing humidity (%u %%)...\n", _humidity);
#endif
  updateBrightness();
}

void CDisplay::showTimeBin()
{
  //if not showing brightness
#ifdef DEBUG
  printf("[Display] Showing binairy time... %u:%02u\n", _timeHour, _timeMinute);
#endif
  updateBrightness();
  RtcDisplay.update();
}

void CDisplay::showBrightness()
{
  //show for 3 sec
#ifdef DEBUG
  printf("[Display] Showing brightness... %u:%02u\n", _timeHour, _timeMinute);
#endif
  updateBrightness();
}

void CDisplay::showBrightnessManual()
{
  //show for 3 sec
#ifdef DEBUG
  printf("[Display] Showing manual brightness setting... %u:%02u\n", _timeHour, _timeMinute);
#endif
  updateBrightness();
}