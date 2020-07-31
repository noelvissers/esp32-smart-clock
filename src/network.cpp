#include "network.h"
#include <WiFiManager.h> 

char location[100] = "CITY_NAME,COUNTRY_CODE"; //TODO: SAFE/READ TO/FROM EEPROM!!!

bool CNetwork::autoConnect()
{
    Serial.println("[Network] Starting auto connect...");
    WiFiManager wm;
    wm.setDebugOutput(false);

    //Add setting fields to config portal
    WiFiManagerParameter location_config("Location", "Weather Location", location, 100);
    wm.addParameter(&location_config);

    //Configure portal
    Serial.println("[Network] Setting up portal...");
    wm.setShowPassword(false);
    wm.setConfigPortalTimeout(180); //Set 3 minute timeout if not configured
    wm.setConnectTimeout(60);       //Set 1 minute timeout for connecting
    wm.setClass("invert");          //Dark mode

    Serial.println("[Network] Setting up connection (or connecting to saved network)...");
    if (wm.autoConnect("SmartClock")){
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