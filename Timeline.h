
#ifndef TWEEN_DUINO_TIMELINE_H
#define TWEEN_DUINO_TIMELINE_H


#ifndef TIMELINE_SIZE
  #define TIMELINE_SIZE 10
#endif

#include <Tween.h>

namespace TweenDuino {

class Timeline {
public:
    Timeline();

    void update(unsigned long time);

    bool add(TweenDuino::Tween &tween);
    bool isComplete();


private:
    class TimelineEntry {
    public:
        TimelineEntry();
        Tween *tween;
        unsigned long startTime;
    };

    TweenDuino::Timeline::TimelineEntry tweens[TIMELINE_SIZE];

    unsigned long totalDuration;
    unsigned long totalTime;
    bool completed;
};

}

#endif