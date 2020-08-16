#pragma once
#include <stdint.h>

//Hardware settings:
extern uint8_t _pinButtonPlus;
extern uint8_t _pinButtonSelect;
extern uint8_t _pinButtonMin;

extern uint8_t _pinDisplayMOSI; //Already auto defined in lc lib
extern uint8_t _pinDisplaySCK;  //Already auto defined in lc lib
extern uint8_t _pinDisplaySS;

extern uint8_t _pinStatusLed;

extern uint8_t _pinRtcSCL; //Already auto defined in rtc lib
extern uint8_t _pinRtcSDA; //Already auto defined in rtc lib

extern uint8_t _pinLDR;

//General settings:
extern bool _autoBrightness;
extern bool _autoCycle;
extern bool _use24h;
extern bool _useDdmm;
extern bool _useCelcius;

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