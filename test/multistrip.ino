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


unsigned long sTimer = 0;
unsigned long sInt = 0;

void progSegments() {
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

