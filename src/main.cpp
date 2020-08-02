/**
 * @project       Smart Clock
 * @documentation https://github.com/noelvissers/esp32-smart-clock 
 */

#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "network.h"
#include "weather.h"
#include "rtc.h"
#include "time.h"

//create objects
TaskHandle_t TasksCore_0;
CConfig Config;
CDisplay Display;
CNetwork Network;
CWeather Weather;
CRtc Rtc;
CTime Time;

//main loop on core 0 (network functions)
void Core_0(void *parameter)
{
  for (;;)
  {
    //Update online time/date/weather/etc...
    /**
     * 
     */
    delay(150000); //Do an update every 2.5 minutes.
  }
}

//ISRs
void IRAM_ATTR ISR_buttonPlus()
{
  //Button handler - Plus
  /**
   * 
   */
}

void IRAM_ATTR ISR_buttonSelect()
{
  //Button handler - Select
  /**
   * 
   */
}

void IRAM_ATTR ISR_buttonMin()
{
  //Button handler - Min
  /**
   * 
   */
}

void setup()
{
  Network.resetSettings();
  //start serial communication for debugging
  Serial.begin(115200);
  Serial.println("[Status] Initializing...");

  //init classes
  Rtc.init();
  Config.initPinModes();

  //attatch interrupts for buttons
  attachInterrupt(_pinButtonPlus, ISR_buttonPlus, FALLING);
  attachInterrupt(_pinButtonSelect, ISR_buttonSelect, FALLING);
  attachInterrupt(_pinButtonMin, ISR_buttonMin, FALLING);

  Config.loadSettings();
  Serial.println("[Status] Initializing [-][-][-]");

  //Setup network connection
  if (!Network.autoConnect())
  {
    //Handle error
  }
  Serial.println("[Status] Initializing [X][-][-] - Network configuration done.");

  //Get Time
  Rtc.update();
  if(!Time.update())
  {
    //Handle error
  }
  Serial.println("[Status] Initializing [X][X][-] - Syncing online time done.");

  //Get Weather
  if (!Weather.update())
  {
    //Handle error
  }
  Serial.println("[Status] Initializing [X][X][X] - Weather data received.");

  //Create thread in core 0
  xTaskCreatePinnedToCore(
      Core_0,
      "TasksCore_0",
      1000,
      NULL,
      1,
      &TasksCore_0,
      0);

  Config.saveSettings();
  Serial.println("[Status] Initializing done.");
  delay(1000);
}

//main loop on core 1
void loop()
{
  /**
   * TODO: Menu (only update time when needed)
   */
  delay(50);
}