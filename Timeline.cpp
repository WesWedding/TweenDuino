
#include <Timeline.h>


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

TweenDuino::Timeline::TimelineEntry::TimelineEntry(): tween(nullptr) {}

TweenDuino::Timeline::Timeline(): 
    totalDuration(0), 
    totalTime(0), 
    startTime(0), 
    completed(false), 
    initialized(false),
    lastUpdateTime(0) {}

int TweenDuino::Timeline::maxChildren() {
    return TWEEN_TIMELINE_SIZE;
}

bool TweenDuino::Timeline::isActive() {
    return initialized && lastUpdateTime >= startTime && !completed;
}

bool TweenDuino::Timeline::isComplete() {
    return completed;
}

TweenDuino::Tween* TweenDuino::Timeline::addTo(float& target, float to, unsigned long duration) {
    Tween* tween = TweenDuino::Tween::to(target, duration, to);
    add(*tween);
    return tween;
}

TweenDuino::Tween* TweenDuino::Timeline::addTo(float& target, float to, unsigned long duration, TweenDuino::Tween::Ease e, TweenDuino::Tween::EaseType type) {
    Tween* tween = TweenDuino::Tween::to(target, duration, to, e, type);
    add(*tween);
    return tween;
}

/**
 * Add tweens to this timeline.
 *
 * Child tweens will be unaware of any delays or timeline restarts; the 1st tween
 * will always think it's starting at 0 millis, the 2nd exactly at the end of the
 * 1st tween, etc.
 */
bool TweenDuino::Timeline::add(TweenDuino::Tween &tween) {

    unsigned long nextStartTime = 0;
    int entryIndex = 0;
    // Maintence Note: Very similar looping logic in TweenDuino::Timeline::update & restart.
    // If you change this line here, you might need to change it there.
    for (; entryIndex < TWEEN_TIMELINE_SIZE && tweens[entryIndex].tween != nullptr; entryIndex++) {
        unsigned long duration = tweens[entryIndex].tween->getDuration();
        nextStartTime += duration;
    }

    TweenDuino::Timeline::TimelineEntry &entry = tweens[entryIndex];
    if (entry.tween != nullptr) {
        return false;
    }
    // i is pointing at an "empty" TimelineEntry at this point.  This tween's new home!
    entry.tween = &tween;
    entry.tween->begin(nextStartTime);
    totalDuration += entry.tween->getDuration();

    return true;
}

void TweenDuino::Timeline::begin(unsigned long timeMs) {
    startTime = timeMs;
    totalTime = timeMs;
    initialized = true;
}

void TweenDuino::Timeline::update(unsigned long newTime) {
    
    if (!initialized) {
        begin(newTime);
    }

    unsigned long prevTime = totalTime;    

    if (newTime >= totalDuration + startTime) {
        totalTime = totalDuration + startTime;
        completed = true;
    } else {
        totalTime = newTime;
    }

    lastUpdateTime = newTime;

    // Save ourselves some cycles if we haven't moved ahead in time.
    if (totalTime == prevTime) {
        return;
    }
    
    unsigned long curTime = totalTime - startTime;
    // Serial.print("curTime: "); Serial.println(curTime);

    // Maintenance Note: Very similar looping logic in TweenDuino::Timeline::add & restart.
    // If you change this line here, you might need to change it there.
    for (int i = 0; i < TWEEN_TIMELINE_SIZE && tweens[i].tween != nullptr; i++) {
        TimelineEntry entry = tweens[i];
        Tween *tween = entry.tween;
        //Serial.print("About to check next start");  delay(100);
        const unsigned long started = tween->getStartTime();
        //Serial.print("Start is: "); delay(1000); Serial.println(started);
        // TODO: Remove pointless elses.  Left in for future debug for now.
        // Serial.print("tween starting time: "); Serial.println(started);
        if (curTime >= started) {
            if (!tween->isComplete()) {
            //   Serial.print("updating with: "); Serial.println(curTime);
              tween->update(curTime);
            } else {
            }
        } else {

        }
    }
}

unsigned long TweenDuino::Timeline::getDuration() {
    return totalDuration;
}

void TweenDuino::Timeline::restartFrom(unsigned long newTime) {

    completed = false;
    startTime = newTime;
    lastUpdateTime = newTime;
    totalTime = 0;

    unsigned long entryStart = 0;
    // Maintenance Note: Very similar looping logic in TweenDuino::Timeline::add & update
    // If you change this line here, you might need to change it there.
    for (int i = 0; i < TWEEN_TIMELINE_SIZE && tweens[i].tween != nullptr; i++) {
        TimelineEntry entry = tweens[i];

        entry.tween->restartFrom(entryStart);        

        // Next entry starts at...
        entryStart += entry.tween->getDuration();
    }
}

