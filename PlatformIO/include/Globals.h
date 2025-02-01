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
  WebServer server(80);
  WiFiMulti WiFiMulti;
#elif defined(PLATFORM_ESP8266)
  #include <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
  #include <ESP8266WiFiMulti.h>
  ESP8266WebServer server(80);
  ESP8266WiFiMulti WiFiMulti;
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
extern DNSServer dnsServer;
extern int newBrightness;  // Declaration for brightness control variable
extern bool routerOption;  // Declaration for router configuration flag
extern int wifiModeChooser;  // Declaration for WiFi mode selection
extern int patternChooser;
extern int pattern;
extern int apChannel;
extern int addrNumA;
extern int addrNumB;
extern int addrNumC;
extern int addrNumD;
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

// Shared Functions
void fastLEDInit();
void fastLEDIndicate();
void fastLEDIndicateFast();

#endif
