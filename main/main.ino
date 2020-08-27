

//works with postTXTtoPoi processing sketch

//#define SPIFFS_CACHE true //enable ram cache for pics - not necessary I think

#include "user_interface.h" //for testing
//#include "lwip/tcp_impl.h" //more testing
//void tcpCleanup()
//{
//  while(tcp_tw_pcbs!=NULL)
//  {
//    tcp_abort(tcp_tw_pcbs);
//  }
//}
/////////////////////////////////////FSBrowser2/////////////////////////////////////////////////
// #include "FS.h"
#include "LittleFS.h" //SPIFFS DEPRECIATED! using LittleFS now. Faster

File fsUploadFile;

///////////////////////////////////End FSBrowser2////////////////////////////////////////////
int newINT = 0;

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
//#include <ESP8266WiFi.h>
//#include <SPI.h>true
//#include <WiFi.h>
#include <WiFiUdp.h>

//#include <user_interface.h> //for frequency update - from: https://github.com/esp8266/Arduino/issues/579
//////////////////////////////////////////FastLED code:////////////
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 37
//#define NUM_LEDS 73

#define NUM_PX 36
//#define NUM_PX 72

int newBrightness = 1; //setting 220 for battery and so white is not too much! //20 for testing ok
#define DATA_PIN D2    //D2 for D1Mini, 2 for ESP-01
#define CLOCK_PIN D1   //D1 for D1Mini, 0 for ESP-01

boolean auxillary = false; //second poi is true

// Define the array of leds
CRGB leds[NUM_LEDS];
///////////////////////////////////////////end FastLED code//////////////////

File f;
File w;
File g;
File a;
//File html; //removed to save space...

//settings init:
File settings;

const int maxPX = 4320; //Todo: change to 5184 for 36x144 - after removing other messages
//lets try using a maximum number of pixels so very large array to hold any number:
uint8_t message1Data[maxPX]; //this is much larger than our image - max image 36 down, 120 across
//Todo: remove below not needed data
uint8_t message2Data[4320]; //this is much larger than our image - max image 36 down, 120 across
uint8_t message3Data[4320]; //this is much larger than our image - max image 36 down, 120 across
uint8_t message4Data[4320]; //this is much larger than our image - max image 36 down, 120 across
uint8_t message5Data[4320]; //this is much larger than our image - max image 36 down, 120 across

//   the above arrays are where the memory goes to . Reduce this somehow?
//   for eg:
//   below reduces to 62% Global variables as opposed to 78% for above. Do we need 144px across? This will be double for the 72px poi what then?
/* 
  int maxPX = 5184; //5184
  //lets try using a maximum number of pixels so very large array to hold any number:
  uint8_t message1Data[5184]; //this is much larger than our image - max image 36 down, 144 across
  uint8_t message2Data[5184]; //this is much larger than our image - max image 36 down, 144 across
  uint8_t message3Data[5184]; //this is much larger than our image - max image 36 down, 144 across
  uint8_t message4Data[5184]; //this is much larger than our image - max image 36 down, 144 across
  uint8_t message5Data[5184]; //this is much larger than our image - max image 36 down, 144 across
*/

int picToShow = 1;
const int maxPicsToShow = 5;

int counter = 0;

int pxDown = 36;
int pxAcross = 36;     //this will change with the image
int pxAcrossArray[10]; //array for saving different px across
int pxDownCounter = 0;
int pxAcrossCounter = 0;
volatile int message1DataCounter = 0;
volatile int message2DataCounter = 0;

////////////////////////////////////////////////////Mostly networking stuff: ////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress apIPauxillary(192, 168, 1, 78);
DNSServer dnsServer;
ESP8266WebServer server(80);

int status = WL_IDLE_STATUS;
//char ssid[] = "ROUTER_NAME"; //  your network SSID (name) - now read from SPIFFS, no need for hard coding
//char pass[] = "ROUTER_PASSWORD";    // your network password (use for WPA, or use as key for WEP)
char apName[] = "Smart_Poi_1";
char apPass[] = "SmartOne";
int apChannel = 1;
int keyIndex = 0; // your network key Index number (needed only for WEP)

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

//IPAddress ipMulti(224,0,0,1);
//unsigned int portBroad = 5656;
//unsigned int portMulti = 6000;      // local port to listen on

const unsigned int localPort = 2390; // local port to listen on

byte packetBuffer[NUM_PX]; //buffer to hold incoming packet
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

String responseHTML;

