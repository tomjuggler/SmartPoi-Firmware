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

// File management handlers
void handleFileList() {
  String output = "[";
  File dir = LittleFS.open("/");
  File entry = dir.openNextFile();
  while(entry) {
    if(output != "[") output += ',';
    output += "{\"name\":\"";
    output += entry.name();
    output += "\",\"size\":";
    output += entry.size();
    output += ",\"type\":\"";
    output += (entry.isDirectory() ? "dir" : "file");
    output += "\"}";
    entry = dir.openNextFile();
  }
  output += "]";
  server.send(200, "application/json", output);
}

void handleFileRead() {
  String path = server.arg("path");
  if(path.endsWith("/")) path += "index.htm";
  
  String contentType = getContentType(path);
  File file = LittleFS.open(path, "r");
  
  if(file) {
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void handleFileCreate() {
  String path = server.arg("path");
  if(path.isEmpty()) {
    server.send(400, "text/plain", "Bad request");
    return;
  }
  
  if(LittleFS.exists(path)) {
    server.send(409, "text/plain", "File exists");
    return;
  }
  
  File file = LittleFS.open(path, "w");
  if(file) {
    file.close();
    server.send(200, "text/plain", "Created");
  } else {
    server.send(500, "text/plain", "Create failed");
  }
}

void handleFileDelete() {
  String path = server.arg("path");
  if(path.isEmpty()) {
    server.send(400, "text/plain", "Bad request");
    return;
  }
  
  if(!LittleFS.remove(path)) {
    server.send(500, "text/plain", "Delete failed");
    return;
  }
  
  server.send(200, "text/plain", "Deleted");
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START) {
    if(!checkFileSpace(upload.contentLength)) {
      upload.status = UPLOAD_FILE_ABORTED;
      server.send(507, "text/plain", "Insufficient storage");
      return;
    }
    fsUploadFile = LittleFS.open(upload.filename, "w");
  } else if(upload.status == UPLOAD_FILE_WRITE) {
    if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END) {
    if(fsUploadFile) {
      fsUploadFile.close();
      server.send(200, "text/plain", "Upload complete");
    }
  }
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(204);
}

// Add implementations for other handlers here...

void webServerSetupLogic(String router, String pass) {
  // Add HTML content handling
  File html = LittleFS.open("/site.htm", "r");
  String responseHTML = "";
  if (html) {
    while (html.available()) {
      responseHTML += (char)html.read();
    }
    html.close();
  }

  // Update server routes
  server.on("/router", HTTP_GET, handleRouterSettings);
  server.on("/pattern", HTTP_GET, handlePatternSettings);
  server.on("/intervalChange", HTTP_GET, handleIntervalChange);
  server.on("/brightness", HTTP_GET, handleBrightness);
  server.on("/setting", HTTP_GET, handleGeneralSettings);
  
  // Add missing routes
  server.on("/resetimagetouse", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.sendHeader("Access-Control-Allow-Credentials", "true");
    imageToUse = 0;
    previousMillis3 = millis();
    server.send(200, "text/plain", "");
  });

  server.on("/returnsettings", HTTP_GET, []() {
    File settings = LittleFS.open("/settings.txt", "r");
    String content = settings.readStringUntil('\n') + "," +
                    settings.readStringUntil('\n') + "," +
                    String(apChannel) + "," +
                    String(addrNumA) + "," +
                    String(addrNumB) + "," +
                    String(addrNumC) + "," +
                    String(addrNumD) + "," +
                    String(patternChooser);
    settings.close();
    server.send(200, "text/html", content);
  });

  // Update notFound handler to match original
  server.onNotFound([]() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.sendHeader("Access-Control-Allow-Credentials", "true");
    server.send(200, "text/html", responseHTML);
  });

  // Keep existing setup
  server.on("/get-pixels", HTTP_GET, handleGetPixels);
  server.on("/options", HTTP_OPTIONS, handleOptions);
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/edit", HTTP_GET, handleFileRead);
  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {}, handleFileUpload);
  
  server.begin();
}
