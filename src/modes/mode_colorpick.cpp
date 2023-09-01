// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Colorpick
// =============================================================================

#include "strip_modes.h"
#include "../ColorUtilities/color_utilities.h"
#include "hardware.h"

void Mode::colorpick(uint16_t functionValue)
{
    const uint8_t colorIndex = map(functionValue, 0, 1024, 0, 255);
    const CRGB outputColor = Color::getColorValue(colorIndex);

    for (uint8_t i = 0; i < NUM_LEDS; i++)
        Hardware::leds[i] = outputColor;

    FastLED.show();
}