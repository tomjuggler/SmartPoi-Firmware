void eepromBrightnessChooser(int addr) {
  ////////////////////////////////////////////////////////Eeprom Brightness///////////////////////////////////////////////////////////////////////

  //EEPROM.write(15, newBrightness); //testing only
  //EEPROM.commit();
  int readBRTeprom = int(EEPROM.read(addr)); //read channel info and change from ascii
  newBrightness = readBRTeprom; //monitor this, had some problems with ascii
  if (newBrightness > 254 || newBrightness < 1) {
    EEPROM.write(15, 20); //set back to default dim brightness 
    newBrightness = 20;
    ////Serial.println("EEProm error, newBrightness reset to 200");
  }
  else {
    ////Serial.print("newBrightness set to ");
    ////Serial.println(newBrightness);
  }

  FastLED.setBrightness(  newBrightness ); //should I be removing this becos https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
  FastLED.showColor( CRGB::Black );

}

void eepromRouterOptionChooser(int addr){
  int newRouter = int(EEPROM.read(addr));
 if (newRouter == 0 || newRouter > 1) { //this should take care of first run as well, when eeprom is not set (usually 255 or 0)
        newRouter = 0;
        routerOption = false;
      } else { // 1 for router
        routerOption = true;
      }

      EEPROM.write(100, newRouter);
//      EEPROM.commit(); //this is done later I think
}

void eepromWifiModeChooser(int addr) {
  ///////////////////////////////////////wifiModeChooser/////////////////////////////////////////////////////////////////////////////
  if(routerOption){
    wifiModeChooser = int(EEPROM.read(addr));
    wifiModeChooser++;
    if (wifiModeChooser == 2) { 
      EEPROM.write(5, 2); //save
    }
    if (wifiModeChooser > 2) {
      wifiModeChooser = 1;
      EEPROM.write(5, 1); //set back to default
      wifiModeChooser = 1;
    }
    if (wifiModeChooser < 1) { //never happens, except for first run...
      wifiModeChooser = 1;
      EEPROM.write(5, 1); //set back to default
      wifiModeChooser = 1;
    }
  } else{ //skip router option
//    EEPROM.write(5, 1); //not necessary
    wifiModeChooser = 1; //1 means main AP mode, with auxillary connected to main.
  }



  ///////////////////////////////////////end wifi mode chooser////////////////////////////////////////////////////////////////////////////////

}

void eepromPatternChooser(int addr) {
  //EEProm Pattern chooser: //changes every time you restart poi //now with 3 options!
  //soon to be user configurable via the app, remembers which pattern you picked and starts up with that
  patternChooser = int(EEPROM.read(addr));
  ////Serial.print("read PatternChooser as: ");
  ////Serial.println(patternChooser);
//  patternChooser++; //no more incrementing, only choose from app and stick with it
if (patternChooser == 1) {
    pattern = 1;
    ////Serial.println("pictures chosen patternChooser == 1");
    EEPROM.write(10, 1); //save
  }
  if (patternChooser == 2) {
    pattern = 2;
    ////Serial.println("pictures chosen patternChooser == 2");
    EEPROM.write(10, 2); //save
  }
  if (patternChooser == 3) {
    pattern = 3;
    ////Serial.println("pictures chosen patternChooser == 3");
    EEPROM.write(10, 3); //save
  }
  if (patternChooser == 4) {
    pattern = 4;
    ////Serial.println("pictures chosen patternChooser == 4");
    EEPROM.write(10, 4); //save
  }
  if (patternChooser == 5) {
    pattern = 5;
    ////Serial.println("pictures chosen patternChooser == 5");
    EEPROM.write(10, 5); //save
  }
  if(patternChooser == 6){ //hard coded, need variable here to add more patterns?
    ////Serial.println("pictures chosen patternChooser == 6");
    EEPROM.write(10, 6); //save
    readAnotherPatternEEProm();
  }
  if (patternChooser > 6) {
    ////Serial.println("patternChooser > 6");
    EEPROM.write(10, 1); //set back to default
    pattern = 1;
//    patternChooser = 1; //set in above function
  }
  if (patternChooser < 1) {
    ////Serial.println("patternChooser < 1");
    EEPROM.write(10, 1); //set back to default
    pattern = 1;
  }

  //  patternChooser = 1; //for testing only
  ////Serial.println("patternChooser is: ");
  ////Serial.println(patternChooser);
 
  //////////////////////////////////////////////////////////end Pattern Chooser///////////////////////////////////////////
}

