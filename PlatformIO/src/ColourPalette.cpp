#include "ColourPalette.h"
#include "Globals.h"

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

CRGBPalette16 currentPalette;
TBlendType currentBlending = NOBLEND;
int paletteVar = 1;

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void FillStripesFromPaletteColors(uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + colorIndex % 4;
  }
}

void FillPatternStripesFromPaletteColors(uint8_t colorIndex, uint8_t var)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + var;
  }
}

void PaletteSetup() {
  currentPalette = RainbowStripeColors_p;
  currentBlending = NOBLEND;
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand == 15)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    if ( secondHand == 25)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    if ( secondHand == 35)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    if ( secondHand == 45)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    if ( secondHand == 55)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
  }
}

void ChangePalettePeriodically3()
{
  uint8_t secondHand = (millis() / 1000) % 120;
  static uint8_t lastSecond = 121;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand == 1)   {
      SetupVariablePalette3(CRGB::Red, CRGB::Black, CRGB::Blue);
    }
    if ( secondHand == 10)  {
      SetupVariablePalette3(CRGB::Magenta, CRGB::Black, CRGB::Green);
    }
    if ( secondHand == 20)  {
      SetupVariablePalette3(CRGB::Cyan, CRGB::Black, CRGB::Yellow);
    }
    if ( secondHand == 30)  {
      SetupVariablePalette3(CRGB::Red, CRGB::Black, CRGB::Green);
    }
    if ( secondHand == 40)  {
      SetupVariablePalette3(CRGB::Cyan, CRGB::Black, CRGB::Magenta);
    }
    if ( secondHand == 50)  {
      SetupVariablePalette3(CRGB::Black, CRGB::Red, CRGB::Cyan);
    }
    if ( secondHand == 60)  {
      SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Blue, CRGB::Black);
    }
    if ( secondHand == 70)  {
      SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Yellow, CRGB::Black);
    }
    if ( secondHand == 80)  {
      SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Green, CRGB::Black);
    }
    if ( secondHand == 90)  {
      SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Magenta, CRGB::Black);
    }
    if ( secondHand == 100) {
      SetupVariablePalette(CRGB::Green, CRGB::Black, CRGB::Blue, CRGB::Black);
    }
    if ( secondHand == 60)  {
      SetupVariablePalette5(CRGB::BlueViolet, CRGB::Black, CRGB::Amethyst, CRGB::Black, CRGB::Coral);
    }
    if ( secondHand == 70)  {
      SetupVariablePalette5(CRGB::Chocolate, CRGB::Black, CRGB::DarkCyan, CRGB::Black, CRGB::DarkGoldenrod);
    }
    if ( secondHand == 80)  {
      SetupVariablePalette5(CRGB::DarkOliveGreen, CRGB::DodgerBlue, CRGB::Green, CRGB::Black, CRGB::DarkOrchid);
    }
    if ( secondHand == 90)  {
      SetupVariablePalette5(CRGB::Lavender, CRGB::Black, CRGB::IndianRed, CRGB::Black, CRGB::FloralWhite);
    }
    if ( secondHand == 100) {
      SetupVariablePalette5(CRGB::MediumTurquoise, CRGB::Black, CRGB::Blue, CRGB::Black, CRGB::Lime);
    }
    if ( secondHand == 110) {
      if (lines == false) {
        lines = true;
      }
      else {
        lines = false;
      }
      setting = 2;
    }
  }
}

void ChangeStripesPeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand == 1 )  {
      SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Yellow, CRGB::Black);
    }
    if ( secondHand == 10)  {
      SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Blue, CRGB::Black);
    }
    if ( secondHand == 20)  {
      SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Yellow, CRGB::Black);
    }
    if ( secondHand == 30)  {
      SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Green, CRGB::Black);
    }
    if ( secondHand == 40)  {
      SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Magenta, CRGB::Black);
    }
    if ( secondHand == 50)  {
      SetupVariablePalette(CRGB::Green, CRGB::Black, CRGB::Blue, CRGB::Black);
    }
    if ( secondHand == 59)  {
      setting = static_cast<int>(setting) + 1;
      if (setting > 3) {
        setting = 1;
      }
    }
  }
}

