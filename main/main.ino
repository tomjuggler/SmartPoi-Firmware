
// ESP-01 Arduino 1.8.5 settings for this to work: 
// 80mhz flash, 160mhz clock, 26mhz crystal
// Flash mode: DOUT (for the chips with the numbers on, V3 printed on back)
// upload speed: 115200 for D1 and ESP-01
// size 1m 512 SPIFFS
// lwIP variant v1.4 Higher Bandwidth v2.0 works fine also
// builtin led 1

//todo: maybe have some 10 pre-defined images which can't be deleted? (one option)


// todo: change if else main/auxillary code to #ifdef syntax, to save on program space (applied at compile time)

#include "user_interface.h" //for testing

/////////////////////////////////////FSBrowser2/////////////////////////////////////////////////
// #include "FS.h"
#include "LittleFS.h" //SPIFFS DEPRECIATED! using LittleFS now. Faster

File fsUploadFile;

///////////////////////////////////End FSBrowser2////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
#include <WiFiUdp.h>

//////////////////////////////////////////FastLED code:////////////
#include <FastLED.h>



int newBrightness = 20; //setting 20 for battery and so white is not too much! 
#define DATA_PIN D2    //D2 for D1Mini, 2 for ESP-01
#define CLOCK_PIN D1  //D1 for D1Mini, 0 for ESP-01





File f;
File a;

//settings init:
File settings;

/////////////////////////////MAIN OR AUXILLARY POI? //////////////////////////////////////
boolean auxillary = false; //true for second (auxillary) poi - auxillary don't work alone!!!
/////////////////////////////////////////////////////////////////////////////////////


////////////////////////////HOW MANY PIXELS? 36 OR 72 - 2 variables to edit-  //////////////////

//#define NUM_LEDS 37
 #define NUM_LEDS 73

// Define the array of leds
CRGB leds[NUM_LEDS];

//#define NUM_PX 36
 #define NUM_PX 72

// const int maxPX = 20736; //enough for 72x288 or 36x576
// const int maxPX = 10368; //enough for 72x144 or 36x288
const int maxPX = 14400; //enough for 72x200 or 36x400

//lets try using a maximum number of pixels so very large array to hold any number:
uint8_t message1Data[maxPX]; //this is much larger than our image 

int pxDown = NUM_PX;

int pxAcross = pxDown; //this will change with the image
//////////////////////////////END HOW MANY PIXELS///////////////////////

////////////////////////////////////////////////////Mostly networking stuff: ////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress apIPauxillary(192, 168, 1, 78);
DNSServer dnsServer;
ESP8266WebServer server(80);

int status = WL_IDLE_STATUS;
//char ssid[] = "RouterName"; //  your network SSID (name) - now read from SPIFFS, no need for hard coding
//char pass[] = "RouterPassword";    // your network password (use for WPA, or use as key for WEP)
char apName[] = "Smart_Poi_4"; //"Smart_Poi_2";
char apPass[] = "SmartOne"; //"password";
int apChannel = 1;
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//IPAddress ipBroadcast(255, 255, 255, 255);
//IPAddress ipMulti(0, 0, 0, 0);

IPAddress ipSubnet(255, 255, 255, 0);
IPAddress ipGateway(192, 168, 8, 1);
IPAddress ipGatewayauxillary(192, 168, 1, 1);
//IPAddress ipDns(8, 8, 8, 8);
IPAddress ip(192, 168, 8, 77);

uint8_t addrNumA = 192;
uint8_t addrNumB = 168;
uint8_t addrNumC = 8;
uint8_t addrNumD = 78;

const unsigned int localPort = 2390;      // local port to listen on

byte packetBuffer[NUM_PX]; // buffer to hold incoming packet
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

String responseHTML;

String content;
int statusCode;

//////////////////////////////////////////////////////////////end mostly networking stuff////////////////////////////////////////////////

