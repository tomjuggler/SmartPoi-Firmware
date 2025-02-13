#ifndef WEBSERVERSETUP_H
#define WEBSERVERSETUP_H

#include <Arduino.h>
#include "Globals.h"

// Platform-specific server header
#if defined(PLATFORM_ESP32)
  #include <WebServer.h>
  #include <DNSServer.h>
#elif defined(PLATFORM_ESP8266)
  #include <ESP8266WebServer.h>
  #include <DNSServer.h>
#endif

// External declarations
extern String responseHTML;
extern File fsUploadFile;
extern int imageToUse;
extern unsigned long previousMillis3;
extern uint8_t addrNumA, addrNumB, addrNumC, addrNumD;
extern int patternChooser;
extern int apChannel; 
extern long interval;
extern uint8_t message1Data[MAX_PX];

// Handler declarations
size_t getTotalSpace();
size_t getRemainingSpace();
size_t getUsedSpace();
String formatBytes(size_t bytes);
void handleAllServers();
void handleGetPixels();
void handleOptions();
void handleFileRead();
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();
void clearArray();
void handleResetImageToUse();
void handleReturnSettings();
void handleRouterSettings();
void handlePatternSettings();
void handleIntervalChange();
void handleBrightness();
void handleGeneralSettings();

// Main setup function
void webServerSetupLogic(String router, String pass);

#endif
