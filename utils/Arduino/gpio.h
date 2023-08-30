// =============================================================================
// Utils | GPIO
// =============================================================================

#include <stdint.h>
#include <Arduino.h>

#ifndef UTILS_GPIO_
#define UTILS_GPIO_

#include "../../src/hardware.h"


namespace GPIO
{
    /// Enumeration of digital and analog ports.
    ///
    /// This enum represents the available digital and analog ports.
    /// Digital ports are labeled as D0, D1, etc., while analog ports
    /// are labeled as A0, A1, etc.
    #ifdef ARDUINO_AVR_NANO
    enum class Port : uint8_t
    {
        D0 = 0,
        D1 = 1,
        D2 = 2,
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D9 = 9,
        D10 = 10,
        D11 = 11,
        D12 = 12,
        D13 = 13,
        D14 = 14,
        D15 = 15,
        D16 = 16,
        D17 = 17,
        D18 = 18,
        D19 = 19,
        A0 = A0,
        A1 = A1,
        A2 = A2,
        A3 = A3,
        A4 = A4,
        A5 = A5
    };
    #endif

} // namespace GPIO


#endif // UTILS_GPIO_