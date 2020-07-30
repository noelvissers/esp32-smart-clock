#pragma once

//Smart Clock settings:
extern int _brightness;

//Hardware settings:
extern unsigned int _pinButtonPlus;
extern unsigned int _pinButtonSelect;
extern unsigned int _pinButtonMin;

extern unsigned int _pinDisplayMOSI; //SPI_MOSI
extern unsigned int _pinDisplaySS; //SPI_SS
extern unsigned int _pinDisplaySCK; //SPI_SCK

extern unsigned int _pinStatusLed;

extern unsigned int _pinRtcSCL; //I2C_SCL
extern unsigned int _pinRtcSDA; //I2C_SDA

extern unsigned int _pinLDR; //AI

class CSettings
{
    public:
    void initPinModes();
};