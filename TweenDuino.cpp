/*--------------------------------------------------------------------
  This file is part of the TweenDuino tweening/animation library.

  Created by Weston Wedding and heavily influenced by the Greensock
  animation library, GSAP.

  This work is licensed under a Creative Commons
  Attribution_ShareAlike 3.0 Unported License.
  http://creativecommons.org/licenses/by_sa/3.0/
  --------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <TweenDuino.h>

TweenDuino::TweenDuino(float& t, unsigned long duration, float finalVal) 
  : target(t), duration(duration), finalVal(finalVal) {
    initialized = false;
    active = false;
    time = 0;
    ratio = 0;
    startVal = t;
    totalChange = finalVal - startVal;
  }

TweenDuino::~TweenDuino() {
  if (ease) {
    delete ease;
  }
}
TweenDuino *TweenDuino::to(float& target, unsigned long duration, float to) {
  return new TweenDuino(target, duration, to);
}

bool TweenDuino::isActive() {
  return initialized && time < duration;
}

void TweenDuino::update(unsigned long updTime) {

  unsigned long prevTime = time;

  // Set some times before potentially updating state further (if there's any time left);
  if (updTime >= duration) {
    totalTime = duration;
    time = duration;
    ratio = 1;
  } else {
    totalTime = updTime;
    time = updTime;
    ratio = getRatio((float)time / (float)duration);
  }

  // Save ourselves some cycles if we're not moving forward.
  if (time == prevTime) {
    return;
  }

  if (!initialized) {
    begin();
  }
  
  target = totalChange * ratio + startVal;
}

void TweenDuino::begin() {
  ease = new SineEase();

  // These are somewhat arbitrary values. 
  // The actual tween result will be derived from actual time passage and what value we're tweening to/from.
  ease->setDuration(1);
  ease->setTotalChangeInPosition(1);

  initialized = true;
}

double TweenDuino::getRatio(float t) {
  if (!ease) {
    return 0.0d;
  }
  return ease->easeOut(t);
}
