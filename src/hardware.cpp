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

    uint16_t audioValue = 0;

    Potentiometer potFunction(static_cast<uint8_t>(Port::PotFunction),
                              Direction::UP);

    Potentiometer potBrightness(static_cast<uint8_t>(Port::PotBrightness),
                                Direction::UP);

    CRGB leds[NUM_LEDS];


    void init()
    {
        FastLED.addLeds<WS2811, (uint8_t)Port::Strip, GRB>(leds, NUM_LEDS);
    }

    void updateHardware()
    {
        buttonState = GPIO::getPort(static_cast<uint8_t>(Port::Button));
        audioValue = GPIO::getAnalogPort(static_cast<uint16_t>(Port::MicrophoneIn));
    }
} // namespace Hardware