#ifndef WEBSERVERSETUP_H
#define WEBSERVERSETUP_H

#include "Globals.h"

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
