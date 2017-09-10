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

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0 to 0.0 would crash.
 */
test(finalAndInitialAreZero)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(val, 0.0);
  tween.update(56735UL);
  assertEqual(val, 0.0);
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0 to 0.0 would crash.
 */
test(valsetToZeroDuringTweenToZero)
{
  float val = 100.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  assertNotEqual(val, 0.0);
  val = 0.0;
  tween.update(1000UL);
  assertNotEqual(val, 0.0);
  tween.update(56734UL);
  assertEqual(val, 0.0);
  tween.update(56735UL);
  assertEqual(val, 0.0);
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

/**
 * See: https://github.com/stickywes/TweenDuino/issues/8
 * 
 * Strange behavior would appear when adding too many tweens.
 */
test(rejectTooManyTweens)
{
  TweenDuino::Timeline tl;
  const int maxTweens = tl.maxChildren();
  float val = 123.0;
  bool added = false;
  for (int i = 0; i < maxTweens; i++)
  {
    added = tl.add(*TweenDuino::Tween::to(val, 100UL, 123.0));
    assertTrue(added);
  }

  added = tl.add(*TweenDuino::Tween::to(val, 100UL, 123.0));
  assertFalse(added);
}

test(emptyTimelineCompletes)
{
  TweenDuino::Timeline tl;
  tl.update(0UL);
  assertTrue(tl.isComplete());
}

test(timelineCompletesOnFinalMS)
{
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

test(timelineDurationMatchesTweenDurations)
{
  TweenDuino::Timeline tl;

  float val = 0.0;
  const float stop1 = 100.0;
  const unsigned long dur1 = 400UL;
  TweenDuino::Tween tween1(val, dur1, stop1);

  const float stop2 = 200.0;
  const unsigned long dur2 = 300UL;
  TweenDuino::Tween tween2(val, dur2, stop2);

  tl.add(tween1);
  tl.add(tween2);

  assertEqual(tl.getDuration(), tween1.getDuration() + tween2.getDuration());
}

test(timelineStartingAfterZeroMillisFinishesWhenExpected)
{
  TweenDuino::Timeline tl;

  float val = 0.0;
  const float stop1 = 100.0;
  const unsigned long dur1 = 400UL;
  TweenDuino::Tween tween1(val, dur1, stop1);

  const float stop2 = 200.0;
  const unsigned long dur2 = 300UL;
  TweenDuino::Tween tween2(val, dur2, stop2);

  tl.add(tween1);
  tl.add(tween2);

  // Start time becomes 100UL.  So finish will be dur1+dur2+100UL.
  // NOT just dur1 + dur2.
  tl.update(100UL);
  tl.update(dur1 + dur2);
  assertFalse(tl.isComplete());
  tl.update(dur1 + dur2 + 99UL);
  assertFalse(tl.isComplete());
  tl.update(dur1 + dur2 + 100UL);
  assertTrue(tl.isComplete());
}

test(timelineStartingAfterZeroMillisUpdatesValueCorrectly)
{
  TweenDuino::Timeline tl;

  float val = 0.0;

  const float stop1 = 100.0;
  const unsigned long dur1 = 400UL;
  TweenDuino::Tween tween1(val, dur1, stop1);

  const float stop2 = 200.0;
  const unsigned long dur2 = 300UL;
  TweenDuino::Tween tween2(val, dur2, stop2);

  tl.add(tween1);
  tl.add(tween2);

  tl.update(100UL);
  assertEqual(val, 0.0);
  tl.update(100UL + dur1);
  assertEqual(val, stop1);
  tl.update(100UL + dur1 + dur2);
  assertEqual(val, stop2);
}

test(restartTimelineRestartsTweens)
{
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
  tl.update(dur1 + dur2 + dur3);
  assertTrue(tl.isComplete());
  tl.restartFrom(300UL);
  assertFalse(tween1.isComplete());
  assertFalse(tween2.isComplete());
  assertFalse(tween3.isComplete());
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  // Test::exclude("name") go here!
  Test::exclude("tweenWontExceedFinalValueAtEdges"); // Low priority at the moment.

  // This can cause memory errors now that our suite is so big.  Turn down the
  // timeline max value defined in Timeline.h to avoid crashes.
  Test::exclude("rejectTooManyTweens");

  //   Test::exclude("increasingTween");
  //   Test::exclude("decreasingTween");
  //   Test::exclude("finalAndInitialAreSame");
  //   Test::exclude("finalAndInitialAreZero");
  //   Test::exclude("valsetToZeroDuringTweenToZero");
  //   Test::exclude("tweenHitsMaxEvenIfFinalMilliSkipped");
  //   Test::exclude("tweenStartingAfterZero");
  //   Test::exclude("emptyTimelineCompletes");
  //   Test::exclude("restartTimelineRestartsTweens");
  //   Test::exclude("timelineCompletesOnFinalMS");
  //   Test::exclude("tweenCompleteAtDurationReached");
  //   Test::exclude("restartingCompletedTweenNotComplete");
  //   Test::exclude("restartingTweenStartsFromNewVal");
  //   Test::exclude("timelineDurationMatchesTweenDurations");
  //   Test::exclude("timelineStartingAfterZeroMillisFinishesWhenExpected");
  //   Test::exclude("timelineStartingAfterZeroMillisUpdatesValueCorrectly");
  //   Test::exclude("tweenStartingAfterZeroMillis");

  // Convenient to wait for serial monitor, so you don't miss results.
  while (!Serial)
    ;
}

void loop()
{
  Test::run();
}