int cnti = 0; 

void showLittleFSImage()
{
  a = LittleFS.open(bin, "r"); //open every time?

  if (!a)
  {
//go to next message and return
    imageToUse++;
    // FastLED.showColor(CRGB::Blue);
    // Serial.println("Code Blue - no file found!");
  }
  else
  {
    size_t size = a.size();
    a.read(message1Data, size);
    // Serial.print("size is: ");
    // Serial.println(size);
    cnti++;
    if (cnti >= pxDown)
    {
      cnti = 0;
    }
    a.close();
  }

  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      //  Serial.println(X);
      X = message1Data[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            Serial.print(R1); Serial.print(", "); Serial.print(G1); Serial.print(", "); Serial.println(M1);
    }
    FastLED.show();
    //Todo: check delay(1) effect on 72px poi
    // FastLED.delay(1); // for 160mhz

  }
}


