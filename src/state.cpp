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

            case State::st_rainbow:
                Mode::rainbow(functionPotValue);
                break;

            default:
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

        FastLED.show();
        FastLED.setBrightness(brightnessValue);


        return;


        exception:
            for(;;) {}
    }

//------------------------------------------------------------------------------

    void buttonHandler()
    {
        // If the button is pressed, start the buttonPress timer
        if(Hardware::button.getEdgePos())
            buttonPress.start();

        // If the button is released and the buttonPress timer has been running
        // for longer than the OFF_TIMER_START_DURATION, start the shutdown
        // sequence
        if(Hardware::button.getEdgeNeg())
        {
            if(buttonPress.elapsed(OFF_TIMER_START_DURATION))
            {
                shutdown.start();
            }
            // If the button was pressed and released quickly, cycle through the
            // different states of the program
            else
            {
                state = static_cast<States>((static_cast<uint8_t>(state) + 1) %
                        static_cast<uint8_t>(States::NUM_STATES));
            }

            // Stop the buttonPress timer
            buttonPress.stop();
        }
    }

    void timerStartSequence()
    {
        // Get the current brightness from the potentiometer
        const uint8_t currentBrightness = map(Hardware::potBrightness.get(),
                                              0, 1024, 1, 255);

        // Fade out to black
        for(uint8_t i = currentBrightness; i > 0; i--)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(1);
        }
        // Fade back in
        for(uint8_t i = 0; i < currentBrightness; i++)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(1);
        }

        // Start the shutdown timer
        shutdown.start();
    }

    void shutdownSequence()
    {
        // Get the current brightness from the potentiometer
        const uint8_t currentBrightness = map(Hardware::potBrightness.get(),
                                              0, 1024, 1, 255);

        // Fade out to black
        for(uint8_t i = currentBrightness; i > 0; i--)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(200);

            // Check for button presses while fading
            Hardware::updateHardware();
            EdgeDetection::updateEdges();

            // If the button is pressed, stop the shutdown timer
            if(Hardware::button.getEdgePos())
                break;
        }
    }
} // namespace State

//------------------------------------------------------------------------------

// Other Functions