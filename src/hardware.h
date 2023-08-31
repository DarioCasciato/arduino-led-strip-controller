// =============================================================================
// Arduino-LED-Strip-Controller | Hardware
// =============================================================================

#ifndef _LED_STRIP_CONTROLLER_HARDWARE_
#define _LED_STRIP_CONTROLLER_HARDWARE_

#include "EdgeDetection.h"
#include "Potentiometer.h"
#include "Adafruit_NeoPixel.h"
#include "configurations.h"
#include "gpio.h"


namespace Hardware
{
    // Example entries
    enum class Port : uint8_t
    {
        PotFunction         = (uint8_t) GPIO::Port::A3,
        PotBrightness       = (uint8_t) GPIO::Port::A4,
        MicrophoneIn        = (uint8_t) GPIO::Port::A5,
        Button              = (uint8_t) GPIO::Port::D2,
        Strip               = (uint8_t) GPIO::Port::D4
    };


    extern uint8_t buttonState;
    extern EdgeDetection button;

    extern Potentiometer potFunction;
    extern Potentiometer potBrightness;

    extern Adafruit_NeoPixel strip;


    /// @brief Initializes hardware
    void init();

    /// @brief Fetches hardware values
    void updateHardware();
} // namespace Hardware


#endif // _LED_STRIP_CONTROLLER_HARDWARE_