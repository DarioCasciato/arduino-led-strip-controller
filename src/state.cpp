// =============================================================================
// Template-Project | State
// =============================================================================

#include "state.h"
#include "hardware.h"
#include "configurations.h"
#include "Timer.h"

Timer counter;

//------------------------------------------------------------------------------

namespace State
{
    States state = States::st_idle;

    void stateDriver()
    {
        switch (State::state)
        {
        case State::st_idle: stateIdle(); break;
        case State::st_error: stateError(); break;

        default:    // catch invalid state (implement safety backup)
        goto exception;
            break;
        }

        return;

        exception:
            for(;;) {}
    }

    // State implementations
    void stateIdle()
    {

    }

    void stateError()
    {

    }
} // namespace State

//------------------------------------------------------------------------------

// Other Functions