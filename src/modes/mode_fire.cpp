// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Fire
// =============================================================================

#include "strip_modes.h"
#include "hardware.h"
#include "FastLED.h"
#include "Timer.h"

namespace
{
    Timer fireEffectTimer;
    Timer pixelUpdateTimer;
    uint32_t fireEffectInterval = random(40, 125);
    uint32_t pixelUpdateInterval = random(40, 125);
}


void fireMode(uint16_t functionValue)
{
    // You might use functionValue for some parameterization, if needed

    // Initialize color values for the fire effect
    int r = 255;
    int g = 96;
    int b = 12;

    // Check if it's time to update the fire effect
    if(fireEffectTimer.elapsed(fireEffectInterval)) {
        // Generate a new random interval for the fire effect
        fireEffectInterval = random(40, 125);

        for(int x = 0; x < NUM_LEDS; x++) {
            // If it's time to update this particular pixel
            if(pixelUpdateTimer.elapsed(pixelUpdateInterval)) {
                // Generate a new random interval for the pixel update
                pixelUpdateInterval = random(40, 125);

                int flicker = random(8, 40);
                int r1 = r - flicker;
                int g1 = g - flicker;
                int b1 = b - flicker;

                if (g1 < 0) g1 = 0;
                if (r1 < 0) r1 = 0;
                if (b1 < 0) b1 = 0;

                // Set the pixel color
                Hardware::leds[x] = CRGB(r1, g1, b1);

                // Reset the pixel update timer for the next iteration
                pixelUpdateTimer.start();
            }
        }

        // Push the new color data to the strip
        FastLED.show();

        // Reset the fire effect timer for the next iteration
        fireEffectTimer.start();
    }
}