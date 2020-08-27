int i = 0; //bad name change this!

void showLittleFSImage()
{
  a = LittleFS.open(bin, "r"); //open every time?

  if (!a)
  {
    FastLED.showColor(CRGB::Blue);
    // Serial.println("Code Blue - no file found!");
  }
  else
  {
    size_t size = a.size();
    a.read(message1Data, size);
    // Serial.print("size is: ");
    // Serial.println(size);
    i++;
    if (i >= pxDown)
    {
      i = 0;
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
  }
}

void showSpiffsImage()
{
  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      X = message1Data[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
    }
    FastLED.show();
  }
}

void showSpiffsImage1()
{
  //  if(!preloaded){
  ////what to do here
  //  }
  //  else{

  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      X = message1Data[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
    }
    FastLED.show();
  }
  //  }
}

void showSpiffsImage2(uint8_t *message)
{
  //  if(!preloaded){
  ////what to do here
  //  }
  //  else{

  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      X = message[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
    }
    FastLED.show();
    FastLED.delay(1); // for 160mhz
  }
  //  //Serial.println(pxAcross); //should change with size of image
  //  }
}

///////////////////////////////////////////////////todo: load 1 byte at a time per row of pixels displayed. Takes about 1ms to load (apparently) so should work.
//how to do this???????? >
/*
void loadWhileShowingSpiffsImage1()
{
  //  if(!preloaded){
  ////what to do here
  //  }
  //  else{

  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      X = message1Data[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
      ///////////////////////////////////////////////load other image to spiffs//////////////////////////////////////////////
      if (!preloaded)
      {
        String line = g.readStringUntil(',');
        int Y = line.toInt();
        message2Data[byteCounter] = Y;
      }
      else
      {
        g.close();
        preloaded = true;
      }
    }
    byteCounter++;
    if (byteCounter > 1296)
    {
      byteCounter = 0;
      preloaded = true;
    }
    FastLED.show();
  }
  //  }
}

void loadWhileShowingSpiffsImage2()
{
  //  if(!preloaded){
  ////what to do here
  //  }
  //  else{

  int counter = 0;
  for (int j = 0; j < pxAcross; j++)
  {
    for (int i = 0; i < pxDown; i++)
    {
      byte X;
      X = message2Data[counter++];
      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //            //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
      ///////////////////////////////////////////////load other image to spiffs//////////////////////////////////////////////
      if (!preloaded)
      {
        String line = g.readStringUntil(',');
        int Y = line.toInt();
        message1Data[byteCounter] = Y;
      }
      else
      {
        g.close();
        preloaded = true;
      }
    }
    byteCounter++;
    if (byteCounter > 1296)
    {
      byteCounter = 0;
      preloaded = true;
    }
    FastLED.show();
  }
  //  }
}
*/