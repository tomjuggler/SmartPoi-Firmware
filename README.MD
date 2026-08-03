# SmartPoi ESP8266 Firmware

Combined 36, 60, and 120px version of SmartPoi ESP8266 firmware.  
Works well on D1 mini (fully tested) and may work OK on ESP-01 (not recommended anymore).
Other ESP8266 boards - mileage may vary. 

## Features
- Web UI for pattern, brightness, interval, and file management
- LittleFS image storage
- **OTA firmware & filesystem updates via ElegantOTA** (essential for updating while the poi is sealed in its housing!)
- Pattern file existence cache + load-once image caching (fewer filesystem hits)
- Smooth brightness ramp (targetBrightness)
- Cylon multicolour pattern (70)

## Arduino Libraries Required
Install via Arduino IDE Library Manager:
- **FastLED** — LED strip control
- **ElegantOTA** — OTA firmware/filesystem updates (by Ayush Sharma). Requires a 4MB flash board (e.g. D1 mini).
- **ArduinoJson** — JSON parsing for the SmartPoi API check-in (use version 6.x, e.g. 6.21.1, to match the ESP32 version).

Built into ESP8266 core (no install needed):
- ESP8266WiFi, ESP8266WebServer, ESP8266WiFiMulti
- ESP8266HTTPClient, WiFiClientSecure (BearSSL), DNSServer, WiFiUdp, EEPROM, LittleFS

## SmartPoi API Check-in

In Router mode only (not AP mode), the poi sends a one-time HTTPS GET check-in to `https://smartpoifirmware.circusscientist.com/api/smartpoi-checkin` at startup (backported from the ESP32 version). The JSON response (status/ip/timestamp) is logged over Serial.

> HTTPS on ESP8266 uses BearSSL `WiFiClientSecure` with `setInsecure()` (no X.509 validation) — the ESP32's `http.begin(url)` does not verify certificates by default either. TLS needs the "SSL: All" core option (already set in `tools/compile_check.sh`).

- DNSServer, WiFiUdp, EEPROM, LittleFS

## OTA Updates (ElegantOTA)

OTA lets you update firmware (and the LittleFS image filesystem) over WiFi - no need to open the housing!

- Open `http://<poi-ip>/update` (or `/elegant`) in a browser while connected to the poi's AP (default `192.168.1.1`).
- Upload a compiled firmware `.bin` (from File > Preferences > Show verbose output, or `arduino-cli compile`) for firmware updates.
- Upload a `.littlefs.bin` filesystem image for image/site updates.
- The poi reboots automatically after a successful update (`ElegantOTA.setAutoReboot(true)`).

### Required board settings (Arduino IDE, D1 mini)

OTA needs a 4MB flash layout with an OTA partition. In Arduino IDE select:

- Board: **LOLIN(WEMOS) D1 mini** (or clone)
- CPU Frequency: **160 MHz**
- Flash Size: **4MB (FS:1MB OTA:~1019KB)** — i.e. the `4M1M` layout. Do **not** use `4MB (FS:3MB OTA:~512KB)` - it has barely enough headroom for the current firmware (~350KB).
- lwIP Variant: **V2 Higher Bandwidth**

> The 1MB filesystem fits all stock images (~216KB). If you need more image storage at the cost of OTA headroom, `4MB (FS:2MB OTA:~1019KB)` also works.

### Compile check (arduino-cli)

`tools/compile_check.sh` compiles the sketch for the D1 mini with the OTA-capable `4M1M` layout:

```bash
bash tools/compile_check.sh
```

## Control App
**Android Controls** — download from Google Play:  
[https://play.google.com/store/apps/details?id=com.circusscientist.smartpoicontrols](https://play.google.com/store/apps/details?id=com.circusscientist.smartpoicontrols)

## Install Tutorial
See the step-by-step guide:  
[https://www.circusscientist.com/2026/05/21/installing-smartpoi-d1-mini-version-with-arduino-ide-v2/](https://www.circusscientist.com/2026/05/21/installing-smartpoi-d1-mini-version-with-arduino-ide-v2/)

## Newer ESP32 Version
A newer, more capable ESP32 version of SmartPoi is available:  
[https://github.com/tomjuggler/SmartPoi_ESP32](https://github.com/tomjuggler/SmartPoi_ESP32) — the ESP8266 version here is kept on par with it (ElegantOTA, pattern caching, unified LittleFS paths) while targeting Arduino IDE.

## Magic Poi (ESP32)
Coming soon — see [magicpoi.com](http://magicpoi.com)  
Support development on Patreon: [https://patreon.com/c/circusscientist](https://patreon.com/c/circusscientist)

---
*Please note this is a very old project, based on code I wrote 10 years ago while learning C++. It's messy!*

Under active development.
