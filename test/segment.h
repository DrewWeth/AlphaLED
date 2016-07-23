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
