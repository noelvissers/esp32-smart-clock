#pragma once

//Hardware settings:
extern unsigned int _pinButtonPlus;
extern unsigned int _pinButtonSelect;
extern unsigned int _pinButtonMin;

extern unsigned int _pinDisplayMOSI;
extern unsigned int _pinDisplaySS;
extern unsigned int _pinDisplaySCK;

extern unsigned int _pinStatusLed;

//extern unsigned int _pinRtcSCL; //Already auto defined in rtc lib
//extern unsigned int _pinRtcSDA; //Already auto defined in rtc lib

extern unsigned int _pinLDR;

//General settings:
extern bool _autoBrightness;
extern bool _autoCycle;
extern char _timeFormat[4];
extern char _dateFormat[5];
extern char _temperatureFormat[2];

//Weather settings:
extern char _weatherCityName[64];
extern char _weatherCountryCode[3];
extern char _weatherEndpoint[64];
extern char _weatherApiKey[64];

//Time settings:
extern char _timeEndpoint[64];

//Buttons:
extern bool _buttonPlusPressed;
extern unsigned long _lastButtonPlusPress;
extern bool _buttonSelectPressed;
extern unsigned long _lastButtonSelectPress;
extern bool _buttonMinPressed;
extern unsigned long _lastButtonMinPress;

class CConfig
{
public:
  void initPinModes();
  bool saveSettings();
  bool loadSettingsWeather();
  bool loadSettings();
  void formatSettings();
};