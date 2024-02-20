#include <ArduinoUnitTests.h>
#include <TweenDuino.h>

#include "config.h"

float increasingFloat = 0.0;
TweenDuino::Tween increasing(increasingFloat, CONTINUOUS_DURATION, 100.0);

unittest(increasingTween)
{
  unsigned long t = 100;  // No millis() for us, alas.  Not sure why, though.
  increasing.update(t);
  assertLessOrEqual(increasingFloat, 100.0);
  assertMoreOrEqual(increasingFloat, 0.0);

  const unsigned long endTime = increasing.getStartTime() + increasing.getDuration();
  if (t >= endTime)
  {
    assertEqual(increasingFloat, 100.0);
    assertTrue(increasing.isComplete());
  }
}

float decreasingFloat = 100.0;
TweenDuino::Tween decreasing(decreasingFloat, CONTINUOUS_DURATION, 0.0);

unittest(decreasingTween)
{
  unsigned long t = 100;  // No millis() for us, alas.  Not sure why, though.
  decreasing.update(t);
  assertLessOrEqual(decreasingFloat, 100.0);
  assertMoreOrEqual(decreasingFloat, 0.0);

  const unsigned long endTime = increasing.getStartTime() + increasing.getDuration();
  if (t >= endTime)
  {
    assertEqual(0.0, decreasingFloat);
    assertTrue(decreasing.isComplete());
  }
}

unittest(tweenCompleteAtDurationReached)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  tween.update(56734UL);
  assertTrue(tween.isComplete());
  tween.update(56850UL);
  assertTrue(tween.isComplete());
}

unittest(finalAndInitialAreSame)
{
  float val = 4640.0;
  TweenDuino::Tween tween(val, 56734UL, 4640.0);
  tween.update(0UL);
  assertEqual(4640.0, val);
  tween.update(1000UL);
  assertEqual(4640.0, val);
  tween.update(56734UL);
  assertEqual(4640.0, val);
  tween.update(56735UL);
  assertEqual(4640.0, val);
}

unittest(tweenHitsMaxEvenIfFinalMilliSkipped)
{
  float val = 10.0;
  TweenDuino::Tween tween(val, 8732UL, 9820.34);
  tween.update(0UL);
  tween.update(1999UL);
  tween.update(8600UL);
  // Skip 8732UL.  Perhaps, for instance, a sketch had a delay() in it.
  tween.update(8790UL);
  assertEqualFloat(9820.34, val, 2);
  assertTrue(tween.isComplete());
}

unittest(tweenWontExceedFinalValueAtEdges)
{
  float val = 10.0;
  TweenDuino::Tween tween(val, 8732UL, 9820.34);
  tween.update(0UL);
  tween.update(1999UL);
  tween.update(8731UL);
  assertEqualFloat(9820.34, val, 2);
}

unittest(tweenStartingAfterZeroMillis)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 400UL, 100.0);
  tween.update(100UL);
  assertEqual(0.0, val);
  tween.update(300UL);
  assertLess(val, 100.0);
  assertMore(val, 0.0);
}

unittest(restartingCompletedTweenNotComplete)
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

unittest(restartingTweenStartsFromNewVal)
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

unittest(changeEase)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 400UL, 100.0);
  tween.setEase(TweenDuino::Tween::Ease::SINE, TweenDuino::Tween::EaseType::INOUT);
  tween.setEase(TweenDuino::Tween::Ease::QUART, TweenDuino::Tween::EaseType::OUT);
  tween.update(0UL);
  tween.update(900UL);
  assertTrue(tween.isComplete());
}

unittest_main()
