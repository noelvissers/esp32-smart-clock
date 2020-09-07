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
#include "pong.h"

//create objects
TaskHandle_t TasksCore_0;
CConfig Config;
CDisplay Display;
CNetwork Network;
CWeather Weather;
CRtc Rtc;
CTime Time;
CPong Pong;

unsigned long lastAutoCycle = 0;
unsigned int state = 0;

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

//ISRs//Buttons:
void IRAM_ATTR ISR_buttonPlus()
{
  if (!digitalRead(_pinButtonPlus)) //Chek if falling edge
  {
    if ((millis() - 25 > _lastButtonPlusPress) && !_buttonPlusPressed) //Debounce
    {
      //Set button states
      Serial.print("↓ ");
      _buttonPlusSet = true;
      _buttonPlusPressed = true;
      _lastButtonPlusPress = millis();
    }
  }
  else //Rising edge
  {
    if (_buttonPlusPressed) //Check if it was pressed to filter out false releases from bounce
    {
      Serial.print("↑ ");
      _buttonPlusPressed = false; //Reset button state
    }
  }
  _lastButtonPlusPress = millis();
}

void IRAM_ATTR ISR_buttonSelect()
{
  if (!digitalRead(_pinButtonSelect))
  {
    if ((millis() - 50 > _lastButtonSelectPress) && !_buttonSelectPressed)
    {
      Serial.print("↓ ");
      _buttonSelectSet = true;
      _buttonSelectPressed = true;
      _lastButtonSelectPress = millis();
    }
  }
  else
  {
    if (_buttonSelectPressed)
    {
      Serial.print("↑ ");
      _buttonSelectPressed = false;
    }
  }
  _lastButtonSelectPress = millis();
}

void IRAM_ATTR ISR_buttonMin()
{
  if (!digitalRead(_pinButtonMin))
  {
    if ((millis() - 25 > _lastButtonMinPress) && !_buttonMinPressed)
    {
      Serial.print("↓ ");
      _buttonMinSet = true;
      _buttonMinPressed = true;
      _lastButtonMinPress = millis();
    }
  }
  else
  {
    if (_buttonMinPressed)
    {
      Serial.print("↑ ");
      _buttonMinPressed = false;
    }
  }
  _lastButtonMinPress = millis();
}

void setup()
{
  //start serial communication for debugging
  Serial.begin(115200);
  Serial.println("[Status] Initializing...");

  //init classes
  Rtc.init();
  Config.initPinModes();
  Config.loadSettings();
  Display.init();

  EStatus statusNetwork = EStatus::Init;
  EStatus statusTime = EStatus::Init;
  EStatus statusWeather = EStatus::Init;
  Display.showStatus(statusNetwork, statusTime, statusWeather);
  Serial.println("[Status] Initializing [-][-][-]");
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Setup network connection
  if (!Network.autoConnect())
  {
    statusNetwork = EStatus::Error;
    Serial.println("[E][Status] Error while configuring network.");
  }
  else
  {
    statusNetwork = EStatus::Done;
    Serial.println("[Status] Initializing [X][-][-] - Network configuration done.");
  }
  Display.showStatus(statusNetwork, statusTime, statusWeather);
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Get Time
  Rtc.update();
  if (!Time.update())
  {
    statusTime = EStatus::Error;
    Serial.println("[E][Status] Error while configuring online time.");
  }
  else
  {
    statusTime = EStatus::Done;
    Serial.println("[Status] Initializing [X][X][-] - Syncing online time done.");
  }
  Display.showStatus(statusNetwork, statusTime, statusWeather);
  delay(1000); //Add delay to show status on screen, or it will skip too fast

  //Get Weather
  if (!Weather.update())
  {
    statusWeather = EStatus::Error;
    Serial.println("[E][Status] Error while configuring weather.");
  }
  else
  {
    statusWeather = EStatus::Done;
    Serial.println("[Status] Initializing [X][X][X] - Weather data received.");
  }
  Display.showStatus(statusNetwork, statusTime, statusWeather);

  //Create thread in core 0
  xTaskCreatePinnedToCore(Core_0, "TasksCore_0", 4096, NULL, 1, &TasksCore_0, 0);

  Config.saveSettings();

  //Attatch interrupts for buttons
  attachInterrupt(_pinButtonPlus, ISR_buttonPlus, CHANGE);
  attachInterrupt(_pinButtonSelect, ISR_buttonSelect, CHANGE);
  attachInterrupt(_pinButtonMin, ISR_buttonMin, CHANGE);

  Serial.println("[Status] Initializing done.");
  delay(1000); //Add delay to show status on screen, or it will skip too fast
  if ((statusNetwork == EStatus::Error) || (statusTime == EStatus::Error) || (statusWeather == EStatus::Error))
  {
    delay(4000); //add more delay if an error occured
  }
}

