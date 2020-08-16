#include <Arduino.h>
#include "config.h"
#include "ldr.h"

float lastLdrValue = 0;
int lastBrightness = 0;

unsigned int CLdr::read()
{
  float ldrValue = analogRead(_pinLDR);

  //Check if new read is above threshold
  if (((ldrValue - _ldrError) > lastLdrValue) || ((ldrValue + _ldrError) < lastLdrValue))
  {
    //Calculate new value
    int brightness = int(((ldrValue - _ldrLowerLimit) / _ldrUpperLimt) * 15);
    if (brightness < 0)
      brightness = 0;
    if (brightness > 15)
      brightness = 15;

    //Set new values
    lastLdrValue = ldrValue;
    lastBrightness = brightness;
    return brightness;
  }
  else
    return lastBrightness;
}