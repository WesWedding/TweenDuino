#include <ArduinoUnit.h>
#include <TweenDuino.h>

#include "config.h"

float increasingFloat = 0.0;
TweenDuino::Tween increasing(increasingFloat, CONTINUOUS_DURATION, 100.0);

testing(increasingTween)
{
  unsigned long t = millis();
  increasing.update(t);
  assertLessOrEqual(increasingFloat, 100.0);
  assertMoreOrEqual(increasingFloat, 0.0);

  const unsigned long endTime = increasing.getStartTime() + increasing.getDuration();
  if (t >= endTime)
  {
    assertEqual(increasingFloat, 100.0);
    assertTrue(increasing.isComplete());
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

  const unsigned long endTime = increasing.getStartTime() + increasing.getDuration();
  if (t >= endTime)
  {
    assertEqual(decreasingFloat, 0.0);
    assertTrue(decreasing.isComplete());
    pass();
  }
}

test(tweenCompleteAtDurationReached)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  tween.update(56734UL);
  assertTrue(tween.isComplete());
  tween.update(56850UL);
  assertTrue(tween.isComplete());
}

test(finalAndInitialAreSame)
{
  float val = 4640.0;
  TweenDuino::Tween tween(val, 56734UL, 4640.0);
  tween.update(0UL);
  assertEqual(val, 4640.0);
  tween.update(1000UL);
  assertEqual(val, 4640.0);
  tween.update(56734UL);
  assertEqual(val, 4640.0);
  tween.update(56735UL);
  assertEqual(val, 4640.0);
}

test(tweenHitsMaxEvenIfFinalMilliSkipped)
{
  float val = 10.0;
  TweenDuino::Tween tween(val, 8732UL, 9820.34);
  tween.update(0UL);
  tween.update(1999UL);
  tween.update(8600UL);
  // Skip 8732UL.  Perhaps, for instance, a sketch had a delay() in it.
  tween.update(8790UL);
  assertEqual(val, 9820.34);
  assertTrue(tween.isComplete());
}

test(tweenWontExceedFinalValueAtEdges)
{
  float val = 10.0;
  TweenDuino::Tween tween(val, 8732UL, 9820.34);
  tween.update(0UL);
  tween.update(1999UL);
  tween.update(8731UL);
  assertEqual(val, 9820.34);
}

test(tweenStartingAfterZeroMillis)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 400UL, 100.0);
  tween.update(100UL);
  assertEqual(val, 0.0);
  tween.update(300UL);
  assertLess(val, 100.0);
  assertMore(val, 0.0);
}

test(restartingCompletedTweenNotComplete)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 400UL, 100.0);
  tween.update(0UL);
  tween.update(200UL);
  tween.update(400UL);
  tween.restartFrom(200UL);
  tween.update(200UL);
  assertFalse(tween.isComplete());
}

test(restartingTweenStartsFromNewVal)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 400UL, 100.0);
  tween.update(0UL);
  tween.update(200UL);
  tween.update(400UL);
  tween.restartFrom(200UL);
  tween.update(200UL);
  assertEqual(100.0, val);
  // Val at start of tween is 100, so it should just stay 100 at this point.
  tween.update(300UL);
  assertEqual(100.0, val);
}