#include <Arduino.h>
#include "display.h"
#include "config.h"
#include "weather.h"
#include "rtc.h"
#include "ldr.h"
#include "LedControl_HW_SPI.h"
//#include "LedControl_SW_SPI.h"

LedControl_HW_SPI lc = LedControl_HW_SPI();
//LedControl_SW_SPI lc = LedControl_SW_SPI();
CRtc RtcDisplay;
CLdr Ldr;
CConfig ConfigDisplay;

unsigned int brightness = 0; //0..15
unsigned long displayDelayTime = 0;

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
char charA[3] = {0b01110000, 0b00101000, 0b01110000};                        //A
char charU[3] = {0b01110000, 0b01000000, 0b01110000};                        //U
char charT[2] = {0b01111100, 0b01010000};                                    //T
char charO[3] = {0b01110000, 0b01010000, 0b01110000};                        //O

char charBrightness[6] = {0b01000010, 0b00011000, 0b00100100, 0b00100100, 0b00011000, 0b01000010};                    //Sun
char charCycle[8] = {0b00111000, 0b01000100, 0b10000010, 0b10000010, 0b10000010, 0b01010000, 0b00110000, 0b01110000}; //↻
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
  lc.setIntensity(0, brightness);
  lc.setIntensity(1, brightness);
}

void CDisplay::init()
{
  lc.begin(_pinDisplaySS, 2);
  //lc.begin(_pinDisplayMOSI, _pinDisplaySCK, _pinDisplaySS, 2);
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.setIntensity(0, 1);
  lc.setIntensity(1, 1);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
}

void CDisplay::brightnessUp()
{
  //Serial.println("[Display] Turning up brightness...");
  if (_autoBrightness)
  {
    Serial.println("[Display] Turning off auto brightness...");
    _autoBrightness = false;
    ConfigDisplay.saveSettings();
  }
  if (brightness < 15)
  {
    brightness++;
    updateBrightness();
  }
  showBrightness();
  printf("[Display] Bightness: %u/15\n", brightness);
}

void CDisplay::brightnessDown()
{
  //Serial.println("[Display] Turning down brightness...");
  if (_autoBrightness)
  {
    Serial.println("[Display] Turning off auto brightness...");
    _autoBrightness = false;
    ConfigDisplay.saveSettings();
  }
  if (brightness > 0)
  {
    brightness--;
    updateBrightness();
  }
  showBrightness();
  printf("[Display] Bightness: %u/15\n", brightness);
}

