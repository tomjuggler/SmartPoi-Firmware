#include "ShowLittleFSImage.h"
#include "Globals.h"

int cnti = 0;

void showLittleFSImage() {
    a = LittleFS.open(bin, "r");

    if (!a) {
        imageToUse++;
    } else {
        size_t size = a.size();
        
        if (size > MAX_PX) {
            FastLED.showColor(CRGB::Blue);
            imageToUse++;
        } else {
            pxAcross = int(size / pxDown);
            a.read(message1Data, size);
            
            cnti = (cnti >= pxDown) ? 0 : cnti + 1;
            a.close();
        }
    }

    int counter = 0;
    for (int j = 0; j < pxAcross; j++) {
        for (int i = 0; i < pxDown; i++) {
            byte X = message1Data[counter++];
            leds[i].r = (X & 0xE0);
            leds[i].g = ((X << 3) & 0xE0);
            leds[i].b = (X << 6);
        }
        FastLED.show();
        yield();
    }
}
