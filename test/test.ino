#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_DT 3                                             // Data pin to connect to the strip.
#define LED_CK 13                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE LPD8806                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define MAX_LOOPS 10000
#define NUM_ANIMS 4
#define MAX_BRIGHTNESS 120
#define NUM_LEDS 48                                           // Number of LED's.
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 5
#define USE_MATRIX false

// Constants for Matrix set up
bool matrixForStrip = false; // So it can be changed later.
uint8_t SpacerLEDSForGridWrap = 2;
const uint8_t kMatrixWidth  = MATRIX_WIDTH; // Watch out when changing, there are spacers that overflow the total LED_NUMS
const int8_t kMatrixHeight = MATRIX_HEIGHT; // Watch out when changing, there are spacers that overflow the total LED_NUMS
const bool kMatrixSerpentineLayout = true;

class Segment {
  public:
    bool isHorizontal;
    uint8_t startIndex;
    uint8_t endIndex;
} segment;

typedef void (*FuncPtr) ();
uint8_t max_bright = MAX_BRIGHTNESS;                                      // Overall brightness definition. It can be changed on the fly.
//struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
//CRGB* const ledsPtr(leds + 1);
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);
Segment segments[kMatrixHeight];

unsigned long interationStart;

FuncPtr animationFunc;
int loopCounter = 0;

FuncPtr functions[NUM_ANIMS];

void setup() {
  interationStart = millis();
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);  // Use this for WS2801 or APA102
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Use this for WS2801 or APA102


  FastLED.setBrightness(max_bright);
  clearAnim();
  // Defining functions of animations.
  FuncPtr scopedAnimationList[NUM_ANIMS] = {
    progLightning,
    progJuggle,
    progHueGrid,
    progSegmentTest
  };

  // After compiled, we can put function pointers into our global 'functions' variable
  for (int i = 0; i < NUM_ANIMS; i++)
    functions[i] = scopedAnimationList[i];

  // Setting initial animation.
  animationFunc = functions[0];

  setupSegments(segments);
}

void loop() {
  // Parse input from Wifi module

  // Executing LED display
  //  if (loopCounter < MAX_LOOPS) {
  if (true) {
    // Pick random animation every 3 seconds.
    if (millis() - interationStart > 3000) {
      int seq = random() % NUM_ANIMS;
      Serial.println("Starting new sequence: " + String(seq));
      animationFunc = functions[seq];
      interationStart = millis();
      clearAnim();
    }


    //    progSegmentTest();
    //progJuggle();
    progFire();
    //    (*animationFunc)();
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

