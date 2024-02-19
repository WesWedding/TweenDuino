/**
 * A more advanced example: Light up a single LED in a strip of Adafruit NeoPixels 
 * make it travel along the entire strip in a loop, one LED at a time.
*/
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>

using namespace TweenDuino;

// How many LEDs are on the NeoPixel strip?  Change this as appropriate to your setup.
#define NUM_LEDS 7
// Which PIN controls the NeoPixel strip?
#define LEDS_PIN 9

// A value to pass into a tween.
float position = 0;

// This will be used to keep track of the previous position in loop(), to check if we've moved to a new LED.
int previousPosition = 0;

Tween tween(position, 10000, NUM_LEDS);

// Set up a strip of 4-color "RGBW" LEDs.  Change the 3rd argument to better suit
// the type of LED you're using.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_RGBW);

// Pick a random color! Numbers are: Green, Red, Blue, White
uint32_t color = strip.Color(random(256), random(256), random(256), 0);

void setup() {
    strip.begin();
    tween.begin(millis());

    // Set the strip to show nothing.
    strip.clear();
    strip.show();
}
void loop() {
    uint32_t time = millis();

    // Tell the tween that time has changed and to adjust its calculations.
    tween.update(time);

    // Pick a new random color if we'ved stepped into a new LED on the Neopixel strip.
    // Because the value we're tweening is a float, we need to round it to an integer.  There isn't such a thing as the 1.5th LED, for example.
    const int roundedPosition = round(position);

    if (roundedPosition != previousPosition) {
        // Only randomize as high as 150; LEDs start to look almost completely white when the RGBs are set to much higher.    
        color = strip.Color(random(150), random(150), random(150), 0);        
    }

    // Clear clears the colors for all the LEDs in the strip.  No color means the LED remains off.
    strip.clear();
    // Turn color only the LED we want to show.
    strip.setPixelColor(position, color);
    // Show the strip, which should only be the LED we set to a color.
    strip.show();

    // Update our previous position so that we don't uneccesarilly try to change colors the next time loop() runs.
    previousPosition = position; 

    // Keep going when we've run out of LEDs to light up by returning to the start.
    if (tween.isComplete()) {
        position = 0;

        // The tween needs to be told what the new start time is, which should be "now."
        tween.restartFrom(time);
    }
}