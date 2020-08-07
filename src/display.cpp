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
unsigned long brightnessDisplayTime = 0;
unsigned long AutoBrightneDisplassyTime = 0;

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
  showBrightness();
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
  showBrightness();
}

void CDisplay::showTime()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    printf("[Display] Showing time... %u:%02u\n", _timeHour, _timeMinute);
    RtcDisplay.update();
    //show time disp
    //48 easter egg
  }
}

void CDisplay::showDate()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    printf("[Display] Showing date (dd/mm: %02u/%02u)...\n", _timeDay, _timeMonth);
    printf("[Display] Showing date (mm/dd: %02u/%02u)...\n", _timeMonth, _timeDay);
    RtcDisplay.update();
    //show date disp
    //check format
  }
}

void CDisplay::showTemperature()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    printf("[Display] Showing temperature (%i °C)...\n", int((_temperature - 273.15) + 0.5));
    printf("[Display] Showing temperature (%i °F)...\n", int(((_temperature - 273.15) * 1.8) + 32.5));
    //Check for error (temp < 0)
    //show temp disp
    //check unit C/F
  }
}

void CDisplay::showHumidity()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    printf("[Display] Showing humidity (%u %%)...\n", _humidity);
    //check for error (humidity < 0)
    //show humidity disp
  }
}

void CDisplay::showTimeBin()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    printf("[Display] Showing binairy time... %u:%02u\n", _timeHour, _timeMinute);
    RtcDisplay.update();
    //show time disp
    //write manual that bin time is always 24h
  }
}

void CDisplay::showBrightness()
{
  Serial.println("[Display] Showing brightness...");
  //show xxxxx------ etc
  brightnessDisplayTime = millis();
  updateBrightness();
}

void CDisplay::showAutoBrightness()
{
  Serial.println("[Display] Showing auto brightness setting...");
  //show 'auto'
  AutoBrightneDisplassyTime = millis();
  updateBrightness();
}