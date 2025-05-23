#include "tasks.h"
#include "Globals.h"
#include "LittleFS.h"


extern long interval;
#include <EEPROM.h>
#include <FastLED.h>

extern AsyncWebServer server;

String getContentType(String filename) {
  if(filename.endsWith(".htm")) return "text/html";
  if(filename.endsWith(".html")) return "text/html";
  if(filename.endsWith(".css")) return "text/css";
  if(filename.endsWith(".js")) return "application/javascript";
  if(filename.endsWith(".png")) return "image/png";
  if(filename.endsWith(".gif")) return "image/gif";
  if(filename.endsWith(".jpg")) return "image/jpeg";
  if(filename.endsWith(".ico")) return "image/x-icon";
  if(filename.endsWith(".xml")) return "text/xml";
  if(filename.endsWith(".pdf")) return "application/x-pdf";
  if(filename.endsWith(".zip")) return "application/x-zip";
  if(filename.endsWith(".gz")) return "application/x-gzip";
  if(filename.endsWith(".bin")) return "application/octet-stream";
  return "text/plain";
}


unsigned long ota_progress_millis = 0;
TaskHandle_t elegantOTATaskHandle = NULL;

bool checkFileSpace(size_t fileSize) {
  size_t totalSpace = LittleFS.totalBytes();
  size_t maxAllowedSize = totalSpace - MAX_PX - 1024;
  return (fileSize <= maxAllowedSize);
}

/**
 * @brief Get the total space in LittleFS
 * @return size_t Total space in bytes
 */
size_t getTotalSpace() {
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes;
#elif defined(ESP32)
    return LittleFS.totalBytes();
#endif
}

/**
 * @brief Get the remaining space in LittleFS
 * @return size_t Remaining space in bytes
 */
size_t getRemainingSpace() {
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes - fs_info.usedBytes;
#elif defined(ESP32)
    return LittleFS.totalBytes() - LittleFS.usedBytes();
#endif
}

/**
 * @brief Get the used space in LittleFS
 * @return size_t Used space in bytes
 */
size_t getUsedSpace() {
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
  while(dblBytes >= 1024 && i < 3) {
    dblBytes /= 1024;
    i++;
  }
  return String(dblBytes, 2) + suffixes[i];
}

// server code:
String loadSiteHtml() {
  if (!LittleFS.begin()) {
    Serial.println("An error occurred while mounting LittleFS");
    return "Error loading page";
  }

  File file = LittleFS.open("/site.htm", "r");
  if (!file) {
    Serial.println("Failed to open index.html");
    return "Error loading page";
  }

  String content = file.readString();
  file.close();
  return content;
}

String loadIndexHtml() {
  if (!LittleFS.begin()) {
    Serial.println("An error occurred while mounting LittleFS");
    return "Error loading page";
  }

  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    Serial.println("Failed to open index.html");
    return "Error loading page";
  }

  String content = file.readString();
  file.close();
  return content;
}

