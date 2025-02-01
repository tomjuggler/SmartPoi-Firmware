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
  extern WebServer server;
  extern WiFiMulti WiFiMulti;
#elif defined(PLATFORM_ESP8266)
  #include <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
  #include <ESP8266WiFiMulti.h>
  extern ESP8266WebServer server;
  extern ESP8266WiFiMulti WiFiMulti;
#endif

// Configuration Constants
constexpr int NUM_LEDS = 37;
constexpr int NUM_PX = 36;
constexpr int DNS_PORT = 53;
constexpr unsigned int LOCAL_PORT = 2390;
constexpr int MAX_PX = 10368;
constexpr int DEFAULT_BRIGHTNESS = 20;

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

// Global Extern Variables
extern CRGB leds[NUM_LEDS];
extern WiFiUDP Udp;
#if defined(PLATFORM_ESP32)
  #include <DNSServer.h>
#elif defined(PLATFORM_ESP8266)
  #include <DNSServer.h>
#endif
extern DNSServer dnsServer;
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
extern bool auxillary;
extern File settings;
extern String Field;
extern size_t maxPX;
extern IPAddress apIP;
extern IPAddress apIPauxillary;
extern IPAddress ipGatewayauxillary;
extern IPAddress ipSubnet;
extern const char* apName;
extern const char* apPass;

// WiFi Mode Constants
#if defined(PLATFORM_ESP32)
  #define WIFI_STA WIFI_MODE_STA
  #define WIFI_AP WIFI_MODE_AP
#elif defined(PLATFORM_ESP8266)
  #define WIFI_STA STATION_MODE
  #define WIFI_AP SOFTAP_MODE
#endif

// Shared Functions
void fastLEDInit();
void fastLEDIndicate();
void fastLEDIndicateFast();

#endif