void CDisplay::showTime()
{
  updateBrightness();
  if ((millis() - 3000) > displayDelayTime)
  {
    if (!_onlineSync)
      RtcDisplay.update();

    uint8_t timeHour = _timeHour;

    if (!_use24h && (timeHour > 12))
    {
      timeHour = timeHour - 12;
    }

    lc.clearDisplay(0);
    lc.clearDisplay(1);

    //if ((timeHour / 10 % 10) != 0) //Hide leading 0
    {
      printDigit(0, 0, timeHour / 10 % 10);
    }
    printDigit(0, 4, timeHour % 10);

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
  if ((millis() - 3000) > displayDelayTime)
  {
    if (!_onlineSync)
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
  if ((millis() - 3000) > displayDelayTime)
  {
    lc.clearDisplay(0);
    lc.clearDisplay(1);

    if (_temperature < 0)
    {
      //Error
      printChar(0, 3, charUnknown, sizeof(charUnknown));
      printChar(0, 7, charUnknown, sizeof(charUnknown));
    }
    else
    {
      int temperature;
      //Calculate temperature from Kelvin
      if (_useCelcius)
        temperature = int((_temperature - 273.15) + 0.5);
      else
        temperature = int(((_temperature - 273.15) * 1.8) + 32.5);

      //Check if temperature is below 9
      if (temperature <= -10)
      {
        printChar(0, 0, charMinus, sizeof(charMinus));
        printDigit(0, 3, (temperature * -1) / 10 % 10);
        printDigit(0, 7, (temperature * -1) % 10);
      }
      //Check if temperature is below 0
      else if (temperature < 0)
      {
        printChar(0, 4, charMinus, sizeof(charMinus));
        printDigit(0, 7, (temperature * -1) % 10);
      }
      //Check if temperature is above 100
      else if (temperature >= 100)
        printChar(0, 1, charOne, sizeof(charOne));
      //check if temperature is over 9
      if (temperature >= 10)
        printDigit(0, 3, temperature / 10 % 10);
      printDigit(0, 7, temperature % 10);
    }
    //Display unit
    printChar(1, 3, charDegree, sizeof(charDegree));
    if (_useCelcius)
      printChar(1, 5, charCelcius, sizeof(charCelcius));
    else
      printChar(1, 5, charFahrenheit, sizeof(charFahrenheit));
  }
}

void CDisplay::showHumidity()
{
  updateBrightness();
  if ((millis() - 3000) > displayDelayTime)
  {
    lc.clearDisplay(0);
    lc.clearDisplay(1);

    if ((_humidity < 0) || (_humidity > 100))
    {
      printChar(0, 2, charUnknown, sizeof(charUnknown));
      printChar(0, 6, charUnknown, sizeof(charUnknown));
    }
    else
    {
      if (_humidity > 99)
        printChar(0, 0, charOne, sizeof(charOne));
      if (_humidity > 10)
        printDigit(0, 2, _humidity / 10 % 10);
      printDigit(0, 6, _humidity % 10);
    }
    printChar(1, 3, charPercentage, sizeof(charPercentage));
  }
}

void CDisplay::showTimeBin()
{
  updateBrightness();
  if ((millis() - 3000) > displayDelayTime)
  {
    if (!_onlineSync)
      RtcDisplay.update();

    lc.clearDisplay(0);
    lc.clearDisplay(1);

    lc.setRow(0, 0, (digitsBin[_timeHour / 10 % 10] | 0b00000001));
    lc.setRow(0, 1, (digitsBin[_timeHour % 10] | 0b00000001));

    lc.setRow(0, 3, (digitsBin[_timeMinute / 10 % 10] | 0b00000001));
    lc.setRow(0, 4, (digitsBin[_timeMinute % 10] | 0b00000001));

    lc.setRow(0, 6, (digitsBin[_timeSecond / 10 % 10] | 0b00000001));
    lc.setRow(0, 7, (digitsBin[_timeSecond % 10] | 0b00000001));

    if (!_useDdmm)
    {
      lc.setRow(1, 3, (digitsBin[_timeMonth / 10 % 10] | 0b00000001));
      lc.setRow(1, 4, (digitsBin[_timeMonth % 10] | 0b00000001));

      lc.setRow(1, 6, (digitsBin[_timeDay / 10 % 10] | 0b00000001));
      lc.setRow(1, 7, (digitsBin[_timeDay % 10] | 0b00000001));
    }
    else
    {
      lc.setRow(1, 3, (digitsBin[_timeDay / 10 % 10] | 0b00000001));
      lc.setRow(1, 4, (digitsBin[_timeDay % 10] | 0b00000001));

      lc.setRow(1, 6, (digitsBin[_timeMonth / 10 % 10] | 0b00000001));
      lc.setRow(1, 7, (digitsBin[_timeMonth % 10] | 0b00000001));
    }
  }
  //write manual that bin time is always 24h
}

void CDisplay::showBrightness()
{
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  printChar(0, 5, charBrightness, sizeof(charBrightness));

  if (brightness == 0)
  {
    lc.setRow(0, 0, 0b00000000);
    lc.setRow(1, 7, 0b00000000);
  }
  else if (brightness == 1 || brightness == 2)
  {
    lc.setRow(0, 0, 0b10000000);
    lc.setRow(1, 7, 0b10000000);
  }
  else if (brightness == 3 || brightness == 4)
  {
    lc.setRow(0, 0, 0b11000000);
    lc.setRow(1, 7, 0b11000000);
  }
  else if (brightness == 5 || brightness == 6)
  {
    lc.setRow(0, 0, 0b11100000);
    lc.setRow(1, 7, 0b11100000);
  }
  else if (brightness == 7 || brightness == 8)
  {
    lc.setRow(0, 0, 0b11110000);
    lc.setRow(1, 7, 0b11110000);
  }
  else if (brightness == 9 || brightness == 10)
  {
    lc.setRow(0, 0, 0b11111000);
    lc.setRow(1, 7, 0b11111000);
  }
  else if (brightness == 11 || brightness == 12)
  {
    lc.setRow(0, 0, 0b11111100);
    lc.setRow(1, 7, 0b11111100);
  }
  else if (brightness == 13 || brightness == 14)
  {
    lc.setRow(0, 0, 0b11111110);
    lc.setRow(1, 7, 0b11111110);
  }
  else if (brightness == 15)
  {
    lc.setRow(0, 0, 0b11111111);
    lc.setRow(1, 7, 0b11111111);
  }

  displayDelayTime = millis();
  updateBrightness();
}

void CDisplay::showAutoBrightness()
{
  Serial.println("[Display] Showing auto brightness setting...");
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  printChar(0, 1, charA, sizeof(charA));
  printChar(0, 5, charU, sizeof(charU));
  printChar(1, 1, charT, sizeof(charT));
  printChar(1, 4, charO, sizeof(charO));
  displayDelayTime = millis();
  updateBrightness();
}

void CDisplay::showAutoCycle()
{
  Serial.println("[Display] Showing auto cycle setting...");
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  printChar(0, 4, charCycle, sizeof(charCycle));
  displayDelayTime = millis();
  updateBrightness();
}

void CDisplay::showStatus(EStatus state_1, EStatus state_2, EStatus state_3)
{
  //Print state 1
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  switch (state_1)
  {
  case EStatus::Init:
    printChar(0, 0, charLoadingInit, sizeof(charLoadingInit));
    break;
  case EStatus::Done:
    printChar(0, 0, charLoadingDone, sizeof(charLoadingDone));
    break;
  default: //Error
    printChar(0, 0, charLoadingError, sizeof(charLoadingError));
    break;
  }
  //Print state 2
  switch (state_2)
  {
  case EStatus::Init:
    printChar(0, 6, charLoadingInit, sizeof(charLoadingInit));
    break;
  case EStatus::Done:
    printChar(0, 6, charLoadingDone, sizeof(charLoadingDone));
    break;
  default: //Error
    printChar(0, 6, charLoadingError, sizeof(charLoadingError));
    break;
  }
  //Print state 3
  switch (state_3)
  {
  case EStatus::Init:
    printChar(1, 4, charLoadingInit, sizeof(charLoadingInit));
    break;
  case EStatus::Done:
    printChar(1, 4, charLoadingDone, sizeof(charLoadingDone));
    break;
  default: //Error
    printChar(1, 4, charLoadingError, sizeof(charLoadingError));
    break;
  }
}

void CDisplay::showReset()
{
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  printChar(0, 6, charLoadingInit, sizeof(charLoadingInit));
}