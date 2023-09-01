// =============================================================================
// Arduino-LED-Strip-Controller | Strip-Modes
// =============================================================================

#include <stdint.h>
#include "../ColorUtilities/color_utilities.h"

#ifndef LED_STRIP_CONTROLLER_STRIP_Modes_
#define LED_STRIP_CONTROLLER_STRIP_Modes_


namespace Mode {

    void white(uint16_t functionValue);

    void colorpick(uint16_t functionValue);

    void cycle(uint16_t functionValue);

    void fire(uint16_t functionValue);

    void rainbow(uint16_t functionValue);

} // namespace Mode


#endif // LED_STRIP_CONTROLLER_STRIP_Modes_