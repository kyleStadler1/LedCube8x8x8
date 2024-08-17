#define printf(format, ...) \
char ___string[100]; \
sprintf(___string, format, __VA_ARGS__); \
Serial.print(___string);
typedef uint8_t (*animationFunction_t) (uint8_t x, uint8_t y, uint8_t z, uint8_t frame);
typedef uint8_t (*animationSetup_t) (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs);
unsigned long actualFrame = 0;
////

int oe = 6; //low enables  blue
int srclk = 4; //normal clocknbb    blue short
int rclk = 3; //latch clock  yellow
int ser = 2; //ser portc
int clr = 5; //keep high to not clr    green


const int clockdelay = 0; // microseconds
void clockTic(){
  delayMicroseconds(clockdelay);
  digitalWrite(srclk, HIGH);
  delayMicroseconds(clockdelay);
  digitalWrite(srclk, LOW);
  delayMicroseconds(clockdelay);
}

void latchTic(){
  delayMicroseconds(clockdelay);
  digitalWrite(rclk, HIGH);
  delayMicroseconds(clockdelay);
  digitalWrite(rclk, LOW);
  delayMicroseconds(clockdelay);
}
static animationSetup_t gAnimationSequence[] = {
  
  animationSetupCubePlanesCrossing,
  animationSetupCubeRain,
  animationSetupCubePulseToCorners,
  animationSetupCubePlanesFalling,
  animationSetupCubePulse,
  //animationSetupCubeBounce,
};

static uint8_t gAnimationSequenceCount = sizeof(gAnimationSequence)/sizeof(gAnimationSequence[0]);

void runAnimationFrame(uint8_t *frame, unsigned long frameTime)
{
  unsigned long start = millis();
  unsigned long now;
  do {
    digitalWrite(oe, LOW);
    uint8_t *ptr = frame;
    for (int z = 0; z < 8; ++z) {
      for (int i = 7; i >= 0; --i) {
        digitalWrite(ser, (i == z) ? HIGH : LOW);
        clockTic();
      }
      for (int y = 7; y >= 0; --y) {
        uint8_t xbyte = *ptr++;
        for (int x = 0; x < 8; ++x) {
          uint8_t value = (xbyte >> x) & 1;
          digitalWrite(ser, value ? HIGH : LOW);
          clockTic();
        }
      }
      latchTic(); 
    }
    now = millis();
  } while ((now - start) < frameTime);
}

void prepareAndRunAnimation(uint8_t frameCount, char *animationName, animationFunction_t animationFunction, unsigned long frameTimeMs)
{
  //-----
  digitalWrite(oe, HIGH);
  unsigned long now, start = millis();
  //digitalWrite(oe, LOW);
  //digitalWrite(oe, HIGH);
  uint8_t *memory = (uint8_t *)malloc(frameCount * 8 * 8);
  uint8_t *ptr = memory;
  for (uint8_t frame=0; frame<frameCount; ++frame) {
    actualFrame++; 
    for (int z = 0; z < 8; ++z) {
      for (int y = 7; y >= 0; --y) {
        uint8_t xbyte = 0;
        for (int x = 0; x < 8; ++x) {
          uint8_t value = animationFunction(x, y, z, frame);
          xbyte |= value << x;
        }
        *ptr++ = xbyte;
      }
    }
  }
  now = millis();
  printf("%s(prep time %ums)\n", animationName, now - start);
  start = millis();
  //latchTic(); ///----
  do {
    for (uint8_t frame=0; frame<frameCount; ++frame) {
      runAnimationFrame(memory + frame * 8 * 8, frameTimeMs);
    }
    now = millis();
  } while ((now - start) < 8000); // 10 seconds per animation
  free(memory);
  
}

void runAnimationSequence()
{
  for (uint8_t i=0; i<gAnimationSequenceCount; ++i) {
    char *animationName;
    animationFunction_t animationFunction;
    unsigned long frameTimeMs;
    uint8_t frameCount = gAnimationSequence[i](&animationName, &animationFunction, &frameTimeMs); //THIS LINE
    prepareAndRunAnimation(frameCount, animationName, animationFunction, frameTimeMs);
  }
}
  
