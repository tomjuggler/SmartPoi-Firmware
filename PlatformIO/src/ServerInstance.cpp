#include "Globals.h"

#if defined(PLATFORM_ESP32)
  WebServer server(80);
#elif defined(PLATFORM_ESP8266)
  ESP8266WebServer server(80);
#endif
