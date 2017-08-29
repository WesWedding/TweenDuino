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
  }

TweenDuino::Tween::~Tween() {
  if (ease) {
    delete ease;
  }
}
TweenDuino::Tween *TweenDuino::Tween::to(float& target, unsigned long duration, float to) {
  return new Tween(target, duration, to);
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

void TweenDuino::Tween::update(unsigned long updTime) {

  unsigned long prevTime = time;

  // Set some times before potentially updating state further (if there's any time left);
  if (updTime >= duration) {
    totalTime = duration;
    time = duration;
    ratio = 1;
    completed = true;
  } else {
    totalTime = updTime;
    time = updTime;
    ratio = getRatio((float)time / (float)duration);
  }

  // Save ourselves some cycles if haven't moved ahead in time.
  if (time == prevTime) {
    return;
  }

  if (!initialized) {
    begin();
  }

  if (!completed) {  
    target = totalChange * ratio + startVal;
  }
}

void TweenDuino::Tween::begin() {
  ease = new SineEase();

  // These are somewhat arbitrary values. 
  // The actual tween result will be derived from actual time passage and what value we're tweening to/from.
  ease->setDuration(1);
  ease->setTotalChangeInPosition(1);

  initialized = true;
}

double TweenDuino::Tween::getRatio(float t) {
  if (ease == nullptr) {
    return 0.0d;
  }
  return ease->easeOut(t);
}
