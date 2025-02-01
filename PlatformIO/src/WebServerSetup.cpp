#include "WebServerSetup.h"
#include "Globals.h"
#include <Arduino.h>

// Add platform-specific server declaration
#if defined(PLATFORM_ESP32)
  extern WebServer server;
#elif defined(PLATFORM_ESP8266)
  extern ESP8266WebServer server;
#endif

// Helper functions
bool checkFileSpace(size_t fileSize) {
  size_t totalSpace = LittleFS.totalBytes();
  size_t maxAllowedSize = totalSpace - MAX_PX - 1024;
  return (fileSize <= maxAllowedSize);
}

String formatBytes(size_t bytes) {
  const char* suffixes[] = {"B", "KB", "MB", "GB"};
  uint8_t i = 0;
  double dblBytes = bytes;
  while (dblBytes >= 1024 && i < 3) {
    dblBytes /= 1024;
    i++;
  }
  return String(dblBytes, 2) + suffixes[i];
}

String getContentType(String filename) {
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

// Request handlers
void handleGetPixels() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(200, "text/plain", String(NUM_PX));
}

void handleResetImageToUse() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  imageToUse = 0;
  previousMillis3 = millis();
  server.send(200, "text/plain", "");
}

void handleReturnSettings() {
  File settings = LittleFS.open("/settings.txt", "r");
  String settingsSSID = settings.readStringUntil('\n');
  String settingsPASS = settings.readStringUntil('\n');
  settings.close();
  
  String content = settingsSSID + "," + settingsPASS + "," + String(apChannel) + "," + 
                 String(addrNumA) + "," + String(addrNumB) + "," + 
                 String(addrNumC) + "," + String(addrNumD) + "," + 
                 String(patternChooser);
  server.send(200, "text/html", content);
}

// Add implementations for other handlers here...

void webServerSetupLogic(String router, String pass) {
  server.on("/get-pixels", HTTP_GET, handleGetPixels);
  server.on("/resetimagetouse", HTTP_GET, handleResetImageToUse);
  server.on("/returnsettings", HTTP_GET, handleReturnSettings);
  
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/edit", HTTP_GET, handleFileRead);
  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, [](){}, handleFileUpload);
  
  // Add remaining route handlers...
  
  server.begin();
}
