#include <Arduino.h>
#include "settings.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

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

char _weatherCityName[64] = "";
char _weatherCountryCode[3] = "";
char _weatherEndpoint[64] = "http://api.openweathermap.org/data/2.5/weather?q=";
char _weatherApiKey[64] = "";

void CSettings::initPinModes()
{
  pinMode(_pinButtonPlus, INPUT);
  pinMode(_pinButtonSelect, INPUT);
  pinMode(_pinButtonMin, INPUT);
  pinMode(_pinStatusLed, OUTPUT);
}

bool CSettings::saveSettings()
{
  Serial.println("[Settings] Saving...");

  if (SPIFFS.begin(true))
  {
    File fSettings = SPIFFS.open("/config.json", FILE_WRITE);

    if (!fSettings)
    {
      DynamicJsonDocument doc(1024);
      doc["general"]["brightness"] = 1; //migrate to top of file (so its accessable)
      doc["general"]["autoBrightness"] = true;
      doc["general"]["autoCycle"] = false;
      doc["general"]["timeFormat"] = "24h";
      doc["general"]["dateFormat"] = "ddmm";

      doc["weather"]["weatherCityName"] = _weatherCityName;
      doc["weather"]["weatherCountryCode"] = _weatherCountryCode;
      doc["weather"]["weatherApiKey"] = _weatherApiKey;

      doc["time"]["timeApiKey"] = "";

      serializeJson(doc, Serial);    //print
      serializeJson(doc, fSettings); //print to file

      fSettings.close();

      Serial.println("[Settings] Saving done.");
      return true;
    }
    else
    {
      Serial.println("[E][Settings] An error occurred while opening the file.");
    }
  }
  else
  {
    Serial.println("[E][Settings] An error occurred while mounting SPIFFS.");
  }
  return false;
}

bool CSettings::loadSettings()
{
  Serial.println("[Settings] Loading...");

  if (SPIFFS.begin(true))
  {
    if (SPIFFS.exists("/config.json"))
    {
      File fSettings = SPIFFS.open("/config.json", FILE_READ);

      if (fSettings)
      {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, fSettings);
        if (!error)
        {
          strcpy(_weatherCityName, doc["weather"]["weatherCityName"]);
          strcpy(_weatherCountryCode, doc["weather"]["weatherCountryCode"]);
          strcpy(_weatherApiKey, doc["weather"]["weatherApiKey"]);
          strcpy(_weatherCityName, doc["weather"]["weatherCityName"]);

          fSettings.close();
          Serial.println("[Settings] Loading done.");
          return true;
        }
        else
        {
          Serial.println("[E][Settings] A deserialization error occurred:");
          Serial.println(error.c_str());
        }
      }
      else
      {
        Serial.println("[E][Settings] An error occurred while opening the file.");
      }
    }
    else
    {
      Serial.println("[E][Settings] File does not exist.");
    }
  }
  else
  {
    Serial.println("[E][Settings] An error occurred while mounting SPIFFS.");
  }
  return false;
}

void CSettings::formatSettings()
{
  if (!SPIFFS.format())
  {
    Serial.println("[E][Settings] An error occurred while formatting SPIFFS.");
  }
  Serial.println("[Settings] Restarting ESP32...");
  ESP.restart();
}