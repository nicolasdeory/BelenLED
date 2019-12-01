// BELEN LED
// Nicolas de Ory 2018

#include "FastLED.h"

FASTLED_USING_NAMESPACE


#define DATA_PIN    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    52

byte mode = 0;
CRGB leds[NUM_LEDS];
boolean onLeds[NUM_LEDS];

#define BRIGHTNESS          100
#define PASSIVE_FPS 120

DEFINE_GRADIENT_PALETTE( GP_Day ) {
   0,  255, 221, 53,
   100, 255, 144, 0,
   140, 79, 104, 132,
   200, 79, 104, 132,
   255, 255, 221, 53 };


#define MODE4_PALETTE_COUNT 6

CRGBPalette16 mode4Palettes[] = {
  GP_Day
};

#define MODE5_PALETTE_COUNT 2




uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t customHue = 0;




void setup() {
  delay(2000); // Safety delay
  
  // LED SETUP
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  pinMode(13,OUTPUT); // DEBUG LED

}




void loop() {

    FastLED.show(); 
    FastLED.delay(1000/PASSIVE_FPS); 
 

EVERY_N_MILLISECONDS(250) { 
  for(size_t i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(mode4Palettes[0],gHue);
  }
  if(gHue > 251) gHue = 0; else gHue++;
  }


}
