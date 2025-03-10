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

String getContentType(String filename) {
  if(filename.endsWith(".htm")) return "text/html";
  if(filename.endsWith(".html")) return "text/html";
  if(filename.endsWith(".css")) return "text/css";
  if(filename.endsWith(".js")) return "application/javascript";
  if(filename.endsWith(".png")) return "image/png";
  if(filename.endsWith(".gif")) return "image/gig";
  if(filename.endsWith(".jpg")) return "image/jpeg";
  if(filename.endsWith(".ico")) return "image/x-icon";
  if(filename.endsWith(".xml")) return "text/xml";
  if(filename.endsWith(".pdf")) return "application/x-pdf";
  if(filename.endsWith(".zip")) return "application/x-zip";
  if(filename.endsWith(".gz")) return "application/x-gzip";
  if(filename.endsWith(".bin")) return "application/octet-stream";
  return "text/plain";
}

bool checkFileSpace(size_t fileSize) {
  size_t totalSpace = LittleFS.totalBytes();
  size_t maxAllowedSize = totalSpace - MAX_PX - 1024;
  return (fileSize <= maxAllowedSize);
}

unsigned long ota_progress_millis = 0;
TaskHandle_t elegantOTATaskHandle = NULL;

bool checkFileSpace(size_t fileSize) {
  size_t totalSpace = LittleFS.totalBytes();
  size_t maxAllowedSize = totalSpace - MAX_PX - 1024;
  return (fileSize <= maxAllowedSize);
}

// server code:
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
  String path = request->arg("file");
  if(LittleFS.exists(path)) {
    request->send(LittleFS, path, getContentType(path));
  } else {
    request->send(404, "text/plain", "File not found");
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

void handleFileUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) {
    static File fsUploadFile;
    static size_t totalSize = 0;
    
    if(!index) {
        totalSize = 0;
        if(!checkFileSpace(request->contentLength()) || 
           request->contentLength() > MAX_PX || 
           request->contentLength() > LittleFS.totalBytes() - LittleFS.usedBytes()) {
            request->send(507, "text/plain", "File size exceeds limit");
            return;
        }
        
        // Add filename validation logic here
        String fname = filename;
        if (!fname.startsWith("/")) fname = "/" + fname;
        if (fname.length() != 6 || images.indexOf(fname[1]) == -1) {
            request->send(400, "text/plain", "Invalid filename");
            return;
        }
        
        fsUploadFile = LittleFS.open(fname, "w");
        if(!fsUploadFile) {
            request->send(500, "text/plain", "Upload failed");
            return;
        }
    }
    
    if(len) {
        fsUploadFile.write(data, len);
        totalSize += len;
    }
    
    if(final) {
        fsUploadFile.close();
        request->send(200, "text/plain", "Upload complete");
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
  server.on("/elegant", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", loadIndexHtml());
  });
  
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

  server.on("/edit", HTTP_DELETE, [](AsyncWebServerRequest *request) {
    if(request->hasArg("path")) {
      handleFileDelete(request);
    } else {
      request->send(400, "text/plain", "Missing path parameter");
    }
  });

  server.on("/get-pixels", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(NUM_PX));
  });

  server.on("/options", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    request->send(204);
  });

  // Existing migrated routes (keep these):
  server.on("/resetimagetouse", HTTP_GET, [](AsyncWebServerRequest *request) {
    imageToUse = 0;
    previousMillis3 = millis();
    request->send(200, "text/plain", "");
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

  // Add notFound handler
  server.onNotFound([](AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/plain");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->setCode(404);
    response->print("Not found");
    request->send(response);
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
