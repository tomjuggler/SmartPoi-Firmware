

// Todo: add a function to update interval

/**
 * @brief Changes the pattern periodically based on the set interval.
 *
 * Checks the current time against the previous pattern change time to determine if the interval has passed.
 * If the interval has passed, increments the pattern index and updates the previous pattern change time.
 * If the pattern index exceeds the maximum allowed value, resets it to the minimum allowed value.
 *
 * @note This function is intended to be called repeatedly to ensure timely pattern changes.
 * @note The interval is set using the `/intervalChange` route.
 */
void ChangePatternPeriodically()
{
  unsigned long currentMillis3 = millis();
  if (currentMillis3 - previousMillis3 >= interval) // should not ever be true if udp is sending at correct speed!
  {
    imageToUse++;
    previousMillis3 = currentMillis3;
    if (imageToUse > maxImages)
    {
      imageToUse = minImages;
    }
    Serial.print("Changed to image: ");
    Serial.print(imageToUse);
    Serial.print(" Pattern: ");
    Serial.println(pattern);
  }
  yield();
}
