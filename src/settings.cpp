#include <Arduino.h>
#include "settings.h"
#include "EEPROM.h"

//Hardware settings:
unsigned int _pinButtonPlus = 12;
unsigned int _pinButtonSelect = 13;
unsigned int _pinButtonMin = 14;

unsigned int _pinDisplayMOSI = 36; //SPI_MOSI
unsigned int _pinDisplaySS = 22;   //SPI_SS
unsigned int _pinDisplaySCK = 35;  //SPI_SCK

unsigned int _pinStatusLed = 11;

unsigned int _pinRtcSCL = 39; //I2C_SCL
unsigned int _pinRtcSDA = 42; //I2C_SDA

unsigned int _pinLDR = 10; //AI

void CSettings::initPinModes()
{
  pinMode(_pinButtonPlus, INPUT);
  pinMode(_pinButtonSelect, INPUT);
  pinMode(_pinButtonMin, INPUT);
  pinMode(_pinStatusLed, OUTPUT);
}

void CSettings::saveSettings()
{
}

void CSettings::loadSettings()
{
}

bool saveToFlash(const char *dataToStore, int startAddr)
{
  int i = 0;
  for (; i < strlen(dataToStore); i++)
  {
    EEPROM.write(startAddr + i, dataToStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  return EEPROM.commit();
}

String readFromFlash(int startAddr)
{
  char dataArray[128];
  char dataChar;
  for (int i = 0; i < 128; i++)
  {
    dataChar = EEPROM.read(startAddr + i);
    dataArray[i] = dataChar;
    if (dataChar == '\0')
      break;
  }
  return String(dataArray);
}