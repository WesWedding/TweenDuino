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

TweenDuino::TweenDuino(double& t, unsigned long duration, double finalVal) 
  : target(t), duration(duration), finalVal(finalVal), active(false), initialized(false) {
  }

TweenDuino::~TweenDuino() {
  if (ease) {
    delete ease;
  }
}

TweenDuino *TweenDuino::to(double& target, unsigned long duration, double to) {
  return new TweenDuino(target, duration, to);
}

bool TweenDuino::isActive() {
  return initialized && time <= (startTime + duration);
}

void TweenDuino::update(unsigned long newTime) {
  unsigned long prevTime = time;

  // Set some times before potentially updating state further (if there's any time left);
  if (time >= duration && time >= 0) {
    totalTime = newTime;
    time = newTime;
    ratio = 1;
  } else if (time < 0) {
    totalTime = 0;
    time = 0;
    ratio = 0;
  } else {
    totalTime = newTime;
    time = newTime;
    ratio = getRatio((double)time / (double)duration);
  }

  if (time == prevTime) {
    return;
  }

  if (!initialized) {
    init();
    
    startVal = target;
    startTime = newTime;

    changeRate = (double) finalVal - startVal;
    if (changeRate < 0.0d) {
      changeRate = 0.0d;
    }
  }

  target = changeRate * ratio + startVal;
}

void TweenDuino::init() {
  ease = new SineEase();

  // These are somewhat arbitrary values. 
  // The actual tween result will be derived from actual time passage and what value we're tweening to/from.
  ease->setDuration(1);
  ease->setTotalChangeInPosition(1);

  initialized = true;
}

double TweenDuino::getRatio(double t) {
  if (!ease) {
    return 0.0d;
  }
  return ease->easeOut(t);
}
