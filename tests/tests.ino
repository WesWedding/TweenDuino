#include <ArduinoUnit.h>
#include <TweenDuino.h>

#define CONTINUOUS_DURATION 7000UL
#define SERIAL_BAUD 115200

float increasingFloat = 0.0;
TweenDuino::Tween increasing(increasingFloat, CONTINUOUS_DURATION, 100.0);

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
TweenDuino::Tween decreasing(decreasingFloat, CONTINUOUS_DURATION, 0.0);

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
  TweenDuino::Tween tween(val, 56734UL, 0.0d);
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
  TweenDuino::Tween tween(val, 56734UL, 0.0d);
  tween.update(0UL);
  tween.update(1000UL);
  val = 0.0;
  tween.update(56734UL);
  assertEqual(val, 0.0d);
}

test(finalAndInitialAreSame) {
  float val = 4640.0;
  TweenDuino::Tween tween(val, 56734UL, 4640.0d);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(val, 4640.0d);
}

test(emptyTimelineCompletes) {
  TweenDuino::Timeline tl;
  tl.update(0UL);
  assertTrue(tl.isComplete());
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0d to 0.0d would crash.
 */
test(timelineCompletesOnFinalMS) {
  TweenDuino::Timeline tl;

  float val = 0.0;
  const float stop1 = 100.0;
  const unsigned long dur1 = 400UL;
  TweenDuino::Tween tween1(val, dur1, stop1);

  const float stop2 = 200.0;
  const unsigned long dur2 = 300UL;
  TweenDuino::Tween tween2(val, dur2, stop2);

  const float stop3 = 130.0;
  const unsigned long dur3 = 100UL;
  TweenDuino::Tween tween3(val, dur3, stop3);

  tl.add(tween1);
  tl.add(tween2);
  tl.add(tween3);

  tl.update(0UL);
  assertEqual(val, 0.0);
  assertFalse(tl.isComplete());
  assertFalse(tween1.isComplete());

  tl.update(dur1);
  assertEqual(val, stop1);
  assertFalse(tl.isComplete());
  assertTrue(tween1.isComplete());

  tl.update(dur1 + dur2);
  assertEqual(val, stop2);
  assertFalse(tl.isComplete());
  assertTrue(tween2.isComplete());

  tl.update(dur1 + dur2 + dur3);
  assertEqual(val, stop3);
  assertTrue(tl.isComplete());
  assertTrue(tween3.isComplete());

  // One last check... just for the heck of it.
  tl.update(dur1 + dur2 + dur3 + 1000UL);
  assertEqual(val, stop3);
  assertTrue(tl.isComplete());
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/8
 * 
 * Strange behavior would appear when adding too many tweens.
 */
test(rejectTooManyTweens) {
  TweenDuino::Timeline tl;
  const int maxTweens = tl.maxChildren();
  float val = 123.0;
  bool added = false;
  for (int i = 0; i < maxTweens; i++) {
    added = tl.add(*TweenDuino::Tween::to(val, 100UL, 123.0));
    assertTrue(added);
  }

  added = tl.add(*TweenDuino::Tween::to(val, 100UL, 123.0));
  assertFalse(added);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  // Convenient for waiting until serial monitor connects.
  while(!Serial);
}

void loop() {
  Test::run();
}