void readAnotherPatternEEProm(){
  //EEProm Pattern chooser: //changes every time you restart poi //now with 3 options!
  //soon to be user configurable via the app, remembers which pattern you picked and starts up with that
  pattern = int(EEPROM.read(11));
  ////Serial.print("read pattern as: ");
  ////Serial.println(pattern);
  pattern++;
  if (pattern == 2) {
    ////Serial.println("shuffle all chosen pattern == 2");
    EEPROM.write(11, 2); //save
  }
  if (pattern == 3) {
    ////Serial.println("shuffle all chosen pattern == 3");
    EEPROM.write(11, 3); //save
  }
  if (pattern == 4) {
    ////Serial.println("shuffle all chosen pattern == 4");
    EEPROM.write(11, 4); //save
  }
  if (pattern == 5) {
    ////Serial.println("pictures chosen patternChooser == 5");
    EEPROM.write(11, 5); //save
  }
  if (pattern > 5) {
    ////Serial.println("pattern > 5");
    EEPROM.write(11, 1); //set back to default
    pattern = 1;
  }
  if (pattern < 1) {
    ////Serial.println("pattern < 1");
    EEPROM.write(11, 1); //set back to default
    pattern = 1;
  }

  //  patternChooser = 1; //for testing only
  ////Serial.println("pattern is now: ");
  ////Serial.println(pattern);
//  EEPROM.commit(); //doing this already in setup()
  //////////////////////////////////////////////////////////end Pattern Chooser///////////////////////////////////////////

}

//Todo: add interval load, save and update here
void eepromReadChannelAndAddress(int addr1, int addr2, int addr3, int addr4, int addr5) {
  ////////////////////////////////////////////////////////////////EEPROM READ CHANNEL AND ADDRESS://////////////////////////////////////////////////////////////////
  int readAPEeprom = int(EEPROM.read(addr1)); //read channel info (from EEPROM13) and change from ascii
  apChannel = readAPEeprom; //monitor this, had some problems with ascii

  //  if (apChannel > 11 || apChannel < 1) { //11 channels only for US!
  if (apChannel > 13 || apChannel < 1) { //13 for SA, is it 14 for Australia?
    EEPROM.write(13, 1); //clearing
    apChannel = 4;
    ////Serial.println("EEProm error, ApChannel reset to 4");
  }
  else {
    ////Serial.print("apChannel set to ");
    ////Serial.println(apChannel);
  }
  //SET IP:
  uint8_t readAddrEeprom = uint8_t(EEPROM.read(addr2)); //read Address info (from EEPROM14) and change from ascii
  addrNumD = readAddrEeprom; //
  ////Serial.print("addrNumD set to ");
  ////Serial.println(addrNumD);
  ////Serial.println(" ");

  readAddrEeprom = uint8_t(EEPROM.read(addr3)); //read Address info (from EEPROM14) and change from ascii
  addrNumA = readAddrEeprom; //
  ////Serial.print("addrNumA set to ");
  ////Serial.println(addrNumA);
  ////Serial.println(" ");

  readAddrEeprom = uint8_t(EEPROM.read(addr4)); //read Address info (from EEPROM14) and change from ascii
  addrNumB = readAddrEeprom; //
  ////Serial.print("addrNumB set to ");
  ////Serial.println(addrNumB);
  ////Serial.println(" ");

  readAddrEeprom = uint8_t(EEPROM.read(addr5)); //read Address info (from EEPROM14) and change from ascii
  addrNumC = readAddrEeprom; //
  ////Serial.print("addrNumC set to ");
  ////Serial.println(addrNumC);
  ////Serial.println(" ");
  ////Serial.print("apChannel is: ");
  ////Serial.println(apChannel);
  //////////////////////////////////////////////////END EEPROM READ CHANNEL AND ADDRESS/////////////////////////////////////////////////////////////////////////

}

