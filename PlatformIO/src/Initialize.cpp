#include "Initialize.h"
#include "Globals.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <EEPROM.h>
#if defined(PLATFORM_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiMulti.h>
#include <DNSServer.h>
// WebServer server(80); //moved to WEbServerSetup
WiFiMulti WiFiMulti;
DNSServer dnsServer;

#elif defined(PLATFORM_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <DNSServer.h>
// ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;
DNSServer dnsServer;
#endif

// #define LED_APA102 true

void eepromBrightnessChooser(int addr)
{
    int readBRTeprom = EEPROM.read(addr);
    newBrightness = readBRTeprom;
    if (newBrightness > 254 || newBrightness < 1)
    {
        EEPROM.write(15, DEFAULT_BRIGHTNESS);
        newBrightness = DEFAULT_BRIGHTNESS;
    }
    FastLED.setBrightness(newBrightness);
    FastLED.showColor(CRGB::Black);
}

void eepromRouterOptionChooser(int addr)
{
    int newRouter = EEPROM.read(addr);
    routerOption = (newRouter == 1);
    EEPROM.write(100, newRouter);
}

void eepromWifiModeChooser(int addr)
{
    if (routerOption)
    {
        wifiModeChooser = EEPROM.read(addr);
        wifiModeChooser++;
        if (wifiModeChooser > 2)
        {
            wifiModeChooser = 1;
        }
        EEPROM.write(5, wifiModeChooser);
    }
    else
    {
        wifiModeChooser = 1;
    }
}

void eepromPatternChooser(int addr)
{
    patternChooser = EEPROM.read(addr);
    if (patternChooser < 1 || patternChooser > 6)
    {
        patternChooser = 1;
    }
    pattern = patternChooser;
    EEPROM.write(10, patternChooser);
    if (patternChooser == 6)
    {
        readAnotherPatternEEProm();
    }
}

void readAnotherPatternEEProm()
{
    pattern = EEPROM.read(11);
    pattern++;
    if (pattern > 5)
    {
        pattern = 1;
    }
    EEPROM.write(11, pattern);
}

void eepromReadChannelAndAddress(int addr1, int addr2, int addr3, int addr4, int addr5)
{
    apChannel = EEPROM.read(addr1);
    if (apChannel < 1 || apChannel > 11)
    {
        apChannel = 1;
        EEPROM.write(13, 1);
    }
    addrNumD = EEPROM.read(addr2);
    addrNumA = EEPROM.read(addr3);
    addrNumB = EEPROM.read(addr4);
    addrNumC = EEPROM.read(addr5);
}

void littleFSLoadSettings()
{
    settings = LittleFS.open("/settings.txt", "r");
    Field = settings.readStringUntil('\n');
    char router_array[Field.length() + 1];
    Field.toCharArray(router_array, Field.length() + 1);

    Field = settings.readStringUntil('\n');
    char pwd_array[Field.length() + 1];
    Field.toCharArray(pwd_array, Field.length() + 1);

    settings.close();
    wifiChooser(router_array, pwd_array);
}

void checkFilesInSetup()
{
    File root = LittleFS.open("/");
    if (!root)
    {
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        size_t fileSize = file.size();
        if (fileSize > MAX_PX)
        {
            LittleFS.remove(file.name());
        }
        else
        {
            uint8_t buffer[10];
            if (file.read(buffer, sizeof(buffer)) != sizeof(buffer))
            {
                LittleFS.remove(file.name());
            }
        }
        file.close();
        file = root.openNextFile();
    }
    root.close();
}

void wifiChooser(char router_array[], char pwd_array[])
{
    #if C_THREE
        WiFi.setTxPower(WIFI_POWER_8_5dBm);  // for ESP32 C3 Super Mini - helps WiFi be more reliable! 
    #endif
    if (wifiModeChooser == 1)
    {
        if (auxillary)
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(apName, apPass);
            WiFi.config(apIPauxillary, ipGatewayauxillary, ipSubnet, ipGatewayauxillary);
        }
        else
        {
            WiFi.mode(WIFI_AP);
            WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
            WiFi.softAP(apName, apPass, apChannel);
            dnsServer.start(DNS_PORT, "*", apIP);
        }
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFiMulti.addAP(router_array, pwd_array);
        Serial.print("Connecting to: ");
        Serial.println(router_array);
        Serial.print("Attempt: ");
        byte wifiConnectAttemptCount = 0;
        while (WiFiMulti.run() != WL_CONNECTED)
        {
            Serial.print(int(wifiConnectAttemptCount));
            Serial.print(", ");
            wifiConnectAttemptCount++;
            if (wifiConnectAttemptCount > 18)
            {
                ESP.restart();
            }
            delay(500);
        }
    }
}

void handleDNSServer(){
    dnsServer.processNextRequest();
}

void fastLEDInit()
{

////////////////////////////////////////////////Fast LED Setup: ////////////////////////////////////////////////////////////////////////////////////////////////
// APA102 or WS2812 now defined in platformio.ini. #define LED_APA102=false to change to WS2812

#if LED_APA102
    // For APA102/SK9822 LEDs
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
#else
    // For WS2812/NeoPixel LEDs
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
#endif

    FastLED.setBrightness(newBrightness); // should be low figure here, for startup battery saving...

    FastLED.showColor(CRGB::Black);

    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    ///////////////////////////////////////////////////////////////////////end FastLED setup///////////////////////////////////////////////////////////////////
}

void fastLEDIndicate()
{
    if (wifiModeChooser == 1)
    {
        CRGB color = auxillary ? CRGB::Red : CRGB::Blue;
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = color;
            FastLED.show();
            leds[i] = CRGB::Black;
            FastLED.delay(10);
        }
    }
    else
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Green;
            FastLED.show();
            leds[i] = CRGB::Black;
            FastLED.delay(10);
        }
    }
    FastLED.showColor(CRGB::Black);
}

void fastLEDIndicateFast()
{
    if (wifiModeChooser == 1)
    {
        CRGB color = auxillary ? CRGB::Magenta : CRGB::Cyan;
        for (int i = 0; i < NUM_LEDS; i += 2)
        {
            leds[i] = color;
            FastLED.show();
            leds[i] = CRGB::Black;
            FastLED.delay(15);
        }
    }
    else
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Green;
            FastLED.show();
            leds[i] = CRGB::Black;
            FastLED.delay(15);
        }
    }
    FastLED.showColor(CRGB::Black);
}
