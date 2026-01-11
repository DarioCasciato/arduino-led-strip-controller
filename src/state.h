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
        st_rainbow,
        //st_audio,
        st_fortnite,
        NUM_STATES
    };

    /// @brief Get mode activation/deactivation commands from serial
    void checkForModeCommands();

    /// @brief button handler for state change, and timer
    void buttonHandler();

    /// @brief Sequence to illustrate the starting of timer
    void timerStartSequence();

    /// @brief Sequence to illustrate the exiting of fortnite mode
    void exitFortnite();

    /// @brief Sequence is called when timer hit timer
    void shutdownSequence();

    /// @brief State driver function
    void stateDriver();
} // namespace State

#endif // _LED_STRIP_CONTROLLER_STATE_