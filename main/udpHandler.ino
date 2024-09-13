void handleUDP(){
  if (currentMillis2 - previousMillis2 > interval * 2) { //message received after long wait, may be config message? check it
      // save the last time you checked the time
      previousMillis2 = currentMillis2;
      //check the UDP packet for correct config message values later:
      checkit = true;
    }
    previousMillis = currentMillis;
    state = 0; //udp good signal state 

    // read the packet into packetBufffer
    len = Udp.read(packetBuffer, NUM_PX);
    if (len > 0) packetBuffer[len] = 0;
    ////////////////////////////////////FastLED Code://///////////
    for (int i = 0; i < NUM_PX; i++)
    {
      
      ///////////////////////////////////////////////////////settings mode////////////////////////////////////////////////////////////////////////
      if (checkit) { //need some sort of cylon thingy here to show that poi is doing something!!!
        Y = packetBuffer[i];

        switch (i) {
          case 0:
            if (Y == 0) { //Y is 0 at 1st packetBuffer array member
              ////Serial.println("checked 0, signal received");
            }
            else {
              checkit = false; //not on track, try again next time
            }
            break;
          case 1:
            if (Y == 1) {
              ////Serial.println("checked 1, signal received");
            }
            else {
              checkit = false; //not on track, try again next time
            }
            break;
          case 2:
            if (Y == 2) { //default 2
              ////Serial.println("checked 2, Brightness change signal received");
            } else if (Y == 3) {
              ////Serial.println("checked 2, Channel change signal received");
              channelChange = true;
            } //add another option for pattern change here?

            else {
              checkit = false; //not on track, try again next time
            }
            break;
          case 3:
            if (channelChange) {
              if ((int)Y > 11 || (int)Y < 1) {
                //wrong channel information received - error checking
              } else {
                EEPROM.write(13, 1); //clearing
                EEPROM.commit();
                int newChannel2; //temp variable
                newChannel2 = int(Y);
                EEPROM.write(13, newChannel2);
                EEPROM.commit();
                // Serial.print("channel changed to :");
                // Serial.println(newChannel2);
                // ESP.restart(); //not using this right now but see https://github.com/esp8266/Arduino/issues/1722#issuecomment-192829825
                FastLED.showColor(CRGB::Magenta); //visual indicator
                channelChange = false;
                checkit = false; //Finished settings, exit
              }
            } else {
              //set eeprom:
              ////Serial.print("received Y: ");
              ////Serial.println(Y);
              EEPROM.write(15, int(Y)); //set for next time
              EEPROM.commit();
              newBrightness = int(Y);
              FastLED.setBrightness(  newBrightness );
              ////Serial.println(" ");
              ////Serial.print("Brightness now set to: ");
              ////Serial.println(newBrightness);
              FastLED.showColor(CRGB::Blue); //visual indicator
              checkit = false; //Finished settings, exit
            }
            break;
          default:
            
            break;
            //etc for 32, can write values according to signal received

        }

      }
      ///////////////////////////////////////////////////////////////////////////////////////end settings mode///////////////////////////

      ///////////////////////////////convert byte to signed byte:////
      X = packetBuffer[i] - 127;
      /////////////////////////////end convert byte to signed byte////

     R1 = (X & 0xE0);
      leds[i].r = R1; //
     G1 =  ((X << 3) & 0xE0);
      leds[i].g = G1;
     M1 = (X << 6);
      leds[i].b = M1;
      // FastLED.delay(1);
      // Serial.print(R1); // Serial.print(", "); // Serial.print(G1); // Serial.print(", "); // Serial.println(M1);
    }

    //FastLED.delay(2); //not just for emulator!
     LEDS.show();
//    delayMicroseconds(200);
//    FastLED.delay(1);
    ///////////////////////////////////end FastLed Code//////////////

    // send a reply, to the IP address and port that sent us the packet we received
    //    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //    Udp.write(ReplyBuffer);
    //    Udp.endPacket();
}
