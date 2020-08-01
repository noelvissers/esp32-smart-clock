#include "network.h"
#include "config.h"
#include <WiFiManager.h>

bool CNetwork::autoConnect()
{
  Serial.println("[Network] Starting auto connect...");
  WiFiManager wm;
  wm.setDebugOutput(false);

  //Add setting fields to config portal
  WiFiManagerParameter city_config("CITY_NAME", "City name", _weatherCityName, 64, "placeholder=\"Amsterdam\"");
  WiFiManagerParameter country_config("COUNTRY_CODE", "Country code", _weatherCountryCode, 3, "placeholder=\"NL\"");
  WiFiManagerParameter key_config("API_KEY", "API key", _weatherApiKey, 64, "placeholder=\"<key>\"");
  wm.addParameter(&city_config);
  wm.addParameter(&country_config);
  wm.addParameter(&key_config);

  //Configure portal
  Serial.println("[Network] Setting up portal...");
  wm.setShowPassword(false);
  wm.setConfigPortalTimeout(180); //Set 3 minute timeout if not configured
  wm.setConnectTimeout(60);       //Set 1 minute timeout for connecting
  wm.setClass("invert");          //Dark mode

  Serial.println("[Network] Setting up connection (or connecting to saved network)...");
  if (wm.autoConnect("SmartClock"))
  {
    Serial.println("[Network] Connected.");

  	strcpy(_weatherCityName, city_config.getValue());
    strcpy(_weatherCountryCode, country_config.getValue());
    strcpy(_weatherApiKey, key_config.getValue());

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