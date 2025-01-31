#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <FastLED.h>

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
extern WebServer server;
extern int newBrightness;  // Declaration for brightness control variable
extern bool routerOption;  // Declaration for router configuration flag
extern int wifiModeChooser;  // Declaration for WiFi mode selection

// Shared Functions
void fastLEDInit();
void fastLEDIndicate();
void fastLEDIndicateFast();

#endif
