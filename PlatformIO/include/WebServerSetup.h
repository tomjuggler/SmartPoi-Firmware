#ifndef WEBSERVERSETUP_H
#define WEBSERVERSETUP_H

// Add these extern declarations
extern String responseHTML;
extern int imageToUse;
extern unsigned long previousMillis3;
extern uint8_t addrNumA, addrNumB, addrNumC, addrNumD;
extern int patternChooser;
extern int apChannel;
extern long interval;

#include "Globals.h"
#include <WebServer.h>
#include <HTTP_Method.h>

// Handler declarations
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
