// Notes: Adding a ton of LEDs may cause int overflow errors, check their sizes.
// Vectors handle their own memory deallocation, the objects in the vectors do not. Potential mem problem.

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include <FastLED.h>
#include "all_headers.h"

using namespace std;

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_DT 11                                             // Data pin to connect to the strip.
#define LED_CK 13                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE LPD8806                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define MAX_LOOPS 10000
#define MAX_BRIGHTNESS 10
#define NUM_LEDS 48                                           // Number of LED's.
#define INIT_MATRIX_WIDTH 8
#define INIT_MATRIX_HEIGHT 4
#define INIT_SPACER_WRAP 2
#define USE_MATRIX false
#define USE_SERPENTINE true

// Constants for Matrix set up
class Main {
    static bool matrixForStrip = USE_MATRIX; // So it can be changed later.
    static uint8_t SpacerLEDSForGridWrap = INIT_SPACER_WRAP;
    static uint8_t Main::kMatrixWidth  = INIT_MATRIX_WIDTH; // Watch out when changing, there are spacers that overflow the total LED_NUMS
    static uint8_t Main::kMatrixHeight = INIT_MATRIX_HEIGHT; // Watch out when changing, there are spacers that overflow the total LED_NUMS
    static const bool kMatrixSerpentineLayout = USE_SERPENTINE;

    static typedef void (*FuncPtr) ();
    static uint8_t max_bright = MAX_BRIGHTNESS;                                      // Overall brightness definition. It can be changed on the fly.
    //struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
    //CRGB* const ledsPtr(leds + 1);
    static CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
    static CRGB* const leds( leds_plus_safety_pixel + 1);

    static vector<Segment> segments;

    static unsigned long changeAnim;
    static unsigned long loopCounter = 0;

    static vector<Animation*> animations;
    static int animationIndex = 0;
}


void setup() {
  // USB communication
  Serial.begin(9600);

  // LED set up
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Use this for WS2801 or APA102
  FastLED.setBrightness(max_bright);

  // Defining animations of animations.

  animations.push_back(new Lightning());
  //  animations.push_back(new Juggle());
  //  animations.push_back(new HueGrid());
  //  animations.push_back(new Segments());
  //  animations.push_back(new Fire());
  //  animations.push_back(new Noise());
  //  animations.push_back(new Demo());
  //  animations.push_back(new Blast());
  //  animations.push_back(new Bounce());

  // Set ups.
  setupSegments(&segments);

  setupBounce();
  setupNoise();

  changeAnim = millis();
}

void loop() {
  // Parse input from Wifi module

  // Executing LED display
  //  if (loopCounter < MAX_LOOPS) {
  if (true) {
    // Pick random animation every 3 seconds.
    if (millis() - changeAnim > 3000) {
      int seq = random() % animations.size();
      Serial.println("Starting new sequence: " + String(seq));
      animationIndex = req;
      changeAnim = millis();
      clearAnim();
    }

    //    progHueGrid();
    //  progBlast();
    //        progDemo();
    //    progBounce();
    //progNoise();
    //        progLightning();
    //  progStarting();
    //        progSegments();
    //progJuggle();
    //    progFire();
    //    (*animationFunc)();
    animations[animationIndex].run();
  } else {
    serialFlush();
    delay(100000);
  }
  loopCounter += 1;
}

// Clears animation from the strip so old animation state stays.
void clearAnim() {
  fadeToBlackBy(leds, NUM_LEDS, 255);
}

void serialFlush() {
  Serial.println("== End program.");
  for (int i = 0; i < 3; i++)
    Serial.println("");
}

void progStarting() {
  Serial.println("Starting...");
}

// Set up segments left to right.
void setupSegments(vector<Segment>* segments) {
  if (matrixForStrip == false) {
    kMatrixHeight = 1;
    kMatrixWidth = NUM_LEDS;
  }

  for (int i = 0; i < kMatrixHeight; i++) {

    uint8_t startIndex;
    uint8_t endIndex;

    if (matrixForStrip) {
      startIndex = i * kMatrixWidth + (i * (SpacerLEDSForGridWrap ));
      endIndex = startIndex + kMatrixWidth - 1;
    } else {
      startIndex = i * (NUM_LEDS / kMatrixHeight);
      endIndex = startIndex + (NUM_LEDS / kMatrixHeight);
    }
    segments->push_back(Segment(startIndex, endIndex));
  }
}
