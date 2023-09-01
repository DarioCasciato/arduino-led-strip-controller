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

    Timer buttonPress;
    Timer shutdown;


    void stateDriver()
    {
        uint16_t functionPotValue = Hardware::potFunction.get();
        uint16_t brightnessValue = map(Hardware::potBrightness.get(),
                                       0, 1024, 1, 255);

        buttonHandler();


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

        if(shutdown.elapsed(SHUTDOWN_TIME))
        {
            shutdownSequence();
            shutdown.stop();

            while(true)
            {
                Hardware::updateHardware();
                EdgeDetection::updateEdges();

                if(Hardware::button.getEdgePos())
                    break;
            }
        }

        FastLED.setBrightness(brightnessValue);


        return;


        exception:
            for(;;) {}
    }

//------------------------------------------------------------------------------

    void buttonHandler()
    {
        if(Hardware::button.getEdgePos())
            buttonPress.start();

        if(Hardware::button.getEdgeNeg())
        {
            if(buttonPress.elapsed(OFF_TIMER_START_DURATION))
            {
                timerStartSequence();
                shutdown.start();
            }
            else
                state = static_cast<States>((static_cast<uint8_t>(state) + 1) %
                        static_cast<uint8_t>(States::NUM_STATES));

            buttonPress.stop();
        }
    }


    void timerStartSequence()
    {
        const uint8_t currentBrightness = map(Hardware::potBrightness.get(),
                                              0, 1024, 1, 255);

        for(uint8_t i = currentBrightness; i > 0; i--)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(3);
        }
        for(uint8_t i = currentBrightness; i < currentBrightness; i++)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(3);
        }
    }


    void shutdownSequence()
    {
        const uint8_t currentBrightness = map(Hardware::potBrightness.get(),
                                              0, 1024, 1, 255);

        for(uint8_t i = currentBrightness; i > 0; i--)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(50);
        }
    }

} // namespace State

//------------------------------------------------------------------------------

// Other Functions