#pragma once

extern float _temperature;
extern int _humidity;
extern int _pressure;

class CWeather
{
public:
  bool update();
};