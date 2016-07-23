
// Notes: Adding a ton of LEDs may cause int overflow errors, check their sizes.
// Vectors handle their own memory deallocation, the objects in the vectors do not. Potential mem problem.

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include <FastLED.h>

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
bool matrixForStrip = USE_MATRIX; // So it can be changed later.
uint8_t SpacerLEDSForGridWrap = INIT_SPACER_WRAP;
uint8_t kMatrixWidth  = INIT_MATRIX_WIDTH; // Watch out when changing, there are spacers that overflow the total LED_NUMS
uint8_t kMatrixHeight = INIT_MATRIX_HEIGHT; // Watch out when changing, there are spacers that overflow the total LED_NUMS
const bool kMatrixSerpentineLayout = USE_SERPENTINE;

typedef void (*FuncPtr) ();
uint8_t max_bright = MAX_BRIGHTNESS;                                      // Overall brightness definition. It can be changed on the fly.
//struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
//CRGB* const ledsPtr(leds + 1);
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);


unsigned long changeAnim;
unsigned long loopCounter = 0;

class Segment {
  public:
    float orientation;
    uint8_t startIndex;
    uint8_t endIndex;

    uint8_t numLEDs() {
      return endIndex - startIndex;
    };

    Segment(uint8_t _start, uint8_t _end) {
      orientation = 0;
      startIndex = _start;
      endIndex = _end;
    };
};


vector<Segment> segments;
vector<FuncPtr> animations;
int animationIndex = 0;

void setup() {
  // USB communication
  Serial.begin(9600);

  // LED set up
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Use this for WS2801 or APA102
  FastLED.setBrightness(max_bright);

  animations.push_back(progLightning);
  animations.push_back(progJuggle);
  animations.push_back(progHueGrid);
  
  animations.push_back(progSegments);
  animations.push_back(progFire);
  animations.push_back(progNoise);
  
  animations.push_back(progDemo);
  animations.push_back(progBlast);
  animations.push_back(progBounce);

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
      animationIndex = seq;
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
    (*animations[animationIndex])();
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