/////////////////////////////////////////////////////////////setup timer: ////////////////////////////////////////////////////////////////
unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
long interval = 5000; // after this interval switch over to internal
//above also used as interval for change of image. Todo: Need new updateable variable
boolean checkit = false;
boolean channelChange = false;
boolean savingToSpiffs = false;

unsigned long previousFlashy = 0;        // will store last time LED was updated
const long intervalBetweenFlashy = 5;           // after this interval switch over to internal
boolean black = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int state = 0;

//colour palette code://////////////////////////
boolean upDown = true;
boolean lines = true;

#define UPDATES_PER_SECOND 30000
CRGBPalette16 currentPalette;
TBlendType    currentBlending = NOBLEND;

int paletteVar = 1;

int motionSpeed = 1;
int maxStartIndex = 70;
int minStartIndex = 0;
volatile int setting = 2;

int patternChooser = 2;
int pattern = 2;
int wifiModeChooser = 1;
int imageChooser = 1;
boolean preloaded = false;
int byteCounter = 0;

boolean tempSwitch = true; //testing 2 images toggle

//tmp:
IPAddress tmpGateway(192, 168, 8, 1);
IPAddress tmpIP(192, 168, 8, 77);
//int testAddrNum = 77;

String Field;

int imageToUse = 0;
//max and min images need to be saved in spiffs (settings.txt? EEProm?) and updateable via the app
int maxImages = 52; //how many can we have? 50 is enough for big poi, memory wise
int minImages = 0; //start of block - change according to pattern!
//below is a hack! need a better address system
String images = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; //need MORE for small poi
String bin = "a.bin"; //one more than chars

int uploadCounter = 1;

boolean wifiEventDetect = false;
boolean start = true;

boolean routerOption = false;


