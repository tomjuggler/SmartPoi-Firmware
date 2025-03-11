#include "TimeFunc.h"
#include "Globals.h"
#include <Arduino.h>

/**
 * @brief Changes the pattern periodically based on the set interval.
 * 
 * Uses global timing variables and pattern/image indexes from main.cpp
 * Maintains original functionality with proper PlatformIO organization
 */
void ChangePatternPeriodically()
{
  unsigned long currentMillis3 = millis();
  if (currentMillis3 - previousMillis3 >= interval)
  {
    imageToUse++;
    previousMillis3 = currentMillis3;
    if (imageToUse > maxImages)
    {
      imageToUse = minImages;
    }
    // Serial.print("Changed to image: ");
    // Serial.print(imageToUse);
    // Serial.print(" Pattern: ");
    // Serial.println(pattern);
  }
  yield();
}
