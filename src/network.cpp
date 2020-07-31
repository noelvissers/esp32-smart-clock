#include "network.h"
#include <WiFiManager.h>

char weather_city_name[100] = "CITY_NAME";        //TODO: SAFE/READ TO/FROM EEPROM!!!
char weather_country_code[100] = "COUNTRY_CODE";  //TODO: SAFE/READ TO/FROM EEPROM!!!
char weather_api_key[100] = "API_KEY";            //TODO: SAFE/READ TO/FROM EEPROM!!!

bool CNetwork::autoConnect()
{
  Serial.println("[Network] Starting auto connect...");
  WiFiManager wm;
  wm.setDebugOutput(false);

  //Add setting fields to config portal
  WiFiManagerParameter city_config("CITY_NAME", "City name", weather_city_name, 100);
  WiFiManagerParameter country_config("COUNTRY_CODE", "Country code", weather_country_code, 100);
  WiFiManagerParameter api_key_config("API_KEY", "API key", weather_api_key, 100);
  wm.addParameter(&city_config);
  wm.addParameter(&country_config);
  wm.addParameter(&api_key_config);

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
    return true;
  }

  Serial.println("[Network] ERROR: Failed to connect to network.");
  return false;
}

void CNetwork::resetSettings()
{
  WiFiManager wm;
  wm.setDebugOutput(false);
  wm.resetSettings();
  Serial.println("[Network] Settings reset.");
}