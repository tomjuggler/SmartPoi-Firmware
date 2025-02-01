#include "Globals.h"

#if defined(PLATFORM_ESP32)
#include <WebServer.h>  // Explicit include for ESP32 WebServer
WebServer server(80);
#elif defined(PLATFORM_ESP8266)
#include <ESP8266WebServer.h>  // Explicit include for ESP8266WebServer
ESP8266WebServer server(80);
#endif
