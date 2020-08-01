#include "time.h"
#include "config.h"
#include "rtc.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

long _unixtime = -1;
int _dayOfWeek = -1;
char _datetime[64] = "";

bool timeSync()
{
  Serial.println("[Time] Syncing with RTC...");

  if (false /*current time != rtc time*/) //if more than 2mins difference
  {
    Serial.println("[I][Time] RTC is out of sync by more than 2 minutes. Setting time...");
    //set time rtc with online time
    Serial.println("[I][Time] RTC set.");
  }
  else
  {
    Serial.println("[Time] RTC is already in sync.");
    return true;
  }
}

bool CTime::update()
{
  Serial.println("[Time] Getting time data...");
  if (WiFi.isConnected())
  {
    Serial.println("[Time] Connected to network.");
    HTTPClient client;
    Serial.println("[Time] Connecting to server...");
    client.begin(_timeEndpoint);
    int httpCode = client.GET();

    if (httpCode != 0)
    {
      DynamicJsonDocument doc(1024);

      String timeData = client.getString();
      DeserializationError error = deserializeJson(doc, timeData);
      if (!error)
      {
        _unixtime = doc["unixtime"];
        _dayOfWeek = doc["day_of_week"];
        strcpy(_datetime, doc["datetime"]);

        Serial.println("[Time] Data received.");

        timeSync();
        Serial.println("[Time] Done.");
        
        return true;
      }
      else
      {
        Serial.print(F("[E][Time] DeserializeJson() failed: "));
        Serial.println(error.c_str());
      }
    }
    else
    {
      Serial.println("[E][Time] Failed getting HTTP request.");
    }
  }
  else
  {
    Serial.println("[E][Time] No network connection.");
  }
  return false;
}

// add fuction to get current hours/minutes etc...
// Check all possible daylight saving times > if true, check online time 5Head (in rtc)