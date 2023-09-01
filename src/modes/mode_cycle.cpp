// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Cycle
// =============================================================================

#include "strip_modes.h"
#include "hardware.h"
#include "Timer.h"

void Mode::cycle(uint16_t functionValue)
{
    static Timer delayTime;
    static uint8_t colorIndex = 0;
    const CRGB outputColor = Color::getColorValue(colorIndex);
    const uint8_t changeDelay = map(functionValue, 0, 1023, 0, 255);

    for (uint8_t i = 0; i < NUM_LEDS; i++)
        Hardware::leds[i] = outputColor;

    FastLED.show();

    // check if timer was started the first time
    if(0 == delayTime.elapsedStart())
        delayTime.start();

    if(delayTime.elapsed(changeDelay))
    {
        colorIndex++;
        delayTime.start();
    }
}