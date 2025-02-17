
// Platform detection
#if defined(ESP32)
  #define PLATFORM_ESP32
#elif defined(ESP8266)
  #define PLATFORM_ESP8266
#else
  #error "Unsupported platform"
#endif

// ESP-01 Arduino 1.8.5 settings for this to work:
// 80mhz flash, 160mhz clock, 26mhz crystal
// Flash mode: DOUT (for the chips with the numbers on, V3 printed on back)
// upload speed: 115200 for D1 and ESP-01
// size 1m 512 SPIFFS
// lwIP variant v1.4 Higher Bandwidth v2.0 works fine also
// builtin led 1

// ESP-01 no longer supported, try D1 mini ESP8266 with 4MB+ flash and 80KB ram
// Recommended D1 mini Arduino settings: 160mhz clock, LWIP: V2 Higher Bandwidth

// todo: maybe have some 10 pre-defined images which can't be deleted? (one option)

// todo: change if else main/auxillary code to #ifdef syntax, to save on program space (applied at compile time)

#ifdef PLATFORM_ESP8266
  #include "user_interface.h" // ESP8266-specific header
#endif

/////////////////////////////////////FSBrowser2/////////////////////////////////////////////////
// #include "FS.h"
#include <LittleFS.h> //SPIFFS DEPRECIATED! using LittleFS now. Faster

File fsUploadFile;

///////////////////////////////////End FSBrowser2////////////////////////////////////////////

#ifdef PLATFORM_ESP32
  #include <WiFi.h>
  #include <DNSServer.h>
  #include <WebServer.h>
  #include <EEPROM.h>
  #include <WiFiMulti.h>
  WebServer server(80);
  WiFiMulti WiFiMulti;
#else
  #include <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
  #include <ESP8266WiFiMulti.h>
  ESP8266WebServer server(80);
  ESP8266WiFiMulti WiFiMulti;
#endif

#include <WiFiUdp.h>

//////////////////////////////////////////FastLED code:////////////
#include <FastLED.h>

int newBrightness = 20; // setting 20 for battery and so white is not too much! This is re-set on startup, for safety and battery
#ifdef PLATFORM_ESP32
  #define DATA_PIN 1    // 1 for ESP32 S3, 4 for ESP32 C3
  #define CLOCK_PIN 13  // 13 for ESP32 S3, 5 for ESP32 C3
#else
  #define DATA_PIN D2   // D2 for D1Mini, 2 for ESP-01
  #define CLOCK_PIN D1  // D1 for D1Mini, 0 for ESP-01
#endif

File f;
File a;

// settings init:
File settings;

/////////////////////////////MAIN OR AUXILLARY POI? //////////////////////////////////////
boolean auxillary = false; // true for second (auxillary) poi - auxillary don't work alone!!!
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////TYPE OF LED's to use///////////////////////////////////////
#define LED_APA102 //comment this line to use WS2812, uncomment for APA102
///////////////////////////////////////////////////////////////////////////////////////

////////////////////////////HOW MANY PIXELS? - 2 variables to edit-  //////////////////

//#define NUM_LEDS 37
// #define NUM_LEDS 61
//   #define NUM_LEDS 73
#define NUM_LEDS 37

// Define the array of leds
CRGB leds[NUM_LEDS];

//#define NUM_PX 36
// #define NUM_PX 60
//   #define NUM_PX 72
#define NUM_PX 36

// 24000 is too large - oom error, 120x200
// const int maxPX = 20736 // 144x144 - very large, may be unstable?
// const int maxPX = 19200; // 120x160 - very large, may be unstable? 
// const int maxPX = 20736; //enough for 72x288 or 36x576 - very large, may be unstable?
const int maxPX = 10368; //enough for 72x144 or 36x288
// const int maxPX = 14400; //enough for 72x200 or 36x400

// lets try using a maximum number of pixels so very large array to hold any number:
uint8_t message1Data[maxPX]; // this is much larger than our image

int pxDown = NUM_PX;

int pxAcross = pxDown; // this will change with the image
//////////////////////////////END HOW MANY PIXELS/////////////////////////////////////////

////////////////////////////////////////////////////Mostly networking stuff: ////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress apIPauxillary(192, 168, 1, 78);
DNSServer dnsServer;

int status = WL_IDLE_STATUS;
// char ssid[] = "RouterName"; //  your network SSID (name) - now read from SPIFFS, no need for hard coding
// char pass[] = "RouterPassword";    // your network password (use for WPA, or use as key for WEP)
char apName[] = "Smart_Poi7"; //"Smart_Poi_2";
char apPass[] = "SmartOne";   //"password";
int apChannel = 1;
int keyIndex = 0; // your network key Index number (needed only for WEP)

IPAddress ipSubnet(255, 255, 255, 0);
IPAddress ipGateway(192, 168, 8, 1);
IPAddress ipGatewayauxillary(192, 168, 1, 1);
// IPAddress ipDns(8, 8, 8, 8);
IPAddress ip(192, 168, 8, 77);