void spiffsLoadSettings() {
  //////////////////////////////////////////////////////////////Read Settings//////////////////////////////////////////////////////////////////////////////////
  //from: http://www.esp8266.com/viewtopic.php?f=29&t=8194
  //causing out of memory shutdown errors!
  //how to not have these? need a good look at the code, remove some memory usage :)
  settings = LittleFS.open("/settings.txt", "r");
  //note: password is stored in plain text, security risk?
  //currently settings.txt avaliable on demand over http
  //  if (!settings) {
  //    FastLED.showColor( CRGB::Orange );
  //  } else {
  //    for (int i = 1; i <= 10; i++) { //was using <= 10 but was causing memory usage instability
  ////      S[i] = settings.readStringUntil('\n');
  //      //Serial.print(i);
  //      //Serial.print(": ");
  //      //Serial.println(settings.readStringUntil('\n'));
  //    }

  //load spiffs into char arrays for router setup:
  // Define
  Field = settings.readStringUntil('\n');

  // Length (with one extra character for the null terminator)
  //int router_len = router.length() + 1;
  // Prepare the character array (the buffer)
  //below should be part of Struct
  char router_array[Field.length() + 1];
  // Copy it over
  Field.toCharArray(router_array, Field.length() + 1);

  //  if (Field[1] == ssid[1]) {
  //    //Serial.println("ssid same same");
  //  }

  // delay(40);
  //and again for password:
  // Define
  Field = settings.readStringUntil('\n');
  // Length (with one extra character for the null terminator)
  //int pwd_len = pwd.length() + 1;
  // Prepare the character array (the buffer)
  char pwd_array[Field.length() + 1];
  // Copy it over
  Field.toCharArray(pwd_array, Field.length() + 1);

  ////Serial.print("pwd is: ");
  ////Serial.println(Field);
  ////Serial.print("pwd_array is: ");
  ////Serial.println(pwd_array);
  ////Serial.print("str() is: ");
  ////Serial.println(String(pwd_array));
//  String str = str((char*)pwd_array); //s
  //Serial.print("str((char* is: ");
  //Serial.println(str);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //unless I am sorely mistaken, the below code is not being used... using eeprom for ip instead right now
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Field = settings.readStringUntil('\n');
  char ipField1Arr[Field.length() + 1];
  Field.toCharArray(ipField1Arr, Field.length() + 1);

  Field = settings.readStringUntil('\n');
  char ipField2Arr[Field.length() + 1];
  Field.toCharArray(ipField2Arr, Field.length() + 1);

  Field = settings.readStringUntil('\n');
  char ipField3Arr[Field.length() + 1];
  Field.toCharArray(ipField3Arr, Field.length() + 1);

  Field = settings.readStringUntil('\n');
  char ipField4Arr[Field.length() + 1];
  Field.toCharArray(ipField4Arr, Field.length() + 1);

  //Serial.print("FIELD 1 is: ");
  //Serial.println(ipField1Arr);
  //Serial.print("FIELD 2 is: ");
  //Serial.println(ipField2Arr);
  //Serial.print("FIELD 3 is: ");
  //Serial.println(ipField3Arr);
  //Serial.print("FIELD 4 is: ");
  //Serial.println(ipField4Arr);
*/
  // delay(40);


  settings.close();
  // delay(100);
  ///////////////////////////////////////////////////////////////End Read Settings////////////////////////////////////////////////////////////////////////////
  //more setup below, has to be inside this function...???
  wifiChooser(router_array, pwd_array);
  webServerSetupLogic(router_array, pwd_array); //why not do this only on AP mode?
}