String content;
int statusCode;

//////////////////////////////////////////////////////////////end mostly networking stuff////////////////////////////////////////////////

/////////////////////////////////////////////////////////////setup timer: ////////////////////////////////////////////////////////////////

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

//const unsigned long firstRunMillis = 22000;

// constants won't change :
const long interval = 5000; // after this interval switch over to internal
//const long longWait = 10000;           // interval to wait before checking setting on udp send //using above to save space!?
boolean checkit = false;
boolean channelChange = false;
boolean savingToSpiffs = false;

unsigned long previousFlashy = 0;     // will store last time LED was updated
const long intervalBetweenFlashy = 5; // after this interval switch over to internal
boolean black = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int state = 0;

//colour palette code://////////////////////////
boolean upDown = true;
boolean lines = true;

#define UPDATES_PER_SECOND 30000
CRGBPalette16 currentPalette;
TBlendType currentBlending = NOBLEND;

//extern CRGBPalette16 myRedWhiteBluePalette;
//extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

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
String image = "f.txt";
String bin = "a.txt";
boolean reload = true; //for converting all .txt to .bin - not really necessary..

//todo: remove below once .bin loading is implemented
String imgToShow1 = "/f.txt";
String imgToShow2 = "/g.txt";
String imgToShow3 = "/h.txt";
String imgToShow4 = "/i.txt";
String imgToShow5 = "/j.txt";

String imgToShow6 = "/a.txt";
String imgToShow7 = "/b.txt";
String imgToShow8 = "/c.txt";
String imgToShow9 = "/d.txt";
String imgToShow10 = "/e.txt";

String imgToShow11 = "/k.txt";
String imgToShow12 = "/l.txt";
String imgToShow13 = "/m.txt";
String imgToShow14 = "/n.txt";
String imgToShow15 = "/o.txt";

String imgToShow16 = "/p.txt";
String imgToShow17 = "/q.txt";
String imgToShow18 = "/r.txt";
String imgToShow19 = "/s.txt";
String imgToShow20 = "/t.txt";

int uploadCounter = 1;

boolean wifiEventDetect = false;
boolean start = false;

boolean routerOption = false;

void setup()
{
  //  WiFi.onEvent(WiFiEvent,WIFI_EVENT_ANY); //is this thing causing problems? not sure what it's doing here!
  fastLEDInit(); //try get led's responding quicker here!
                 //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial.println(""); //new line for readability

  //////////////////////////////////////////////read eeprom settings://////////////////////////////////////////////////////////////////
  EEPROM.begin(512);
  //EEPROM storage:
  //Brightness 15, wifiMode 5, PatternChooser 10, pattern 11, apChannel 13, addrNumD 14, addrNumA 16, addrNumB 17, addrNumC 18
  ///////////////////////////test init addrNum A-C:
  //EEPROM.write(15, 20); //BRT back to low to fix mistake
  //EEPROM.write(16, 192); //addrNumA
  //EEPROM.write(17, 168); //addrNumB
  //EEPROM.write(18, 8); //addrNumC
  eepromBrightnessChooser(15);    //up to 220 max
  eepromRouterOptionChooser(100); //do we try to connect to router or not? default is no.
  eepromWifiModeChooser(5);       //AP or STA mode
  eepromPatternChooser(10);       //3 settings
  eepromReadChannelAndAddress(13, 14, 16, 17, 18);
  EEPROM.commit(); //save any changes made above
  ///////////////////////////////////////////////////////SPIFFS: /////////////////////////////////////////////////////////
  // always use this to "mount" the filesystem
  bool result = LittleFS.begin();
  String router;

  spiffsLoadSettings();
  // 2 functions below have to be inside spiffsLoadSettings or else: ERROR: router_array was not declared in this scope
  //  wifiChooser(router_array, pwd_array);
  //  webServerSetupLogic(router_array, pwd_array);

  fastLEDIndicate(); //indicates AP (red) or router (green)

  //  dnsServer.start(DNS_PORT, "*", apIP); //AP mode only, surely?? Moved to wifiChooser()

  Udp.begin(localPort);

  loadPatternChooser(); //in redoLoadSpiffs tab
}

