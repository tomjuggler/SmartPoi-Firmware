#include "WebServerSetup.h"
#include "Globals.h"
#include <Arduino.h>
#include <EEPROM.h> // Add this line at the top

// Add platform-specific server declaration
#if defined(PLATFORM_ESP32)
  WebServer server(80);
#elif defined(PLATFORM_ESP8266)
  ESP8266WebServer server(80);
#endif

void handleAllServers()
{
    server.handleClient();
}

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
  if(filename.endsWith(".htm"))  return "text/html";
  if(filename.endsWith(".html")) return "text/html";
  if(filename.endsWith(".css"))  return "text/css";
  if(filename.endsWith(".js"))   return "application/javascript";
  if(filename.endsWith(".png"))  return "image/png";
  if(filename.endsWith(".gif"))  return "image/gif";
  if(filename.endsWith(".jpg"))  return "image/jpeg";
  if(filename.endsWith(".ico"))  return "image/x-icon";
  if(filename.endsWith(".xml"))  return "text/xml";
  if(filename.endsWith(".pdf"))  return "application/x-pdf";
  if(filename.endsWith(".zip"))  return "application/x-zip";
  if(filename.endsWith(".gz"))   return "application/x-gzip";
  if(filename.endsWith(".bin"))  return "application/octet-stream";
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
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");

  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  String output = "[";

#ifdef ESP8266
  Dir dir = LittleFS.openDir(path);
  while(dir.next()) {
    File entry = dir.openFile("r");
    if(output != "[") output += ',';
    output += "{\"type\":\"";
    output += (entry.isDirectory() ? "dir" : "file");
    output += "\",\"name\":\"";
    output += String(entry.name());
    output += "\"}";
    entry.close();
  }
#elif defined(ESP32)
  File root = LittleFS.open(path);
  File file = root.openNextFile();
  while(file) {
    if(output != "[") output += ',';
    output += "{\"type\":\"";
    output += (file.isDirectory() ? "dir" : "file");
    output += "\",\"name\":\"";
    output += String(file.name());
    output += "\"}";
    file = root.openNextFile();
  }
#endif

  output += "]";
  server.send(200, "application/json", output);
}

void handleFileRead() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");

  if (!server.hasArg("file")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("file");
  String contentType = getContentType(path);
  
  if(LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
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
  static size_t fileSize = 0;

  if(upload.status == UPLOAD_FILE_START) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.sendHeader("Access-Control-Allow-Credentials", "true");

    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    
    // Validate filename format
    if (filename.length() != 6 || images.indexOf(filename[1]) == -1) {
      server.send(400, "text/plain", "Invalid filename");
      return;
    }

    fileSize = 0;
    fsUploadFile = LittleFS.open(filename, "w");
  } 
  else if(upload.status == UPLOAD_FILE_WRITE) {
    fileSize += upload.currentSize;
    
    if (!checkFileSpace(fileSize) || fileSize > MAX_PX || fileSize > getRemainingSpace()) {
      if(fsUploadFile) {
        fsUploadFile.close();
        LittleFS.remove(upload.filename);
      }
      server.send(507, "text/plain", "File size exceeds limit");
      return;
    }
    
    if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  } 
  else if(upload.status == UPLOAD_FILE_END) {
    if(fsUploadFile) {
      fsUploadFile.close();
      server.send(200, "text/plain", "Upload complete");
    }
    fileSize = 0;
  }
  else if(upload.status == UPLOAD_FILE_ABORTED) {
    if(fsUploadFile) {
      fsUploadFile.close();
      LittleFS.remove(upload.filename);
    }
    server.send(500, "text/plain", "Upload aborted");
    fileSize = 0;
  }
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(204);
}

// Route handler implementations
void handleRouterSettings() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onRouter = server.arg("router");
  if (onRouter.length() > 0) {
    EEPROM.write(100, 0);
    EEPROM.commit();
    int newRouter = onRouter.toInt();
    routerOption = (newRouter == 1);
    EEPROM.write(100, newRouter);
    server.send(200, "application/json", "{\"Success\":\" your pattern is set \"}");
  } else {
    server.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
  EEPROM.commit();
  FastLED.showColor(CRGB::Black);
}

