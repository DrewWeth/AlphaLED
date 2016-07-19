//  Lightnings is a program that lets you make an LED strip look like a 1D cloud of lightning
//  Original by: Daniel Wilson, 2014
//  Modified by: Andrew Tuline 2015
//
//  This modified version creates lightning along various sections of the strip. Looks great inside my poly fill constructed cloud.

// Global variables can be changed on the fly.
uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;

uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;                                               // Length of a flash


void progLightning() {
  if (matrixForStrip) {
    uint8_t randomRow = (random() % kMatrixHeight);
    ledstart = XY(random8(kMatrixWidth), randomRow);           // Determine starting location of flash
    ledlen = random8(kMatrixWidth);  // Determine length of flash (not to go beyond NUM_LEDS-1)
    if(ledlen + ledstart > NUM_LEDS - 1) // Make sure flashes dont go off the strip
      ledlen = NUM_LEDS - ledstart - 1; 
  } else {
    ledstart = random8(NUM_LEDS);           // Determine starting location of flash
    ledlen = random8(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
  }

  for (int flashCounter = 0; flashCounter < random8(3, flashes); flashCounter++) {
    if (flashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1, 3);          // return strokes are brighter than the leader
    fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4, 10));                // each flash only lasts 4-10 milliseconds
    fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
    FastLED.show();
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50 + random8(100));             // shorter delay between strokes
  } // for()
  if (matrixForStrip)
    delay(random8(frequency) * 100 / kMatrixHeight);        // delay between strikes
  else
    delay(random8(frequency) * 100);        // delay between strikes

}

