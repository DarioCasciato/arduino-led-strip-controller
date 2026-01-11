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
    States previousState = States::st_white;  // Stores state before Fortnite mode
    bool fortniteAutoActivated = false;        // Track if Fortnite was auto-activated

    Timer buttonPress;
    Timer shutdown;


    void stateDriver()
    {
        uint16_t functionPotValue = Hardware::potFunction.get();
        uint16_t brightnessValue = map(Hardware::potBrightness.get(),
                                       0, 1024, 1, 255);

        checkForModeCommands();
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

            case State::st_fortnite:
                Mode::fortnite(functionPotValue);
                break;

            default:
            goto exception;
                break;
        }

        if(shutdown.elapsed(SHUTDOWN_TIME))
        {
            shutdownSequence();

            FastLED.clear();
            FastLED.show();

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

    void logMsg(const char* msg)
    {
    #if DEBUG_SERIAL
        Serial.println(msg);
    #endif
    }


    void checkForModeCommands()
    {
        if (Serial.available() == 0) return;
        if (Serial.peek() != 'M') return;

        Serial.read();
        logMsg("CMD M received");

        unsigned long startTime = millis();
        while (Serial.available() < 1 && (millis() - startTime) < 100) {}

        if (Serial.available() == 0)
        {
            logMsg("CMD M timeout");
            return;
        }

        uint8_t modeValue = Serial.read();

        if (modeValue == 1)
        {
            if (state == States::st_fortnite)
            {
                logMsg("Fortnite mode already active");
                return;
            }

            previousState = state;
            state = States::st_fortnite;
            fortniteAutoActivated = true;
            logMsg("Fortnite mode activated");
            return;
        }

        if (modeValue == 0)
        {
            if (state != States::st_fortnite || !fortniteAutoActivated)
            {
                logMsg("Fortnite mode not active or not auto");
                return;
            }

            state = previousState;
            fortniteAutoActivated = false;
            logMsg("Fortnite mode deactivated");
        }
    }





    void buttonHandler()
    {
        if(state == States::st_fortnite)
            return;

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
                timerStartSequence();
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
            delay(3);
        }
        // Fade back in
        for(uint8_t i = 0; i < currentBrightness; i++)
        {
            FastLED.setBrightness(i);
            FastLED.show();
            delay(3);
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
