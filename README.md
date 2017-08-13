# TweenDuino Animation Library
Easily transition numeric variables or class members to/from final values gradually over time, using an intuitive API.

## Additional Library Required
Currently depends on [Andy Brown's easing library][1] for the easing math.

## Examples

### Increase a double from 20.0 to 255.0 over 10 seconds.
```
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
    
    Serial.print(value); Serial.print(", ");
}
```

Output:
`Values: 0.00, 0.02, 0.04, 0.07, ... , 254.98, 255.00, 255.00, 255.00, ... `


[1]: http://andybrown.me.uk/downloads/