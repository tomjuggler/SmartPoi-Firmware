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
    if (size > maxPX)
    {
      //nope
      FastLED.showColor(CRGB::Blue); //error
      imageToUse++;
    }
    else
    {
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
    FastLED.show(); SendToProcessing();
    //Todo: check delay(1) effect on 72px poi - definitely needed for 36px
    if (NUM_LEDS < 72)
    {
      FastLED.delay(10); // for 160mhz
    }
    else
    {
      // no delay
    }
  }
}

/****The below two functions are needed for sending to Processing. */
// Copy from here to the bottom.
//--------------------
// Waits for Processing to respond and then sends the number of pixels.
void firstContact() {
  uint16_t nLEDS = NUM_LEDS;  // Number to send to Processing.  (Allows up to 65,535 pixels)
  if (testing == true) {
    linkedUp = true;  // If testing, pretend we are already connected to Processing.
    Serial.print("NUM_LEDS: "); Serial.println(nLEDS);  // Number of pixels in our LED strip.
    Serial.print("  High Byte = "); Serial.print(highByte(nLEDS));  // The high byte.
    Serial.print(" x 256 = "); Serial.println(highByte(nLEDS) * 256);
    Serial.print("  Low Byte  = "); Serial.println(lowByte(nLEDS));  // The low byte.
    delay(3000);
  }
  else {
    while (Serial.available() <= 0) {  // Repeats until Processing responds. Hello?
      Serial.print('A');  // send an ASCII A (byte of value 65)
      delay(100);
    }
    // Once Processing responds send the number of pixels as two bytes.
    Serial.write(highByte(nLEDS));  // Send the high byte to Processing.
    Serial.write(lowByte(nLEDS));  // Send the low byte to Processing.
    Serial.print('#');  // send an ASCII # (byte of value 35) as a flag for Processing.
    linkedUp = true;  // Now that Processing knows number of pixels set linkedUp to true.
    delay(500);
  }
}

//--------------------
// This function sends ALL the pixel data to Processing.
void SendToProcessing() {
  if (testing == true) { // Print pixel data. If NUM_LEDS is large this will be a lot of data!
    Serial.println("-------------------------------------------------------");
    for (uint16_t d = 0; d < NUM_LEDS; d++) {
      Serial.print("p: "); Serial.print(d);
      Serial.print("\tr: "); Serial.print(leds[d].r);
      Serial.print("\tg: "); Serial.print(leds[d].g);
      Serial.print("\tb: "); Serial.println(leds[d].b);
    }
    Serial.println(" ");
    delay(500);  // Add some extra delay while testing.
  }
  else {  // Send ALL the pixel data to Processing!
    for (uint16_t d = 0; d < NUM_LEDS; d++) {
      Serial.write(d);          // Pixel number
      Serial.write(leds[d].r);  // Red channel data
      Serial.write(leds[d].g);  // Green channel data
      Serial.write(leds[d].b);  // Blue channel data
    }
    delay(sendDelay);  // Delay to slow things down if needed.
  }
}

//--------------------
/****End of the functions needed for sending to Processing.*/
