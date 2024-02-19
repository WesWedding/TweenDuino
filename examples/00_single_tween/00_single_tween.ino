/**
    Ease the value stored in "value" from 20.0 to 255.0 gradually
    over 10 seconds.

    Watch the serial monitor to see the change in action.
*/
#include <TweenDuino.h>

using namespace TweenDuino;

// A value to pass into a tween.
float value = 20.0;

const float finalValue = 255.0;

// The Tween will run for 10,000 milliseconds (10 secs).
const unsigned long duration = 10000;


// This will transition from value (20.0) to finalValue (255.0) over duration (10 seconds).
Tween tween(value, duration, finalValue);

// The setup in an Arduino sketch.
void setup() {

    // Use a Sine InOut: https://easings.net/#easeInOutSine
    tween.setEase(Tween::Ease::SINE, Tween::EaseType::INOUT);
    Serial.print("Values: ");
}

// The main loop of an Arduino sketch.
void loop() {
    // The current time, according to the processor on the board (not necessarily real clock time).
    const int currentTime = millis();

    // Tell the tween that time has progressed via update().  Millis() gives us the current time according to the CPU running.
    tween.update(millis());
  
    // Display our results until the tween is done.  Watch this on the Serial Monitor.
    if (tween.isActive()) {
        Serial.print(value); Serial.print(", ");
    }
}