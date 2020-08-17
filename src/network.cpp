#include "network.h"
#include "config.h"
#include <WiFiManager.h>

bool CNetwork::autoConnect()
{
  Serial.println("[Network] Starting auto connect...");
  WiFiManager wm;
  wm.setShowPassword(false);
  wm.setDebugOutput(false);
  //Bug with ESP, sometimes it opens portal when it can reconnect. Bug in WM lib: wm.setEnableConfigPortal(false) doesnt always work, so can't implement a retry.
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  wm.setEnableConfigPortal(true);

  //Add setting fields to config portal
  WiFiManagerParameter cityConfig("CITY_NAME", "City name", _weatherCityName, 64, "placeholder=\"Amsterdam\"");
  WiFiManagerParameter countryConfig("COUNTRY_CODE", "Country code", _weatherCountryCode, 3, "placeholder=\"NL\"");
  WiFiManagerParameter keyConfig("API_KEY", "API key", _weatherApiKey, 64, "placeholder=\"<key>\"");

  char customHtmlAutoBrightness[24] = "type=\"checkbox\"";                                                                             //Create custom html checkbox
  if (_autoBrightness)                                                                                                                 //Check if setting is on
    strcat(customHtmlAutoBrightness, " checked");                                                                                      //Check checkbox
  WiFiManagerParameter brightnessConfig("AUTO_BRIGHTNESS", "Automatic brightness", "T", 2, customHtmlAutoBrightness, WFM_LABEL_AFTER); //Create config, return "T" is checked

  char customHtmlAutoCycle[24] = "type=\"checkbox\"";
  if (_autoCycle) //add to config
    strcat(customHtmlAutoCycle, " checked");
  WiFiManagerParameter autoCycleConfig("AUTO_CYCLE", "Automatic cycling", "T", 2, customHtmlAutoCycle, WFM_LABEL_AFTER);
  
  char customHtmlDateFormat[24] = "type=\"checkbox\"";
  if (_useDdmm) //add to config
    strcat(customHtmlDateFormat, " checked");
  WiFiManagerParameter dateFormatConfig("DATE_FORMAT", "Use DDMM (day-month) format", "T", 2, customHtmlDateFormat, WFM_LABEL_AFTER);

  char customHtmlClockFormat[24] = "type=\"checkbox\"";
  if (_use24h) //add to config
    strcat(customHtmlClockFormat, " checked");
  WiFiManagerParameter clockFormatConfig("CLOCK_FORMAT", "Use 24-Hour clock", "T", 2, customHtmlClockFormat, WFM_LABEL_AFTER);

  char customHtmlTemperatureUnit[24] = "type=\"checkbox\"";
  if (_useCelcius) //add to config
    strcat(customHtmlTemperatureUnit, " checked");
  WiFiManagerParameter temperatureUnitConfig("TEMPERATURE_UNIT", "Use °C as temperature unit", "T", 2, customHtmlTemperatureUnit, WFM_LABEL_AFTER);

  //Add all custom parameters to portal
  wm.addParameter(&cityConfig);
  wm.addParameter(&countryConfig);
  wm.addParameter(&keyConfig);
  wm.addParameter(&brightnessConfig);
  wm.addParameter(&autoCycleConfig);
  wm.addParameter(&dateFormatConfig);
  wm.addParameter(&clockFormatConfig);
  wm.addParameter(&temperatureUnitConfig);

  //Configure portal
  Serial.println("[Network] Setting up portal...");
  wm.setShowPassword(false);
  wm.setConfigPortalTimeout(180); //Set 3 minute timeout if not configured
  wm.setConnectTimeout(60);       //Set 1 minute timeout for connecting
  wm.setClass("invert");          //Dark mode

  if (wm.autoConnect("SmartClock"))
  {
    Serial.println("[Network] Connected.");

    //Read values from portal
    strcpy(_weatherCityName, cityConfig.getValue());
    strcpy(_weatherCountryCode, countryConfig.getValue());
    strcpy(_weatherApiKey, keyConfig.getValue());
    _autoBrightness = (strncmp(brightnessConfig.getValue(), "T", 1) == 0); //Compare value (returns "T" if checked) with "T", returns 0 if match. 0 == 0 if checked.
    _autoCycle = (strncmp(autoCycleConfig.getValue(), "T", 1) == 0);
    _useDdmm = (strncmp(dateFormatConfig.getValue(), "T", 1) == 0);
    _use24h = (strncmp(clockFormatConfig.getValue(), "T", 1) == 0);
    _useCelcius = (strncmp(temperatureUnitConfig.getValue(), "T", 1) == 0);

    return true;
  }
  Serial.println("[E][Network] Failed to connect to network.");
  return false;
}

void CNetwork::resetSettings()
{
  WiFiManager wm;
  wm.setDebugOutput(false);
  wm.resetSettings();
  Serial.println("[Network] Settings reset.");
}