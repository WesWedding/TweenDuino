# TweenDuino Animation Library
Easily transition numeric variables or class members to/from final values gradually over time, using an intuitive API.


## Timelines and Tweens
The fundamental components of this library are two classes: the Timeline and Tween.

Tweens are used to change a value nicely along a curve from a starting values
to an ending value.  This value can be something like a brightness value used
in some LED logic, or the speed of a motor.

Timelines are containers that store a series of Tweens and play them in order.
Using a timeline allows you use this tween sequence to do complicated animations
or behaviors that would otherwise require potentially hard to understand for loops or mathmatical formulae.

## Additional Library Required
Currently depends on [Andy Brown's easing library][1] for the easing math.


## Usage
By default, a Timeline comes with enough storage for 10 tweens.  If you want to reduce the memory footprint (or increase the number of tweens you want to store) you will want to use a `#define TIMELINE_SIZE XXX` statement, where "XXX" is an integer value, to control this.  Make sure this #define happens before your include statement!

```
...

#define TIMELINE_SIZE 10
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

...
```

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