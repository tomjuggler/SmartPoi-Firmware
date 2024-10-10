/**
 * @brief Retrieves and sets the brightness from EEPROM.
 *
 * Reads the brightness value from EEPROM, checks for validity, and sets the brightness accordingly.
 *
 * @param addr The EEPROM address where the brightness value is stored.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Brightness is set to the retrieved value or a default value if invalid.
 *
 * @note This function is used to initialize the brightness setting from EEPROM.
 * @note Uses global variables such as newBrightness.
 * @note Calls other functions such as EEPROM.read(), EEPROM.write(), and FastLED.setBrightness().
 */
void eepromBrightnessChooser(int addr)
{
  ////////////////////////////////////////////////////////Eeprom Brightness///////////////////////////////////////////////////////////////////////

  // EEPROM.write(15, newBrightness); //testing only
  // EEPROM.commit();
  int readBRTeprom = int(EEPROM.read(addr)); // read channel info and change from ascii
  newBrightness = readBRTeprom;              // monitor this, had some problems with ascii
  if (newBrightness > 254 || newBrightness < 1)
  {
    EEPROM.write(15, 20); // set back to default dim brightness
    newBrightness = 20;
    ////Serial.println("EEProm error, newBrightness reset to 200");
  }
  else
  {
    newBrightness = 20; // just start with low brightness please, better this way
    ////Serial.print("newBrightness set to ");
    ////Serial.println(newBrightness);
  }

  FastLED.setBrightness(newBrightness); // should I be removing this becos https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
  FastLED.showColor(CRGB::Black);
}

/**
 * @brief Retrieves and sets the router option from EEPROM.
 *
 * Reads the router option value from EEPROM, checks for validity, and sets the router option accordingly.
 *
 * @param addr The EEPROM address where the router option value is stored.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Router option is set to the retrieved value or a default value if invalid.
 *
 * @note This function is used to initialize the router option setting from EEPROM.
 * @note Uses global variables such as routerOption.
 * @note Calls other functions such as EEPROM.read() and EEPROM.write().
 * @note Valid router option values are 0 (disabled) or 1 (enabled).
 */
void eepromRouterOptionChooser(int addr)
{
  int newRouter = int(EEPROM.read(addr));
  if (newRouter == 0 || newRouter > 1)
  { // this should take care of first run as well, when eeprom is not set (usually 255 or 0)
    newRouter = 0;
    routerOption = false;
  }
  else
  { // 1 for router
    routerOption = true;
  }

  EEPROM.write(100, newRouter);
  //      EEPROM.commit(); //this is done later I think
}

/**
 * @brief Retrieves and sets the Wi-Fi mode from EEPROM.
 *
 * Reads the Wi-Fi mode value from EEPROM, checks for validity, and sets the Wi-Fi mode accordingly.
 *
 * @param addr The EEPROM address where the Wi-Fi mode value is stored.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Wi-Fi mode is set to the retrieved value or a default value if invalid.
 *
 * @note This function is used to initialize the Wi-Fi mode setting from EEPROM.
 * @note Uses global variables such as wifiModeChooser and routerOption.
 * @note Calls other functions such as EEPROM.read() and EEPROM.write().
 * @note Valid Wi-Fi mode values are 1 (main AP mode) and 2 (auxiliary AP mode).
 * @note If routerOption is false, Wi-Fi mode is set to 1 (main AP mode) by default.
 * @note wifiModeChooser used in wifiChooser()
 */
void eepromWifiModeChooser(int addr)
{  
  if (routerOption) // ok this is set to toggle between two modes - I guess to make sure I don't get stuck?
  { 
    wifiModeChooser = int(EEPROM.read(addr));
    wifiModeChooser++; // take this out and it stays on router?
    if (wifiModeChooser == 2)
    {
      EEPROM.write(5, 2); // save
    }
    if (wifiModeChooser > 2)
    {
      wifiModeChooser = 1;
      EEPROM.write(5, 1); // set back to default
      wifiModeChooser = 1;
    }
    if (wifiModeChooser < 1)
    { // never happens, except for first run...
      wifiModeChooser = 1;
      EEPROM.write(5, 1); // set back to default
      wifiModeChooser = 1;
    }
  }
  else
  {                      // skip router option
    wifiModeChooser = 1; // 1 means main AP mode, with auxillary connected to main.
  }
}

