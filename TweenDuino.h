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
    
    TweenDuino(double &val, unsigned long duration, double to);
    ~TweenDuino();

    static TweenDuino *to(double& target, unsigned long duration, double to);
    void update(unsigned long newTime);
    bool isActive();

private:
    double &target;

    unsigned long duration;
    unsigned long totalDuration;
    unsigned long time;
    unsigned long totalTime;
    unsigned long startTime;
    bool initialized;
    bool active;
    double finalVal;
    double startVal;
    double ratio;
    double totalChange;

    EasingBase *ease;

    void begin();
    double getRatio(double);
};

#endif