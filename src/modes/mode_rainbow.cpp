// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Rainbow
// =============================================================================

#include "FastLED.h"
#include "Timer.h"
#include "strip_modes.h"
#include "hardware.h"

namespace
{
    Timer rainbowTimer;
    uint32_t lastUpdate = 0;
    uint8_t currentColorIndex = 0;
}

void Mode::rainbow(uint16_t functionValue)
{
    // Map the function value to delay time
    const uint8_t delayTime = map(functionValue, 0, 1023, 0, 255);

    // Check if it's time to update the rainbow
    if(rainbowTimer.elapsed(delayTime)) {

        // Increment the color index
        currentColorIndex = (currentColorIndex + 1) % 256;

        for(int i = 0; i < NUM_LEDS; i+=2) {
            // Calculate color for the current pixel based on the color index
            CRGB color = Color::getColorValue((currentColorIndex + i) % 256);

            // Set color for two adjacent pixels
            Hardware::leds[i] = color;
            if (i + 1 < NUM_LEDS) {
                Hardware::leds[i + 1] = color;
            }
        }

        // Push the new color data to the strip
        FastLED.show();

        // Reset the timer
        rainbowTimer.start();
    }
}
