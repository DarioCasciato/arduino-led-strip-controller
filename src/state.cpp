// =============================================================================
// Arduino-LED-Strip-Controller | State
// =============================================================================

#include "state.h"
#include "hardware.h"
#include "configurations.h"
#include "Timer.h"
#include "modes/strip_modes.h"

//------------------------------------------------------------------------------

namespace State
{
    States state = States::st_white;

    void stateDriver()
    {
        uint16_t functionPotValue = Hardware::potFunction.get();
        uint16_t brightnessValue = map(Hardware::potBrightness.get(), 0, 1024, 1, 255);

        if(Hardware::button.getEdgePos())
            changeState();


        switch (State::state)
        {
            case State::st_white:
                Mode::white(functionPotValue);
                break;

            case State::st_colorpick:
                Mode::colorpick(functionPotValue);
                break;

            case State::st_cycle:
                Mode::cycle(functionPotValue);
                break;

            case State::st_fire:
                Mode::fire(functionPotValue);
                break;


            default:    // catch invalid state (implement safety backup)
            goto exception;
                break;
        }

        Hardware::strip.setBrightness(brightnessValue);

        return;


        exception:
            for(;;) {}
    }


    void changeState()
    {
        state = static_cast<States>((static_cast<uint8_t>(state) + 1) % static_cast<uint8_t>(States::NUM_STATES));
    }

} // namespace State

//------------------------------------------------------------------------------

// Other Functions