//Main loop on core 1
void loop()
{
  // Button checks
  if (_buttonPlusSet && _buttonMinSet && !_buttonSelectSet) //Both plus and min are pressed
  {
    while (_buttonPlusPressed && _buttonMinPressed)
    {
      if ((millis() - 3000) > _lastButtonPlusPress)
      {
        _buttonMinSet = false;
        _buttonSelectSet = false;
        _buttonPlusSet = false;

        Pong.start();
        break;
      }
    }
    _buttonMinSet = false;
    _buttonSelectSet = false;
    _buttonPlusSet = false;
  }
  else if (_buttonPlusSet && !_buttonMinSet && !_buttonSelectSet) //Only plus was pressed
  {
    bool hold = false;
    while (_buttonPlusPressed && !_buttonMinPressed)
    {
      if ((millis() - 3000) > _lastButtonPlusPress)
      {
        _autoBrightness = true;
        Display.showAutoBrightness();
        Serial.println("[Status] Autobrightness set.");
        Config.saveSettings();
        hold = true;
        break;
      }
    }
    if (!_buttonMinSet)
      _buttonPlusSet = false;
    _buttonSelectSet = false;
    if (!hold && !_buttonMinSet)
      Display.brightnessUp();
  }
  else if (!_buttonPlusSet && _buttonMinSet && !_buttonSelectSet) //Only min was pressed
  {
    bool hold = false;
    while (!_buttonPlusPressed && _buttonMinPressed)
    {
      if ((millis() - 3000) > _lastButtonMinPress)
      {
        _autoCycle = true;
        Display.showAutoCycle();
        lastAutoCycle = millis();
        Serial.println("[Status] AutoCycle set.");
        Config.saveSettings();
        hold = true;
        break;
      }
    }
    if (!_buttonPlusSet)
      _buttonMinSet = false;
    _buttonSelectSet = false;
    if (!hold && !_buttonPlusSet)
      Display.brightnessDown();
  }
  else if (!_buttonPlusSet && !_buttonMinSet && _buttonSelectSet) //Only select was pressed
  {
    while (_buttonSelectPressed)
    {
      if ((millis() - 5000) > _lastButtonSelectPress)
      {
        Serial.println("[I][Status] Resetting ESP...");
        Display.showReset();
        detachInterrupt(_pinButtonMin); //Without detach ESP crashes when releasing the button during reset.
        detachInterrupt(_pinButtonPlus);
        detachInterrupt(_pinButtonSelect);
        Network.resetSettings();
        Config.formatSettings();
        ESP.restart();
        for (;;)
          ;
      }
    }

    if (_autoCycle)
    {
      Serial.println("[Status] Turning off auto cycling...");
      _autoCycle = false;
      Config.saveSettings();
    }
    state++;
    _buttonMinSet = false;
    _buttonSelectSet = false;
    _buttonPlusSet = false;
  }
  else
  {
    _buttonMinSet = false;
    _buttonSelectSet = false;
    _buttonPlusSet = false;
  }

  switch (state)
  {
  case 0:
    Display.showTime();
    break;
  case 1:
    Display.showDate();
    break;
  case 2:
    Display.showTemperature();
    break;
  case 3:
    Display.showHumidity();
    break;
  /*
  case 4:
    Display.showTimeBin();
    break;
  */
  default:
    state = 0;
    break;
  }
  if ((((millis() - _autoCycleTime) > lastAutoCycle) && _autoCycle) && (millis() > _autoCycleTime))
  {
    lastAutoCycle = millis();
    state++;
  }
  delay(20);
}

//Implement DST