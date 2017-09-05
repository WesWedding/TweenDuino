
#include <Timeline.h>


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

TweenDuino::Timeline::TimelineEntry::TimelineEntry(): tween(nullptr), startTime(0) {}

TweenDuino::Timeline::Timeline(): totalDuration(0), totalTime(0), completed(false) {}

bool TweenDuino::Timeline::isComplete() {
    return completed;
}

bool TweenDuino::Timeline::add(TweenDuino::Tween &tween) {

    unsigned long nextStartTime = 0;
    int entryIndex = 0;
    // Maintence Note: Very similar looping logic in TweenDuino::Timeline::update.
    // If you change this line here, you might need to change it there.
    for (; entryIndex < TWEEN_TIMELINE_SIZE && tweens[entryIndex].tween != nullptr; entryIndex++) {
        unsigned long duration = tweens[entryIndex].tween->getDuration();
        nextStartTime += duration;
    }

    if (entryIndex >= TWEEN_TIMELINE_SIZE) {
        return false;
    }
    
    // i is pointing at an "empty" TimelineEntry at this point.  This tween's new home!
    TweenDuino::Timeline::TimelineEntry &entry = tweens[entryIndex];
    entry.tween = &tween;
    
    entry.startTime = nextStartTime;
    totalDuration += entry.tween->getDuration();

    return true;
}

void TweenDuino::Timeline::update(unsigned long newTime) {

    unsigned long prevTime = totalTime;
    if (newTime >= totalDuration) {
        totalTime = totalDuration;
        completed = true;
    } else {
        totalTime = newTime;
    }

    // Save ourselves some cycles if we haven't moved ahead in time.
    if (totalTime == prevTime) {
        return;
    }
    
    unsigned long curTime = totalTime;

    // Maintenance Note: Very similar looping logic in TweenDuino::Timeline::add
    // If you change this line here, you might need to change it there.
    for (int i = 0; i < TWEEN_TIMELINE_SIZE && tweens[i].tween != nullptr; i++) {
        TimelineEntry entry = tweens[i];

        // TODO: Remove pointless elses.  Left in for future debug for now.
        if (curTime >= entry.startTime) {
            Tween *tween = entry.tween;
            if (!tween->isComplete()) {
              tween->update(totalTime - entry.startTime);
            } else {
            }
        } else {

        }
    }
}

