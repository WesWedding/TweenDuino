#include <ArduinoUnitTests.h>
#include <TweenDuino.h>

#include "config.h"

unittest(emptyTimelineCompletes)
{
  TweenDuino::Timeline tl;
  tl.update(0UL);
  assertTrue(tl.isComplete());
}

unittest(timelineCompletesOnFinalMS)
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

  const float stop3 = 130.0;
  const unsigned long dur3 = 100UL;
  TweenDuino::Tween* tween3Ptr = tl.addTo(val, dur3, stop3);

  tl.update(0UL);
  assertEqual(val, 0.0);
  assertFalse(tl.isComplete());
  assertFalse(tween1.isComplete());
  assertFalse(tl.isComplete());

  tl.update(dur1);
  assertEqual(val, stop1);
  assertFalse(tl.isComplete());
  assertTrue(tween1.isComplete());
  assertFalse(tl.isComplete());

  tl.update(dur1 + dur2);
  assertEqual(val, stop2);
  assertFalse(tl.isComplete());
  assertTrue(tween2.isComplete());
  assertFalse(tl.isComplete());

  tl.update(dur1 + dur2 + dur3);
  assertEqual(val, stop3);
  assertTrue(tl.isComplete());
  assertTrue(tween3Ptr->isComplete());

  // One last check... just for the heck of it.
  tl.update(dur1 + dur2 + dur3 + 1000UL);
  assertEqual(val, stop3);
  assertTrue(tl.isComplete());
}

unittest(timelineDurationMatchesTweenDurations)
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

unittest(timelineStartingAfterZeroMillisFinishesWhenExpected)
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

unittest(timelineStartingAfterZeroMillisUpdatesValueCorrectly)
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

unittest(restartTimelineRestartsTweens)
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
  assertTrue(tween1.isComplete());
  assertTrue(tween2.isComplete());
  assertTrue(tween3.isComplete());
  assertEqual(stop3, val);
  tl.restartFrom(300UL);
  assertEqual(stop3, val);
  tl.update(300UL);
  assertTrue(tween1.isActive());
  assertFalse(tween2.isActive());
  assertFalse(tween3.isActive());
  assertFalse(tween1.isComplete());
  assertFalse(tween2.isComplete());
  assertFalse(tween3.isComplete());
  assertFalse(tl.isComplete());
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/8
 * 
 * Strange behavior would appear when adding too many tweens.
 */
unittest(rejectTooManyTweens)
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

unittest_main()
