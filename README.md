# TweenDuino Animation Library

Easily transition numeric variables or class members to/from final values gradually over time, using an intuitive API.

Main: ![Build Status (main)](https://github.com/WesWedding/TweenDuino/actions/workflows/arduino_ci.yml/badge.svg?branch=main)
Dev:  ![Build Status (dev)](https://github.com/WesWedding/TweenDuino/actions/workflows/arduino_ci.yml/badge.svg?branch=dev)

## Timelines and Tweens

The fundamental components of this library are two classes: the Timeline and Tween.

Tweens are used to change a value nicely along a curve from a starting value
to an ending value.  This value can be something like a brightness value used
in some LED logic, or the speed or angle of a motor.

Timelines are containers that store a series of Tweens and play them in sequence.
Using a timeline allows you use this tween sequence to do complicated animations
or behaviors that would otherwise require potentially hard to understand `for` loops or mathmatical formulae.

## Additional Library Required

Currently depends on the [Easing library][1], a version of [Andy Brown's original easing library][2] for the easing math.

If you are using VSCode's Arduino extension or the official Arduino IDE, you
can install this dependency via the appropriate Library Manager.

## Usage
See the examples folder for full working examples, however the basic approach is...

```cpp
. . .

Timeline timeline;
int value = 0.0;

setup() {

    // Add a series of 3 tweens as follows...

    // Move value up "100" gradually over 5 seconds.
    timeline.addTo(value, 100, 5000);
    // Move value down to "1" gradually over 3 seconds.
    timeline.addTo(value, 1, 3000);
    // Move value to "9001" gradually over 5 seconds.
    timeline.addTo(value, 9001, 8000);
}

// Watch the number smoothly change as the sketch cycles in loop().
loop() {
    const int now = millis();
    timeline.update(now);
}

. . .

```

### Technical Note: Max timeline size

By default, a Timeline comes with enough storage for 10 tweens.  If you want to reduce the memory footprint (or increase the number of tweens you want to store) you will want to use a `#define TIMELINE_SIZE XXX` statement, where "XXX" is an integer value, to control this.  Make sure this #define happens before your include statement!

```cpp
...

#define TIMELINE_SIZE 10
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

...
```

## Code of Conduct

This project adheres to the Contributor Covenenant [code of conduct](code-of-conduct.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to conduct@wawco.com.

[1]: https://github.com/hideakitai/Easing
[2]: http://andybrown.me.uk/downloads/
[3]: https://support.arduino.cc/hc/en-us/articles/4415103213714-Find-sketches-libraries-board-cores-and-other-files-on-your-computer
