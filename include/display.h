#pragma once

extern unsigned int _state;

enum class EStatus
{
  Init,
  Error,
  Done
};

class CDisplay
{
public:
  void init();
  void showTime();
  void showDate();
  void showTemperature();
  void showHumidity();
  void showTimeBin();
  void showBrightness();
  void brightnessUp();
  void brightnessDown();
  void showAutoBrightness();
  void showStatus(EStatus state_1, EStatus state_2, EStatus state_3);
  void showReset();
};