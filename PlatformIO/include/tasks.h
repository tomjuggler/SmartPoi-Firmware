#include "Globals.h"
#include <FastLED.h>
#include <EEPROM.h>
#include <ElegantOTA.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Forward declarations
class AsyncWebServerRequest;

// Function declarations
void setupElegantOTATask();
void elegantOTATask(void *pvParameters);
void handleFileUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) {
    static File fsUploadFile;
    static size_t totalSize = 0;
    
    if(!index) {
        totalSize = 0;
        // Add size validation from WebServerSetup
        if(!checkFileSpace(request->contentLength()) || 
           request->contentLength() > MAX_PX || 
           request->contentLength() > LittleFS.totalBytes() - LittleFS.usedBytes()) {
            request->send(507, "text/plain", "File size exceeds limit");
            return;
        }
        
        // Keep existing filename validation...
    }
    
    // Add size tracking
    totalSize += len;
    if(totalSize > MAX_PX) {
        if(fsUploadFile) {
            fsUploadFile.close();
            LittleFS.remove(filename);
        }
        request->send(507, "text/plain", "File size exceeds limit");
        return;
    }
    
    // Rest of existing implementation...
}
void handleGeneralSettings(AsyncWebServerRequest* request);
void handleRouterSettings(AsyncWebServerRequest* request); 
void handlePatternSettings(AsyncWebServerRequest* request);
void handleIntervalChange(AsyncWebServerRequest* request);
void handleBrightness(AsyncWebServerRequest* request);
