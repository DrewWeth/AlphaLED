class Juggle : public Animation {
    /* Juggle

      By: Originally by Mark Kriegsman

      Modified By: Andrew Tuline

      Date: February 2015

      Juggle just moves some balls back and forth. A single ball could be a Cylon effect. I've added several variables to this simple routine.
    */

    // Routine specific variables
    uint8_t    numdots =   4;                                     // Number of dots in use.
    uint8_t   faderate =   2;                                     // How long should the trails be. Very low value = longer trails.
    uint8_t     hueinc =  16;                                     // Incremental change in hue between each dot.
    uint8_t    thishue =   0;                                     // Starting hue.
    uint8_t     curhue =   0;                                     // The current hue
    uint8_t    thissat = 255;                                     // Saturation of the colour.
    uint8_t thisbright = max_bright;                                     // How bright should the LED/display be.
    uint8_t   basebeat =   5;                                     // Higher = faster movement.

  public:
    void run() {
      if (matrixForStrip) {
        for (int i = 0; i < kMatrixHeight; i++) {
          ChangeMe();
          juggle(segments[i].startIndex, kMatrixWidth, kMatrixHeight);
        }
      } else {
        ChangeMe();
        juggle(0, NUM_LEDS, 1);
      }
      FastLED.show(); // Power managed display of LED's.
    }

    void juggle(uint8_t start, uint8_t numleds, uint8_t basebeatMult) {                                               // Several colored dots, weaving in and out of sync with each other
      curhue = thishue;                                           // Reset the hue values.
      fadeToBlackBy(leds, NUM_LEDS, faderate);
      for ( int i = 0; i < numdots; i++) {
        leds[beatsin16((basebeat * basebeatMult) + i + numdots, start, start + numleds)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
        curhue += hueinc;
      }
    }

    void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
      uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
      static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
      if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
        lastSecond = secondHand;
        switch (secondHand) {
          case  0: numdots = 1; basebeat = 20; hueinc = 16; faderate = 2; thishue = 0; break;                  // You can change values here, one at a time , or altogether.
          case 10: numdots = 4; basebeat = 10; hueinc = 16; faderate = 8; thishue = 128; break;
          case 20: numdots = 8; basebeat =  3; hueinc =  0; faderate = 8; thishue = random8(); break;         // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
          case 30: break;
        }
      }
    } // ChangeMe()
};
