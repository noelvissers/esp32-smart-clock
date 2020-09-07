#include <Arduino.h>
#include "config.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

//Hardware settings:
uint8_t _pinButtonPlus = 32;
uint8_t _pinButtonSelect = 33;
uint8_t _pinButtonMin = 25;

uint8_t _pinDisplayMOSI = 23; //SPI_MOSI, Already auto defined in lc lib
uint8_t _pinDisplaySCK = 18;  //SPI_SCK, Already auto defined in lc lib
uint8_t _pinDisplaySS = 5;    //SPI_SS

uint8_t _pinStatusLed = 35; //Not used

uint8_t _pinRtcSCL = 22; //SCL, Already auto defined in rtc lib
uint8_t _pinRtcSDA = 21; //SDA, Already auto defined in rtc lib

uint8_t _pinLDR = 34; //Analog input

//General settings:
bool _autoBrightness = true;
bool _autoCycle = false;
bool _use24h = true;
bool _useDdmm = true;
bool _useCelcius = true;
unsigned long _autoCycleTime = 10000; //Time it takes to cycle to new view (ms)

//Button states:
unsigned long _lastButtonPlusPress = 0; //Last button press millis()
bool _buttonPlusPressed = false;        //Current state of button (resets on release)
bool _buttonPlusSet = false;            //Toggled state of button (resets when button action is done / manually)
unsigned long _lastButtonSelectPress = 0;
bool _buttonSelectPressed = false;
bool _buttonSelectSet = false;
unsigned long _lastButtonMinPress = 0;
bool _buttonMinPressed = false;
bool _buttonMinSet = false;

//Weather settings:
char _weatherCityName[64] = "";
char _weatherCountryCode[3] = "";
char _weatherEndpoint[64] = "http://api.openweathermap.org/data/2.5/weather?q=";
char _weatherApiKey[64] = "";

//Time settings:
char _timeEndpoint[64] = "http://worldtimeapi.org/api/ip";

//Display settings:
uint16_t _ldrLowerLimit = 0;   //0 min ADC/LDR (making this value higher means the display reaches its min brightness sooner)
uint16_t _ldrUpperLimt = 4095; //4095 max ADC/LDR (making this value higher, lowers the max brightness | making this value lower means the display reaches its max brightness sooner)
int _ldrError = 125;           //Schmidt trigger threshold

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
  Serial.println("[Config] Saving...");

  if (SPIFFS.begin(true)) //Start SPIFFS
  {
    File fSettings = SPIFFS.open("/config.json", FILE_WRITE); //Open file in write mode

    DynamicJsonDocument doc(1024); //Create temp. document to store string data in from SPIFFS

    //Save settings
    //General settings
    doc["general"]["autoBrightness"] = _autoBrightness;
    doc["general"]["autoCycle"] = _autoCycle;
    doc["general"]["useDdmmFormat"] = _useDdmm;
    doc["general"]["use24hFormat"] = _use24h;
    doc["general"]["useCelciusUnit"] = _useCelcius;

    //Weather settings
    doc["weather"]["weatherCityName"] = _weatherCityName;
    doc["weather"]["weatherCountryCode"] = _weatherCountryCode;
    doc["weather"]["weatherApiKey"] = _weatherApiKey;

    serializeJson(doc, fSettings); //Convert doc objects to strings and put them in fSettings file
    fSettings.close();             //Close file

    Serial.println("[Config] Saving done.");
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
        //General settings
        if (doc["general"]["autoBrightness"])
          _autoBrightness = doc["general"]["autoBrightness"];
        else
          _autoBrightness = false;

        if (doc["general"]["autoCycle"])
          _autoCycle = doc["general"]["autoCycle"];
        else
          _autoCycle = false;

        if (doc["general"]["useDdmmFormat"])
          _useDdmm = doc["general"]["useDdmmFormat"];
        else
          _useDdmm = false;

        if (doc["general"]["use24hFormat"])
          _use24h = doc["general"]["use24hFormat"];
        else
          _use24h = false;

        if (doc["general"]["useCelciusUnit"])
          _useCelcius = doc["general"]["useCelciusUnit"];
        else
          _useCelcius = false;

        //Weather API settings
        if (doc["weather"]["weatherCityName"])
          strcpy(_weatherCityName, doc["weather"]["weatherCityName"]);
        if (doc["weather"]["weatherCountryCode"])
          strcpy(_weatherCountryCode, doc["weather"]["weatherCountryCode"]);
        if (doc["weather"]["weatherApiKey"])
          strcpy(_weatherApiKey, doc["weather"]["weatherApiKey"]);

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
  Serial.println("[Config] SPIFFS formatted.");
}