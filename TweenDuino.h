/*--------------------------------------------------------------------
  This file is part of the TweenDuino tweening and animation library.

  Created by Weston Wedding and heavily influenced by the Greensock
  animation library, GSAP.

  This work is licensed under a Creative Commons
  Attribution_ShareAlike 3.0 Unported License.
  http://creativecommons.org/licenses/by_sa/3.0/
  --------------------------------------------------------------------*/

#ifndef TWEEN_DUINO_H
#define TWEEN_DUINO_H

#include <EasingLibrary.h>

class TweenDuino {
public:
    enum Ease { EASE, LINEAR };
    enum EaseType { IN, OUT, INOUT, NONE };
    
    TweenDuino(float &val, unsigned long duration, float to);

    static TweenDuino *to(float& target, unsigned long duration, float to);
    void update(unsigned long newTime);
    bool isActive();

private:
    float &target;

    unsigned long duration;
    unsigned long totalDuration;
    unsigned long time;
    unsigned long totalTime;
    unsigned long startTime;
    bool initialized;
    bool active;
    float finalVal;
    float startVal;
    float ratio;
    float totalChange;

    EasingBase *ease;

    void begin();
    double getRatio(float);
};

#endif