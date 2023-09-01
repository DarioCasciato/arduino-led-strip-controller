// =============================================================================
// Arduino-LED-Strip-Controller | Hardware
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "gpioHandler.h"
#include "Potentiometer.h"
#include "EdgeDetection.h"


namespace Hardware
{
    uint8_t buttonState = 0;
    EdgeDetection button(&buttonState);

    Potentiometer potFunction((uint8_t) Port::PotFunction,
                              Direction::UP);

    Potentiometer potBrightness((uint8_t) Port::PotBrightness,
                                Direction::UP);

    CRGB leds[NUM_LEDS];


    void init()
    {
        FastLED.addLeds<WS2811, (uint8_t)Port::Strip, GRB>(leds, NUM_LEDS);
    }

    void updateHardware()
    {
        buttonState = GPIO::getPort((uint8_t) Port::Button);
    }
} // namespace Hardware