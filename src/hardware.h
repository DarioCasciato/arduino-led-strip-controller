// =============================================================================
// Arduino-LED-Strip-Controller | Hardware
// =============================================================================

#ifndef _LED_STRIP_CONTROLLER_HARDWARE_
#define _LED_STRIP_CONTROLLER_HARDWARE_

#include "EdgeDetection.h"
#include "Potentiometer.h"
#include "FastLED.h"
#include "configurations.h"
#include "gpio.h"


namespace Hardware
{
    // Example entries
    enum class Port : uint8_t
    {
        PotFunction         = static_cast<uint8_t>(GPIO::Port::A3),
        PotBrightness       = static_cast<uint8_t>(GPIO::Port::A4),
        MicrophoneIn        = static_cast<uint8_t>(GPIO::Port::A6),
        Button              = static_cast<uint8_t>(GPIO::Port::D2),
        Strip               = static_cast<uint8_t>(GPIO::Port::D4)
    };


    extern uint8_t buttonState;
    extern EdgeDetection button;

    extern uint16_t audioValue;

    extern Potentiometer potFunction;
    extern Potentiometer potBrightness;

    extern CRGB leds[NUM_LEDS];


    /// @brief Initializes hardware
    void init();

    /// @brief Fetches hardware values
    void updateHardware();
} // namespace Hardware


#endif // _LED_STRIP_CONTROLLER_HARDWARE_