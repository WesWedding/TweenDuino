#include <ArduinoUnit.h>
#include <TweenDuino.h>

#define CONTINUOUS_DURATION 7000UL

double increasingDouble = 100.0d;
TweenDuino increasing(increasingDouble, CONTINUOUS_DURATION, 100.0d);

double decreasingDouble = 100.0d;
TweenDuino decreasing(decreasingDouble, CONTINUOUS_DURATION, 0.0d);

testing(increasingTween) 
{
  unsigned long t = millis();
  increasing.update(t);
  assertLessOrEqual(increasingDouble, 100.0d);
  assertMoreOrEqual(increasingDouble, 0.0d);

  if (t >= CONTINUOUS_DURATION) {
    pass();
  }
}

testing(decreasingTween)
{
  unsigned long t = millis();
  decreasing.update(t);
  assertLessOrEqual(decreasingDouble, 100.0d);
  assertMoreOrEqual(decreasingDouble, 0.0d);

  if (t >= CONTINUOUS_DURATION) {
    pass();
  }
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0d to 0.0d would crash.
 */
test(finalAndInitialAreZero) {
  double val = 0.0d;
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
  double val = 100.0d;
  TweenDuino tween(val, 56734UL, 0.0d);
  tween.update(0UL);
  tween.update(1000UL);
  val = 0.0d;
  tween.update(56734UL);
  assertEqual(val, 0.0d);
}

test(finalAndInitialAreSame) {
  double val = 4640.0d;
  TweenDuino tween(val, 56734UL, 4640.0d);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(val, 4640.0d);
}

void setup() {
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  // Currently causes a crash.
  Test::exclude("finalAndInitialAreZero");
  Test::exclude("valsetToZeroDuringTweenToZero");
}

void loop() {
  Test::run();
}