// PIANO LED
// Nicolas de Ory 2017
// Programa que funciona con la tira de LEDs WS2812B y el teclado Yamaha CLP320
// MIRAR ISSUE GITHUB MIDI LIB
// Deberiamos comprobar available SRAM en void loop() para ver que pasa
//#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"

FASTLED_USING_NAMESPACE


#define DATA_PIN    3
//#define CLK_PIN   4
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


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon};

//int lastAnalogRead;
int currAnalogRead;
void loop() {
 /*
 // PROCESS DIGITAL INPUT
 // Button
   if (digitalRead(button_pin) == LOW) {
      digitalWrite(13, HIGH);
      delay(500);
      if (mode == 7) { mode = 0; } else { mode++; }
   } else digitalWrite(13, LOW); 
 // Potentiometer
 currAnalogRead = analogRead(POTENTIOMETER_PIN);
  if(mode == 4) {
    int idx = constrain(floor(map(currAnalogRead,0,1023,0,MODE4_PALETTE_COUNT)),0,MODE4_PALETTE_COUNT-1);
    if(mode4PalIndex != idx) {
      digitalWrite(13, HIGH);
      mode4PalIndex = idx;
      // FILL PALETTE TO PREVIEW (in reverse) //
      int incr = floor(255/NUM_LEDS);
      int colorIndex = 255;
      for( uint16_t i = 0; i < NUM_LEDS; i++) { 
        leds[i] = ColorFromPalette(mode4Palettes[mode4PalIndex], colorIndex);
        colorIndex -= incr;
      }
      /////
      delay(100);
      digitalWrite(13, LOW);
    }
  } else if (mode == 5)
  {
    int idx = constrain(floor(map(currAnalogRead,0,1023,0,MODE5_PALETTE_COUNT)),0,MODE5_PALETTE_COUNT-1);
    if(mode5PalIndex != idx) {
      digitalWrite(13, HIGH);
      mode5PalIndex = idx;
      delay(100);
      digitalWrite(13, LOW);
    }
  } else {
    customHue = map(currAnalogRead,0,1023,0,255);
  }*/
  //lastAnalogRead = currAnalogRead;
 //////
 
    //gPatterns[gCurrentPatternNumber](); 
    FastLED.show(); 
    FastLED.delay(1000/PASSIVE_FPS); 
 

EVERY_N_MILLISECONDS(250) { 
  for(size_t i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(mode4Palettes[0],gHue);
  }
  if(gHue > 251) gHue = 0; else gHue++;
  }

EVERY_N_MILLISECONDS(40) {
  /*if (mode != 1) {
    for (byte i=0;i < NUM_LEDS; i++) { // THIS MIGHT NOT TURN OFF THE LEDS"!!
          if (leds[i].getAverageLight() == 0) {
              onLeds[i] = false;
              leds[i] = CRGB::Black;
        
          } else if (onLeds[i] == false) {
            if (sustain) {
              leds[i].fadeToBlackBy(PEDAL_STRENGTH);
            } else {
              leds[i].fadeToBlackBy(NO_PEDAL_STRENGTH);
            }
          } else {
            if(!DONT_FADE_NOTES)
              leds[i].fadeToBlackBy(NOTE_HOLD_FADE);
          }
    }
    FastLED.show();
  }*/
}
EVERY_N_SECONDS(20) { nextPattern(); }

}

// PASSIVE PATTERNS (MODE 0)
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(20);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}
