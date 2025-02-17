Combined 36 and 72px (and now 120px) version of SmartPoi firmware for ESP8266 and ESP32
Works well on D1 mini (ESP8266), ESP32. 

see https://circusscientist.com for details on how to get this to work

DOCUMENTATION: https://tomjuggler.github.io/SmartPoi-Firmware/html/
*please note that this is a very old project, based on code I did 10 years ago while learning C++. It's messy! 

To upload images and control the poi please see smartpoi-js-utilities (works in a browser) or the smartpoi-controls Android app. 
 
*Under active development*

Now supports ESP32! Also coming soon: "Magic Poi" for ESP32 - see http://magicpoi.circusscientist.com 

PlatformIO version is still under development and not working 100%
__main branch is ESP8266 Arduino only *but stable*.__ 



## CODE AUDIT Feb 2025 (branch: PlatformIO_ESP32_2025): 
- much testing needed as this branch will be replacing main branch soon. 

### ARDUINO: 
- ESP-01: Compiles, untested - __not supported__
- D1 Mini: Compiles. Everything works - *supported until end 2025*
- ESP32 C3: __failing__ Compiles. LittleFS Upload seems to work but "Failed to open file for reading"
- ESP32 S3:  __failing__ Compiles. LittleFS Upload seems to work but "Failed to open file for reading"

### PLATFORMIO:
- D1 Mini: __Untested__
- ESP32 C3: Compiles. LittleFS not working (mklittlefs and esptool) - says it works but no files. Uploading using SmartPoi-js-utilities works, controls etc. 
- ESP32 S3: Compiles. LittleFS not working (mklittlefs and esptool) - says it works but no files. Uploading using SmartPoi-js-utilities works, controls etc. 

### 2026: Complete switch to PlatformIO
- due to Magic Poi coming out, there will no longer be the option to support Arduino IDE version after the end of this year. 
- hopefully the project will be completely stable by then anyway. Plus it's open source, have fun. 

## NOTES: 
- ESP32 C3 please use CDC enable (enables Serial ouput for debugging) in Arduino IDE