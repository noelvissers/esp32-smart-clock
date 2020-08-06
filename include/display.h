#pragma once

extern unsigned int _state;

class CDisplay
{
public:
  void showTime();
  void showDate();
  void showTemperature();
  void showHumidity();
  void showTimeBin();
  void showBrightness();
  void brightnessUp();
  void brightnessDown();
  void showBrightnessAuto();
};