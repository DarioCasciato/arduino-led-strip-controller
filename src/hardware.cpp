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

    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS,
                                                (uint8_t) Port::Strip,
                                                NEO_RGB + NEO_KHZ800);


    void init()
    {
        strip.begin();
	    strip.show();
    }

    void updateHardware()
    {
        buttonState = GPIO::getPort((uint8_t) Port::Button);
    }
} // namespace Hardware