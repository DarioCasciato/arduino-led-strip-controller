// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Colorpick
// =============================================================================

#include "strip_modes.h"
#include "../ColorUtilities/color_utilities.h"
#include "hardware.h"
#include "gpioHandler.h"
#include "Timer.h"

namespace
{
    Timer audioTimer;  // Utilize a timer object for managing update intervals
}

void fillDefinedColors()
{
    uint8_t index = 247;

    for (int i = NUM_LEDS - 1; i >= 0; --i)
    {
        Hardware::leds[i] = Color::getColorValue(index);
        index = (index + 3) % 256;  // Increment the color index
    }
}

void Mode::audio(uint16_t functionValue, uint16_t audioValue)
{
    // Start the timer if it hasn't been started
    if (!audioTimer.elapsedStart())
        audioTimer.start();

    // Update the LEDs every 20ms
    if (audioTimer.elapsed(20))
    {
        // Map functionValue to adjust the sensitivity range
        uint16_t senseVal = map(functionValue, 0, 1023, 100, 8000);
        // Map audioValue to LED indices, incorporating sensitivity
        uint8_t audioMapped = map(audioValue, 509, 1023, 0, NUM_LEDS * (senseVal / 8000.0));  // Adjust the mapping as needed

        // Ensure audioMapped does not exceed the number of LEDs
        audioMapped = min(audioMapped, static_cast<uint8_t>(NUM_LEDS));

        fillDefinedColors();  // Fill the strip with predefined colors

        // Turn off LEDs that are beyond the current audio level
        for (uint8_t i = audioMapped; i < NUM_LEDS; i++)
        {
            Hardware::leds[i] = CRGB::Black;
        }

        // Debug: Output the raw audio value
        Serial.println(audioValue);

        // Reset the timer for the next update cycle
        audioTimer.start();
    }
}