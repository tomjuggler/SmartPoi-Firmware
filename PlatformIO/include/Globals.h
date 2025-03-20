#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include <FastLED.h>

#if defined(PLATFORM_ESP32)
  #include <WiFi.h>
  #include <DNSServer.h>
  #include <WebServer.h>
  #include <EEPROM.h>
  #include <WiFiMulti.h>
  // extern WebServer poiserver;
  extern WiFiMulti WiFiMulti;
  extern DNSServer dnsServer;
#elif defined(PLATFORM_ESP8266)
  #include <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
  #include <ESP8266WiFiMulti.h>
  extern ESP8266WebServer poiserver;
  extern ESP8266WiFiMulti WiFiMulti;
  extern DNSServer dnsServer;
#endif

// Configuration Constants
constexpr int NUM_LEDS = NUMLEDS; //from platformio.ini
constexpr int NUM_PX = NUMPX; //from platformio.ini
constexpr int DNS_PORT = 53;
constexpr unsigned int LOCAL_PORT = 2390;
// constexpr int MAX_PX = 12240; //moved to platformio.ini
constexpr int MAX_PX = MAXPX;
constexpr int DEFAULT_BRIGHTNESS = 20;

// Platform detection
#if defined(ESP32)
  #define PLATFORM_ESP32
#elif defined(ESP8266)
  #define PLATFORM_ESP8266
#else
  #error "Unsupported platform"
#endif

#ifdef PLATFORM_ESP32
  constexpr int DATA_PIN = DATAPIN;
  constexpr int CLOCK_PIN = CLOCKPIN;
#else
  constexpr int DATA_PIN = DATAPIN;
  constexpr int CLOCK_PIN = CLOCKPIN;
#endif


// Global Extern Variables
extern CRGB leds[NUM_LEDS];
extern WiFiUDP Udp;
extern int newBrightness;  // Declaration for brightness control variable
extern bool routerOption;  // Declaration for router configuration flag
extern int wifiModeChooser;  // Declaration for WiFi mode selection
extern int patternChooser;
extern int pattern;
extern int apChannel;
extern int imageToUse;
extern unsigned long previousMillis3;
extern uint8_t addrNumA;
extern uint8_t addrNumB;
extern uint8_t addrNumC;
extern uint8_t addrNumD;
// extern bool auxillary; //moved to platformio.ini
extern File settings;
extern String Field;
extern size_t maxPX;
extern IPAddress apIP;
// UDP Handler variables
extern unsigned long currentMillis2;
extern unsigned long previousMillis2;
extern bool checkit;
extern int len;
extern uint8_t packetBuffer[255];
extern uint8_t Y;
extern int state;
extern int X;
extern uint8_t R1;
extern uint8_t G1;
extern uint8_t M1;
extern bool channelChange;
extern IPAddress apIPauxillary;
extern IPAddress ipGatewayauxillary;
extern IPAddress ipSubnet;
extern const char* apName;
extern const char* apPass;
extern String images; 

// WiFi Mode Constants
#if defined(PLATFORM_ESP32)
  #define WIFI_STA WIFI_MODE_STA
  #define WIFI_AP WIFI_MODE_AP
#elif defined(PLATFORM_ESP8266)
  #define WIFI_STA STATION_MODE
  #define WIFI_AP SOFTAP_MODE
#endif

// Shared Functions
// void fastLEDInit();
// void fastLEDIndicate();
// void fastLEDIndicateFast();

#endif
