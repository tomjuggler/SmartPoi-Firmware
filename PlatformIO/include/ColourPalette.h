#ifndef COLOUR_PALETTE_H
#define COLOUR_PALETTE_H

#include "Globals.h"

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;
extern int paletteVar;
extern volatile int setting;
extern bool lines;
extern bool upDown;
extern int motionSpeed;
extern int maxStartIndex;
extern int minStartIndex;

void FillLEDsFromPaletteColors(uint8_t colorIndex);
void FillStripesFromPaletteColors(uint8_t colorIndex);
void FillPatternStripesFromPaletteColors(uint8_t colorIndex, uint8_t var);
void PaletteSetup();
void ChangePalettePeriodically();
void ChangePalettePeriodically3();
void ChangeStripesPeriodically();
void ChangePaletteEveryTime();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
void SetupVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d);
void SetupVariablePalette3(CRGB a, CRGB b, CRGB c);
void SetupVariablePalette5(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e);
void SetupRandomVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d);
void SetupRandomVariablePalette2(CRGB a, CRGB b, CRGB c, CRGB d);
void add_glitter();
void funColourJam();

extern const TProgmemPalette16 myRedWhiteBluePalette_p;

#endif