void handlePatternSettings() {
  Serial.print("pattern change requested: ");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onAddress = server.arg("patternChooserChange");
  if (onAddress.length() > 0) {
    int newPatt = onAddress.toInt();
    Serial.println(newPatt);
    patternChooser = newPatt;
    EEPROM.write(10, newPatt);
    EEPROM.commit();
    
    if (newPatt < 6 && newPatt > 0) {
      pattern = patternChooser;
      EEPROM.write(11, newPatt);
      EEPROM.commit();
    }
    else if (newPatt == 7) {
      FastLED.showColor(CRGB::Black);
      pattern = patternChooser;
    }
    server.send(200, "application/json", "{\"Success\":\" your pattern is set \"}");
  } else {
    server.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleIntervalChange() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String newInterval = server.arg("interval");
  if (newInterval.length() > 0) {
    int tmp = newInterval.toInt();
    interval = (tmp < 1) ? 500L : 
              (tmp > 1800) ? 1800L * 1000L : 
              tmp * 1000L;
    server.send(200, "application/json", "{\"Success\":\" your interval is set \"}");
  } else {
    server.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleBrightness() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onNewBRT = server.arg("brt");
  if (onNewBRT.length() > 0) {
    newBrightness = constrain(onNewBRT.toInt(), 20, 255);
    FastLED.setBrightness(newBrightness);
    FastLED.showColor(CRGB::Black);
    EEPROM.write(15, newBrightness);
    EEPROM.commit();
    server.send(200, "application/json", "{\"Success\":\" your brightness is set \"}");
  } else {
    server.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleGeneralSettings() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");

  // Handle settings file
  File settings = LittleFS.open("/settings.txt", "w");
  if (settings) {
    settings.print(server.arg("ssid") + "\n" + server.arg("pwd"));
    settings.close();
  }

  // Handle channel setting
  String onChannel = server.arg("channel");
  if (onChannel.length() > 0) {
    int newChannel = onChannel.toInt();
    EEPROM.write(13, newChannel);
    EEPROM.commit();
  }

  // Handle address settings
  String addresses[] = {"addressA", "addressB", "addressC"};
  int eepromAddrs[] = {16, 17, 18};
  for (int i = 0; i < 3; i++) {
    String arg = server.arg(addresses[i]);
    if (arg.length() > 0) {
      EEPROM.write(eepromAddrs[i], arg.toInt());
      EEPROM.commit();
    }
  }

  // Handle pattern chooser
  String patternArg = server.arg("patternChooserChange");
  if (patternArg.length() > 0) {
    int newPatt = patternArg.toInt();
    patternChooser = newPatt;
    EEPROM.write(10, newPatt);
    EEPROM.commit();
    
    if (newPatt < 6 && newPatt > 0) {
      pattern = patternChooser;
      EEPROM.write(11, newPatt);
      EEPROM.commit();
    }
  }

  server.send(200, "application/json", "{\"Success\":\"Settings updated\"}");
}

void webServerSetupLogic(String router, String pass) {
  // Add HTML content handling
  File html = LittleFS.open("/site.htm", "r");
  responseHTML = "";
  if (html) {
    while (html.available()) {
      responseHTML += (char)html.read();
    }
    html.close();
  }

  // Register routes first
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/edit", HTTP_GET, handleFileRead);
  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {}, handleFileUpload);
  server.on("/get-pixels", HTTP_GET, handleGetPixels);
  server.on("/options", HTTP_OPTIONS, handleOptions);
  server.on("/resetimagetouse", HTTP_GET, handleResetImageToUse);
  server.on("/returnsettings", HTTP_GET, handleReturnSettings);
  server.on("/router", HTTP_GET, handleRouterSettings);
  server.on("/pattern", HTTP_GET, handlePatternSettings);
  server.on("/intervalChange", HTTP_GET, handleIntervalChange);
  server.on("/brightness", HTTP_GET, handleBrightness);
  server.on("/setting", HTTP_GET, handleGeneralSettings);

  // Set notFound handler last
  server.onNotFound([]() {
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
  server.on("/list", HTTP_GET, handleFileList); //TODO: The error message [E][WebServer.cpp:793] _handleRequest() specifically indicates the ESP web server received a request for an unregistered path. Double-check your route registration order and handler function availability.
  server.on("/edit", HTTP_GET, handleFileRead);
  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {}, handleFileUpload);
  
  server.begin();
}
