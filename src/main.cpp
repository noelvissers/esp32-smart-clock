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

//Main loop on core 0 (network functions)
void Core_0(void *parameter)
{
  for (;;)
  {
    delay(150000); //Do an update every 2.5 minutes
    if (!Weather.update())
    {
      Serial.println("[E][CORE_0] Error while updating weather.");
    }
    else
    {
      Serial.println("[CORE_0] Weather data received.");
    }
    if (!Time.update())
    {
      Serial.println("[E][CORE_0] Error while updating online time.");
    }
    else
    {
      Serial.println("[CORE_0] Syncing online time done.");
    }
  }
}

//ISRs
void IRAM_ATTR ISR_buttonPlus()
{
  if (digitalRead(_pinButtonPlus))
  {
    if ((millis() - 25 > _lastButtonPlusPress) && !_buttonPlusPressed)
    {
      _buttonPlusPressed = true;
      _lastButtonPlusPress = millis();
      //Press action
    }
  }
  else
  {
    if (_buttonPlusPressed)
    {
      _buttonPlusPressed = false;
    }
  }
  _lastButtonPlusPress = millis();
}

void IRAM_ATTR ISR_buttonSelect()
{
  if (digitalRead(_pinButtonSelect))
  {
    if ((millis() - 25 > _lastButtonSelectPress) && !_buttonSelectPressed)
    {
      _buttonSelectPressed = true;
      _lastButtonSelectPress = millis();
      //Press action
    }
  }
  else
  {
    if (_buttonSelectPressed)
    {
      _buttonSelectPressed = false;
    }
  }
  _lastButtonSelectPress = millis();
}

void IRAM_ATTR ISR_buttonMin()
{
  if (digitalRead(_pinButtonMin))
  {
    if ((millis() - 25 > _lastButtonMinPress) && !_buttonMinPressed)
    {
      _buttonMinPressed = true;
      _lastButtonMinPress = millis();
      //Press action
    }
  }
  else
  {
    if (_buttonMinPressed)
    {
      _buttonMinPressed = false;
    }
  }
  _lastButtonMinPress = millis();
}

void setup()
{
  //Network.resetSettings();
  //Config.formatSettings();
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
  //Display status
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Setup network connection
  if (!Network.autoConnect())
  {
    Serial.println("[E][Status] Error while configuring network.");
  }
  else
  {
    //Set network status to enabled
    Serial.println("[Status] Initializing [X][-][-] - Network configuration done.");
  }
  //Display status
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Get Time
  Rtc.update();
  if (!Time.update())
  {
    Serial.println("[E][Status] Error while configuring online time.");
  }
  else
  {
    //Set time status to enabled
    Serial.println("[Status] Initializing [X][X][-] - Syncing online time done.");
  }
  //Display status
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Get Weather
  if (!Weather.update())
  {
    Serial.println("[E][Status] Error while configuring weather.");
  }
  else
  {
    //Set weather status to enabled
    Serial.println("[Status] Initializing [X][X][X] - Weather data received.");
  }
  //Display status

  //Create thread in core 0
  xTaskCreatePinnedToCore(
      Core_0,
      "TasksCore_0",
      2048,
      NULL,
      1,
      &TasksCore_0,
      0);

  Config.saveSettings();
  Serial.println("[Status] Initializing done.");
  delay(1000); //Add delay to show status on screen, or it will skip too fast
}

//main loop on core 1
void loop()
{
  /**
   * TODO: Menu (only update time when needed)
   */
  Rtc.update();
  printf("%04u/%02u/%02u - %u:%02u:%02u - Temperature: %i C - Humidity: %u %% - Pressure: %u hPa\n", _timeYear, _timeMonth, _timeDay, _timeHour, _timeMinute, _timeSecond, int((_temperature - 273.15)+0.5), _humidity, _pressure);
  delay(1000);
}

//TODO: button handler:
//  Brightness controll (plus/min button)
//  Reset function (hold select for 5 sec)
//  Cycle functions