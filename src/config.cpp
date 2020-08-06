#include <Arduino.h>
#include "config.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

//Hardware settings:
unsigned int _pinButtonPlus = 32;
unsigned int _pinButtonSelect = 33;
unsigned int _pinButtonMin = 25;

unsigned int _pinDisplayMOSI = 23; //SPI_MOSI
unsigned int _pinDisplaySS = 5;    //SPI_SS
unsigned int _pinDisplaySCK = 18;  //SPI_SCK

unsigned int _pinStatusLed = 35;

//unsigned int _pinRtcSCL = 22; //SCL, Already auto defined in rtc lib
//unsigned int _pinRtcSDA = 21; //SDA, Already auto defined in rtc lib

unsigned int _pinLDR = 34; //AI

//General settings:
bool _autoBrightness = true;
bool _autoCycle = false;
char _timeFormat[4] = "24H";      //try to get this as option between 24H and 12H so no false strings can be put there.
char _dateFormat[5] = "ddmm";     //try to get this as option between 'Day/Month' and 'Month/Day' so no false strings can be put there.
char _temperatureFormat[2] = "C"; //try to get this as option between 'C' and 'F' so no false strings can be put there.

//Weather settings:
char _weatherCityName[64] = "";
char _weatherCountryCode[3] = "";
char _weatherEndpoint[64] = "http://api.openweathermap.org/data/2.5/weather?q=";
char _weatherApiKey[64] = "";

//Time settings:
char _timeEndpoint[64] = "http://worldtimeapi.org/api/ip";

//Set pinmodes for IO
void CConfig::initPinModes()
{
  pinMode(_pinButtonPlus, INPUT);
  pinMode(_pinButtonSelect, INPUT);
  pinMode(_pinButtonMin, INPUT);
  pinMode(_pinStatusLed, OUTPUT);
}

//Save all settings to memory
bool CConfig::saveSettings()
{
#ifdef DEBUG
  Serial.println("[Config] Saving...");
#endif

  if (SPIFFS.begin(true)) //Start SPIFFS
  {
    File fSettings = SPIFFS.open("/config.json", FILE_WRITE); //Open file in write mode

    DynamicJsonDocument doc(1024); //Create temp. document to store string data in from SPIFFS

    //Save settings
    doc["weather"]["weatherCityName"] = _weatherCityName;
    doc["weather"]["weatherCountryCode"] = _weatherCountryCode;
    doc["weather"]["weatherApiKey"] = _weatherApiKey;

    serializeJson(doc, fSettings); //Convert doc objects to strings and put them in fSettings file
    fSettings.close();             //Close file

#ifdef DEBUG
    Serial.println("[Config] Saving done.");
#endif
    return true;
  }
  else
  {
    Serial.println("[E][Config] An error occurred while mounting SPIFFS.");
  }
  return false;
}

//Load all settings from memory
bool CConfig::loadSettings()
{
  Serial.println("[Config] Loading...");

  if (SPIFFS.begin(true)) //Start SPIFFS
  {
    if (SPIFFS.exists("/config.json")) //Check if file exsists on drive
    {
      File fSettings = SPIFFS.open("/config.json", FILE_READ); //Open file in read only mode

      DynamicJsonDocument doc(1024);                                //Make temp. document to store string data in from SPIFFS
      DeserializationError error = deserializeJson(doc, fSettings); //Convert strings from fSettings to objects and put them in doc
      if (!error)                                                   //Chech if no errors occured while converting to objects
      {
        //Load all settings, so that in network config the saved values are showed. If they're empty it means they need to be filled in.
        if (doc["weather"]["weatherCityName"])
        {
          strcpy(_weatherCityName, doc["weather"]["weatherCityName"]);
        }
        if (doc["weather"]["weatherCountryCode"])
        {
          strcpy(_weatherCountryCode, doc["weather"]["weatherCountryCode"]);
        }
        if (doc["weather"]["weatherApiKey"])
        {
          strcpy(_weatherApiKey, doc["weather"]["weatherApiKey"]);
        }

        fSettings.close(); //Close file
        Serial.println("[Config] Loading done.");
        return true;
      }
      else
      {
        Serial.println("[E][Config] A deserialization error occurred:");
        Serial.println(error.c_str());
      }
    }
    else
    {
      Serial.println("[E][Config] File does not exist.");
    }
  }
  else
  {
    Serial.println("[E][Config] An error occurred while mounting SPIFFS.");
  }
  return false;
}

//Clear memory so no more settings are saved
void CConfig::formatSettings()
{
  if (!SPIFFS.format())
  {
    Serial.println("[E][Config] An error occurred while formatting SPIFFS.");
  }
  // Serial.println("[Config] Restarting ESP32...");
  //ESP.restart();
}