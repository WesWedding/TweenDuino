/**
    Chain together a series of changes, using a Timeline.

    Watch the serial monitor to see the change in action!
*/
#include <TweenDuino.h>

using namespace TweenDuino;

// A value to pass into a tweens.
float value = 20.0;

// The 3 values we will be tweening the "value" to, one after another.
const float firstStop = 255.0;
const float secondStop = 255.0;
const float thirdStop = 40000.54;

const unsigned long firstDuration = 5000; // 5 seconds
const unsigned long secondDuration = 8500; // 8.5 seconds
const unsigned long thirdDuration = 2900; // 2.9 seconds

// An empty timeline, which we will be adding 3 tweens to.
Timeline timeline;

// The setup in an Arduino sketch.
void setup() {
    // Add 3 tweens to the timeline.  Notice that each tween is acting on the same "value" variable!
    // Add a Quadratic EaseIn tween.  See: https://easings.net/#easeInQuad
    timeline.addTo(value, firstStop, firstDuration, Tween::Ease::QUAD, Tween::EaseType::IN);

    // This tween uses the default Linear easing, which is a gradual constant increase or decrease.
    timeline.addTo(value, secondStop, secondDuration);

    // This tween uses Sine Out easing.  See: https://easings.net/#easeOutSine
    timeline.addTo(value, thirdStop, thirdDuration, Tween::Ease::SINE, Tween::EaseType::OUT);

    Serial.print("Values: ");
}

// The main loop of an Arduino sketch.
void loop() {
    // The current time, according to the processor on the board (not necessarily real clock time).
    const int currentTime = millis();

    // Tell the timeline that time has progressed via update().  This must happen every time loop() runs.
    timeline.update(currentTime);

    // Display our results until the tween is done.  Watch this on the Serial Monitor.
    if (timeline.isActive()) {
        Serial.print(value); Serial.print(", ");
    }
}