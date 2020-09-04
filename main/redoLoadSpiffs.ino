
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
//todo: change to .bin load code here
//Todo: remove redundant extra 10 file loads
 if (reload)
  {
    for (int i = 0; i < 20; i++)
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
      case 10:
        image = "k.txt";
        bin = "k.bin";
        break;
      case 11:
        image = "l.txt";
        bin = "l.bin";
        break;
      case 12:
        image = "m.txt";
        bin = "m.bin";
        break;
      case 13:
        image = "n.txt";
        bin = "n.bin";
        break;
      case 14:
        image = "o.txt";
        bin = "o.bin";
        break;
      case 15:
        image = "p.txt";
        bin = "p.bin";
        break;
      case 16:
        image = "q.txt";
        bin = "q.bin";
        break;
      case 17:
        image = "r.txt";
        bin = "r.bin";
        break;
      case 18:
        image = "s.txt";
        bin = "s.bin";
        break;
      case 19:
        image = "t.txt";
        bin = "t.bin";
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
  /*
  if (pattern == 2) {
    //    String imgToShow1 = "/f.txt";
    //    String imgToShow2 = "/g.txt";
    //    String imgToShow3 = "/h.txt";
    //    String imgToShow4 = "/i.txt";
    //    String imgToShow5 = "/j.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow1, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow2, message2Data, 2);
    preloaded = false;
    loadSpiffsImage2(imgToShow3, message3Data, 3);
    preloaded = false;
    loadSpiffsImage2(imgToShow4, message4Data, 4);
    preloaded = false;
    loadSpiffsImage2(imgToShow5, message5Data, 5);
  } else if (pattern == 3) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/a.txt";
    //    String imgToShow7 = "/b.txt";
    //    String imgToShow8 = "/c.txt";
    //    String imgToShow9 = "/d.txt";
    //    String imgToShow10 = "/e.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow6, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow7, message2Data, 2);
    preloaded = false;
    loadSpiffsImage2(imgToShow8, message3Data, 3);
    preloaded = false;
    loadSpiffsImage2(imgToShow9, message4Data, 4);
    preloaded = false;
    loadSpiffsImage2(imgToShow10, message5Data, 5);
  } else if (pattern == 4) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/k.txt";
    //    String imgToShow7 = "/l.txt";
    //    String imgToShow8 = "/m.txt";
    //    String imgToShow9 = "/n.txt";
    //    String imgToShow10 = "/o.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow11, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow12, message2Data, 2);
    preloaded = false;
    loadSpiffsImage2(imgToShow13, message3Data, 3);
    preloaded = false;
    loadSpiffsImage2(imgToShow14, message4Data, 4);
    preloaded = false;
    loadSpiffsImage2(imgToShow15, message5Data, 5);
  }else if (pattern == 5) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/k.txt";
    //    String imgToShow7 = "/l.txt";
    //    String imgToShow8 = "/m.txt";
    //    String imgToShow9 = "/n.txt";
    //    String imgToShow10 = "/o.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow16, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow17, message2Data, 2);
    preloaded = false;
    loadSpiffsImage2(imgToShow18, message3Data, 3);
    preloaded = false;
    loadSpiffsImage2(imgToShow19, message4Data, 4);
    preloaded = false;
    loadSpiffsImage2(imgToShow20, message5Data, 5);
  }
  */
    picToShow = 1; //back to first pic, to sync poi
}
