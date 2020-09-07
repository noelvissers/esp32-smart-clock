#pragma once
#include <stdint.h>

//Hardware settings:
extern uint8_t _pinButtonPlus;
extern uint8_t _pinButtonSelect;
extern uint8_t _pinButtonMin;

extern uint8_t _pinDisplayMOSI;
extern uint8_t _pinDisplaySCK;
extern uint8_t _pinDisplaySS;

extern uint8_t _pinStatusLed;

extern uint8_t _pinRtcSCL;
extern uint8_t _pinRtcSDA;

extern uint8_t _pinLDR;

//General settings:
extern bool _autoBrightness;
extern bool _autoCycle;
extern bool _use24h;
extern bool _useDdmm;
extern bool _useCelcius;
extern unsigned long _autoCycleTime;

//Button states:
extern unsigned long _lastButtonPlusPress;
extern bool _buttonPlusPressed;
extern bool _buttonPlusSet;
extern unsigned long _lastButtonSelectPress;
extern bool _buttonSelectPressed;
extern bool _buttonSelectSet;
extern unsigned long _lastButtonMinPress;
extern bool _buttonMinPressed;
extern bool _buttonMinSet;

//Weather settings:
extern char _weatherCityName[64];
extern char _weatherCountryCode[3];
extern char _weatherEndpoint[64];
extern char _weatherApiKey[64];

//Time settings:
extern char _timeEndpoint[64];

//Display settings:
extern uint16_t _ldrLowerLimit;
extern uint16_t _ldrUpperLimt;
extern int _ldrError;

class CConfig
{
public:
  void initPinModes();
  bool saveSettings();
  bool loadSettingsWeather();
  bool loadSettings();
  void formatSettings();
};