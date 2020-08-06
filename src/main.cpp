/**
 * @project       Smart Clock
 * @documentation https://github.com/noelvissers/esp32-smart-clock 
 */
#define DEBUG //Debug mode outputs the status of the clock over the serial port @115200. When this is off, only errors ([E]) and critical info ([I]) are outputted.

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
#ifdef DEBUG
      Serial.println("[CORE_0] Weather data received.");
#endif
    }
    if (!Time.update())
    {
      Serial.println("[E][CORE_0] Error while updating online time.");
    }
    else
    {
#ifdef DEBUG
      Serial.println("[CORE_0] Syncing online time done.");
#endif
    }
  }
}

//ISRs//Buttons:
bool buttonPlusPressed = false;        //Current state of button (resets on release)
bool buttonPlusSet = false;            //Toggled state of button (resets when button action is done / manually)
unsigned long lastButtonPlusPress = 0; //last button press millis()
void IRAM_ATTR ISR_buttonPlus()
{
  if (!digitalRead(_pinButtonPlus))
  {
    if ((millis() - 25 > lastButtonPlusPress) && !buttonPlusPressed)
    {
      buttonPlusSet = true;
      buttonPlusPressed = true;
      lastButtonPlusPress = millis();
    }
  }
  else
  {
    if (buttonPlusPressed)
    {
      buttonPlusPressed = false;
    }
  }
  lastButtonPlusPress = millis();
}

bool buttonSelectPressed = false;
bool buttonSelectSet = false;
unsigned long lastButtonSelectPress = 0;
void IRAM_ATTR ISR_buttonSelect()
{
  if (!digitalRead(_pinButtonSelect))
  {
    if ((millis() - 25 > lastButtonSelectPress) && !buttonSelectPressed)
    {
      buttonSelectSet = true;
      buttonSelectPressed = true;
      lastButtonSelectPress = millis();
    }
  }
  else
  {
    if (buttonSelectPressed)
    {
      buttonSelectPressed = false;
    }
  }
  lastButtonSelectPress = millis();
}

bool buttonMinPressed = false;
bool buttonMinSet = false;
unsigned long lastButtonMinPress = 0;
void IRAM_ATTR ISR_buttonMin()
{
  if (!digitalRead(_pinButtonMin))
  {
    if ((millis() - 25 > lastButtonMinPress) && !buttonMinPressed)
    {
      buttonMinSet = true;
      buttonMinPressed = true;
      lastButtonMinPress = millis();
    }
  }
  else
  {
    if (buttonMinPressed)
    {
      buttonMinPressed = false;
    }
  }
  lastButtonMinPress = millis();
}

void setup()
{
  //Network.resetSettings();
  //Config.formatSettings();
  //start serial communication for debugging
  Serial.begin(115200);
#ifdef DEBUG
  Serial.println("[Status] Initializing...");
#endif

  //init classes
  Rtc.init();
  Config.initPinModes();

  Config.loadSettings();
#ifdef DEBUG
  Serial.println("[Status] Initializing [-][-][-]");
#endif
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
#ifdef DEBUG
    Serial.println("[Status] Initializing [X][-][-] - Network configuration done.");
#endif
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
#ifdef DEBUG
    Serial.println("[Status] Initializing [X][X][-] - Syncing online time done.");
#endif
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
#ifdef DEBUG
    Serial.println("[Status] Initializing [X][X][X] - Weather data received.");
#endif
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

  //attatch interrupts for buttons
  attachInterrupt(_pinButtonPlus, ISR_buttonPlus, CHANGE);
  attachInterrupt(_pinButtonSelect, ISR_buttonSelect, CHANGE);
  attachInterrupt(_pinButtonMin, ISR_buttonMin, CHANGE);

#ifdef DEBUG
  Serial.println("[Status] Initializing done.");
#endif
  delay(1000); //Add delay to show status on screen, or it will skip too fast
}

//main loop on core 1

/**
 * - set start time
 * - while held down > wait
 * - if held for >3 sec > autobrightness on
 * - display setting
 * - display brightness for 3 seconds in display class (set a time that brightness was pressed, check in other functions if this time expired)
 * - autobrightness in disp.cpp
 */

void loop()
{                                                        // Button checks
  if (buttonPlusSet && buttonMinSet && !buttonSelectSet) //Both plus and min are pressed
  {
    while (buttonPlusPressed && buttonMinPressed)
    {
      if (lastButtonPlusPress - millis() > 3000)
      {
        _autoBrightness = true;
#ifdef DEBUG
        Serial.println("[Status] Autobrightness set.");
#endif
        break;
      }
    }
    buttonMinSet = false;
    buttonSelectSet = false;
    buttonPlusSet = false;
  }
  else if (buttonPlusSet && !buttonMinSet && !buttonSelectSet) //only plus was pressed
  {
    Display.brightnessUp();
    while (buttonPlusPressed && !buttonMinPressed)
      ;
    if (!buttonMinSet)
      buttonPlusSet = false;
    buttonSelectSet = false;
  }
  else if (!buttonPlusSet && buttonMinSet && !buttonSelectSet) //only min was pressed
  {
    Display.brightnessDown();
    while (!buttonPlusPressed && buttonMinPressed)
      ;
    if (!buttonPlusSet)
      buttonMinSet = false;
    buttonSelectSet = false;
  }
  else if (!buttonPlusSet && !buttonMinSet && buttonSelectSet) //Only select was pressed
  {
    while (buttonSelectPressed)
    {
      if (lastButtonSelectPress - millis() > 5000)
      {
        Serial.println("[I][Status] Resetting ESP.");
        buttonMinSet = false;
        buttonSelectSet = false;
        buttonPlusSet = false;
        Network.resetSettings();
        Config.formatSettings();
        ESP.restart();
      }
    }
    //ESP did not reset
    _state++;
    buttonMinSet = false;
    buttonSelectSet = false;
    buttonPlusSet = false;
  }
  else
  {
    buttonMinSet = false;
    buttonSelectSet = false;
    buttonPlusSet = false;
  }

  switch (_state)
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
  case 4:
    Display.showTimeBin();
    break;
  default:
    _state = 0;
    break;
  }
  delay(1000);
}

//TODO:
//  LDR