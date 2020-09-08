
void redoLoadSpiffs() {
  String fileName = "f";
  String fileVar = String("/" + fileName + ".txt");
  //    f = LittleFS.open("/f.txt", "r");
  f = LittleFS.open(fileVar, "r");
  if (!f) {
    FastLED.showColor( CRGB::Blue );
  } else {
    int i = 0;
    byte X;
    // we could open the file
    while (f.available()) {
      if (message1DataCounter > maxPX) { //message exeeding limit

      }
      else {
        String line = f.readStringUntil(',');
        int Y = line.toInt();
        message1Data[i] = Y; //need error checking to make sure the file is not too large for the array!
        i++;
        message1DataCounter++;
      }
      //read until reaching "," delimeter:

    }
    pxAcross = i / pxDown;
    message1DataCounter = 0;
    f.close();
  }

}

//do we need this function?
void loadSpiffsImage2(String imgPath, uint8_t* message, int num) {
  if (!preloaded) { //need to load once per image change here, not every time it's too slow!
    g = LittleFS.open(imgPath, "r");
    //    g = LittleFS.open("/f.txt", "r");
    if (!g) {
      fastLEDIndicate();
      // FastLED.showColor( CRGB::Blue );
    } else {
      int i = 0;
      byte X;
      // we could open the file
      while (g.available()) {
        if (message1DataCounter > maxPX) { //message exeeding limit

        }
        else {
          String line = g.readStringUntil(',');
          int Y = line.toInt();
          message[i] = Y; //need error checking to make sure the file is not too large for the array!
          i++;
          message1DataCounter++;
        }
        //read until reaching "," delimeter:

      }
      pxAcross = i / pxDown;
      pxAcrossArray[num] = pxAcross;
      message1DataCounter = 0;
      g.close();
      preloaded = true;
    }
  }
  else {
    //already loaded
    //test open but don't do anything
    //   /
    //seems that this does not take much time... good news
  }
}

/////////////////////////////////////////////////////////EXPERIMENTAL //////////////////////////////////
/*
void loadSpiffsImage2(String imgPath, uint8_t* message, int num) {
  if (!preloaded) { //need to load once per image change here, not every time it's too slow!
    g = LittleFS.open(imgPath, "r");
    //    g = LittleFS.open("/f.txt", "r");
    if (!g) {
      FastLED.showColor( CRGB::Blue );
    } else {
      int i = 0;
      byte X;
      // we could open the file
      while (g.available()) {
        if (message1DataCounter > maxPX) { //message exeeding limit

        }
        else {
          String line = g.readStringUntil(',');
          int Y = line.toInt();
          message[i] = Y; //need error checking to make sure the file is not too large for the array!

///////////////////////////////////////////////
          byte R1 = (Y & 0xE0);
          leds[i].r = R1; //
          byte G1 =  ((Y << 3) & 0xE0);
          leds[i].g = G1;
          byte M1 = (Y << 6);
          leds[i].b = M1;
/////////////////////////////////////////////
      
          i++;
          message1DataCounter++;
        }
        
        //read until reaching "," delimeter:

      }
      /////////////////////////////////////////////
        FastLED.show(); // Will this work???????
        /////////////////////////////////////////////
      pxAcross = i / pxDown;
      pxAcrossArray[num] = pxAcross;
      message1DataCounter = 0;
      g.close();
      preloaded = true;
    }
  }
  else {
    //already loaded
    //test open but don't do anything
    //   /
    //seems that this does not take much time... good news
  }
}
*/
//////////////////////////////////////////////////////END EXPERIMENTAL ///////////////////////////////
void loadPatternChooser() {
 if (reload)
  {
    for (int i = 0; i < 10; i++)
    {
      switch (i)
      {
      case 0:
        image = "a.txt";
        bin = "a.bin";
        break;
      case 1:
        image = "b.txt";
        bin = "b.bin";
        break;
      case 2:
        image = "c.txt";
        bin = "c.bin";
        break;
      case 3:
        image = "d.txt";
        bin = "d.bin";
        break;
      case 4:
        image = "e.txt";
        bin = "e.bin";
        break;
      case 5:
        image = "f.txt";
        bin = "f.bin";
        break;
      case 6:
        image = "g.txt";
        bin = "g.bin";
        break;
      case 7:
        image = "h.txt";
        bin = "h.bin";
        break;
      case 8:
        image = "i.txt";
        bin = "i.bin";
        break;
      case 9:
        image = "j.txt";
        bin = "j.bin";
        break;
      
      }

      f = LittleFS.open(image, "r");
      if (!f)
      {
        fastLEDIndicateFast();
        // FastLED.showColor(CRGB::Blue);
      }
      else
      {
        int i = 0;
        byte X;
        // we could open the file

        while (f.available())
        {

          //read until reaching "," delimeter:
          String line = f.readStringUntil(',');
          int Y = line.toInt();
          message1Data[i] = Y; //need error checking to make sure the file is not too large for the array!
          i++;
        }
        pxAcross = i / pxDown;
        f.close();
     
      //remove unecessary .txt file:
      LittleFS.remove(image);
      // memset(&message1Data, 0, sizeof(message1Data)); //crashes FastLED..?
      a = LittleFS.open(bin, "w");
      // a.write(message1Data, sizeof(a))
      if (a)
      {
        if (a.write(message1Data, sizeof(message1Data)))
        {
          // Serial.println("record added");
        }
        a.close();
      }
 }
      // Serial.println("start finished");
    }
  }
  
    imageToUse = 1; //back to first pic, to sync poi
}
