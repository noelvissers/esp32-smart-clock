/**
 * @project       Smart Clock
 * @documentation https://github.com/noelvissers/esp32-smart-clock 
 * @author        noelvissers
 * @date          31/07/2020
 */

#include <Arduino.h>
#include "settings.h"
#include "display.h"
#include "network.h"

//create objects
TaskHandle_t TasksCore_0;

CSettings Settings;
CDisplay Display;
CNetwork Network;

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
  //start serial communication for debugging
  Serial.begin(115200);
  Serial.println("[Status] Initializing...");

  //init classes
  Settings.initPinModes();

  //attatch interrupts for buttons
  attachInterrupt(_pinButtonPlus, ISR_buttonPlus, FALLING);
  attachInterrupt(_pinButtonSelect, ISR_buttonSelect, FALLING);
  attachInterrupt(_pinButtonMin, ISR_buttonMin, FALLING);

  //TODO: Display.setBrightness(_brightness);
  Serial.println("[Status] Initializing [-][-][-]");

  //Setup network connection
  Network.resetSettings();
  if (!Network.autoConnect())
  {
    //Handle error
  }
  Serial.println("[Status] Initializing [X][-][-] - Network configuration done.");

  //Get Time
  /**
   * TODO:...
   */
  Serial.println("[Status] Initializing [X][X][-] - Syncing online time done.");

  //Get Weather
  /**
   * TODO:...
   */
  Serial.println("[Status] Initializing [X][X][X] - Weather data received.");

  //create thread in core 0
  xTaskCreatePinnedToCore(
      Core_0,
      "TasksCore_0",
      1000,
      NULL,
      1,
      &TasksCore_0,
      0);

  Serial.println("[Status] Initializing done.");
  delay(1000);
}

//main loop on core 1
void loop()
{
  /**
   * TODO: Menu (only update time when needed)
   */
}