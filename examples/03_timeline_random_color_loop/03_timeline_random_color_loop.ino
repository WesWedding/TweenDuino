/**
 * Animate an LED in one random color, followed by the
 * same animation in another random color, followed by 
 * the same animation in another color, followed by...
*/
#include <TweenDuino.h>
#include <Adafruit_NeoPixel.h>

// How many LEDs are on the NeoPixel strip?  Change this as appropriate to your setup.
#define NUM_LEDS 7
// Which PIN controls the NeoPixel strip?  Change this as appropriate to your setup.
#define LEDS_PIN 9

// This will be the brightness of our colors.  0.0 = no brightess, 1.0 = full brightness.
float brightness = 0.0;

// This will be our animation timeline, used to change the brightness variable.
TweenDuino::Timeline timeline;

// Set up a strip of 4-color "RGBW" LEDs.  Change the 3rd argument to better suit
// the type of LED you're using.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_RGBW);

// Our 3 RGB color components, which will be randomized after timelines finish.
int red = random(256);
int blue = random(256);
int green = random(256);
int white = 0; // White LEDs on the NeoPixels really overwhelm the other colors.

// Pick a random color! Numbers are: Green, Red, Blue, White
uint32_t color = strip.Color(green * brightness, red * brightness, blue * brightness, white * brightness);

void setup() {
    Serial.begin(115200);
    strip.begin();

    
    // Add a series of tweens to the timeline.  Tween from 0 brightness to max brightness,
    // hold for a moment, and then tween back down.
    timeline.addTo(brightness, 1.0, 2500);
    timeline.addTo(brightness, 1.0, 1000);
    timeline.addTo(brightness, 0.0, 2500);
    
    timeline.begin(millis());

    // Set the strip to show nothing.
    strip.clear();
    strip.show();
}
void loop() {
    uint32_t time = millis();

    // Tell the tween that time has changed and to adjust its calculations.
    timeline.update(time);
    
    // Update our color to reflect brightness changes after the timeline update.
    color = strip.Color(green * brightness, red * brightness, blue * brightness, white * brightness);

    // Pick new random RGB values if our timeline has finished, for our next loop.
    if (timeline.isComplete()) {
      Serial.println("tl complete");
      red = random(256);
      blue = random(25);
      green = random(256);

      // Restart the timeline while we're at it!
      timeline.restartFrom(time);
    }

    strip.clear();

    for(uint16_t i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
}