void loop()
{
  //this only works once:
  if (start == false)
  {
    if (routerOption)
    {
      if (millis() > interval * 4)
      { //perhaps wait a little longer...?
        start = true;
      }
    }
    else
    {
      if (millis() > interval)
      { //wait for less time...
        start = true;
      }
    }
  }
  //  if(wifiEventDetect){
  //    Serial.println("detected!!!!!!!!!!!!!!!!!!!!!!");
  //  }
  ////Serial.println(checkit); //
  dnsServer.processNextRequest();
  server.handleClient();
  //printWifiStatus();
  //////////////////////////////////////////////////////////// check if there is no signal ////////////////////////////////////////////////
  unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();
  //  //Serial.print("STATE IS: ");
  //  //Serial.print(state);
  //  //Serial.println(" one for no signal, zero for signal");
  //if(millis() > firstRunMillis){ //do only on first run???
  //if(wifiEventDetect && !auxillary){ //main poi
  ChangePatternPeriodically(); //trying a new way

  if (start)
  {
    //
    if (currentMillis - previousMillis >= interval)
    { //should not ever be true if udp is sending at correct speed!
      //    Serial.println(millis());
      // save the last time you checked the time
      previousMillis = currentMillis;
      state = 1; //udp no signal state
                 //      tempSwitch = !tempSwitch; //for switching picture at interval test
                 //      //first run sync
                 //      picToShow++;
                 //      //      Serial.print("first run millis: ");
                 //      //      Serial.println(millis());
                 //      if (picToShow > maxPicsToShow) {
                 //        picToShow = 1;
                 //      }
      ////Serial.println("state changed to 1");
    }
  }
  //else{ //auxillary poi
  //  if (currentMillis - previousMillis >= interval) {   //should not ever be true if udp is sending at correct speed!
  ////    Serial.println(millis());
  //    // save the last time you checked the time
  //    previousMillis = currentMillis;
  //    state = 1; //udp no signal state
  //    tempSwitch = !tempSwitch; //for switching picture at interval test
  //     //first run sync
  //      picToShow++;
  ////      Serial.print("first run millis: ");
  ////      Serial.println(millis());
  //    if (picToShow > maxPicsToShow) {
  //      picToShow = 1;
  //    }
  //    ////Serial.println("state changed to 1");
  //  }
  //}
  //}
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) // if udp packet is received:
  {
    if (currentMillis2 - previousMillis2 > interval * 2)
    { //message received after long wait, may be config message? check it
      // save the last time you checked the time
      previousMillis2 = currentMillis2;
      ////Serial.println("long wait hey!");
      //check the UDP packet for correct config message values later:
      checkit = true;
    }
    previousMillis = currentMillis;
    state = 0; //udp good signal state - need to work out optimum timing for this millis
    //    //Serial.println("0");
    //    //Serial.print("Received packet of size ");
    //    //Serial.println(packetSize);
    //    //Serial.print("From ");
    //      IPAddress remoteIp = Udp.remoteIP();
    //    //Serial.print(remoteIp);
    //    //Serial.print(", port ");
    //    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, NUM_PX);
    if (len > 0)
      packetBuffer[len] = 0;
    //    //Serial.println("Contents:");
    //    //Serial.println(packetBuffer);
    ////////////////////////////////////FastLED Code://///////////
    for (int i = 0; i < NUM_PX; i++)
    {
      byte X;
      byte Y;
      ///////////////////////////////////////////////////////settings mode////////////////////////////////////////////////////////////////////////
      if (checkit)
      { //need some sort of cylon thingy here to show that poi is doing something!!!
        Y = packetBuffer[i];

        switch (i)
        {
        case 0:
          if (Y == 0)
          { //Y is 0 at 1st packetBuffer array member
            ////Serial.println("checked 0, signal received");
          }
          else
          {
            checkit = false; //not on track, try again next time
          }
          break;
        case 1:
          if (Y == 1)
          {
            ////Serial.println("checked 1, signal received");
          }
          else
          {
            checkit = false; //not on track, try again next time
          }
          break;
        case 2:
          if (Y == 2)
          { //default 2
            ////Serial.println("checked 2, Brightness change signal received");
          }
          else if (Y == 3)
          {
            ////Serial.println("checked 2, Channel change signal received");
            channelChange = true;
          } //add another option for pattern change here?

          else
          {
            checkit = false; //not on track, try again next time
          }
          break;
        case 3:
          if (channelChange)
          {
            if ((int)Y > 11 || (int)Y < 1)
            {
              //wrong channel information received - error checking
            }
            else
            {
              EEPROM.write(13, 1); //clearing
              EEPROM.commit();
              int newChannel2; //temp variable
              newChannel2 = int(Y);
              EEPROM.write(13, newChannel2);
              EEPROM.commit();
              ////Serial.print("channel changed to :");
              ////Serial.println(newChannel2);
              //              ESP.restart(); //not using this right now but see https://github.com/esp8266/Arduino/issues/1722#issuecomment-192829825
              FastLED.showColor(CRGB::Magenta); //visual indicator
              channelChange = false;
              checkit = false; //Finished settings, exit
            }
          }
          else
          {
            //set eeprom:
            ////Serial.print("received Y: ");
            ////Serial.println(Y);
            EEPROM.write(15, int(Y)); //set for next time
            EEPROM.commit();
            newBrightness = int(Y);
            FastLED.setBrightness(newBrightness);
            ////Serial.println(" ");
            ////Serial.print("Brightness now set to: ");
            ////Serial.println(newBrightness);
            FastLED.showColor(CRGB::Blue); //visual indicator
            checkit = false;               //Finished settings, exit

            //            if (Y == 3) {
            //              //Serial.println("checked 3, signal received");
            //            }
            //            else{
            //             checkit = false; //not on track, try again next time
            //            }
          }
          break;
        default:
          ////Serial.println(Y);
          //checkit = false;
          break;
          //etc for 32, can write values according to signal received
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////////end settings mode///////////////////////////

      ///////////////////////////////convert byte to signed byte:////
      X = packetBuffer[i] - 127;
      /////////////////////////////end convert byte to signed byte////

      byte R1 = (X & 0xE0);
      leds[i].r = R1; //
      byte G1 = ((X << 3) & 0xE0);
      leds[i].g = G1;
      byte M1 = (X << 6);
      leds[i].b = M1;
      //FastLED.delay(1);
      //        //Serial.print(R1); //Serial.print(", "); //Serial.print(G1); //Serial.print(", "); //Serial.println(M1);
    }

    //FastLED.delay(2); //not just for emulator!

    LEDS.show();
    delay(1);
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
    case 3:
    case 4:
    case 5:
    { //cases 2, 3, 4 and 5 the same just with different pre-loaded pics! 
    //todo: add some more patterns, pattern 0...
      switch (imageToUse)
      {
      case 0:
        bin = "/a.bin";
        break;
      case 1:
        bin = "/b.bin";
        break;
      case 2:
        bin = "/c.bin";
        break;
      case 3:
        bin = "/d.bin";
        break;
      case 4:
        bin = "/e.bin";
        break;
      case 5:
        bin = "/f.bin";
        break;
      case 6:
        bin = "/g.bin";
        break;
      case 7:
        bin = "/h.bin";
        break;
      case 8:
        bin = "i.bin";
        break;
      case 9:
        bin = "j.bin";
        break;
      case 10:
        bin = "k.bin";
        break;
      case 11:
        bin = "l.bin";
        break;
      case 12:
        bin = "m.bin";
        break;
      case 13:
        bin = "n.bin";
        break;
      case 14:
        bin = "o.bin";
        break;
      case 15:
        bin = "p.bin";
        break;
      case 16:
        bin = "q.bin";
        break;
      case 17:
        bin = "r.bin";
        break;
      case 18:
        bin = "s.bin";
        break;
      case 19:
        bin = "t.bin";
        break;
      }

      showLittleFSImage();

      //  tempSwitch = !tempSwitch;
      /*
            if(tempSwitch){
            pxAcross = pxAcrossArray[1]; //do this only on change?
            showSpiffsImage2(message1Data);
            }
            else{
            pxAcross = pxAcrossArray[2];//do this only on 1st change??
            showSpiffsImage2(message2Data);
            }
          */
      //          //Serial.print("pic number: ");
      //          //Serial.println(picToShow);
      /*
      switch (picToShow)
      {
      case 1:
        pxAcross = pxAcrossArray[1]; //do this only on change?
        showSpiffsImage2(message1Data);
        break;
      case 2:
        pxAcross = pxAcrossArray[2]; //do this only on change?
        showSpiffsImage2(message2Data);
        break;
      case 3:
        pxAcross = pxAcrossArray[3]; //do this only on change?
        showSpiffsImage2(message3Data);
        break;
      case 4:
        pxAcross = pxAcrossArray[4]; //do this only on change?
        showSpiffsImage2(message4Data);
        break;
      case 5:
        pxAcross = pxAcrossArray[5]; //do this only on change?
        showSpiffsImage2(message5Data);
        break;
      }
      */
      //          //Serial.println("Spiffs");
      //sendTestMessage(); //test send message for now...
      break;
    }
    }
  }
  else
  {
    //    //Serial.println("/");
    //nothing for <interval> seconds wait for signal
  }
}

