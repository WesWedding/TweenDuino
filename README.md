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
Currently depends on the [Easing library][1], a version of [Andy Brown's original easing library][2] for the easing math.

### Easing library bug and workaround
The Easing library does not #include the math library by default, which can prevent compilation from being successful because functions like `sqrt`, `cos`, and `sin` are unrecognized.

The workaround is quite simple; you merely need to add `#include <math.h>` to a single file located in your [libraries folder][3].

In file `Easing.h`, change these lines as follows:
```cpp
#pragma once
#ifndef HT_TWEEN_EASING_LIBRARY_H
#define HT_TWEEN_EASING_LIBRARY_H
```

to

```cpp
#pragma once
#ifndef HT_TWEEN_EASING_LIBRARY_H
#define HT_TWEEN_EASING_LIBRARY_H

#include <math.h>
```

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
float value = 20.0;

TweenDuino::Tween *tween;

void setup() {
    // Tween to 255.0 from the existing value over 10,000 milliseconds (10 secs).
    tween = TweenDuino::Tween::to(value, 10000UL, 255);
    Serial.print("Values: ");
}
void loop() {
    tween->update(millis());
    
    Serial.print(value); Serial.print(", ");
}
```

Output:
`Values: 0.00, 0.02, 0.04, 0.07, ... , 254.98, 255.00, 255.00, 255.00, ... `

## Code of Conduct
This project adheres to the Contributor Covenenant [code of conduct](code-of-conduct.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to conduct@wawco.com.

[1]: https://github.com/hideakitai/Easing
[2]: http://andybrown.me.uk/downloads/
[3]: https://support.arduino.cc/hc/en-us/articles/4415103213714-Find-sketches-libraries-board-cores-and-other-files-on-your-computer
