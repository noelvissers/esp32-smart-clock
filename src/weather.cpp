#include "weather.h"
#include "data.h"
#include "WiFi.h"
#include "settings.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

float _temperature = -1.0;
int _humidity = -1;
int _pressure = -1;

HTTPClient client;
DynamicJsonDocument doc(1024);

bool CWeather::update()
{
  Serial.println("[Weather] Getting weather data...");
  if (WiFi.isConnected())
  {
    Serial.println("[Weather] Connected to network.");
    Serial.println("[Weather] Connecting to server...");
    Serial.println((_weatherEndpoint) + String(_weatherCityName) + "," + String(_weatherCountryCode) + "&APPID=" + String(_weatherApiKey));
    client.begin(String(_weatherEndpoint) + String(_weatherCityName) + "," + String(_weatherCountryCode) + "&APPID=" + String(_weatherApiKey));
    int httpCode = client.GET();

    if (httpCode > 0)
    {
      String weather_data = client.getString();
      DeserializationError error = deserializeJson(doc, weather_data);
      if (!error)
      {
        JsonObject root = doc.as<JsonObject>();

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
        Serial.print(F("[Weather] DeserializeJson() failed: "));
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