/**
 * @brief Retrieves and sets the pattern from EEPROM.
 *
 * Reads the pattern value from EEPROM, checks for validity, and sets the pattern accordingly.
 *
 * @param addr The EEPROM address where the pattern value is stored.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Pattern is set to the retrieved value or a default value if invalid.
 *
 * @note This function is used to initialize the pattern setting from EEPROM.
 * @note Uses global variables such as patternChooser and pattern.
 * @note Calls other functions such as EEPROM.read(), EEPROM.write(), and readAnotherPatternEEProm().
 * @note Valid pattern values are 1-6, with 1 being the default.
 * @note If patternChooser is outside the valid range, pattern is set to 1 (default).
 */
void eepromPatternChooser(int addr)
{
  patternChooser = int(EEPROM.read(addr));
  if (patternChooser == 1)
  {
    pattern = 1;
    EEPROM.write(10, 1); 
  }
  if (patternChooser == 2)
  {
    pattern = 2;
    EEPROM.write(10, 2); 
  }
  if (patternChooser == 3)
  {
    pattern = 3;
    EEPROM.write(10, 3); 
  }
  if (patternChooser == 4)
  {
    pattern = 4;
    EEPROM.write(10, 4); 
  }
  if (patternChooser == 5)
  {
    pattern = 5;
    EEPROM.write(10, 5); 
  }
  if (patternChooser == 6) // hard coded, todo: need variable here to add more patterns?
  { 
    EEPROM.write(10, 6); 
    readAnotherPatternEEProm(); // on/off switcher
  }
  if (patternChooser > 6)
  {
    EEPROM.write(10, 1); // set back to default
    pattern = 1;
  }
  if (patternChooser < 1)
  {
    EEPROM.write(10, 1); // set back to default
    pattern = 1;
  }
}

/**
 * @brief Reads and increments the pattern from EEPROM.
 *
 * Reads the pattern value from EEPROM, increments it, and writes it back to EEPROM.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Pattern is incremented and written back to EEPROM.
 *
 * @note This function is used to cycle through patterns on each restart.
 * @note Uses global variable pattern.
 * @note Calls other function EEPROM.write().
 * @note Valid pattern values are 1-5, with 1 being the default.
 * @note If pattern is outside the valid range, it is set back to 1 (default).
 */
void readAnotherPatternEEProm()
{
  // EEProm Pattern chooser: changes every time you restart poi 
  pattern = int(EEPROM.read(11));
  pattern++;
  if (pattern == 2)
  {
    EEPROM.write(11, 2); 
  }
  if (pattern == 3)
  {
    EEPROM.write(11, 3); 
  }
  if (pattern == 4)
  {
    EEPROM.write(11, 4); 
  }
  if (pattern == 5)
  {
    EEPROM.write(11, 5); 
  }
  if (pattern > 5)
  {
    EEPROM.write(11, 1); // set back to default
    pattern = 1;
  }
  if (pattern < 1)
  {
    EEPROM.write(11, 1); // set back to default
    pattern = 1;
  }
}

/**
 * @brief Reads channel and address information from EEPROM.
 *
 * Retrieves channel and address values from EEPROM, performs validation, and sets global variables accordingly.
 *
 * @param addr1 EEPROM address for channel information.
 * @param addr2 EEPROM address for IP address octet D.
 * @param addr3 EEPROM address for IP address octet A.
 * @param addr4 EEPROM address for IP address octet B.
 * @param addr5 EEPROM address for IP address octet C.
 *
 * @return None
 *
 * @pre EEPROM is initialized and ready for use.
 * @post Channel and address information is read and validated.
 *
 * @note only apChannel is actually relevant currently.
 * @note This function is used to initialize channel and address settings from EEPROM.
 * @note Uses global variables apChannel, addrNumD, addrNumA, addrNumB, and addrNumC.
 * @note Calls other function EEPROM.read().
 * @note Valid channel values are 1-11 in USA.
 * @note If channel is outside the valid range, it is set to 4 (default).
 */
void eepromReadChannelAndAddress(int addr1, int addr2, int addr3, int addr4, int addr5)
{
  int readAPEeprom = int(EEPROM.read(addr1)); // read channel info (from EEPROM13) and change 
  apChannel = readAPEeprom;                   

  if (apChannel > 11 || apChannel < 1) // 13 for SA, is it 14 for Australia? 11 for US
  {                      
    EEPROM.write(13, 1); 
    apChannel = 1;
  }
  else
  {
    
  }
  // SET IP:
  uint8_t readAddrEeprom = uint8_t(EEPROM.read(addr2)); 
  addrNumD = readAddrEeprom;   
  readAddrEeprom = uint8_t(EEPROM.read(addr3)); 
  addrNumA = readAddrEeprom;                   
  readAddrEeprom = uint8_t(EEPROM.read(addr4)); 
  addrNumB = readAddrEeprom;                    
  readAddrEeprom = uint8_t(EEPROM.read(addr5));
  addrNumC = readAddrEeprom;                    
}

