/**
    Ease the value stored in "value" from 20.0 to 255.0 gradually
    over 10 seconds.
*/
#include <TweenDuino.h>

// Name a pin attached to an oboard LED.
const int led = 7;

// A value to pass into a tween.
double value = 20.0;

TweenDuino *tween;

void setup() {
    // Tween to 255.0 from the existing value over 10,000 milliseconds (10 secs).
    tween = TweenDuino::to(value, 10000UL, 255);
    Serial.print("Values: ");
}
void loop() {
    tween->update(millis());
    
    if (tween->isActive()) {
      Serial.print(value); Serial.print(", ");
    }
}