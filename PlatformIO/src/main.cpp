#include <WiFi.h>
#include <EEPROM.h>
#include "Initialize.h"
#include "Globals.h"
#include "UDPHandler.h"
#include "ColourPalette.h"
#include "ShowLittleFSImage.h"
#include "TimeFunc.h"
// #include "WebServerSetup.h"
#include "tasks.h"

// Global Variable Definitions
CRGB leds[NUM_LEDS];
WiFiUDP Udp;
AsyncWebServer server(80);  // Create the web server instance

// Additional global variables
File fsUploadFile;
File f;
File a;
File settings;
// UDP Handler variables
unsigned long currentMillis2 = 0;
int state = 0;
uint8_t packetBuffer[255] = {0};
int len = 0;
uint8_t Y = 0;
int X = 0;
uint8_t R1 = 0;
uint8_t G1 = 0;
uint8_t M1 = 0;
// bool auxillary = false; //moved to platformio.ini
int newBrightness = DEFAULT_BRIGHTNESS;
uint8_t message1Data[MAX_PX];
int pxDown = NUM_PX;
int pxAcross = pxDown;
IPAddress apIP(192, 168, 1, 1);
IPAddress apIPauxillary(192, 168, 1, 78);
int status = WiFi.status();
const char* apName = "Smart_Poi9";
const char* apPass = "SmartOne";
int apChannel = 1;
IPAddress ipSubnet(255, 255, 255, 0);
IPAddress ipGateway(192, 168, 8, 1);
IPAddress ipGatewayauxillary(192, 168, 1, 1);
IPAddress ip(192, 168, 8, 77);
uint8_t addrNumA = 192;
uint8_t addrNumB = 168;
uint8_t addrNumC = 8;
uint8_t addrNumD = 78;
String responseHTML;
String content;
int statusCode;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
long interval = 5000;
bool checkit = false;
bool channelChange = false;
bool savingToSpiffs = false;
unsigned long previousFlashy = 0;
const long intervalBetweenFlashy = 5;
bool black = true;
bool upDown = true;
bool lines = true;
#define UPDATES_PER_SECOND 30000
int motionSpeed = 1;
int maxStartIndex = 70;
int minStartIndex = 0;
volatile int setting = 2;
int patternChooser = 2;
int pattern = 2;
int wifiModeChooser = 1;
int imageChooser = 1;
bool preloaded = false;
int byteCounter = 0;
IPAddress tmpGateway(192, 168, 8, 1);
IPAddress tmpIP(192, 168, 8, 77);
String Field;
int imageToUse = 0;
int maxImages = 52;
int minImages = 0;
String images = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
#ifndef ESP32
  String bin = "a.bin";                                                             
#else
  String bin = "/a.bin";
#endif
int uploadCounter = 1;
bool wifiEventDetect = false;
bool start = true;
bool routerOption = false;
volatile unsigned long currentMillis = millis();
volatile int packetSize;




void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  fastLEDInit();
  fastLEDIndicateFast();
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Started");

#ifdef PLATFORM_ESP32
  EEPROM.begin(512);
#else
  EEPROM.begin(512);
#endif

  eepromBrightnessChooser(15);
  eepromRouterOptionChooser(100);
  eepromWifiModeChooser(5);
  eepromPatternChooser(10);
  eepromReadChannelAndAddress(13, 14, 16, 17, 18);
  EEPROM.commit();

#ifdef PLATFORM_ESP32
  bool result = LittleFS.begin(true);
#else
  bool result = LittleFS.begin();
#endif

  littleFSLoadSettings();
  checkFilesInSetup();
  fastLEDIndicate();
  Udp.begin(LOCAL_PORT);
  // webServerSetupLogic(apName, apPass);
  setupElegantOTATask();  // Start the OTA task - todo: is this going to conflict with my own webserver above???
}

void loop() {
  // handleAllServers();
  // handleDNSServer();
  ChangePatternPeriodically();

  currentMillis = millis();
  currentMillis2 = millis();

  if (start) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      state = 1;
    }
  }

  packetSize = Udp.parsePacket();
  if (packetSize) {
    handleUDP();
  } else if (!packetSize && state == 1) {
    switch (pattern) {
      case 1:
        funColourJam();
        break;
      case 2:
        minImages = 0;
        maxImages = 4;
        #ifndef ESP32
          bin.setCharAt(0, images.charAt(imageToUse));
        #else
          bin.setCharAt(1, images.charAt(imageToUse));
        #endif
        showLittleFSImage();
        break;
      case 3:
        minImages = 5;
        maxImages = 10;
        #ifndef ESP32
          bin.setCharAt(0, images.charAt(imageToUse));
        #else
          bin.setCharAt(1, images.charAt(imageToUse));
        #endif
        showLittleFSImage();
        break;
      case 4:
        minImages = 11;
        maxImages = 20;
        #ifndef ESP32
          bin.setCharAt(0, images.charAt(imageToUse));
        #else
          bin.setCharAt(1, images.charAt(imageToUse));
        #endif
        showLittleFSImage();
        break;
      case 5:
        minImages = 0;
        maxImages = 62;
        #ifndef ESP32
          bin.setCharAt(0, images.charAt(imageToUse));
        #else
          bin.setCharAt(1, images.charAt(imageToUse));
        #endif
        showLittleFSImage();
        break;
      case 7:
        Serial.print(">");
        FastLED.delay(100);
        yield();
        break;
      default:
        yield();
    }
    yield();
  } else {
    yield();
  }
  yield();
}

// Add all remaining function implementations here...
