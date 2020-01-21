uint8_t secondHand; 
static uint8_t lastSecond = 99;
uint32_t frei;

void ChangePatternPeriodically()
{
  secondHand = (millis() / 1000) % 50;
  

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
//  frei = system_get_free_heap_size();
//  Serial.println(frei);
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if ( secondHand == 5)  {
      picToShow = 1;
    }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if ( secondHand == 15)  {
     picToShow = 2;
    }
    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if ( secondHand == 25)  {
      picToShow = 3;
    }
    //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if ( secondHand == 35)  {
      picToShow = 4;
    }
    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if ( secondHand == 45)  {
      picToShow = 5;
    }
  }
}

