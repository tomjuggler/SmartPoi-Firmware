# SmartPoi ESP8266 Firmware

Combined 36, 60, and 120px version of SmartPoi ESP8266 firmware.  
Works well on D1 mini and OK on ESP-01.

## Features
- ESP-NOW master-slave synchronisation (main + auxiliary Poi)
- Dual-transport pixel reception (UDP + ESP-NOW)
- Web UI for pattern, brightness, interval, and file management
- LittleFS image storage

## Arduino Libraries Required
Install via Arduino IDE Library Manager:
- **FastLED** — LED strip control
- **LittleFS** — file system (may be built into ESP8266 core)

Built into ESP8266 core (no install needed):
- ESP8266WiFi, ESP8266WebServer, ESP8266WiFiMulti
- DNSServer, WiFiUdp, EEPROM
- espnow (ESP-NOW communication)

## Control App
**Android Controls** — download from Google Play:  
[https://play.google.com/store/apps/details?id=com.circusscientist.smartpoicontrols](https://play.google.com/store/apps/details?id=com.circusscientist.smartpoicontrols)

## Install Tutorial
See the step-by-step guide:  
[https://www.circusscientist.com/2026/05/21/installing-smartpoi-d1-mini-version-with-arduino-ide-v2/](https://www.circusscientist.com/2026/05/21/installing-smartpoi-d1-mini-version-with-arduino-ide-v2/)

## Magic Poi (ESP32)
Coming soon — see [magicpoi.com](http://magicpoi.com)  
Support development on Patreon: [https://patreon.com/c/circusscientist](https://patreon.com/c/circusscientist)

---
*Please note this is a very old project, based on code I wrote 10 years ago while learning C++. It's messy!*

Under active development.
