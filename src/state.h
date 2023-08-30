// =============================================================================
// Arduino-LED-Strip-Controller | State
// =============================================================================

#ifndef _LED_STRIP_CONTROLLER_STATE_
#define _LED_STRIP_CONTROLLER_STATE_

#include "hardware.h"

namespace State
{
    enum States : uint8_t
    {
        st_white,
        st_colorpick,
        st_cycle,
        st_fire,
        NUM_STATES
    };

    /// @brief change state handler
    void changeState();

    /// @brief State driver function
    void stateDriver();
} // namespace State

#endif // _LED_STRIP_CONTROLLER_STATE_