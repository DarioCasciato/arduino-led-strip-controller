// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Fortnite
// =============================================================================

#include "strip_modes.h"
#include "../ColorUtilities/color_utilities.h"
#include "hardware.h"
#include "Timer.h"
#include <FastLED.h>

//------------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------------

enum Events : uint8_t
{
    e_kill,
    e_assist,
    e_death,
    e_downed,
    e_revived,
    NUM_EVENTS
};

enum Stages : uint8_t
{
    st_lobby,
    st_battleBus,
    st_freefall,
    st_inGame,
    st_inStorm,
    NUM_STAGES
};

//------------------------------------------------------------------------------
// Internal State
//------------------------------------------------------------------------------

namespace
{
    // Current stage
    Stages currentStage = st_lobby;

    // Event handling
    bool eventActive = false;
    Events activeEvent;
    Timer eventTimer;

    // Event animation variables
    uint16_t eventAnimationStep = 0;
    uint8_t eventBrightness = 0;

    // Defaults
    const CRGB defaultColor = CRGB(77, 40, 143);
}

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------

// Processing functions
void processStage(uint16_t functionValue);
void processEvent();

// Stage renderers
void renderLobby(uint16_t functionValue);
void renderBattleBus(uint16_t functionValue);
void renderFreefall(uint16_t functionValue);
void renderInGame(uint16_t functionValue);
void renderInStorm(uint16_t functionValue);

// Event renderers
void renderEventInStorm();
void renderEventKill();
void renderEventAssist();
void renderEventDeath();
void renderEventDowned();
void renderEventRevived();
void checkSerialInput();

// Helpers
void fillStripColor(const CRGB& color);

//------------------------------------------------------------------------------
// Main Mode Function
//------------------------------------------------------------------------------

void Mode::fortnite(uint16_t functionValue)
{
    // Check for serial commands from PC
    checkSerialInput();

    // If an event is active, display event animation
    if (eventActive)
    {
        processEvent();
    }
    else
    {
        processStage(functionValue);
    }
}

//------------------------------------------------------------------------------
// Serial Communication Handler
//------------------------------------------------------------------------------

void checkSerialInput()
{
    if (Serial.available() == 0)
    {
        return;
    }

    uint8_t command = Serial.read();

    // 'S' + stage_byte
    if (command == 'S')
    {
        if (Serial.available() == 0)
        {
            return;
        }

        uint8_t stageValue = Serial.read();
        if (stageValue >= NUM_STAGES)
        {
            return;
        }

        currentStage = static_cast<Stages>(stageValue);

        Serial.write('A');
        Serial.write('S');
        Serial.write(stageValue);
        return;
    }

    // 'E' + event_byte
    if (command == 'E')
    {
        if (Serial.available() == 0)
        {
            return;
        }

        uint8_t eventValue = Serial.read();
        if (eventValue >= NUM_EVENTS)
        {
            return;
        }

        activeEvent = static_cast<Events>(eventValue);
        eventActive = true;
        eventTimer.start();
        eventAnimationStep = 0;
        eventBrightness = 0;
    }
}


//------------------------------------------------------------------------------
// Stage Processing
//------------------------------------------------------------------------------

void processStage(uint16_t functionValue)
{
    switch (currentStage)
    {
        case st_lobby:
            renderLobby(functionValue);
            break;

        case st_battleBus:
            renderBattleBus(functionValue);
            break;

        case st_freefall:
            renderFreefall(functionValue);
            break;

        case st_inGame:
            renderInGame(functionValue);
            break;

        default:
            // Fallback: default color
            fillStripColor(defaultColor);
            break;
    }
}

//------------------------------------------------------------------------------
// Event Processing
//------------------------------------------------------------------------------

void processEvent()
{
    switch (activeEvent)
    {
        case e_kill:
            renderEventKill();
            break;

        case e_assist:
            renderEventAssist();
            break;

        case e_death:
            renderEventDeath();
            break;

        case e_downed:
            renderEventDowned();
            break;

        case e_revived:
            renderEventRevived();
            break;

        default:
            break;
    }

    eventAnimationStep++;
}

//------------------------------------------------------------------------------
// Stage Renderers
//------------------------------------------------------------------------------

void renderLobby(uint16_t functionValue)
{
    fillStripColor(defaultColor);
}

void renderBattleBus(uint16_t functionValue)
{
    fillStripColor(CRGB(0, 102, 204)); // Blue color
}

void renderFreefall(uint16_t functionValue)
{
    //grey color
    fillStripColor(CRGB(128, 128, 128));
}

void renderInGame(uint16_t functionValue)
{
    fillStripColor(CRGB(0, 153, 76)); // Green color
}

void renderInStorm()
{
    // Fill strip with storm color (dark purple)
    fillStripColor(CRGB(51, 0, 102));
}

//------------------------------------------------------------------------------
// Event Renderers
//------------------------------------------------------------------------------



void renderEventKill()
{
    // Flash red on kill
    fillStripColor(CRGB(255, 0, 0));

    // End event after short duration
    if (eventTimer.elapsed(500)) // 500 ms
    {
        eventActive = false;
    }
}

void renderEventAssist()
{
    // Flash yellow on assist
    fillStripColor(CRGB(255, 255, 0));

    // End event after short duration
    if (eventTimer.elapsed(500)) // 500 ms
    {
        eventActive = false;
    }
}

void renderEventDeath()
{
    // Flash black on death
    fillStripColor(CRGB(0, 0, 0));

    // End event after short duration
    if (eventTimer.elapsed(1000)) // 1000 ms
    {
        eventActive = false;
    }
}

void renderEventDowned()
{
    // Flash blue on downed
    fillStripColor(CRGB(0, 0, 255));

    // End event after short duration
    if (eventTimer.elapsed(700)) // 700 ms
    {
        eventActive = false;
    }
}

void renderEventRevived()
{
    // Flash green on revived
    fillStripColor(CRGB(0, 255, 0));

    // End event after short duration
    if (eventTimer.elapsed(700)) // 700 ms
    {
        eventActive = false;
    }
}

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------

// Fill entire strip with a color
void fillStripColor(const CRGB& color)
{
    for (uint8_t i = 0; i < NUM_LEDS; i++)
        Hardware::leds[i] = color;
}