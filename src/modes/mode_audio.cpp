// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Colorpick
// =============================================================================

#include "strip_modes.h"
#include "../ColorUtilities/color_utilities.h"
#include "hardware.h"
#include "gpioHandler.h"


void fillDefinedColors()
{
    // Fill 1/6 with blue
    for (uint8_t i = 0; i < (NUM_LEDS / 6); i++)
        Hardware::leds[i] = CRGB::DeepSkyBlue;

    // Fill next 3/6 with green
    for (uint8_t i = (NUM_LEDS / 6); i < (NUM_LEDS / 2) + (NUM_LEDS / 6); i++)
        Hardware::leds[i] = CRGB::LimeGreen;

    // Fill next 1/6 with yellow
    for (uint8_t i = (NUM_LEDS / 2) + (NUM_LEDS / 6); i < 5 * (NUM_LEDS / 6); i++)
        Hardware::leds[i] = CRGB::Yellow;

    // Fill remaining 1/6 with red
    for (uint8_t i = 5 * (NUM_LEDS / 6); i < NUM_LEDS; i++)
        Hardware::leds[i] = CRGB::Red;
}


void Mode::audio(uint16_t functionValue)
{
    const uint8_t audioValue = map(functionValue, 0, 1024, 0, 255);

    fillDefinedColors();

    for (uint8_t i = audioValue; i < NUM_LEDS; i++)
        Hardware::leds[i] = CRGB::Black;

    FastLED.show();
}