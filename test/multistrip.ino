
class Colors : public Animation {
    unsigned long sTimer = 0;
    unsigned long sInt = 0;
  public:
    void run() {
      for (int i = 0; i < kMatrixHeight; i++) {
        uint8_t color = random8(255);

        // Color all pixels in segment.
        for (int j = 0; j < segments[i].numLEDs(); j++) {
          leds[segments[i].startIndex + j] = CHSV(color, 255, 255);
        }
      }

      FastLED.show();
      delay(1000);
      //  for (byte i = 0; i < kMatrixHeight; i++) {
      //    uint8_t color = random8(255);
      //    for (byte j = 0; j < kMatrixWidth; j++) {
      //      leds[XY(j, i)] = CHSV(color, 255, 255);
      //    }
      //  }

    }
};
