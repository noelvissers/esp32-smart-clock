#include <Arduino.h>
#include "display.h"
#include "config.h"
#include "weather.h"
#include "rtc.h"
#include "ldr.h"
#include "LedControl_HW_SPI.h"

LedControl_HW_SPI lc = LedControl_HW_SPI();
CRtc RtcDisplay;
CLdr Ldr;

unsigned int _state = 0;
unsigned int brightness = 0; //0..15
unsigned long brightnessDisplayTime = 0;
unsigned long AutoBrightneDisplassyTime = 0;

const char digits[10][3] = {{0b01111100, 0b01000100, 0b01111100},            //0
                            {0b00000000, 0b00000000, 0b01111100},            //1
                            {0b01110100, 0b01010100, 0b01011100},            //2
                            {0b01000100, 0b01010100, 0b01111100},            //3
                            {0b00011100, 0b00010000, 0b01111100},            //4
                            {0b01011100, 0b01010100, 0b01110100},            //5
                            {0b01111100, 0b01010100, 0b01110100},            //6
                            {0b00000100, 0b00000100, 0b01111100},            //7
                            {0b01111100, 0b01010100, 0b01111100},            //8
                            {0b00011100, 0b00010100, 0b01111100}};           //9
const char digitsBin[10] = {0b00000000,                                      //b0
                            0b10000000,                                      //b1
                            0b01000000,                                      //b2
                            0b11000000,                                      //b3
                            0b00100000,                                      //b4
                            0b10100000,                                      //b5
                            0b01100000,                                      //b6
                            0b11100000,                                      //b7
                            0b00010000,                                      //b8
                            0b10010000};                                     //b9
char charOne[1] = {0b01111100};                                              //1
char charMinus[2] = {0b00010000, 0b00010000};                                //- (2px)
char charUnknown[3] = {0b00010000, 0b00010000, 0b00010000};                  //- (3px)
char charDegree[1] = {0b00000100};                                           //°
char charCelcius[2] = {0b00011100, 0b00010100};                              //C
char charFahrenheit[2] = {0b00111100, 0b00010100};                           //F
char charPercentage[4] = {0b01001000, 0b00100000, 0b00010000, 0b01001000};   //%
char charLoadingInit[4] = {0b00011000, 0b00100100, 0b00100100, 0b00011000};  //[ ] //also used as reset
char charLoadingDone[4] = {0b00011000, 0b00111100, 0b00111100, 0b00011000};  //[X]
char charLoadingError[4] = {0b00000000, 0b00011000, 0b00011000, 0b00000000}; // X
char charBrightnessFilled[1] = {0b00111100};                                 //---
char charBrightnessEmpty[1] = {0b00100100};                                  //===
char charA[3] = {0b01110000, 0b00101000, 0b01110000};                        //A
char charU[3] = {0b01110000, 0b01000000, 0b01110000};                        //U
char charT[2] = {0b01111100, 0b00101000};                                    //T
char charO[3] = {0b01110000, 0b00101000, 0b01110000};                        //O

char charSloth[8] = {0b00111100, 0b01011010, 0b10001011, 0b10100011, 0b10100011, 0b10001011, 0b01011010, 0b00111100}; //Slothface

void printDigit(unsigned int startAddr, unsigned int startRow, unsigned int digit)
{
  int address = startAddr;
  int row = startRow;

  for (int i = 0; i < 3; i++)
  {
    lc.setRow(address, row, digits[digit][i]);

    row++;
    if (row == 8)
    {
      row = 0;
      address++;
    }
  }
}

void printChar(unsigned int startAddr, unsigned int startRow, char data[], unsigned int length)
{
  int address = startAddr;
  int row = startRow;

  for (int i = 0; i < length; i++)
  {
    lc.setRow(address, row, data[i]);

    row++;
    if (row == 8)
    {
      address++;
      row = 0;
    }
  }
}

void updateBrightness()
{
  if (_autoBrightness)
  {
    brightness = Ldr.read();
  }
  //Display.setbrightness(brightness);
}