void funColourJam()
{
  //  unsigned long currentFlashy = millis();
  //
  //  if (currentFlashy - previousFlashy >= intervalBetweenFlashy) {   //should not ever be true if udp is sending at correct speed!
  //    // save the last time you checked the time
  //    previousFlashy = currentFlashy;
  //    //this is nice but the 2 poi will not be synced... meh! simple patterns should be fine but what about pics etc???
  //    //  //Serial.println("Black");
  //    if (black) {
  //      FastLED.showColor( CRGB::Black );
  //      black = !black;
  //    }
  //    else {
  //      int randomOne = random(60, 255);
  //      //  //Serial.println(".......Red");
  //      FastLED.showColor( CRGB(randomOne, 0, 255-randomOne) );
  //      black = !black;
  //    }
  //  }

  //colour palette code://///////////////////////////
  if (setting == 1)
  {

    if (lines == false)
    { //toggled in ChangePalettePeriodically3()
      //ChangePalettePeriodically2();
      //ChangePaletteEveryTime();
      ////Serial.println(paletteVar);
      //PaletteSetup();
      //SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
      //SetupRandomVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      if (upDown == true)
      {
        startIndex = startIndex + motionSpeed; /* motion speed */
        FillLEDsFromPaletteColors(startIndex);
        ////Serial.println(startIndex);
        if (startIndex == maxStartIndex)
        {
          upDown = false;
        }
      }
      else
      {
        startIndex = startIndex - motionSpeed; /* motion speed */
        FillLEDsFromPaletteColors(startIndex);
        ////Serial.println(startIndex);
        if (startIndex == minStartIndex)
        {
          upDown = true;
        }
      }

      //add_glitter();

      FastLED.show();
      FastLED.delay(1); //for 160mhz
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
      /*
        motionSpeed++;
        if(motionSpeed == 10){
        motionSpeed = 1;
        }

        minStartIndex++;
        if(minStartIndex == 20){
        minStartIndex = 0;
        }
        maxStartIndex--;
        if(maxStartIndex == 30){
        maxStartIndex = 70;
        }
      */
    } //end if(lines)
    else
    {
      //ChangePalettePeriodically2();
      //ChangePaletteEveryTime();
      ////Serial.println(paletteVar);
      //PaletteSetup();
      //SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
      //SetupRandomVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      startIndex = startIndex + motionSpeed; /* motion speed */
      FillLEDsFromPaletteColors(startIndex);
      ////Serial.println(startIndex);
      if (startIndex == maxStartIndex)
      {
        startIndex = 0;
      }
      //add_glitter();
      //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
      FastLED.show();
      FastLED.delay(1); //for 160mhz
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
      /*
        motionSpeed++;
        if(motionSpeed == 10){
        motionSpeed = 1;
        }

        minStartIndex++;
        if(minStartIndex == 20){
        minStartIndex = 0;
        }
        maxStartIndex--;
        if(maxStartIndex == 30){
        maxStartIndex = 70;
        }
      */

    } //end else(lines)
  }   //end if(setting ==1)

  else if (setting == 2)
  {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex = 0;
    stripeIndex = stripeIndex + 1;
    FillStripesFromPaletteColors(stripeIndex);
    ////Serial.println(startIndex);
    if (stripeIndex > 15)
    {
      stripeIndex = 0;
    }
    //add_glitter();
    //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
    FastLED.show();
    FastLED.delay(1); //for 160mhz
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
  } //end if(setting == 2)
  else
  {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex2 = 0;
    stripeIndex2 = stripeIndex2 + 1;
    FillPatternStripesFromPaletteColors(stripeIndex2, 4);
    ////Serial.println(startIndex);
    if (stripeIndex2 > 15)
    {
      stripeIndex2 = 0;
    }
    //add_glitter();
    //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
    FastLED.show();
    FastLED.delay(1); //for 160mhz
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
  } //end else 3
  /////////////////////////end colour palette///////////////////////////////////////
}

//more colour palette code:

/*
  void printWifiStatus() {
  // print the SSID of the network you're attached to:
  //Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");
  }
*/
/*
  void sendTestMessage(){
    // send a reply, to the IP address and port that sent us the packet we received
   Udp.beginPacket(ip, localPort);
   Udp.write(ReplyBuffer);
   Udp.endPacket();
  }

*/