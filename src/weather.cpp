#include "weather.h"
#include "data.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=";
String city_name = "Amsterdam";
String country_code = "nl";
String api_key = "e01213e3d481a166153ab05e2af5aa76";

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
    client.begin(endpoint + city_name + "," + country_code + "&APPID=" + api_key);
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
          return true;
        }
        else
        {
          Serial.println("[Weather] ERROR: Wrong City code, Country code or API key.");
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
      Serial.println("[Weather] ERROR: Failed getting HTTP request.");
    }
    client.end();
  }
  else
  {
    Serial.println("[Weather] ERROR: No network connection.");
  }
  _temperature = -1.0;
  _humidity = -1;
  _pressure = -1;
  return false;
}