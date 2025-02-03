#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "Globals.h"
void eepromBrightnessChooser(int addr);
void eepromRouterOptionChooser(int addr);
void eepromWifiModeChooser(int addr);
void eepromPatternChooser(int addr);
void readAnotherPatternEEProm();
void eepromReadChannelAndAddress(int addr1, int addr2, int addr3, int addr4, int addr5);
void littleFSLoadSettings();
void checkFilesInSetup();
void wifiChooser(char router_array[], char pwd_array[]);
void handleAllServers();
void fastLEDInit();
void fastLEDIndicate();
void fastLEDIndicateFast();
#if defined(PLATFORM_ESP32)
extern WebServer server;
#elif defined(PLATFORM_ESP8266)
extern ESP8266WebServer server;
#endif
#endif
