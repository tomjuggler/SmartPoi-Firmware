#include "UDPHandler.h"
#include "Globals.h"
#include <EEPROM.h>  // Required for EEPROM operations

// UDP Handler Variables
extern unsigned long currentMillis2;
extern unsigned long previousMillis2;
constexpr int interval = 1000;  // 1 second default interval
extern bool checkit;
extern int len;
extern uint8_t packetBuffer[255];
extern uint8_t Y;
extern int state;
extern int X;
extern uint8_t R1;
extern uint8_t G1;
extern uint8_t M1;
extern bool channelChange;

void handleUDP() {
    if (currentMillis2 - previousMillis2 > interval * 2) {
        previousMillis2 = currentMillis2;
        checkit = true;
    }
    previousMillis2 = currentMillis2;  // Use locally declared timing variables
    state = 0;

    len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;

    for (int i = 0; i < NUM_PX; i++) {
        if (checkit) {
            Y = packetBuffer[i];
            switch (i) {
                case 0:
                    if (Y != 0) checkit = false;
                    break;
                case 1:
                    if (Y != 1) checkit = false;
                    break;
                case 2:
                    if (Y == 3) channelChange = true;
                    else if (Y != 2) checkit = false;
                    break;
                case 3:
                    if (channelChange) {
                        if (Y > 11 || Y < 1) {
                            // Error handling
                        } else {
                            EEPROM.write(13, 1);
                            EEPROM.commit();
                            int newChannel2 = int(Y);
                            EEPROM.write(13, newChannel2);
                            EEPROM.commit();
                            FastLED.showColor(CRGB::Magenta);
                            channelChange = false;
                            checkit = false;
                        }
                    } else {
                        EEPROM.write(15, int(Y));
                        EEPROM.commit();
                        newBrightness = int(Y);
                        FastLED.setBrightness(newBrightness);
                        FastLED.showColor(CRGB::Blue);
                        checkit = false;
                    }
                    break;
                default:
                    break;
            }
        }

        X = packetBuffer[i] - 127;
        R1 = (X & 0xE0);
        leds[i].r = R1;
        G1 = ((X << 3) & 0xE0);
        leds[i].g = G1;
        M1 = (X << 6);
        leds[i].b = M1;
    }

    FastLED.show();
}