/**
 * @brief Loads settings from LittleFS file system.
 *
 * Reads settings from /settings.txt file and stores them in global variables.
 *
 * @return None
 *
 * @pre LittleFS file system is initialized and ready for use.
 * @post Settings are read and stored in global variables.
 *
 * @note This function is used to initialize settings from LittleFS.
 * @note Uses global variables router_array and pwd_array.
 * @note Calls other functions wifiChooser() and webServerSetupLogic().
 * @warning Password is stored in plain text, posing a security risk.
 * @todo Consider encrypting password for security.
 */
void littleFSLoadSettings()
{
  settings = LittleFS.open("/settings.txt", "r");
  // note: password is stored in plain text, security risk?
  // currently settings.txt avaliable on demand over http!

  // load spiffs into char arrays for router setup:
  //  Define
  Field = settings.readStringUntil('\n');

  // Length (with one extra character for the null terminator)
  // int router_len = router.length() + 1;
  // Prepare the character array (the buffer)
  // below should be part of Struct
  char router_array[Field.length() + 1];
  // Copy it over
  Field.toCharArray(router_array, Field.length() + 1);

  // and again for password:
  // Define
  Field = settings.readStringUntil('\n');
  // Length (with one extra character for the null terminator)
  // int pwd_len = pwd.length() + 1;
  // Prepare the character array (the buffer)
  char pwd_array[Field.length() + 1];
  // Copy it over
  Field.toCharArray(pwd_array, Field.length() + 1);

  settings.close();
  // delay(100);
  ///////////////////////////////////////////////////////////////End Read Settings////////////////////////////////////////////////////////////////////////////
  // more setup below, has to be inside this function...???
  wifiChooser(router_array, pwd_array);
  webServerSetupLogic(router_array, pwd_array); // why not do this only on AP mode?
}

/**
 * @brief Checks files in the LittleFS file system during setup.
 *
 * This function iterates through all files in the root directory ("/"), checks their size,
 * and deletes them if they exceed the maximum allowed size (maxPX). It also checks for
 * file corruption by attempting to read a small portion of each file.
 *
 * @param None
 * @return None
 *
 * @pre LittleFS must be initialized and mounted before calling this function.
 * @post Files exceeding maxPX size or detected as corrupted will be deleted.
 *
 * @note This function uses Serial for printing debug messages.
 * @note maxPX defines the maximum allowed file size.
 */
void checkFilesInSetup()
{
  Dir dir = LittleFS.openDir("/");

  while (dir.next())
  {
    String fileName = dir.fileName();
    File file = dir.openFile("r");

    // Check file size
    size_t fileSize = file.size();
    Serial.print("Checking file: ");
    Serial.print(fileName);
    Serial.print(" - Size: ");
    Serial.println(fileSize);

    // If file size exceeds maxPX, delete it
    if (fileSize > maxPX)
    {
      Serial.print("File too large. Deleting: ");
      Serial.println(fileName);
      LittleFS.remove(fileName);
    }
    else
    {
      // Try to read a small portion of the file to detect corruption
      uint8_t buffer[10];
      if (file.read(buffer, sizeof(buffer)) != sizeof(buffer))
      {
        // File might be corrupted, unable to read
        Serial.print("Corrupted file detected. Deleting: ");
        Serial.println(fileName);
        LittleFS.remove(fileName);
      }
      else
      {
        Serial.println("File is valid.");
      }
    }

    file.close();
  }
  size_t remainingSpace = getRemainingSpace();
  Serial.print("Space remaining on disk: ");
  Serial.println(remainingSpace);
}

