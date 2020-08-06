#pragma once

extern int _state;

class CDisplay
{
public:
  void showTime();
  void showDate();
  void showTemperature();
  void showHumidity();
  void showTimeBin();
  void updateBrightness();
  void showBrightness();
  void brightnessUp();
  void brightnessDown();
  void showBrightnessManual();
};