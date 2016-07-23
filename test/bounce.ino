
#define h0                1                  // Starting height, in meters, of the ball (strip length)

const int ballsCount = 3;
bool shouldBounce = true;
float gravityMultiplier = 0.5;
float gravity = -9.81 * gravityMultiplier;

float h[ballsCount] ;                         // An array of heights
float vImpact0 = sqrt( -2 * gravity * h0 );  // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip
float vImpact[ballsCount] ;                   // As time goes on the impact velocity will change, so make an array to store those values
float tCycle[ballsCount] ;                    // The time since the last time the ball struck the ground
int   pos[ballsCount] ;                       // The integer position of the dot on the strip (LED index)
long  tLast[ballsCount] ;                     // The clock time of the last ground strike
float COR[ballsCount] ;                       // Coefficient of Restitution (bounce damping)
uint8_t ballColor[ballsCount];
bool firstTime[ballsCount];


void setupBounce() {

  for (int i = 0 ; i < ballsCount ; i++) {    // Initialize variables
    tLast[i] = millis();
    h[i] = h0;
    pos[i] = 0;                              // Balls start on the ground
    vImpact[i] = vImpact0;                   // And "pop" up at vImpact0
    tCycle[i] = 0;
    COR[i] = 0.90 - float(i) / pow(ballsCount, 2);
    ballColor[i] = random8(255);
    firstTime[i] = true;
  }
}

void progBounce() {
  for (int i = 0 ; i < ballsCount ; i++) {
    if (shouldBounce)
      tCycle[i] =  millis() - tLast[i] ;     // Calculate the time since the last time the ball was on the ground
    else {
      if (firstTime[i]) {
        tLast[i] -= random(1000);
        firstTime[i] = false;

      }
      tCycle[i] =  millis() - tLast[i] + (-vImpact[i] / gravity * 1000);     // Calculate the time since the last time the ball was on the ground

    }
    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    h[i] = 0.5 * gravity * pow( tCycle[i] / 1000 , 2.0 ) + vImpact[i] * tCycle[i] / 1000;

    if ( h[i] < 0 ) {
      tLast[i] = millis();

      if (shouldBounce) {
        vImpact[i] = COR[i] * vImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
        h[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      }
      else {
        vImpact[i] = vImpact0;  // Set back to normal for fall.

      }


      if ( vImpact[i] < 0.01 ) {
        vImpact[i] = vImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
        ballColor[i] = random8(255);
      }
    }
    pos[i] = round( h[i] * (NUM_LEDS - 1) / h0);       // Map "h" to a "pos" integer index position on the LED strip
  }

  //Choose color of LEDs, then the "pos" LED on
  for (int i = 0 ; i < ballsCount ; i++) leds[pos[i]] = CHSV( ballColor[i] , 255, 255);
  FastLED.show();
  //Then off for the next loop around
  fadeToBlackBy(leds, NUM_LEDS, 20);
}

