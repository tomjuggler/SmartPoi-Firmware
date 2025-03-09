#include <ElegantOTA.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Function declarations
void setupElegantOTATask();
void elegantOTATask(void *pvParameters);