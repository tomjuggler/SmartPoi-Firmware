#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <FastLED.h>

// Function prototypes
void handleUDP();
void showLittleFSImage();
void funColourJam();
void fastLEDInit();
void fastLEDIndicate();
void fastLEDIndicateFast();
void eepromBrightnessChooser(uint8_t address);
void eepromRouterOptionChooser(uint8_t address);
void eepromWifiModeChooser(uint8_t address);
void eepromPatternChooser(uint8_t address);
void eepromReadChannelAndAddress(uint8_t channelAddr, uint8_t dAddr, uint8_t aAddr, uint8_t bAddr, uint8_t cAddr);
void littleFSLoadSettings();
void checkFilesInSetup();
void ChangePatternPeriodically();
void sendTestMessage();

// Move global variables
File fsUploadFile;
CRGB leds[NUM_LEDS];
File f;
File a;
File settings;
boolean auxillary = false;
int newBrightness = 20;
const int maxPX = 10368;
uint8_t message1Data[maxPX];
int pxDown = NUM_PX;
int pxAcross = pxDown;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress apIPauxillary(192, 168, 1, 78);
DNSServer dnsServer;
int status = WL_IDLE_STATUS;
char apName[] = "Smart_Poi7";
char apPass[] = "SmartOne";
int apChannel = 1;
int keyIndex = 0;
IPAddress ipSubnet(255, 255, 255, 0);
IPAddress ipGateway(192, 168, 8, 1);
IPAddress ipGatewayauxillary(192, 168, 1, 1);
IPAddress ip(192, 168, 8, 77);
uint8_t addrNumA = 192;
uint8_t addrNumB = 168;
uint8_t addrNumC = 8;
uint8_t addrNumD = 78;
const unsigned int localPort = 2390;
byte packetBuffer[255];
const size_t bufferSize = 1024;
WiFiUDP Udp;
String responseHTML;
String content;
int statusCode;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
long interval = 5000;
boolean checkit = false;
boolean channelChange = false;
boolean savingToSpiffs = false;
unsigned long previousFlashy = 0;
const long intervalBetweenFlashy = 5;
boolean black = true;
int state = 0;
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
int pattern = 2;
int wifiModeChooser = 1;
int imageChooser = 1;
boolean preloaded = false;
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
boolean wifiEventDetect = false;
boolean start = true;
boolean routerOption = false;
volatile byte X;
volatile byte Y;
volatile byte R1;
volatile byte G1;
volatile byte M1;
volatile unsigned long currentMillis = millis();
volatile unsigned long currentMillis2 = millis();
volatile int packetSize;
volatile int len;

// Platform detection
#if defined(ESP32)
  #define PLATFORM_ESP32
#elif defined(ESP8266)
  #define PLATFORM_ESP8266
#else
  #error "Unsupported platform"
#endif

// LED configuration
#ifdef PLATFORM_ESP32
  #define DATA_PIN 1
  #define CLOCK_PIN 13
#else
  #define DATA_PIN D2
  #define CLOCK_PIN D1
#endif

#define NUM_LEDS 37
#define NUM_PX 36

void setup() {
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

  littleFSLoadSettings();
  checkFilesInSetup();
  fastLEDIndicate();
  Udp.begin(localPort);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
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
