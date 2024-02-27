/*--------------------------------------------------------------------
  This file is part of the TweenDuino tweening and animation library.

  Created by Weston Wedding and heavily influenced by the Greensock
  animation library, GSAP.

  This work is licensed under a Creative Commons
  Attribution_ShareAlike 3.0 Unported License.
  http://creativecommons.org/licenses/by_sa/3.0/
  --------------------------------------------------------------------*/

#ifndef TWEEN_DUINO_TWEEN_H
#define TWEEN_DUINO_TWEEN_H

#include <math.h>
#include <Easing.h>

namespace TweenDuino {

class Tween {
public:
    enum Ease { LINEAR, SINE, QUAD, CUBIC, QUART, QUINT };
    enum EaseType { IN, OUT, INOUT, NONE };
    
    Tween(float &val, unsigned long duration, float to);
    Tween(const Tween&) = delete;
    Tween& operator=(Tween const&) = delete;
    ~Tween();

    static Tween *to(float& target, unsigned long duration, float to);
    static Tween *to(float& target, unsigned long duration, float to, Ease ease, EaseType type);
    void setEase(Ease e, EaseType t);
    void begin(unsigned long startTime);
    void update(unsigned long newTime);
    void restartFrom(unsigned long newStart);
    
    bool isActive();
    bool isComplete();
    unsigned long getDuration();
    unsigned long getStartTime();

private:
    float &target;

    unsigned long duration;
    unsigned long totalDuration;
    unsigned long time;
    unsigned long totalTime;
    unsigned long startTime;
    unsigned long lastUpdateTime;
    bool initialized;
    bool onFirstUpdate;
    bool active;
    bool completed;
    float finalVal;
    float startVal;
    float ratio;
    float totalChange;

    EaseType easeType;
    ht::ease::EasingBase *ease;
    double getRatio(float);
};

}

#endif