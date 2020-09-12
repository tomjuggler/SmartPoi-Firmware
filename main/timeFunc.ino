uint8_t secondHand; 
static uint8_t lastSecond = 99;

//uint32_t frei; //debug code

//Todo: add a function to update interval
void ChangePatternPeriodically()
{
  //new function: 
  unsigned long currentMillis3 = millis();
  if (currentMillis3 - previousMillis3 >= interval)
  { //should not ever be true if udp is sending at correct speed!
    imageToUse++;
    previousMillis3 = currentMillis3;
    if (imageToUse > maxImages)
    {
      imageToUse = 0;
    }
    // Serial.println(imageToUse);
  }

  
}

