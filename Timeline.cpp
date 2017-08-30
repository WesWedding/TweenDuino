
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
    for (; tweens[entryIndex].tween != nullptr && entryIndex < TIMELINE_SIZE; entryIndex++) {
        unsigned long duration = tweens[entryIndex].tween->getDuration();
        
        Serial.print("duration for tween: "); Serial.println(duration); 
        nextStartTime += duration;
        
    }
    
    Serial.print("startTime after checks: "); Serial.println(nextStartTime);       

    if (entryIndex >= TIMELINE_SIZE) {
        return false;
    }
    
    Serial.print("adding tween at: "); Serial.println(entryIndex);

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
    for (int i = 0; tweens[i].tween != nullptr && i < TIMELINE_SIZE; i++) {
        //Serial.print("updating tween at "); Serial.println(i);
        TimelineEntry entry = tweens[i];
        
        Serial.print("startTime set: "); Serial.println(entry.startTime);
        if (curTime >= entry.startTime) {
            Tween *tween = entry.tween;
            if (!tween->isComplete()) {
              Serial.print("progressing Tween: "); Serial.println(i);
              tween->update(totalTime - entry.startTime);
            } else {
              Serial.print("skipped completed Tween: "); Serial.println(i);
            }
        } else {
          Serial.print("Tween no ready: "); Serial.println(i);
        }
    }
}

