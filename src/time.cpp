#include "time.h"
#include "config.h"
#include "rtc.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

uint16_t _onlineTimeYear = 2020;
uint8_t _onlineTimeMonth = 1;
uint8_t _onlineTimeDay = 1;
uint8_t _onlineTimeHour = 0;
uint8_t _onlineTimeMinute = 0;
uint8_t _onlineTimeSecond = 0;

long _onlineTimeUnix = -1; //read by update
int _onlineDayOfWeek = -1; //read by update, 0 = sunday, 6 = saturday
bool _onlineDst = false;   //read by update

char _onlineDatetime[64] = "";

bool convertTime()
{
//Convert datetime to usable vars (this is pretty hardcoded but can't find a better way like istringstream)
#ifdef DEBUGGING
  Serial.println("[Time] Converting 'datetime' to vars...");
#endif
  int count = 0;
  String temp = "";
  for (int i = 0; i < strlen(_onlineDatetime); i++)
  {
    if ((_onlineDatetime[i] == '-') && (count == 0))
    {
      _onlineTimeYear = temp.toInt();
      temp = "";
      count++;
      i++;
    }
    else if ((_onlineDatetime[i] == '-') && (count == 1))
    {
      _onlineTimeMonth = temp.toInt();
      temp = "";
      count++;
      i++;
    }
    else if ((_onlineDatetime[i] == 'T') && (count == 2))
    {
      _onlineTimeDay = temp.toInt();
      temp = "";
      count++;
      i++;
    }
    else if ((_onlineDatetime[i] == ':') && (count == 3))
    {
      _onlineTimeHour = temp.toInt();
      temp = "";
      count++;
      i++;
    }
    else if ((_onlineDatetime[i] == ':') && (count == 4))
    {
      _onlineTimeMinute = temp.toInt();
      temp = "";
      count++;
      i++;
    }
    else if ((_onlineDatetime[i] == '.') && (count == 5))
    {
      _onlineTimeSecond = temp.toInt();
    }
    temp = temp + _onlineDatetime[i];
  }

  return true;
}

bool timeSync()
{
#ifdef DEBUGGING
  Serial.println("[Time] Syncing with RTC...");
#endif

  CRtc RtcSync;
  RtcSync.update();

  if ((_timeYear != _onlineTimeYear) || (_timeMonth != _onlineTimeMonth) || (_timeDay != _onlineTimeDay) || (_timeHour != _onlineTimeHour) || (((_timeMinute + 2) <= _onlineTimeMinute) || ((_timeMinute - 2) >= _onlineTimeMinute))) //if more than 2mins difference
  {
    if (RtcSync.checkRtc())
    {
      Serial.println("[I][Time] RTC is out of sync by more than 2 minutes. Setting time...");
      RtcSync.setTime(_onlineTimeYear, _onlineTimeMonth, _onlineTimeDay, _onlineTimeHour, _onlineTimeMinute, _onlineTimeSecond);
      Serial.println("[I][Time] RTC set.");
      return true;
    }
    else
    {
      Serial.println("[E][Time] Could not find RTC.");
    }
  }
  else
  {
#ifdef DEBUGGING
    Serial.println("[Time] RTC is already in sync.");
#endif
    return true;
  }
  return false;
}

bool CTime::update()
{
#ifdef DEBUGGING
  Serial.println("[Time] Getting time data...");
#endif
  if (WiFi.isConnected())
  {
#ifdef DEBUGGING
    Serial.println("[Time] Connected to network.");
#endif
    HTTPClient client;
#ifdef DEBUGGING
    Serial.println("[Time] Connecting to server...");
#endif
    client.begin(_timeEndpoint);
    int httpCode = client.GET();

    if (httpCode != 0)
    {
      DynamicJsonDocument doc(1024);

      String timeData = client.getString();
      DeserializationError error = deserializeJson(doc, timeData);
      if (!error)
      {
        _onlineTimeUnix = doc["unixtime"];
        _onlineDayOfWeek = doc["day_of_week"];
        _onlineDst = doc["dst"];
        strcpy(_onlineDatetime, doc["datetime"]);

#ifdef DEBUGGING
        Serial.println("[Time] Data received.");
#endif

        convertTime();
        timeSync();
#ifdef DEBUGGING
        Serial.println("[Time] Done.");
#endif

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