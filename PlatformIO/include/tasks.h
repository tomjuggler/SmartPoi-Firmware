#include "Globals.h"
#include <FastLED.h>
#include <EEPROM.h>
#include <ElegantOTA.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

String getContentType(String filename);
bool checkFileSpace(size_t fileSize);
extern uint8_t message1Data[MAX_PX];
// Forward declarations
class AsyncWebServerRequest;

// Function declarations
bool checkFileSpace(size_t fileSize);
void setupElegantOTATask();
void elegantOTATask(void *pvParameters);
void handleFileUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final);
void handleGeneralSettings(AsyncWebServerRequest* request);
void handleRouterSettings(AsyncWebServerRequest* request); 
void handlePatternSettings(AsyncWebServerRequest* request);
void handleIntervalChange(AsyncWebServerRequest* request);
void handleBrightness(AsyncWebServerRequest* request);
void handleFileList(AsyncWebServerRequest* request);
void handleFileRead(AsyncWebServerRequest* request);
void handleFileCreate(AsyncWebServerRequest* request);
void handleFileDelete(AsyncWebServerRequest* request);
