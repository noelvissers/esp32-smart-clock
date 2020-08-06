#include "network.h"
#include "config.h"
#include <WiFiManager.h>

bool CNetwork::autoConnect()
{
#ifdef DEBUG
  Serial.println("[Network] Starting auto connect...");
#endif
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
#ifdef DEBUG
  Serial.println("[Network] Setting up portal...");
#endif
  wm.setShowPassword(false);
  wm.setConfigPortalTimeout(180); //Set 3 minute timeout if not configured
  wm.setConnectTimeout(60);       //Set 1 minute timeout for connecting
  wm.setClass("invert");          //Dark mode

#ifdef DEBUG
  Serial.println("[Network] Setting up connection (or connecting to saved network)...");
#endif
  if (wm.autoConnect("SmartClock"))
  {
#ifdef DEBUG
    Serial.println("[Network] Connected.");
#endif

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
#ifdef DEBUG
  Serial.println("[Network] Settings reset.");
#endif
}