/**
 * @brief Configures Wi-Fi settings based on the selected mode.
 *
 * Sets up Wi-Fi in either Access Point (AP) mode, Station mode, or connects to a pre-defined router.
 *
 * @param router_array Router SSID (used in Station mode).
 * @param pwd_array Router password (used in Station mode).
 *
 * @return None
 *
 * @pre Wi-Fi library is initialized and ready for use.
 * @post Wi-Fi settings are configured according to the selected mode.
 *
 * @note This function is used to configure Wi-Fi settings based on the wifiModeChooser variable.
 * @note Uses global variables wifiModeChooser, auxillary, apName, apPass, apIP, apChannel, addrNumA, addrNumB, addrNumC, and addrNumD.
 * @note Calls other functions WiFi.mode(), WiFi.begin(), WiFi.config(), WiFi.softAP(), dnsServer.start(), and WiFiMulti.addAP().
 * @note Supports up to 18 connection attempts in Station mode.
 * @todo Consider implementing exponential backoff for connection attempts.
 */
void wifiChooser(char router_array[], char pwd_array[])
{
  if (wifiModeChooser == 1)
  { // main AP mode, with auxillary connected to main.
    // this is all that is needed to put main and auxillary poi on one code base:
    if (auxillary)
    {
      // Serial.println("auxillary POI");
      WiFi.mode(WIFI_STA);
      WiFi.begin(apName, apPass);
      WiFi.config(apIPauxillary, ipGatewayauxillary, ipSubnet, ipGatewayauxillary);
      while (WiFi.status() != WL_CONNECTED)
      {
        if (millis() > 10000)
        {
          // don't wait if > 1 minute!
          break;
        }
        // Serial.print(".");
        FastLED.delay(50); // was set to 500, why? todo: does FastLED.delay() work better?
      }
      // LED on test:
      //  digitalWrite(LED_BUILTIN, LOW);
    }
    else
    { // main poi here
      // Serial.println("main POI");
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(apName, apPass, apChannel); // use pre-set values here
      dnsServer.start(DNS_PORT, "*", apIP);   // AP mode only, surely??
      // Serial.print("Wifi Setup: ");
      // Serial.println(apPass);
      // wait for station:
      while (WiFi.softAPgetStationNum() == 0)
      {
        if (millis() > 10000)
        {
          // don't wait if > 1 minute!
          break;
        }
        FastLED.delay(50);
      }
      // LED on test:
      //  digitalWrite(LED_BUILTIN, LOW);
    }
  }
  else
  { // both main and auxillary the same, connected to pre-defined Router
    // Serial.println("ROUTER");
    //////////////////////////////////////////////////////////Connect to Router here://///////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////Input Settings from Spiffs: ////////////////////////////////////////////////////////////////////
    WiFi.mode(WIFI_STA); // disable AP on this one
    tmpIP = IPAddress(addrNumA, addrNumB, addrNumC, addrNumD);
    tmpGateway = IPAddress(addrNumA, addrNumB, addrNumC, 1); // make last another variable? YA!
    WiFiMulti.addAP(router_array, pwd_array); 
    byte wifiConnectAttemptCount = 0;
    byte maxWifiConnectAttemptCount = 18; // this should be in adjustable settings

    while (WiFiMulti.run() != WL_CONNECTED)
    {
      ///////////////////indicate://///////////////////////////////
      FastLED.showColor(CRGB::Black);
      for (int i = 0; i < uploadCounter; i++)
      {
        leds[i * 2] = CRGB::Green; // For max 18 tries, spacing out indicator
      }
      FastLED.show();
      uploadCounter++;
      //////////////////end indicate////////////////////////////////

      delay(500); // delay??? todo: try FastLED.delay() here, also 50ms instead of 500... Must be a better way..?
      wifiConnectAttemptCount++;
      if (wifiConnectAttemptCount > maxWifiConnectAttemptCount)
      {
        ESP.restart(); // Note: this won't work the first time, needs manual restart!!!!!!!!
      }
    }
    
    uploadCounter = 1;
  }

}

/**
 * @brief Initializes FastLED library and sets up LED strip.
 *
 * Configures FastLED with the specified LED type (WS2812B), data pin (DATA_PIN), and color order (GRB), and sets the initial brightness (newBrightness) and color (black).
 *
 * @return None
 *
 * @pre FastLED library is included and ready for use.
 * @post FastLED is initialized and LED strip is set up.
 *
 * @note This function is used to initialize FastLED and set up the LED strip.
 * @note Uses global variables DATA_PIN, CLOCK_PIN, NUM_LEDS, and newBrightness.
 * @note Calls other functions FastLED.addLeds(), FastLED.setBrightness(), and FastLED.showColor().
 * @note Sets initial brightness to a low value for startup battery saving.
 */
