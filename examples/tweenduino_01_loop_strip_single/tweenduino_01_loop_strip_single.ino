/**
 * Light up a single LED in a strip of Adafruit NeoPixels and move it
 * along the entire strip in a loop, one LED at a time.
*/
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 7
#define LEDS_PIN 9

// A value to pass into a tween.
float position = 0;
int lastPosition = 0;

// This be used to switch brightened LEDs from 0 to NUM_LEDS over 10 seconds.
TweenDuino::Tween *tween = TweenDuino::Tween::to(position, 10000, NUM_LEDS);

// Set up a strip of 4-color "RGBW" LEDs.  Change the 3rd argument to better suit
// the type of LED you're using.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_RGBW);

// Pick a random color! Numbers are: Green, Red, Blue, White
uint32_t color = strip.Color(random(256), random(256), random(256), 0);

void setup() {
    strip.begin();
    tween->begin(millis());

    // Set the strip to show nothing.
    strip.clear();
    strip.show();
}
void loop() {
    uint32_t time = millis();

    // Tell the tween that time has changed and to adjust its calculations.
    tween->update(time);

    // Pick a new random color if we're showing a new LED.
    if ((int) position != lastPosition) {
        // Visually a lot of the colors end up looking almost white if we randomize up to 256 on all of the colors.
        color = strip.Color(random(150), random(150), random(150), 0);        
    }

    strip.clear();
    // Loop through all of the pixels and set only the "current" one to our color.

    strip.setPixelColor(position, color);
    strip.show();

    lastPosition = position;

    // Restart the loop if we're finished!
    if (tween->isComplete()) {
        position = 0;
        tween->restartFrom(time);
    }
}