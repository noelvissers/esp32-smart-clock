#include "weather.h"
#include "WiFi.h"
#include "config.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

float _temperature = -1.0;
int _humidity = -1;
int _pressure = -1;

//probs dont want this global.... vv
HTTPClient client;
DynamicJsonDocument doc(1024);

bool CWeather::update()
{
  Serial.println("[Weather] Getting weather data...");
  if (WiFi.isConnected())
  {
    Serial.println("[Weather] Connected to network.");
    Serial.println("[Weather] Connecting to server...");
    client.begin(String(_weatherEndpoint) + String(_weatherCityName) + "," + String(_weatherCountryCode) + "&APPID=" + String(_weatherApiKey));
    int httpCode = client.GET();

    if (httpCode > 0)
    {
      String weatherData = client.getString();
      DeserializationError error = deserializeJson(doc, weatherData);
      if (!error)
      {
        JsonObject root = doc.as<JsonObject>(); //?

        if (root["cod"] != 401)
        {
          _temperature = root["main"]["temp"];
          _humidity = root["main"]["humidity"];
          _pressure = root["main"]["pressure"];

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