

unsigned long blastTime = 0;
unsigned long blastInt = 2000;

unsigned long blastUpdateTime = 0;
unsigned long blastUpdateInt = 5;

unsigned long blastCompleted = false;
uint8_t blastColor = random8(255);
uint16_t blastIndex = 0;
uint8_t directionRight = 1;
uint8_t blastHue = 255;

void progBlast() {
  if (millis() - blastTime > blastInt || blastCompleted) {
    blastTime = millis();
    blastCompleted = false;
    blastUpdateInt = random8(2, 5);
    blastIndex = 0;
    blastColor = random8(255);
    blastInt = random(2000, 6000);
    blastHue = 255;
    //    fill_solid(leds, kMatrixWidth, CRGB::Black );
    clearAnim();
  }

  if (millis() - blastUpdateTime > blastUpdateInt) {
    blastUpdateTime = millis();
    if (blastIndex < kMatrixWidth) {
      leds[blastIndex] = CHSV(blastColor, 255, blastHue);
      blastIndex += 1;
    } else {
      fadeToBlackBy(leds, kMatrixWidth, 10);
    }

    FastLED.show();
  }
}

