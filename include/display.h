#pragma once

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
  void showAutoCycle();
  void showStatus(EStatus state_1, EStatus state_2, EStatus state_3);
  void showReset();
  void renderPong(int posBat, int posBall_X, int posBall_Y, int possWall, bool wall);
  void renderPongReset();
};