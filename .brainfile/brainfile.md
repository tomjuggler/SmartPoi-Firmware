---
schema: https://brainfile.md/v2/board.json
title: SmartPoi ESP8266 Firmware (Arduino IDE)
agent:
  instructions:
    - Task files are individual .md files in board/
    - Completed tasks are in logs/
    - Preserve all IDs
    - Make minimal changes
columns:
  - id: todo
    title: To Do
  - id: in-progress
    title: In Progress
---

# SmartPoi ESP8266 LED Controller (Arduino IDE)

ESP8266-based LED poi controller supporting multiple patterns, WiFi control, and image display. Target: Arduino IDE (not PlatformIO). This board tracks the upgrade to bring the ESP8266 Arduino version on par with the ESP32 PlatformIO version (ElegantOTA, background process management, LittleFS addressing, pattern caching).

## Key Features
- FastLED-based LED control for APA102 or WS2812 strips
- Multiple display patterns (0-70)
- Web-based control interface with file management (ESP8266WebServer, sync)
- LittleFS filesystem for image storage
- WiFi modes: AP or Station with router configuration
- UDP control for real-time LED manipulation

## Upgrade Goals (from ESP32 PlatformIO version)
- OTA firmware updates via ElegantOTA (sync mode, ESP8266WebServer)
- Pattern file caching (avoid repeated LittleFS.exists())
- LittleFS path unification (leading slash)
- uploadInProgress flag to pause LEDs during uploads
- Load-once image caching in showLittleFSImage
- Web handler parity (pattern 0-70, captive portal, elegant route)
- cylonMulticolour pattern 70 + brightness ramp
- NOTE: FreeRTOS multi-tasking NOT feasible on ESP8266 (80KB RAM) - use cooperative loop() instead

## Build/Verify
- arduino-cli compile --fqbn esp8266:esp8266:d1_mini main (see tools/compile_check.sh)
