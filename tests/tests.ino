#include <ArduinoUnit.h>
#include <TweenDuino.h>

#define CONTINUOUS_DURATION 7000UL
#define SERIAL_BAUD 115200

float increasingFloat = 0.0;
TweenDuino increasing(increasingFloat, CONTINUOUS_DURATION, 100.0);

testing(increasingTween) 
{
  unsigned long t = millis();
  increasing.update(t);
  assertLessOrEqual(increasingFloat, 100.0);
  assertMoreOrEqual(increasingFloat, 0.0);

  if (t >= CONTINUOUS_DURATION) {
    assertEqual(increasingFloat, 100.0);
    pass();
  }
}

float decreasingFloat = 100.0;
TweenDuino decreasing(decreasingFloat, CONTINUOUS_DURATION, 0.0);

testing(decreasingTween)
{
  unsigned long t = millis();
  decreasing.update(t);
  assertLessOrEqual(decreasingFloat, 100.0);
  assertMoreOrEqual(decreasingFloat, 0.0);

  if (t >= CONTINUOUS_DURATION) {
    assertEqual(decreasingFloat, 0.0);
    pass();
  }
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0d to 0.0d would crash.
 */
test(finalAndInitialAreZero) {
  float val = 0.0;
  TweenDuino tween(val, 56734UL, 0.0d);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(val, 0.0d);
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0d to 0.0d would crash.
 */
test(valsetToZeroDuringTweenToZero) {
  float val = 100.0;
  TweenDuino tween(val, 56734UL, 0.0d);
  tween.update(0UL);
  tween.update(1000UL);
  val = 0.0;
  tween.update(56734UL);
  assertEqual(val, 0.0d);
}

test(finalAndInitialAreSame) {
  float val = 4640.0;
  TweenDuino tween(val, 56734UL, 4640.0d);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(val, 4640.0d);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  //while(!Serial); // for the Arduino Leonardo/Micro only

  // Currently causes a crash.
  Test::exclude("finalAndInitialAreZero");
  Test::exclude("valsetToZeroDuringTweenToZero");

  Test::exclude("decreasingTween");
}

void loop() {
  Test::run();
}