void CDisplay::init()
{
  lc.begin(_pinDisplaySS, 2);
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.setIntensity(0, 1);
  lc.setIntensity(1, 1);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
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
    //printf("[Display] Showing time... %u:%02u\n", _timeHour, _timeMinute);
    RtcDisplay.update();

    if (!_use24h && (_timeHour > 12))
    {
      _timeHour = _timeHour - 12;
    }

    lc.clearDisplay(0);
    lc.clearDisplay(1);

    if ((_timeHour / 10 % 10) != 0)
    {
      printDigit(0, 0, _timeHour / 10 % 10);
    }
    printDigit(0, 4, _timeHour % 10);

    if (_timeMinute != 48)
    {
      printDigit(1, 1, _timeMinute / 10 % 10);
      printDigit(1, 5, _timeMinute % 10);
    }
    else
    {
      printChar(1, 0, charSloth, 8);
    }
  }
}

void CDisplay::showDate()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    //printf("[Display] Showing date (dd/mm: %02u/%02u)...\n", _timeDay, _timeMonth);
    //printf("[Display] Showing date (mm/dd: %02u/%02u)...\n", _timeMonth, _timeDay);
    RtcDisplay.update();

    lc.clearDisplay(0);
    lc.clearDisplay(1);

    if (_useDdmm) //Day - Month
    {
      if ((_timeDay / 10 % 10) != 0)
        printDigit(0, 0, _timeDay / 10 % 10);
      else
        printDigit(0, 0, 0);

      printDigit(0, 4, _timeDay % 10);
      printChar(1, 0, charMinus, sizeof(charMinus));

      if ((_timeMonth / 10 % 10) != 0)
      {
        printChar(1, 3, charOne, sizeof(charOne));
        printDigit(1, 5, _timeMonth % 10);
      }
      else
        printDigit(1, 3, _timeMonth % 10);
    }
    else //Month - Day
    {
      if ((_timeMonth / 10 % 10) != 0)
        printChar(0, 0, charOne, sizeof(charOne));
      printDigit(0, 2, _timeMonth % 10);

      printChar(0, 6, charMinus, sizeof(charMinus));

      if ((_timeDay / 10 % 10) != 0)
        printDigit(1, 1, _timeDay / 10 % 10);
      else
        printDigit(1, 1, 0);

      printDigit(1, 5, _timeDay % 10);
    }
  }
}

void CDisplay::showTemperature()
{
  updateBrightness();
  if (((millis() - 3000) > brightnessDisplayTime) && (millis() - 3000) > AutoBrightneDisplassyTime)
  {
    //printf("[Display] Showing temperature (%i °C)...\n", int((_temperature - 273.15) + 0.5));
    //printf("[Display] Showing temperature (%i °F)...\n", int(((_temperature - 273.15) * 1.8) + 32.5));

    lc.clearDisplay(0);
    lc.clearDisplay(1);

    if (_temperature < 0)
    {
      //Error
      printChar(0, 3, charUnknown, sizeof(charUnknown));
      printChar(0, 7, charUnknown, sizeof(charUnknown));
      printChar(1, 3, charDegree, sizeof(charDegree));
      if (_useCelcius)
        printChar(1, 5, charCelcius, sizeof(charCelcius));
      else
        printChar(1, 5, charFahrenheit, sizeof(charFahrenheit));
    }
    else
    {
      if (_useCelcius)
      {
        //Celcius
        int temperature = int((_temperature - 273.15) + 0.5);

        if (temperature <= -10)
        {
          printChar(0, 0, charMinus, sizeof(charMinus));
          printDigit(0, 3, (temperature * -1) / 10 % 10);
          printDigit(0, 7, (temperature * -1) % 10);
        }
        else if (temperature < 0)
        {
          printChar(0, 4, charMinus, sizeof(charMinus));
          printDigit(0, 7, (temperature * -1) % 10);
        }
        else
        {
          if ((temperature / 10 % 10) != 0)
          {
            printDigit(0, 3, temperature / 10 % 10);
          }
          printDigit(0, 7, temperature % 10);
        }
        printChar(1, 3, charDegree, sizeof(charDegree));
        printChar(1, 5, charCelcius, sizeof(charCelcius));
      }
      else
      {
        //Fahrenheit
        int temperature = int(((_temperature - 273.15) * 1.8) + 32.5);
      }
    }
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

//showStatus

void CDisplay::showAutoBrightness()
{
  Serial.println("[Display] Showing auto brightness setting...");
  //show 'auto'
  AutoBrightneDisplassyTime = millis();
  updateBrightness();
}