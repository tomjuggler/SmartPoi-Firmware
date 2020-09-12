

//we don't need this! 
/*
void loadPatternChooser() {
 if (reload)
  {
    for (int i = 0; i < 10; i++)
    {
      image[0] = images[i]; //abcde...
      bin[0] = images[i]; //abcde...
      // switch (i)
      // {
      // case 0:
      //    image[0] = 'a';
      //   bin[0] = 'a';
      //   break;
      // case 1:
      //   image = "b.txt";
      //   bin = "b.bin";
      //   break;
      // case 2:
      //   image = "c.txt";
      //   bin = "c.bin";
      //   break;
      // case 3:
      //   image = "d.txt";
      //   bin = "d.bin";
      //   break;
      // case 4:
      //   image = "e.txt";
      //   bin = "e.bin";
      //   break;
      // case 5:
      //   image = "f.txt";
      //   bin = "f.bin";
      //   break;
      // case 6:
      //   image = "g.txt";
      //   bin = "g.bin";
      //   break;
      // case 7:
      //   image = "h.txt";
      //   bin = "h.bin";
      //   break;
      // case 8:
      //   image = "i.txt";
      //   bin = "i.bin";
      //   break;
      // case 9:
      //   image = "j.txt";
      //   bin = "j.bin";
      //   break;
      
      // }

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
  
    imageToUse = 0; //back to first pic, to sync poi
}
*/