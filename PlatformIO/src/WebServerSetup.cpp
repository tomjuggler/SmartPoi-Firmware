#include "WebServerSetup.h"
#include "Globals.h"
#include <Arduino.h>
#include <EEPROM.h> // Add this line at the top

// Add platform-specific server declaration
#if defined(PLATFORM_ESP32)
  WebServer poiserver(80);
#elif defined(PLATFORM_ESP8266)
  ESP8266WebServer poiserver(80);
#endif

void handleAllServers()
{
    poiserver.handleClient();
}

// Helper functions
bool checkFileSpace(size_t fileSize) {
  size_t totalSpace = LittleFS.totalBytes();
  size_t maxAllowedSize = totalSpace - MAX_PX - 1024;
  return (fileSize <= maxAllowedSize);
}

/**
 * @brief Get the total space in LittleFS (ESP8266)
 *
 * @return size_t Total space in bytes
 */
size_t getTotalSpace()
{
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes;
#elif defined(ESP32)
    return LittleFS.totalBytes();
#endif
}

size_t getRemainingSpace()
{
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes - fs_info.usedBytes;
#elif defined(ESP32)
    return LittleFS.totalBytes() - LittleFS.usedBytes();
#endif
}

size_t getUsedSpace()
{
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.usedBytes;
#elif defined(ESP32)
    return LittleFS.usedBytes();
#endif
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
  if(poiserver.hasArg("download")) return "application/octet-stream";
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
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  poiserver.send(200, "text/plain", String(NUM_PX));
}

void handleResetImageToUse() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  imageToUse = 0;
  previousMillis3 = millis();
  poiserver.send(200, "text/plain", "");
}

void handleReturnSettings() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  File settings = LittleFS.open("/settings.txt", "r");
  String settingsSSID = settings.readStringUntil('\n');
  String settingsPASS = settings.readStringUntil('\n');
  settings.close();
  
  String content = settingsSSID + "," + settingsPASS + "," + String(apChannel) + "," + 
                 String(addrNumA) + "," + String(addrNumB) + "," + 
                 String(addrNumC) + "," + String(addrNumD) + "," + 
                 String(patternChooser);
                 poiserver.send(200, "text/html", content);
}

// File management handlers
void handleFileList() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");

  if (!poiserver.hasArg("dir")) {
    poiserver.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = poiserver.arg("dir");
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
  poiserver.send(200, "application/json", output);
}

void handleFileRead() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");

  if (!poiserver.hasArg("file")) {
    poiserver.send(500, "text/plain", "BAD ARGS");
    return;
  }
  
  #ifdef ESP8266
  String path = poiserver.arg("file");
  #elif defined(ESP32)
  String path = "/" + poiserver.arg("file"); //TODO: is this for ESP32 only? It works - test on ESP8266 to confirm - could be only S3? 
  #endif
  String contentType = getContentType(path);

  // Serial.print("handleFileRead for file: ");
  // Serial.println(path);

  if(LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = poiserver.streamFile(file, contentType);
    file.close();
  } else {
    poiserver.send(404, "text/plain", "File not found");
  }
}

void handleFileCreate() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  String path = poiserver.arg("path");
  if(path.isEmpty()) {
    poiserver.send(400, "text/plain", "Bad request");
    return;
  }
  
  if(LittleFS.exists(path)) {
    poiserver.send(409, "text/plain", "File exists");
    return;
  }
  
  File file = LittleFS.open(path, "w");
  if(file) {
    file.close();
    poiserver.send(200, "text/plain", "Created");
  } else {
    poiserver.send(500, "text/plain", "Create failed");
  }
}

void handleFileDelete() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  String path = poiserver.arg("path");
  if(path.isEmpty()) {
    poiserver.send(400, "text/plain", "Bad request");
    return;
  }
  
  if(!LittleFS.remove(path)) {
    poiserver.send(500, "text/plain", "Delete failed");
    return;
  }
  
  poiserver.send(200, "text/plain", "Deleted");
}
// test function to clear memory while uploading.
void clearArray()
{
  memset(message1Data, 0, sizeof(message1Data));
}

void handleFileUpload() { //todo: cors error? 
  Serial.print("handleFileUpload for file");
  clearArray();
  
  HTTPUpload& upload = poiserver.upload();
  static size_t fileSize = 0;

  if(upload.status == UPLOAD_FILE_START) {
    poiserver.sendHeader("Access-Control-Allow-Origin", "*");
    poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    Serial.println(filename);
    // Validate filename format
    if (filename.length() != 6 || images.indexOf(filename[1]) == -1) {
      poiserver.send(400, "text/plain", "Invalid filename");
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
      poiserver.send(507, "text/plain", "File size exceeds limit");
      return;
    }
    
    if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  } 
  else if(upload.status == UPLOAD_FILE_END) {
    if(fsUploadFile) {
      fsUploadFile.close();
      poiserver.send(200, "text/plain", "Upload complete");
    }
    fileSize = 0;
  }
  else if(upload.status == UPLOAD_FILE_ABORTED) {
    if(fsUploadFile) {
      fsUploadFile.close();
      LittleFS.remove(upload.filename);
    }
    poiserver.send(500, "text/plain", "Upload aborted");
    fileSize = 0;
  }
}

void handleOptions() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  poiserver.send(204);
}

