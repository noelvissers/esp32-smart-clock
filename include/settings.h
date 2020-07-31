#pragma once

//Hardware settings:
extern unsigned int _pinButtonPlus;
extern unsigned int _pinButtonSelect;
extern unsigned int _pinButtonMin;

extern unsigned int _pinDisplayMOSI; //SPI_MOSI
extern unsigned int _pinDisplaySS;   //SPI_SS
extern unsigned int _pinDisplaySCK;  //SPI_SCK

extern unsigned int _pinStatusLed;

extern unsigned int _pinRtcSCL; //I2C_SCL
extern unsigned int _pinRtcSDA; //I2C_SDA

extern unsigned int _pinLDR; //AI

//Weather API settings
extern char _weather_city_name[64];
extern char _weather_country_code[3];
extern char _weather_endpoint[64];
extern char _weather_api_key[64];

class CSettings
{
public:
  void initPinModes();
  void saveToFlash(const char *dataToStore, int startAddr);
  void saveSettings();
  void loadSettings();
};