void ChangePaletteEveryTime()
{
  paletteVar = static_cast<int>(paletteVar) + 1;
  if ( paletteVar == 1)   {
    SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
  }
  if ( paletteVar == 2)  {
    SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
  }
  if ( paletteVar == 3)  {
    SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black);
  }
  if ( paletteVar == 4)  {
    SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
  }
  if ( paletteVar == 5)  {
    SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black);
  }
  if ( paletteVar == 6)  {
    SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
  }
  if ( paletteVar == 7)  {
    SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black);
  }
  if ( paletteVar == 8)  {
    SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
  }
  if ( paletteVar == 9)  {
    SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black);
  }
  if ( paletteVar == 10)  {
    SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
  }
  if (paletteVar == 11) {
    paletteVar = 0;
  }
}

void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

void SetupBlackAndWhiteStripedPalette()
{
  fill_solid( currentPalette, 16, CRGB::Black);
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}

void SetupVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)
{
  currentPalette = CRGBPalette16(a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d);
}

void SetupVariablePalette3(CRGB a, CRGB b, CRGB c)
{
  currentPalette = CRGBPalette16(a, b, c, a, b, c, a, b, c, a, b, c, a, b, c, a);
}

void SetupVariablePalette5(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e)
{
  currentPalette = CRGBPalette16(a, b, c, d, e, a, b, c, d, e, a, b, c, d, e, a);
}

void SetupRandomVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)
{
  CRGB rand1 = CHSV( random8(), 255, 255);
  CRGB rand2 = CHSV( random8(), 255, 255);
  CRGB rand3 = rand1 - rand2;
  currentPalette = CRGBPalette16(a, rand1, c, rand2, b, rand1, d, rand2, a, rand1, c, rand2, b, rand1, d, rand2);
}

void SetupRandomVariablePalette2(CRGB a, CRGB b, CRGB c, CRGB d)
{
  fill_solid( currentPalette, 16, CRGB::Black);
  int r1 = random8(13);
  int r2 = random8(13);
  int r3 = random8(13);
  int r4 = random8(13);
  currentPalette[r1] = a;
  currentPalette[r2] = b;
  currentPalette[r3] = c;
  currentPalette[r4] = d;
}

void add_glitter()
{
  int chance_of_glitter =  10;
  int number_of_glitters = 5;

  int r = random8(100);
  if ( r < chance_of_glitter ) {
    for ( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White;
    }
  }
}

void funColourJam()
{
  if (setting == 1) {
    if (lines == false) { 
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      if (upDown == true) {
        startIndex = startIndex + motionSpeed;
        FillLEDsFromPaletteColors(startIndex);
        if (startIndex == maxStartIndex) {
          upDown = false;
        }
      }
      else {
        startIndex = startIndex - motionSpeed;
        FillLEDsFromPaletteColors(startIndex);
        if (startIndex == minStartIndex) {
          upDown = true;
        }
      }
      FastLED.show();
    }
    else {
      ChangePalettePeriodically3();
      static uint8_t startIndex = 0;
      startIndex = startIndex + motionSpeed;
      FillLEDsFromPaletteColors(startIndex);
      if (startIndex == maxStartIndex) {
        startIndex = 0;
      }
      FastLED.show();
    } 
  } 
  else if (setting == 2) {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex = 0;
    stripeIndex = stripeIndex + 1;
    FillStripesFromPaletteColors(stripeIndex);
    if (stripeIndex > 15) {
      stripeIndex = 0;
    }
    FastLED.show();
  } 
  else {
    ChangeStripesPeriodically();
    static uint8_t stripeIndex2 = 0;
    stripeIndex2 = stripeIndex2 + 1;
    FillPatternStripesFromPaletteColors(stripeIndex2, 4);
    if (stripeIndex2 > 15) {
      stripeIndex2 = 0;
    }
    FastLED.show();
  } 
}
