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

//ISRs//Buttons:
bool buttonPlusPressed = false;        //Current state of button (resets on release)
bool buttonPlusSet = false;            //Toggled state of button (resets when button action is done / manually)
unsigned long lastButtonPlusPress = 0; //Last button press millis()
void IRAM_ATTR ISR_buttonPlus()
{
  if (!digitalRead(_pinButtonPlus)) //Chek if falling edge
  {
    if ((millis() - 25 > lastButtonPlusPress) && !buttonPlusPressed) //Debounce
    {
      //Set button states
      Serial.print("↓ ");
      buttonPlusSet = true;
      buttonPlusPressed = true;
      lastButtonPlusPress = millis();
    }
  }
  else //Rising edge
  {
    if (buttonPlusPressed) //Check if it was pressed to filter out false releases from bounce
    {
      Serial.print("↑ ");
      buttonPlusPressed = false; //Reset button state
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
    if ((millis() - 50 > lastButtonSelectPress) && !buttonSelectPressed)
    {
      Serial.print("↓ ");
      buttonSelectSet = true;
      buttonSelectPressed = true;
      lastButtonSelectPress = millis();
    }
  }
  else
  {
    if (buttonSelectPressed)
    {
      Serial.print("↑ ");
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
      Serial.print("↓ ");
      buttonMinSet = true;
      buttonMinPressed = true;
      lastButtonMinPress = millis();
    }
  }
  else
  {
    if (buttonMinPressed)
    {
      Serial.print("↑ ");
      buttonMinPressed = false;
    }
  }
  lastButtonMinPress = millis();
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
}

//Main loop on core 1
void loop()
{
  // Button checks
  if (buttonPlusSet && buttonMinSet && !buttonSelectSet) //Both plus and min are pressed
  {
    while (buttonPlusPressed && buttonMinPressed)
    {
      if ((millis() - 3000) > lastButtonPlusPress)
      {
        _autoBrightness = true;
        Display.showAutoBrightness();
        Serial.println("[Status] Autobrightness set.");
        break;
      }
    }
    buttonMinSet = false;
    buttonSelectSet = false;
    buttonPlusSet = false;
  }
  else if (buttonPlusSet && !buttonMinSet && !buttonSelectSet) //Only plus was pressed
  {
    Display.brightnessUp();
    while (buttonPlusPressed && !buttonMinPressed)
      delay(10);
    if (!buttonMinSet)
      buttonPlusSet = false;
    buttonSelectSet = false;
  }
  else if (!buttonPlusSet && buttonMinSet && !buttonSelectSet) //Only min was pressed
  {
    Display.brightnessDown();
    while (!buttonPlusPressed && buttonMinPressed)
      delay(10);
    if (!buttonPlusSet)
      buttonMinSet = false;
    buttonSelectSet = false;
  }
  else if (!buttonPlusSet && !buttonMinSet && buttonSelectSet) //Only select was pressed
  {
    while (buttonSelectPressed)
    {
      if ((millis() - 5000) > lastButtonSelectPress)
      {
        Serial.println("[I][Status] Resetting ESP...");
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
    //ESP did not reset
    _state++;
    Serial.print("[Status] Changed state: ");
    Serial.println(_state);
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
    Serial.print("[Status] Changed state: ");
    Serial.println(_state);
    break;
  }
  delay(20);
}

//Implement LDR
//Implement DST
