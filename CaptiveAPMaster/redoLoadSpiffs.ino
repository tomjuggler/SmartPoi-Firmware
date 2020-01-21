
void redoLoadSpiffs() {
  String fileName = "f";
  String fileVar = String("/" + fileName + ".txt");
  //    f = SPIFFS.open("/f.txt", "r");
  f = SPIFFS.open(fileVar, "r");
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


void loadSpiffsImage2(String imgPath, uint8_t* message, int num) {
  if (!preloaded) { //need to load once per image change here, not every time it's too slow!
    g = SPIFFS.open(imgPath, "r");
    //    g = SPIFFS.open("/f.txt", "r");
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

void loadPatternChooser() {
  
if(pattern == 1){
  
} else if (pattern == 2) {
    //    String imgToShow1 = "/f.txt";
    //    String imgToShow2 = "/g.txt";
    //    String imgToShow3 = "/h.txt";
    //    String imgToShow4 = "/i.txt";
    //    String imgToShow5 = "/j.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow1, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow2, message2Data, 2);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow3, message3Data, 3);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow4, message4Data, 4);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow5, message5Data, 5);
  } else if (pattern == 3) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/a.txt";
    //    String imgToShow7 = "/b.txt";
    //    String imgToShow8 = "/c.txt";
    //    String imgToShow9 = "/d.txt";
    //    String imgToShow10 = "/e.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow3, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow4, message2Data, 2);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow8, message3Data, 3);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow9, message4Data, 4);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow10, message5Data, 5);
  } else if (pattern == 4) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/k.txt";
    //    String imgToShow7 = "/l.txt";
    //    String imgToShow8 = "/m.txt";
    //    String imgToShow9 = "/n.txt";
    //    String imgToShow10 = "/o.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow5, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow6, message2Data, 2);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow13, message3Data, 3);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow14, message4Data, 4);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow15, message5Data, 5);
  } else if (pattern == 5) { //enough memory for 5 pics at a time, so load 5 different ones into the same arrays!
    //    String imgToShow6 = "/k.txt";
    //    String imgToShow7 = "/l.txt";
    //    String imgToShow8 = "/m.txt";
    //    String imgToShow9 = "/n.txt";
    //    String imgToShow10 = "/o.txt";
    preloaded = false;
    loadSpiffsImage2(imgToShow7, message1Data, 1);
    preloaded = false;
    loadSpiffsImage2(imgToShow8, message2Data, 2);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow18, message3Data, 3);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow19, message4Data, 4);
//    preloaded = false;
//    loadSpiffsImage2(imgToShow20, message5Data, 5);
  } 
    picToShow = 1; //back to first pic, to sync poi

}

