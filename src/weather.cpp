#include "weather.h"
#include "WiFi.h"
#include "config.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

float _temperature = -1.0;
int _humidity = -1;
int _pressure = -1;

//Get weather data from API
bool CWeather::update()
{
  Serial.println("[Weather] Getting weather data...");
  if (WiFi.isConnected()) //Check if connected to wifi
  {
    Serial.println("[Weather] Connected to network.");
    HTTPClient client;
    Serial.println("[Weather] Connecting to server...");
    client.begin(String(_weatherEndpoint) + String(_weatherCityName) + "," + String(_weatherCountryCode) + "&APPID=" + String(_weatherApiKey)); //Use the url with the users location and API key
    int httpCode = client.GET();

    if (httpCode != 0)
    {
      DynamicJsonDocument doc(1024); //Make temp. document to store API data in

      String weatherData = client.getString();                        //Store API data in string
      DeserializationError error = deserializeJson(doc, weatherData); //String to JSON object
      if (!error)
      {
        if (doc["cod"] != 401) //If city/country or key is invalid it will return code 401
        {
          //Store data in vars
          _temperature = doc["main"]["temp"];
          _humidity = doc["main"]["humidity"];
          _pressure = doc["main"]["pressure"];

          Serial.println("[Weather] Data received.");
          return true;
        }
        else
        {
          Serial.println("[E][Weather] Wrong City code, Country code or API key.");
        }
      }
      else
      {
        Serial.print(F("[E][Weather] DeserializeJson() failed: "));
        Serial.println(error.c_str());
      }
    }
    else
    {
      Serial.println("[E][Weather] Failed getting HTTP request.");
    }
    client.end();
  }
  else
  {
    Serial.println("[E][Weather] No network connection.");
  }
  //If the data couldn't be updated, use invalid data, else outdated info would show
  _temperature = -1.0; //Temperature can't go below 0, since this is in Kelvin
  _humidity = -1;
  _pressure = -1;
  return false;
}