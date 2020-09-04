void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3; //this is something to change the pattern nicely!
  }

}

void FillStripesFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + colorIndex % 4; //change this to change pattern nicely!!
  }

}

void FillPatternStripesFromPaletteColors( uint8_t colorIndex, uint8_t var)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + var; //change this to change pattern nicely!!
  }

}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void PaletteSetup() {
  //currentPalette = RainbowColors_p;         currentBlending = BLEND; //not great needs modification for best pov look
  currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  //super awesome rainbows!
  //currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; //also fantastic!
  //SetupPurpleAndGreenPalette();             currentBlending = BLEND; //good one
  //SetupTotallyRandomPalette();              currentBlending = BLEND; //too random
  //SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; //striking...
  //SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; //nice too...
  //currentPalette = CloudColors_p;           currentBlending = BLEND; //needs more contrast
  //currentPalette = PartyColors_p;           currentBlending = BLEND; //also needs contrast
  //currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  //yes yes yes it works
  //currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; //nice
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if ( secondHand == 15)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if ( secondHand == 25)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if ( secondHand == 35)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if ( secondHand == 45)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if ( secondHand == 55)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
  }
}


void ChangePalettePeriodically2()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if ( secondHand == 15)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if ( secondHand == 25)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if ( secondHand == 35)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if ( secondHand == 45)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if ( secondHand == 55)  {
      SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray);
    }
  }
}

void ChangePalettePeriodically3()
{
  uint8_t secondHand = (millis() / 1000) % 120; //last number is number of seconds the whole thing lasts!
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
      //toggle between lines and zigzag pattern (using same palettes above!)
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
  uint8_t secondHand = (millis() / 1000) % 60; //last number is number of seconds the whole thing lasts!
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
      setting++;
      if (setting > 3) {
        setting = 1;
      }
    }


  }
}


//SetupRandomVariablePalette2(CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Magenta);

void ChangePaletteEveryTime()
{
  paletteVar++;
  ////Serial.println(paletteVar);
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

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
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

void SetupVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)//4 colours default
{

  currentPalette = CRGBPalette16(a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d);
}

void SetupVariableStripes(CRGB a, CRGB b, CRGB c, CRGB d)//4 colours default
{

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = a;
  }
}

void SetupVariablePalette3(CRGB a, CRGB b, CRGB c) //3 colours
{
  currentPalette = CRGBPalette16(a, b, c, a, b, c, a, b, c, a, b, c, a, b, c, a);
}

void SetupVariablePalette5(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e) //5 colours
{
  currentPalette = CRGBPalette16(a, b, c, d, e, a, b, c, d, e, a, b, c, d, e, a);
}

void SetupVariablePalette6(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e, CRGB f) //6 colours
{
  currentPalette = CRGBPalette16(a, b, c, d, e, f, a, b, c, d, e, f, a, b, c, d);
}

void SetupVariablePalette7(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e, CRGB f, CRGB g) //7 colours
{
  currentPalette = CRGBPalette16(a, b, c, d, e, f, g, a, b, c, d, e, f, g, a, b);
}



void SetupRandomVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)
{
  CRGB rand1 = CHSV( random8(), 255, 255);
  CRGB rand2 = CHSV( random8(), 255, 255);
  CRGB rand3 = rand1 - rand2;
  //function to mix up order of a, b, c, d here? if 1 currentPalette is order no.1 if 2 currentPalette is order no.2
  currentPalette = CRGBPalette16(a, rand1, c, rand2, b, rand1, d, rand2, a, rand1, c, rand2, b, rand1, d, rand2);
}

void SetupRandomVariablePalette2(CRGB a, CRGB b, CRGB c, CRGB d)
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to whatever.
  int r1 = random8(13);
  int r2 = random8(13);
  int r3 = random8(13);
  int r4 = random8(13);
  currentPalette[r1] = a;
  currentPalette[r2] = b;
  currentPalette[r3] = c;
  currentPalette[r4] = d;
}

void add_glitter() //from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
{
  int chance_of_glitter =  10; // percent of the time that we add glitter
  int number_of_glitters = 5; // number of glitter sparkles to add

  int r = random8(100);
  if ( r < chance_of_glitter ) {
    for ( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
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



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.