uint8_t addrNumA = 192;
uint8_t addrNumB = 168;
uint8_t addrNumC = 8;
uint8_t addrNumD = 78;

////////////////////////////////////// UDP CODE OPTIONAL: ///////////////////////////////////////////////////////////////////////////
const unsigned int localPort = 2390; // local port to listen on

byte packetBuffer[255]; // buffer to hold incoming packet
// char  ReplyBuffer[] = "acknowledged";       // a string to send back
const size_t bufferSize = 1024; // Adjust buffer size as needed

WiFiUDP Udp;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


String responseHTML;

String content;
int statusCode;

//////////////////////////////////////////////////////////////end mostly networking stuff////////////////////////////////////////////////

////////////////////////////////////////////////////////UDP CODE OPTIONAL: //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////setup timer: ////////////////////////////////////////////////////////////////
unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long previousMillis2 = 0;
/////////////////////////////////////////////////////////////////////////////////
unsigned long previousMillis3 = 0;
long interval = 5000; // after this interval switch over to internal
// above also used as interval for change of image. Todo: Need new updateable variable
boolean checkit = false;
boolean channelChange = false;
boolean savingToSpiffs = false;

unsigned long previousFlashy = 0;     // will store last time LED was updated
const long intervalBetweenFlashy = 5; // after this interval switch over to internal
boolean black = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int state = 0;

// colour palette code://////////////////////////
boolean upDown = true;
boolean lines = true;

#define UPDATES_PER_SECOND 30000
CRGBPalette16 currentPalette;
TBlendType currentBlending = NOBLEND;

int paletteVar = 1;

int motionSpeed = 1;
int maxStartIndex = 70;
int minStartIndex = 0;
volatile int setting = 2;

int patternChooser = 2;
int pattern = 1;
int wifiModeChooser = 1;
int imageChooser = 1;
boolean preloaded = false;
int byteCounter = 0;

// tmp:
IPAddress tmpGateway(192, 168, 8, 1);
IPAddress tmpIP(192, 168, 8, 77);

String Field;

int imageToUse = 0;
// max and min images need to be saved in spiffs (settings.txt? EEProm?) and updateable via the app
int maxImages = 52; // how many can we have? 50 is enough for big poi, memory wise
int minImages = 0;  // start of block - change according to pattern!
// Below is a hack! Needs a better address system. Not doing upgrade for SmartPoi, MagicPoi can have any number of files with any filename, depending on Flash size. 
String images = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; 
#ifndef ESP32
  String bin = "a.bin";                                                             
#else
  String bin = "/a.bin";
#endif
int uploadCounter = 1;

boolean wifiEventDetect = false;
boolean start = true;

boolean routerOption = false;

/**
 * @brief Setup function, called once at program start.
 *
 * Initializes digital pins, FastLED, serial communication, and EEPROM settings.
 * Also mounts the LittleFS file system, loads router settings, and checks files.
 *
 * @param None
 * @return None
 *
 * @pre None
 * @post Digital pins, FastLED, serial, EEPROM, and LittleFS are initialized.
 *       Router settings are loaded, and files are checked for corruption or size issues.
 *
 * @note This function is called automatically at program start.
 * @note EEPROM settings are loaded from storage.
 * @note LittleFS file system is mounted and files are checked.
 */
void setup()
{
  // ESP.wdtDisable();
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  fastLEDInit();
  fastLEDIndicateFast();
  // Initialize serial and wait for port to open: - for tests only
  Serial.begin(115200);
  Serial.println(""); // new line for readability
  Serial.println("Started");
  //////////////////////////////////////////////read eeprom settings://////////////////////////////////////////////////////////////////
#ifdef PLATFORM_ESP32
  EEPROM.begin(512); // Initialize EEPROM with 512 bytes
#else
  EEPROM.begin(512);
#endif

  // EEPROM storage:
  // Brightness 15, wifiMode 5, PatternChooser 10, pattern 11, apChannel 13, addrNumD 14, addrNumA 16, addrNumB 17, addrNumC 18

  eepromBrightnessChooser(15);    // up to 220 max
  eepromRouterOptionChooser(100); // do we try to connect to router or not? default is no.
  eepromWifiModeChooser(5);       // AP or STA mode
  eepromPatternChooser(10);       // 5 settings
  eepromReadChannelAndAddress(13, 14, 16, 17, 18);
  EEPROM.commit(); // save any changes made above

  ///////////////////////////////////////////////////////LittleFS: /////////////////////////////////////////////////////////
  // always use this to "mount" the filesystem
#ifdef PLATFORM_ESP32
  bool result = LittleFS.begin(true); // Format if mount fails on ESP32
  if (!result) {
  Serial.println("Failed to mount LittleFS, attempting to format...");
  LittleFS.format();
  result = LittleFS.begin(true);
  if (!result) {
    Serial.println("Failed to format LittleFS");
  } else {
    Serial.println("LittleFS formatted successfully");
  }
}
#else
  bool result = LittleFS.begin();
#endif
  String router; //todo: not used? 

  // The following is related to router settings (using AP mode currently)
  littleFSLoadSettings();
  checkFilesInSetup(); // Check files for corruption or size issues
  fastLEDIndicate(); // indicates AP (Auxillary: Red, Main: Blue) or STA mode (Green)

  /////// UDP OPTIONAL ///////////
  Udp.begin(localPort);
  
  
}