void fastLEDInit()
{

  ////////////////////////////////////////////////Fast LED Setup: ////////////////////////////////////////////////////////////////////////////////////////////////
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS); //DATA_RATE_MHZ(8)
  //  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(newBrightness); // should be low figure here, for startup battery saving...

  FastLED.showColor(CRGB::Black);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  ///////////////////////////////////////////////////////////////////////end FastLED setup///////////////////////////////////////////////////////////////////
}

/**
 * @brief Indicates Wi-Fi mode using FastLED library.
 *
 * Displays a sequence of colors on the LED strip to indicate the current Wi-Fi mode.
 * If wifiModeChooser is 1, (AP Mode) displays a sequence of red or blue colors depending on the auxillary variable.
 * If wifiModeChooser is not 1, (Router Mode) displays a sequence of green colors.
 *
 * @return None
 *
 * @pre FastLED library is initialized and ready for use.
 * @post LED strip displays a sequence of colors indicating the Wi-Fi mode.
 *
 * @note This function is used to provide a visual indication of the Wi-Fi mode.
 * @note Uses global variables wifiModeChooser, auxillary, and NUM_LEDS.
 * @note Calls other function FastLED.show() and FastLED.delay().
 * @todo Consider simplifying the code by reducing repetition.
 */
void fastLEDIndicate()
{
  // indicate wifi mode:
  //  //Serial.println("FASTLED NOW");
  if (wifiModeChooser == 1)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      if (auxillary)
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      if (auxillary)
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      if (auxillary)
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      if (auxillary)
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }

    FastLED.delay(10);
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
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

  FastLED.showColor(CRGB::Black);
}

/**
 * @brief Indicates Wi-Fi mode using FastLED library with a faster sequence.
 *
 * Displays a faster sequence of colors on the LED strip to indicate the current Wi-Fi mode.
 * If wifiModeChooser is 1, (AP Mode) displays a sequence of magenta or cyan colors depending on the auxillary variable.
 * If wifiModeChooser is not 1, (Router Mode) displays a sequence of green colors.
 *
 * @return None
 *
 * @pre FastLED library is initialized and ready for use.
 * @post LED strip displays a faster sequence of colors indicating the Wi-Fi mode.
 *
 * @note This function is used to provide a faster visual indication of the Wi-Fi mode.
 * @note Uses global variables wifiModeChooser, auxillary, and NUM_LEDS.
 * @note Calls other function FastLED.show() and FastLED.delay().
 * @todo Consider simplifying the code by reducing repetition.
 */
void fastLEDIndicateFast()
{
  // indicate wifi mode:
  //  //Serial.println("FASTLED NOW");
  if (wifiModeChooser == 1)
  {
    for (int i = 0; i < NUM_LEDS; i += 2)
    {
      // Set the i'th led to whatever
      if (auxillary)
      {
        leds[i] = CRGB::Magenta;
      }
      else
      {
        leds[i] = CRGB::Cyan;
      }

      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(15);
    }
    for (int i = 0; i < NUM_LEDS; i += 2)
    {
      // Set the i'th led to whatever
      if (auxillary)
      {
        leds[i] = CRGB::Magenta;
      }
      else
      {
        leds[i] = CRGB::Cyan;
      }
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      FastLED.delay(15);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      leds[i] = CRGB::Black;
      // now that we've shown the leds, reset the i'th led to black
      if (auxillary)
      {
        leds[0] = CRGB::Magenta;
        leds[2] = CRGB::Magenta;
        leds[4] = CRGB::Magenta;
        leds[NUM_PX / 2] = CRGB::Magenta;
        leds[NUM_LEDS - 2] = CRGB::Magenta;
        leds[NUM_LEDS - 4] = CRGB::Magenta;
        leds[NUM_LEDS - 6] = CRGB::Magenta;
      }
      else
      {
        leds[0] = CRGB::Cyan;
        leds[2] = CRGB::Cyan;
        leds[4] = CRGB::Cyan;
        leds[NUM_PX / 2] = CRGB::Cyan;
        leds[NUM_LEDS - 2] = CRGB::Cyan;
        leds[NUM_LEDS - 4] = CRGB::Cyan;
        leds[NUM_LEDS - 6] = CRGB::Cyan;
      }
      // Show the leds
      FastLED.show();
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }

    // FastLED.delay(10);
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      // Set the i'th led to whatever
      leds[i] = CRGB::Green;
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      leds[i] = CRGB::Black;
      // Wait a little bit before we loop around and do it again
      // FastLED.delay(10);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
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

  FastLED.showColor(CRGB::Black);
}