void onOTAStart()
{
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final)
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000)
  {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success)
{
  // Log when OTA has finished
  if (success)
  {
    Serial.println("OTA update finished successfully!");
  }
  else
  {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void handlePatternSettings(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");
  
  if(request->hasArg("patternChooserChange")) {
    int newPatt = request->arg("patternChooserChange").toInt();
    
    if(newPatt < 0 || newPatt > 7) {
        response->setCode(400);
        response->print("{\"Error\":\"Invalid pattern\"}");
        request->send(response);
        return;
    }
    
    patternChooser = newPatt;
    EEPROM.write(10, newPatt);
    
    if(newPatt > 0 && newPatt < 6) {
      pattern = patternChooser;
      EEPROM.write(11, newPatt);
    }
    else if(newPatt == 7) {
      FastLED.showColor(CRGB::Black);
      pattern = patternChooser;
    }
    
    EEPROM.commit();
    response->setCode(200);
    response->print("{\"Success\":\"Pattern set\"}");
  } else {
    response->setCode(400);
    response->print("{\"Error\":\"Missing parameter\"}");
  }
  request->send(response);
}

void handleRouterSettings(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  if(request->hasArg("router")) {
    int newRouter = request->arg("router").toInt();
    routerOption = (newRouter == 1);
    EEPROM.write(100, newRouter);
    EEPROM.commit();
    FastLED.showColor(CRGB::Black);
    response->setCode(200);
    response->print("{\"Success\":\"Router mode set\"}");
  } else {
    response->setCode(400);
    response->print("{\"Error\":\"Missing parameter\"}");
  }
  request->send(response);
}

void handleIntervalChange(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  if(request->hasArg("interval")) {
    long tmp = request->arg("interval").toInt();
    interval = (tmp < 1) ? 500L : 
              (tmp > 1800) ? 1800L * 1000L : 
              tmp * 1000L;
    response->setCode(200);
    response->print("{\"Success\":\"Interval updated\"}");
  } else {
    response->setCode(400);
    response->print("{\"Error\":\"Missing parameter\"}");
  }
  request->send(response);
}

void handleBrightness(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  if(request->hasArg("brt")) {
    newBrightness = constrain(request->arg("brt").toInt(), 20, 255);
    FastLED.setBrightness(newBrightness);
    FastLED.show();
    EEPROM.write(15, newBrightness);
    EEPROM.commit();
    response->setCode(200);
    response->print("{\"Success\":\"Brightness updated\"}");
  } else {
    response->setCode(400);
    response->print("{\"Error\":\"Missing parameter\"}");
  }
  request->send(response);
}

// File operations handlers
void handleFileList(AsyncWebServerRequest *request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");
  
  String path = request->hasArg("dir") ? request->arg("dir") : "/";
  String output = "[";
  
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
  output += "]";
  
  response->print(output);
  request->send(response);
}

void handleFileRead(AsyncWebServerRequest *request) {
#ifdef ESP32
  String path = "/" + request->arg("file"); // ESP32 needs leading slash
#else
  String path = request->arg("file");
#endif

  if(LittleFS.exists(path)) {
    // Create response with file contents AND headers
    AsyncWebServerResponse* response = request->beginResponse(LittleFS, path, getContentType(path));
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    request->send(response);
  } else {
    // Create error response with headers
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->setCode(404);
    response->print("File not found");
    request->send(response);
  }
}

void handleFileCreate(AsyncWebServerRequest *request) {
  AsyncResponseStream* response = request->beginResponseStream("text/plain");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  String path = request->arg("path");
  if(path.isEmpty()) {
    response->setCode(400);
    response->print("Bad request");
    request->send(response);
    return;
  }
  
  if(LittleFS.exists(path)) {
    response->setCode(409);
    response->print("File exists");
    request->send(response);
    return;
  }
  
  File file = LittleFS.open(path, "w");
  file.close();
  response->setCode(200);
  response->print("Created");
  request->send(response);
}

void handleFileDelete(AsyncWebServerRequest *request) {
  AsyncResponseStream* response = request->beginResponseStream("text/plain");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  String path = request->arg("path");
  if(path.isEmpty()) {
    response->setCode(400);
    response->print("Bad request");
    request->send(response);
    return;
  }
  
  if(!LittleFS.remove(path)) {
    response->setCode(500);
    response->print("Delete failed");
    request->send(response);
    return;
  }
  
  response->setCode(200);
  response->print("Deleted");
  request->send(response);
}

void handleGeneralSettings(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  response->addHeader("Access-Control-Allow-Headers", "Content-Type");
  response->addHeader("Access-Control-Allow-Credentials", "true");

  // Handle settings.txt
  File settings = LittleFS.open("/settings.txt", "w");
  if (settings) {
    settings.print(request->arg("ssid") + "\n" + request->arg("pwd"));
    settings.close();
  }

  // Channel setting
  if(request->hasArg("channel")) {
    int newChannel = request->arg("channel").toInt();
    EEPROM.write(13, newChannel);
  }

  // Address settings
  const String addresses[] = {"addressA", "addressB", "addressC"};
  const int eepromAddrs[] = {16, 17, 18};
  for(int i=0; i<3; i++) {
    if(request->hasArg(addresses[i])) {
      EEPROM.write(eepromAddrs[i], request->arg(addresses[i]).toInt());
    }
  }

  // Pattern chooser
  if(request->hasArg("patternChooserChange")) {
    int newPatt = request->arg("patternChooserChange").toInt();
    patternChooser = newPatt;
    EEPROM.write(10, newPatt);
    
    if(newPatt > 0 && newPatt < 6) {
      pattern = patternChooser;
      EEPROM.write(11, newPatt);
    }
  }

  EEPROM.commit();
  response->setCode(200);
  response->print("{\"Success\":\"Settings updated\"}");
  request->send(response);
}

void clearArray() {
  memset(message1Data, 0, sizeof(message1Data));
}

/**
 * @brief Handles file uploads with platform-specific path handling
 * @note Contains ESP32-specific path prefix logic
 * @todo Verify CORS handling for upload endpoint
 */
void handleFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    static File fsUploadFile;
    static size_t totalFileSize = 0;

    if(!index) { // Start of upload
        // Clear memory and reset tracking
        clearArray();
        totalFileSize = 0;
        
        // Create a new variable instead of modifying const parameter
        String fullPath = "/" + filename;
        
        // Validate filename format
        if (fullPath.length() != 6 || images.indexOf(fullPath[1]) == -1) {
            request->send(400, "text/plain", "Invalid filename");
            return;
        }

        // Check remaining space before opening file
        size_t remainingSpace = getRemainingSpace();
        if(request->contentLength() > remainingSpace || 
           request->contentLength() > MAX_PX) {
            request->send(507, "text/plain", "File size exceeds limit");
            return;
        }

        // Attempt to open file
        fsUploadFile = LittleFS.open(fullPath, "w");
        if(!fsUploadFile) {
            request->send(500, "text/plain", "Upload failed");
            return;
        }
    }

    // Write received data
    if(len > 0 && fsUploadFile) {
        // Check cumulative size during write
        totalFileSize += len;
        if(totalFileSize > MAX_PX || 
           totalFileSize > getRemainingSpace()) {
            fsUploadFile.close();
            LittleFS.remove(filename);
            request->send(507, "text/plain", "File size exceeds limit");
            return;
        }
        
        fsUploadFile.write(data, len);
    }

    // Finalize upload
    if(final && fsUploadFile) {
        fsUploadFile.close();
        request->send(200, "text/plain", "Upload complete");
    }
    
    // Handle aborted uploads
    if(!final && !fsUploadFile) {
        request->send(500, "text/plain", "Upload aborted");
    }
}

/////////////////////////////////////////////// end elegantOTA code //////////////////////////////////////

/**
 * @brief Sets up ElegantOTA task for WiFi firmware updates and Server
 */
void setupElegantOTATask()
{
  xTaskCreatePinnedToCore(
      elegantOTATask,        // Task function
      "Elegant OTA Task",    // Name of the task
      4096,                  // Stack size (in words, not bytes)
      NULL,                  // Task input parameter
      3,                     // Priority of the task
      &elegantOTATaskHandle, // Task handle
      0                      // Core where the task should run (core 1)
  );
}

/**
 * @brief Periodically checks for WiFi firmware updates and handles Web Server
 * @details Server handling also happens here
 */
void elegantOTATask(void *pvParameters)
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", loadSiteHtml());
  });

  server.on("/site", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/"); // Default alternative
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {                                                                               
    request->redirect("/");  // Android captive portal check                                                                                              
  });                                                                                                                                                     
                                                                                                                                                          
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {                                                                        
    request->redirect("/");  // Apple captive portal check                                                                                                
  });                                                                                                                                                     
                                                                                                                                                          
  server.on("/connectivity-check.html", HTTP_GET, [](AsyncWebServerRequest *request) {                                                                    
    request->redirect("/");  // Windows/Linux captive portal check                                                                                        
  }); 

  server.on("/elegant", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", loadIndexHtml());
  });

  //NOTE: handled by ElegantOTA:
  // "/update" *OTA UPDATE
  
  // Migrated routes from WebServerSetup.cpp
  server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleFileList(request);
  });

  server.on("/edit", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(request->hasArg("file")) {
      handleFileRead(request);
    } else {
      request->send(400, "text/plain", "Missing file parameter");
    }
  });

  server.on("/edit", HTTP_PUT, [](AsyncWebServerRequest *request) {
    if(request->hasArg("path")) {
      handleFileCreate(request);
    } else {
      request->send(400, "text/plain", "Missing path parameter");
    }
  });

  server.on("/edit", HTTP_POST, 
    [](AsyncWebServerRequest *request) {
      // Handle preflight response
      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain");
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
      response->addHeader("Access-Control-Allow-Headers", "Content-Type");
      response->addHeader("Access-Control-Allow-Credentials", "true");
      request->send(response);
    },
    handleFileUpload,  // Upload handler (4th param)
    NULL               // Body handler (5th param)
  );

  // Add specific OPTIONS handler for DELETE preflight on /edit
  server.on("/edit", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "DELETE, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    request->send(response);
  });

  server.on("/edit", HTTP_DELETE, [](AsyncWebServerRequest *request) {
    if(request->hasArg("path")) {
      handleFileDelete(request);
    } else {
      request->send(400, "text/plain", "Missing path parameter");
    }
  });

  server.on("/get-pixels", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->print(String(NUM_PX));
    request->send(response);
  });

  server.on("/options", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    request->send(response);
  });

  // Existing migrated routes (keep these):
  server.on("/resetimagetouse", HTTP_GET, [](AsyncWebServerRequest *request) {
    imageToUse = 0;
    previousMillis3 = millis();
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->print("");
    request->send(response);
  });

  server.on("/returnsettings", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/html");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    
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
    response->print(content);
    request->send(response);
  });

  // notFound handler (for captive portal)
  server.onNotFound([](AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    // response->setCode(404);
    // response->print("Not found");
    // request->send(response);
    request->redirect("/");
  });

  server.on("/pattern", HTTP_GET, [](AsyncWebServerRequest *request) {
    handlePatternSettings(request);
  });

  server.on("/router", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleRouterSettings(request);
  });

  server.on("/intervalChange", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleIntervalChange(request);
  });

  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleBrightness(request);
  });

  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleGeneralSettings(request);
  });
  
  server.begin();
  ElegantOTA.begin(&server); // Start ElegantOTA

  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  // loop handling ElegantOTA:
  for (;;)
  {
    ElegantOTA.loop(); // OTA updates: see https://randomnerdtutorials.com/esp32-ota-over-the-air-vs-code/ for usage
     vTaskDelay(100 / portTICK_PERIOD_MS); // Yield to other tasks
  }
}