volatile byte X;
volatile byte Y;
volatile byte R1;
volatile byte G1;
volatile byte M1;

volatile unsigned long currentMillis = millis();
volatile unsigned long currentMillis2 = millis();
volatile int packetSize;
volatile int len;
////////////////////////////////
/**
 * @brief Main loop function, called repeatedly after setup.
 *
 * Handles various tasks such as:
 * - Waiting for a signal or connection
 * - Processing DNS server requests
 * - Handling UDP packets (optional)
 * - Changing patterns periodically
 * - Displaying patterns using FastLED
 *
 * @param None
 * @return None
 *
 * @pre setup() function has been called
 * @post Continuously runs, handling various tasks and events
 *
 * @note This function is called repeatedly after setup.
 * @note Uses global variables such as start, routerOption, interval, and pattern.
 * @note Calls other functions such as dnsServer.processNextRequest(), server.handleClient(), ChangePatternPeriodically(), and handleUdp().
 */
void loop()
{
  //todo: do we need this? looks like an experiment - maybe remove start completely
  // this only works once:
  // if (start == false)
  // {
  //   if (routerOption)
  //   {
  //     if (millis() > interval * 4)
  //     { // perhaps wait a little longer...?
  //       start = true;
  //     }
  //   }
  //   else
  //   {
  //     if (millis() > interval * 2)
  //     { // wait for less time...
  //       start = true;
  //     }
  //   }
  // }

  dnsServer.processNextRequest();
  server.handleClient();
  ChangePatternPeriodically(); 

///////////////////////////// OPTIONAL: UDP Packet handling (STREAMING from app option): /////////////////////////////////////////////////
  //////////////////////////////////////////////////////////// check if there is no signal ////////////////////////////////////////////////
  currentMillis = millis();
  currentMillis2 = millis();
  

  if (start)
  {
    //
    if (currentMillis - previousMillis >= interval)
    { // should not ever be true if udp is sending at correct speed!
      //    Serial.println(millis());
      // save the last time you checked the time
      previousMillis = currentMillis;
      state = 1; // udp no signal state
    }
  }

  
  // if there's data available, read a packet
   packetSize = Udp.parsePacket();
   if (packetSize) // if udp packet is received:
   {
     handleUDP();
   }
   else if (!packetSize && state == 1)
   { // this is backup, if udp not received ie: connection dropped for > interval millisecs
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  switch (pattern)
  {
  case 1:
  {
    funColourJam();
    // sendTestMessage(); //test send message for now...
    break;
  }
  // more options for patterns and spiffs loading here
  case 2:
  {
    minImages = 0; // start of block
    maxImages = 4; // end of block
    #ifndef ESP32
      bin.setCharAt(0, images.charAt(imageToUse));
    #else
      bin.setCharAt(1, images.charAt(imageToUse));
    #endif
    showLittleFSImage();
    break;
  }
  case 3:
  {
    minImages = 5;  // start of block
    maxImages = 10; // end of block
    #ifndef ESP32
      bin.setCharAt(0, images.charAt(imageToUse));
    #else
      bin.setCharAt(1, images.charAt(imageToUse));
    #endif
    showLittleFSImage();
    break;
  }
  case 4:
  {
    minImages = 11; // start of block
    maxImages = 20; // end of block
    #ifndef ESP32
      bin.setCharAt(0, images.charAt(imageToUse));
    #else
      bin.setCharAt(1, images.charAt(imageToUse));
    #endif
    showLittleFSImage();
    break;
  }
  case 5:
  {
    minImages = 0;  // start of block
    maxImages = 62; // end of block
    #ifndef ESP32
      bin.setCharAt(0, images.charAt(imageToUse));
    #else
      bin.setCharAt(1, images.charAt(imageToUse));
    #endif
    showLittleFSImage();
    break;
  }
  case 7:
  {
    // do nothing - new option for use during uploading
    Serial.print(">");
    FastLED.delay(100);
    yield();
    break;
  }
  default:
  {
    yield();
  }
    // todo: add some more patterns, pattern 0...
  }

  ///////////////////////////////////// Optional: UDP Parse packet (streaming) code: ///////////////////////////////////////////////////
     yield(); // this is to give WiFi process control for tasks
   }
   else {
     //    //Serial.println("/");
     //nothing for <interval> seconds wait for signal
   }
  ///////////////////////////////////// End optional UDP code //////////////////////////////////////////////////////////////////////////

  yield(); // give WiFi and other processor processes time to work
}
