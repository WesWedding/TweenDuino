#include <ArduinoUnit.h>
#include <TweenDuino.h>

#include "config.h"

// Too many tests to fit into a single test; adjust which batch is commented
// out.
#include "Tweens.h"
#include "issues.h"
//#include "Timelines.h"

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