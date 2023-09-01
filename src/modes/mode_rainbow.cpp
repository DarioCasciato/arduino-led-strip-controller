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
    uint8_t currentColorIndex = 0;
}

void Mode::rainbow(const uint16_t functionValue)
{
    const uint8_t delayTime = map(functionValue, 0, 1023, 100, 0);
    const uint8_t step = 3;  // Increment color by this step value for each LED

    // Ensure the timer has started
    if (!rainbowTimer.elapsedStart()) {
        rainbowTimer.start();
    }

    // Time to update the strip?
    if (rainbowTimer.elapsed(delayTime)) {
        currentColorIndex = (currentColorIndex + 1) % 256;  // Increment base color index

        uint8_t index = currentColorIndex;  // Starting color index

        // Reverse the loop direction
        for (int i = NUM_LEDS - 1; i >= 0; --i) {
            Hardware::leds[i] = Color::getColorValue(index);
            index = (index + step) % 256;  // Increase the color index by the step value
        }

        rainbowTimer.start();  // Reset the timer
    }

}