// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Colorpick
// =============================================================================

#include "strip_modes.h"
#include "../ColorUtilities/color_utilities.h"
#include "hardware.h"
#include "gpioHandler.h"
#include "Timer.h"  // Include the Timer header if it's not already included

//! Mode currently not supported!

namespace
{
    Timer audioTimer;  // Create a timer object
}

void fillDefinedColors()
{
    uint8_t index = 247;

    for (int i = NUM_LEDS - 1; i >= 0; --i) {
        Hardware::leds[i] = Color::getColorValue(index);
        index = (index + 3) % 256;  // Increase the color index by the step value
    }
}

void Mode::audio(uint16_t functionValue, uint16_t audioValue)
{
    // If the timer hasn't started yet, start it
    if (!audioTimer.elapsedStart()) {
        audioTimer.start();
    }

    // If 80ms has elapsed, update the visualization
    if (audioTimer.elapsed(20)) {

        uint16_t senseVal = map(functionValue, 0, 1023, 100, 8000);
        uint8_t audioMapped = map(audioValue, 509, 1023, 100, senseVal);  // Added sensitivity adjustment

        // Check if audioMapped exceeds the maximum value of 255
        if(audioMapped > 255) {
            audioMapped = 255;
        }

        fillDefinedColors();  // Fill the LEDs with default colors

        // Turn off LEDs beyond the audio level
        for (uint8_t i = audioMapped; i < NUM_LEDS; i++) {
            Hardware::leds[i] = CRGB::Black;
        }

        Serial.println(audioValue);  // Debugging

        audioTimer.start();  // Reset the timer for the next cycle
    }
}