#include <ElegantOTA.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Forward declarations
class AsyncWebServerRequest;

// Function declarations
void setupElegantOTATask();
void elegantOTATask(void *pvParameters);
void handleFileUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final);
void handleGeneralSettings(AsyncWebServerRequest* request);
void handleRouterSettings(AsyncWebServerRequest* request); 
void handlePatternSettings(AsyncWebServerRequest* request);
void handleIntervalChange(AsyncWebServerRequest* request);
void handleBrightness(AsyncWebServerRequest* request);