void setup() {
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  fastLEDInit(); 
  fastLEDIndicateFast();
  //Initialize serial and wait for port to open: - for tests only
  Serial.begin(115200);
  Serial.println(""); //new line for readability
  Serial.println("Started");
  //////////////////////////////////////////////read eeprom settings://////////////////////////////////////////////////////////////////
  EEPROM.begin(512);
  
  //EEPROM storage:
  //Brightness 15, wifiMode 5, PatternChooser 10, pattern 11, apChannel 13, addrNumD 14, addrNumA 16, addrNumB 17, addrNumC 18
  
  eepromBrightnessChooser(15); //up to 220 max
  eepromRouterOptionChooser(100); //do we try to connect to router or not? default is no. 
  eepromWifiModeChooser(5); //AP or STA mode
  eepromPatternChooser(10); //5 settings
  eepromReadChannelAndAddress(13, 14, 16, 17, 18);
  EEPROM.commit(); //save any changes made above

  ///////////////////////////////////////////////////////LittleFS: /////////////////////////////////////////////////////////
  // always use this to "mount" the filesystem
  bool result = LittleFS.begin();
  String router;

//the following is related to router settings (using AP mode currently)
  spiffsLoadSettings(); 
  fastLEDIndicate(); //indicates AP (red) or router (green)
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

void loop() {
  //this only works once:
  if (start == false) {
    if(routerOption){
      if (millis() > interval * 4) { //perhaps wait a little longer...?
        start = true;
      }
    } else{
        if (millis() > interval * 2) { //wait for less time...
        start = true;
      }
      }
  }
  dnsServer.processNextRequest();
  server.handleClient();

  //////////////////////////////////////////////////////////// check if there is no signal ////////////////////////////////////////////////
 currentMillis = millis();
 currentMillis2 = millis();
 ChangePatternPeriodically(); 
  
  if (start) {
//  
    if (currentMillis - previousMillis >= interval) {   //should not ever be true if udp is sending at correct speed!
      //    Serial.println(millis());
      // save the last time you checked the time
      previousMillis = currentMillis;
      state = 1; //udp no signal state
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if there's data available, read a packet
  packetSize = Udp.parsePacket();
  if (packetSize) // if udp packet is received:
  {
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
  else if (!packetSize && state == 1)
  { // this is backup, if udp not received ie: connection dropped for > interval millisecs
    switch (pattern)
    {
    case 1:
    {
      funColourJam();
      //sendTestMessage(); //test send message for now...
      break;
    }
    //more options for patterns and spiffs loading here
    case 2:
    { 
      minImages = 0; //start of block 
      maxImages = 4; //end of block
      bin.setCharAt(0, images.charAt(imageToUse));    //setCharAt Arduino function is it slow? todo: try c char[0] = char[imageToUse]  
      showLittleFSImage();      
      break;
    }
    case 3:
    { 
      minImages = 5; //start of block 
      maxImages = 10; //end of block
      bin.setCharAt(0, images.charAt(imageToUse));    //setCharAt Arduino function is it slow? todo: try c char[0] = char[imageToUse]  
      showLittleFSImage();      
      break;
    }
    case 4:
    { 
      minImages = 11; //start of block 
      maxImages = 20; //end of block
      bin.setCharAt(0, images.charAt(imageToUse));    //setCharAt Arduino function is it slow? todo: try c char[0] = char[imageToUse]  
      showLittleFSImage();      
      break;
    }
    case 5:
    { 
      minImages = 0; //start of block 
      maxImages = 52; //end of block
      bin.setCharAt(0, images.charAt(imageToUse));    //setCharAt Arduino function is it slow? todo: try c char[0] = char[imageToUse]  
      showLittleFSImage();      
      break;
    }
    //todo: add some more patterns, pattern 0...
    }
  }
  else {
    //    //Serial.println("/");
    //nothing for <interval> seconds wait for signal
  }
}

void funColourJam() {
  //colour palette code://///////////////////////////
  if (setting == 1) {

    if (lines == false) { //toggled in ChangePalettePeriodically3()
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      if (upDown == true) {
        startIndex = startIndex + motionSpeed; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
        //Serial.println(startIndex);
        if (startIndex == maxStartIndex) {
          upDown = false;
        }
      }
      else {
        startIndex = startIndex - motionSpeed; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
        //Serial.println(startIndex);
        if (startIndex == minStartIndex) {
          upDown = true;
        }
      }

      // add_glitter();

      FastLED.show();
      FastLED.delay(1); //for 160mhz
      
    }//end if(lines)
    else {
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      startIndex = startIndex + motionSpeed; /* motion speed */
      FillLEDsFromPaletteColors( startIndex);
      // Serial.println(startIndex);
      if (startIndex == maxStartIndex) {
        startIndex = 0;
      }
      //add_glitter();
      //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
      FastLED.show();
      FastLED.delay(1); //for 160mhz
      //FastLED.delay(1000 / UPDATES_PER_SECOND);


    }//end else(lines)
  }//end if(setting ==1)

  else if (setting == 2)
  {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex = 0;
    stripeIndex = stripeIndex + 1;
    FillStripesFromPaletteColors( stripeIndex);
    ////Serial.println(startIndex);
    if (stripeIndex > 15) {
      stripeIndex = 0;
    }
    //add_glitter();
    //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
    FastLED.show();
    FastLED.delay(1); //for 160mhz
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
  }//end if(setting == 2)
  else
  {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex2 = 0;
    stripeIndex2 = stripeIndex2 + 1;
    FillPatternStripesFromPaletteColors( stripeIndex2, 4);
    ////Serial.println(startIndex);
    if (stripeIndex2 > 15) {
      stripeIndex2 = 0;
    }
    //add_glitter();
    //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
    FastLED.show();
    FastLED.delay(1); //for 160mhz
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
  }//end else 3
  /////////////////////////end colour palette///////////////////////////////////////
}


/*
  void sendTestMessage(){
    // send a reply, to the IP address and port that sent us the packet we received
   Udp.beginPacket(ip, localPort);
   Udp.write(ReplyBuffer);
   Udp.endPacket();
  }

*/
