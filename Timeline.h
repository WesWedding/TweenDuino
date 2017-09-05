
#ifndef TWEEN_DUINO_TIMELINE_H
#define TWEEN_DUINO_TIMELINE_H


#ifndef TWEEN_TIMELINE_SIZE
  #define TWEEN_TIMELINE_SIZE 90
#endif

#include <Tween.h>

namespace TweenDuino {

class Timeline {
public:
    Timeline();

    void update(unsigned long time);

    bool add(TweenDuino::Tween &tween);
    int maxChildren();
    bool isComplete();


private:
    class TimelineEntry {
    public:
        TimelineEntry();
        Tween *tween;
        unsigned long startTime;
    };

    TweenDuino::Timeline::TimelineEntry tweens[TWEEN_TIMELINE_SIZE];

    unsigned long totalDuration;
    unsigned long totalTime;
    bool completed;
};

}

#endif