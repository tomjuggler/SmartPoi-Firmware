#include "tasks.h"
#include "LittleFS.h"

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
  if(request->hasArg("patternChooserChange")) {
    int newPatt = request->arg("patternChooserChange").toInt();
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
    request->send(200, "application/json", "{\"Success\":\"Pattern set\"}");
  }
  else {
    request->send(400, "application/json", "{\"Error\":\"Missing parameter\"}");
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
  // setup part for Server and ElegantOTA - runs once:
  //todo: update to use server routes from WebServerSetup.cpp - async web server in SmartPoi!!
  server.on("/elegant", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", loadIndexHtml());
  });

  // ... TODO: other server routes - migrate from WebServerSetup.cpp! ...

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