void wifiChooser(char router_array[], char pwd_array[]) {
  ////////////////////////////////////////////////////Wifi Chooser: /////////////////////////////////////////////////////////////////////////////////////

  if (wifiModeChooser == 1) { //main AP mode, with auxillary connected to main.
    //this may is all that is needed to put main and auxillary poi on one code base: 
    if (auxillary) {
      //Serial.println("auxillary POI");
      WiFi.mode(WIFI_STA); 
//      WiFiMulti.addAP(apName, apPass);
      WiFi.begin(apName, apPass);
      WiFi.config(apIPauxillary, ipGatewayauxillary, ipSubnet, ipGatewayauxillary);
      //change ipGateway for connect to AP?
      //do we want this anymore? ...leaving for now - no signal no picture
//      WiFi.config(apIPauxillary, ipDns, ipGatewayauxillary);
      //Serial.print("Connecting to AP, IP should be: ");
      //Serial.println(apIPauxillary);
//      while (WiFiMulti.run() != WL_CONNECTED) {
while (WiFi.status() != WL_CONNECTED) {
                //Serial.print(".");
        FastLED.delay(50); //was set to 500, why? todo: does FastLED.delay() work better?
      }
    } else {
      //Serial.println("main POI");
      WiFi.mode(WIFI_AP);
      //WiFi.softAPConfig(IPAddress(192, 168, 1, addrNumD), IPAddress(192, 168, 1, addrNumD), IPAddress(255, 255, 255, 0));
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(apName, apPass, apChannel); //use pre-set values here
      dnsServer.start(DNS_PORT, "*", apIP); //AP mode only, surely??
    }
  } else { //both main and auxillary the same, connected to pre-defined Router
    //Serial.println("ROUTER");
    //////////////////////////////////////////////////////////Connect to Router here://///////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////Input Settings from Spiffs: ////////////////////////////////////////////////////////////////////
    WiFi.mode(WIFI_STA); //disable AP on this one
    //    WiFiMulti.addAP(router_array, pwd_array); //previously read these from Spiffs
    //    //Serial.print("router_array is: ");
    //    //Serial.println(router_array);
    //    //Serial.print("pwd_array is: ");
    //    //Serial.println(pwd_array);



    //WiFiMulti.addAP("RouterName", "RouterPassword"); //default test


    //Serial.println();
    //Serial.println();
    //Serial.print("Connecting to ");
    //Serial.println(router_array);
    //Serial.print("pwd is: ");
    //Serial.println(pwd_array);
    //Serial.println();
    //Serial.println();
    //Serial.print("Wait for WiFi... ");
    ///////////////////////////////////////////////END Read Router Settings from Spiffs /////////////////////////////////////////////////////////////

    //test Delete:
    //    tmpIP = IPAddress(192, 168, 8, addrNumD);
    //tmpIP = IPAddress(192, 168, 8, 78);
    tmpIP = IPAddress(addrNumA, addrNumB, addrNumC, addrNumD);
    //Serial.print("tmpIP is: ");
    //Serial.println(tmpIP);
    tmpGateway = IPAddress(addrNumA, addrNumB, addrNumC, 1); //make last another variable? YA!
    //Serial.print("tmpGateway is: ");
    //Serial.println(tmpGateway);

    WiFi.config(tmpIP, tmpGateway, ipSubnet, tmpGateway);
//    WiFi.config(tmpIP, ipGateway, ipSubnet, ipGateway);
    //    WiFi.config(ip, ipDns, ipGateway); //probably need to be able to change these as well
    //    int connectCount;
    //moved:
    WiFiMulti.addAP(router_array, pwd_array); //previously read these from Spiffs
    //Serial.print("router_array is: ");
    //Serial.println(router_array);
    //Serial.print("pwd_array is: ");
    //Serial.println(pwd_array);
    //
    byte wifiConnectAttemptCount = 0;
    byte maxWifiConnectAttemptCount = 18; //this should be in adjustable settings
    
    while (WiFiMulti.run() != WL_CONNECTED) {
///////////////////indicate://///////////////////////////////
       FastLED.showColor(CRGB::Black); 
        for(int i = 0; i < uploadCounter; i++){
          leds[i*2] = CRGB::Green; //For 18 tries, spacing out indicator
        }
        FastLED.show(); //not working? Why?
        //Serial.println("uploadCounter is: "); //Serial.println(uploadCounter);
        uploadCounter++;
//////////////////end indicate////////////////////////////////
        
        //definitely shouldn't block forever here...
        //Serial.print(".");
        delay(500); //delay??? todo: try FastLED.delay() here, also 50ms instead of 500...
        wifiConnectAttemptCount++;
        if(wifiConnectAttemptCount > maxWifiConnectAttemptCount){
          ESP.restart(); //don't forget this won't work the first time, needs manual restart!!!!!!!!
        }
      }
//if not connected in time, restart poi!
//untested idea from https://github.com/esp8266/Arduino/issues/1959:
///well this didn't work! previous loop is infinite...
//while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    //Serial.println("Fail connecting");
//    delay(1000);
//    ESP.restart();
//  }
    uploadCounter = 1;
    //Serial.println("");
    //Serial.println("WiFi connected");
    //Serial.println("IP address: ");
    //Serial.println(WiFi.localIP());
  }

  //////////////////////////////////////////////////////////////END WIFI CHOOSER TEST////////////////////////////////////////////////////////////////////////////
}

void fastLEDInit() {
  
  ////////////////////////////////////////////////Fast LED Setup: ////////////////////////////////////////////////////////////////////////////////////////////////
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS); //DATA_RATE_MHZ(8)
  //  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  newBrightness ); //should be low figure here, for startup battery saving...
  
  FastLED.showColor( CRGB::Black );
  
 
  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  ///////////////////////////////////////////////////////////////////////end FastLED setup///////////////////////////////////////////////////////////////////

}

void fastLEDIndicate(){
   //indicate wifi mode:
//  //Serial.println("FASTLED NOW");
  if (wifiModeChooser == 1) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Red;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Red;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Red;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Red;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }

    FastLED.delay(10);


  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Green;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }

    FastLED.delay(10);

  }

  FastLED.showColor( CRGB::Black );
}

void fastLEDIndicateFast(){
   //indicate wifi mode:
//  //Serial.println("FASTLED NOW");
  if (wifiModeChooser == 1) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Magenta;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Magenta;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Magenta;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Magenta;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }

    // FastLED.delay(10);


  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Green;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }

    // FastLED.delay(10);

  }

  FastLED.showColor( CRGB::Black );
}
