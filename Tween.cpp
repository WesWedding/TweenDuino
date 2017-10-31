/*--------------------------------------------------------------------
  This file is part of the TweenDuino tweening/animation library.

  Created by Weston Wedding and heavily influenced by API presented
  by the Greensock animation library, GSAP.

  This work is licensed under a Creative Commons
  Attribution_ShareAlike 3.0 Unported License.
  http://creativecommons.org/licenses/by_sa/3.0/
  --------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
  #else
  #include "WProgram.h"
#endif


#include <Tween.h>

TweenDuino::Tween::Tween(float& t, unsigned long duration, float finalVal) 
  : target(t), duration(duration), finalVal(finalVal) {
    initialized = false;
    active = false;
    time = 0;
    ratio = 0;
    startVal = t;
    totalChange = finalVal - startVal;
    completed = false;
    ease = nullptr;
    startTime = 0;
  }

TweenDuino::Tween *TweenDuino::Tween::to(float& target, unsigned long duration, float to) {
  Tween *tween = new Tween(target, duration, to);
  tween->setTween(LINEAR, INOUT); 
  return tween;
}

TweenDuino::Tween *TweenDuino::Tween::to(float& target, unsigned long duration, float to, Ease ease, EaseType type) {
  Tween *tween = new Tween(target, duration, to);
  tween->setTween(ease, type);

  return tween;
}

bool TweenDuino::Tween::isActive() {
  return initialized && time < duration;
}

bool TweenDuino::Tween::isComplete() {
  return completed;
}

unsigned long TweenDuino::Tween::getDuration() {
  return duration;
}

unsigned long TweenDuino::Tween::getStartTime() {
  return startTime;
}

void TweenDuino::Tween::setTween(Ease e, EaseType type) {
  switch(e) { 
    case LINEAR:
      ease = new LinearEase();
      break;
    case SINE:
      ease = new SineEase();
      break;
    case QUAD:
      ease = new QuadraticEase();
      break;
    case QUART:
      ease = new QuarticEase();
      break;
    case QUINT:
      ease = new QuinticEase();
      break;
    case CUBIC:
      ease = new CubicEase();
      break;
  }

  easeType = type;
}

void TweenDuino::Tween::begin(unsigned long timeMs) {
  if (ease == nullptr) {
    setTween(Ease::LINEAR, EaseType::INOUT);
  }
  startTime = timeMs;
  time = timeMs;

  //Serial.print("setting tween start to: ");  Serial.println(timeMs);

  // The target's current state might have changed by now.
  startVal = target;
  totalChange = finalVal - startVal;

  // These are somewhat arbitrary values. 
  // The actual tween result will be derived from actual time passage and what value we're tweening to/from.
  ease->setDuration(1);
  ease->setTotalChangeInPosition(1);

  initialized = true;
}

/**
 * Update the tween.  If the tween has not been initialized with begin(), the first update
 * also becomes the startTime of the tween.
 */
void TweenDuino::Tween::update(unsigned long updTime) {

  if (!initialized) {
    begin(updTime);
  }

  unsigned long prevTime = time;

  // Set some times before potentially updating state further (if there's any time left);

    Serial.print("duration: ");  Serial.println(duration);
    Serial.print("startTime: ");  Serial.println(startTime);
    Serial.print("updTime: ");  Serial.println(updTime);
  if (updTime >= duration + startTime) {
    totalTime = duration;
    time = duration;
    ratio = 1.0;
    completed = true;
  } else if (updTime < startTime) {
    totalTime = 0;
    time = 0;
    ratio = 0.0;
  } else {
    totalTime = updTime;
    time = updTime;
    ratio = getRatio((float)(time - startTime) / (float)duration);
  }

  // Save ourselves some cycles if haven't moved ahead in time,
  // or if we're done rendering.
  // Initialized check is important in case the first update() happens at updTime == 0.
  if (time == prevTime) {
    return;
  }

  target = totalChange * ratio + startVal;
}

void TweenDuino::Tween::restartFrom(unsigned long newStart) {
  completed = false;
  initialized = false;
  time = 0;
  startTime = newStart;
}

double TweenDuino::Tween::getRatio(float t) {
  if (ease == nullptr) {
    return 0.0d;
  }

  if (easeType == IN) {
    return ease->easeIn(t);
  } else if (easeType == OUT) {
    return ease->easeOut(t);
  } else if (easeType == INOUT) {
    return ease->easeInOut(t);
  }

  return 0.0d;
}