// Route handler implementations
void handleRouterSettings() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onRouter = poiserver.arg("router");
  if (onRouter.length() > 0) {
    EEPROM.write(100, 0);
    EEPROM.commit();
    int newRouter = onRouter.toInt();
    routerOption = (newRouter == 1);
    EEPROM.write(100, newRouter);
    poiserver.send(200, "application/json", "{\"Success\":\" your pattern is set \"}");
  } else {
    poiserver.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
  EEPROM.commit();
  FastLED.showColor(CRGB::Black);
}

void handlePatternSettings() {
  Serial.print("pattern change requested: ");
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onAddress = poiserver.arg("patternChooserChange");
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
    poiserver.send(200, "application/json", "{\"Success\":\" your pattern is set \"}");
  } else {
    poiserver.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleIntervalChange() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String newInterval = poiserver.arg("interval");
  if (newInterval.length() > 0) {
    int tmp = newInterval.toInt();
    interval = (tmp < 1) ? 500L : 
              (tmp > 1800) ? 1800L * 1000L : 
              tmp * 1000L;
    poiserver.send(200, "application/json", "{\"Success\":\" your interval is set \"}");
  } else {
    poiserver.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleBrightness() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  
  String onNewBRT = poiserver.arg("brt");
  if (onNewBRT.length() > 0) {
    newBrightness = constrain(onNewBRT.toInt(), 20, 255);
    FastLED.setBrightness(newBrightness);
    FastLED.showColor(CRGB::Black);
    EEPROM.write(15, newBrightness);
    EEPROM.commit();
    poiserver.send(200, "application/json", "{\"Success\":\" your brightness is set \"}");
  } else {
    poiserver.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }
}

void handleGeneralSettings() {
  poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  poiserver.sendHeader("Access-Control-Allow-Credentials", "true");

  // Handle settings file
  File settings = LittleFS.open("/settings.txt", "w");
  if (settings) {
    settings.print(poiserver.arg("ssid") + "\n" + poiserver.arg("pwd"));
    settings.close();
  }

  // Handle channel setting
  String onChannel = poiserver.arg("channel");
  if (onChannel.length() > 0) {
    int newChannel = onChannel.toInt();
    EEPROM.write(13, newChannel);
    EEPROM.commit();
  }

  // Handle address settings
  String addresses[] = {"addressA", "addressB", "addressC"};
  int eepromAddrs[] = {16, 17, 18};
  for (int i = 0; i < 3; i++) {
    String arg = poiserver.arg(addresses[i]);
    if (arg.length() > 0) {
      EEPROM.write(eepromAddrs[i], arg.toInt());
      EEPROM.commit();
    }
  }

  // Handle pattern chooser
  String patternArg = poiserver.arg("patternChooserChange");
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

  poiserver.send(200, "application/json", "{\"Success\":\"Settings updated\"}");
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
  poiserver.on("/list", HTTP_GET, handleFileList);
  poiserver.on("/edit", HTTP_GET, handleFileRead);
  poiserver.on("/edit", HTTP_PUT, handleFileCreate);
  poiserver.on("/edit", HTTP_DELETE, handleFileDelete);
  poiserver.on("/edit", HTTP_POST, []() {}, handleFileUpload);
  poiserver.on("/get-pixels", HTTP_GET, handleGetPixels);
  poiserver.on("/options", HTTP_OPTIONS, handleOptions);
  poiserver.on("/resetimagetouse", HTTP_GET, handleResetImageToUse);
  poiserver.on("/returnsettings", HTTP_GET, handleReturnSettings);
  poiserver.on("/router", HTTP_GET, handleRouterSettings);
  poiserver.on("/pattern", HTTP_GET, handlePatternSettings);
  poiserver.on("/intervalChange", HTTP_GET, handleIntervalChange);
  poiserver.on("/brightness", HTTP_GET, handleBrightness);
  poiserver.on("/setting", HTTP_GET, handleGeneralSettings);

  // // Set notFound handler last
  // poiserver.onNotFound([]() {
  //   poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  //   poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  //   poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  //   poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  //   imageToUse = 0;
  //   previousMillis3 = millis();
  //   poiserver.send(200, "text/plain", "");
  // });

  poiserver.on("/returnsettings", HTTP_GET, []() {
    poiserver.sendHeader("Access-Control-Allow-Origin", "*");
    poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
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
    poiserver.send(200, "text/html", content);
  });

  // Update notFound handler to match original
  // poiserver.onNotFound([]() {
  //   poiserver.sendHeader("Access-Control-Allow-Origin", "*");
  //   poiserver.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  //   poiserver.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  //   poiserver.sendHeader("Access-Control-Allow-Credentials", "true");
  //   poiserver.send(200, "text/html", responseHTML);
  // });

  // Keep existing setup
  // poiserver.on("/get-pixels", HTTP_GET, handleGetPixels);
  // poiserver.on("/options", HTTP_OPTIONS, handleOptions);
  // poiserver.on("/list", HTTP_GET, handleFileList); //TODO: The error message [E][WebServer.cpp:793] _handleRequest() specifically indicates the ESP web server received a request for an unregistered path. Double-check your route registration order and handler function availability.
  // poiserver.on("/edit", HTTP_GET, handleFileRead);
  // poiserver.on("/edit", HTTP_PUT, handleFileCreate);
  // poiserver.on("/edit", HTTP_DELETE, handleFileDelete);
  // poiserver.on("/edit", HTTP_POST, []() {}, handleFileUpload);
  
  poiserver.begin();
}
