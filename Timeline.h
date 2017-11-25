
#ifndef TWEEN_DUINO_TIMELINE_H
#define TWEEN_DUINO_TIMELINE_H


#ifndef TWEEN_TIMELINE_SIZE
  #define TWEEN_TIMELINE_SIZE 20
#endif

#include <Tween.h>

namespace TweenDuino {

class Timeline {
public:
    Timeline();

    void begin(unsigned long startTime);
    bool isActive();
    void update(unsigned long time);

    TweenDuino::Tween* addTo(float& target, unsigned long duration, float to);
    bool add(TweenDuino::Tween &tween);
    void restartFrom(unsigned long newStart);
    int maxChildren();
    unsigned long getDuration();
    bool isComplete();


private:
    class TimelineEntry {
    public:
        TimelineEntry();
        Tween *tween;
    };

    TweenDuino::Timeline::TimelineEntry tweens[TWEEN_TIMELINE_SIZE];

    unsigned long totalDuration;
    unsigned long totalTime;
    unsigned long startTime;
    unsigned long lastUpdateTime;
    bool completed;
    bool initialized;
};

}

#endif