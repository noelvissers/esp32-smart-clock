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

char _weather_city_name[64] = "";
char _weather_country_code[3] = "";
char _weather_endpoint[64] = "http://api.openweathermap.org/data/2.5/weather?q=";
char _weather_api_key[64] = ""; //e01213e3d481a166153ab05e2af5aa76

void CSettings::initPinModes()
{
  pinMode(_pinButtonPlus, INPUT);
  pinMode(_pinButtonSelect, INPUT);
  pinMode(_pinButtonMin, INPUT);
  pinMode(_pinStatusLed, OUTPUT);
}

bool saveToFlash(const char *dataToStore, int startAddr)
{
  int i = 0;
  for (; i < strlen(dataToStore); i++)
  {
    EEPROM.write(startAddr + i, dataToStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  return true;
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

bool CSettings::saveSettings()
{
  Serial.println("[Settings] Saving...");
  //Save all settings
  Serial.println("[Settings] Saving done.");
  return EEPROM.commit();
}

bool CSettings::loadSettingsWeather()
{
  Serial.println("[Weather settings] Loading...");
  if (strlen(_weather_city_name) == 0)
  {
    //load city name
  }
  printf("City name = %s\n", _weather_city_name);
  if (strlen(_weather_country_code) == 0)
  {
    //load coutry code
  }
  printf("Country code = %s\n", _weather_country_code);
  if (strlen(_weather_api_key) == 0)
  {
    //load key
  }
  printf("API key = %s\n", _weather_api_key);
  Serial.println("[Weather settings] Loading done.");
  return true;
}

bool CSettings::loadSettings()
{
  Serial.println("[General settings] Loading...");
  Serial.println("[General settings] Loading done.");
  return true;
}