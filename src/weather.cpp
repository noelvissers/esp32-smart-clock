#include "weather.h"
#include "WiFi.h"
#include "config.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

float _temperature = -1.0;
int _humidity = -1;
int _pressure = -1;

bool CWeather::update()
{
  Serial.println("[Weather] Getting weather data...");
  if (WiFi.isConnected())
  {
    Serial.println("[Weather] Connected to network.");
    HTTPClient client;
    Serial.println("[Weather] Connecting to server...");
    client.begin(String(_weatherEndpoint) + String(_weatherCityName) + "," + String(_weatherCountryCode) + "&APPID=" + String(_weatherApiKey));
    int httpCode = client.GET();

    if (httpCode != 0)
    {
      DynamicJsonDocument doc(1024);

      String weatherData = client.getString();
      DeserializationError error = deserializeJson(doc, weatherData);
      if (!error)
      {
        if (doc["cod"] != 401)
        {
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
  _temperature = -1.0;
  _humidity = -1;
  _pressure = -1;
  return false;
}