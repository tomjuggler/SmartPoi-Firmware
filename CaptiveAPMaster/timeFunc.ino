
static uint8_t lastSecond = 99;
uint8_t secondHand;
//uint32_t frei; //debug code

void ChangePatternPeriodically()
{
  secondHand = (millis() / 1000) % 20;
  
  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
//  frei = system_get_free_heap_size(); //debug code
//  Serial.println(frei);               //debug code
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if ( secondHand == 5)  {
      picToShow = 1;
    }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if ( secondHand == 15)  {
     picToShow = 2;
    }
//    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
//    if ( secondHand == 25)  {
//      picToShow = 3;
//    }
//    //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
//    if ( secondHand == 35)  {
//      picToShow = 4;
//    }
//    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
//    if ( secondHand == 45)  {
//      picToShow = 5;
//    }
  }
}

