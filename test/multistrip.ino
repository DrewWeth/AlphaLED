
// Set up segments left to right.
void setupSegments(Segment segments[]) {
  for (int i = 0; i < kMatrixHeight; i++) {
    segments[i].startIndex = i * kMatrixWidth + (i * (SpacerLEDSForGridWrap ));
//    Serial.println("Start Index: " + String(segments[i].startIndex));
    segments[i].endIndex = segments[i].startIndex + kMatrixWidth - 1;
//    Serial.println("endIndex: " + String(segments[i].endIndex));
  }
}

void progSegmentTest() {
  for(int i = 0; i < kMatrixHeight; i++) {
    uint8_t color = random8(255);
//    Serial.println(segments[i].startIndex);
    for(int j = 0; j < kMatrixWidth; j++){
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

