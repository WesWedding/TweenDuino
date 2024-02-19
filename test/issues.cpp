#include <ArduinoUnitTests.h>
#include <TweenDuino.h>

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0 to 0.0 would crash.
 */
unittest(finalAndInitialAreZero)
{
  float val = 0.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  tween.update(1000UL);
  tween.update(56734UL);
  assertEqual(0.0, val);
  tween.update(56735UL);
  assertEqual(0.0, val);
}

/**
 * See: https://github.com/stickywes/TweenDuino/issues/3
 * 
 * A tween trying to tween from 0.0 to 0.0 would crash.
 */
unittest(valsetToZeroDuringTweenToZero)
{
  float val = 100.0;
  TweenDuino::Tween tween(val, 56734UL, 0.0);
  tween.update(0UL);
  assertNotEqual(0.0, val);
  val = 0.0;
  tween.update(1000UL);
  assertNotEqual(0.0, val);
  tween.update(56734UL);
  assertEqual(0.0, val);
  tween.update(56735UL);
  assertEqual(0.0, val);
}

unittest(valStartsAtExpectedVal) {
  float val = 0.0;
  TweenDuino::Tween tween(val, 56734UL, 255.0);
  tween.update(10UL);
  assertLessOrEqual(val, 100.0);
  tween.update(56734 / 3);
  assertLessOrEqual(val, 255.0 / 3.0);
}

/** 
 * Make sure that tweens in a timeline are cooperating with eachother,
 * and picking up where other tweens left off correctly.
 * 
 * Tweens were remembering the target value at initialization, and not what the
 * value was when they started running.
 * 
 * @see: https://github.com/stickywes/TweenDuino/issues/12
 * */
unittest(valueStartsAtValExpectedInTimeline) {
  float brightness = 0.0f;

  TweenDuino::Timeline tl;

  // Slowly ramp up brightness.
  tl.addTo(brightness, 1.0, 5000);
  tl.addTo(brightness, 0.5, 13000);
  tl.addTo(brightness, 1.0, 5000);

  tl.update(40);
  assertEqual(0.0, brightness);
  tl.update(5002);
  assertMore(brightness, 0.9);
  assertLessOrEqual(brightness, 1.0);
  tl.update(5100);
  assertMore(brightness, 0.9);
  assertLessOrEqual(brightness, 1.0);
  tl.update(18040);
  assertEqual(brightness, 0.5);
  tl.update(19000);
  assertMoreOrEqual(brightness, 0.5);
}

unittest_main()