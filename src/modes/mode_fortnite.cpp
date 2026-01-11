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
    e_gameEnd,
    e_downed,
    e_revived,
    e_fullShield,
    e_fullHealth,
    NUM_EVENTS
};

enum Stages : uint8_t
{
    st_lobby,
    st_battleBus,
    st_freefall,
    st_inGame,
    NUM_STAGES
};

//------------------------------------------------------------------------------
// Internal values
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
    const CRGB defaultColor = CRGB(44, 77, 143);

//---------------------------------------------------

    // Battle Bus state
    Timer battleBusTimer;
    CRGB battleBusColors[NUM_LEDS];
    bool battleBusInitialized = false;

    // Freefall state
    Timer freefallTimer;
    uint8_t freefallLedBrightness[NUM_LEDS];  // Individual brightness for each LED
    bool freefallInitialized = false;
    const CRGB freefallColor = CRGB(44, 77, 143);

//---------------------------------------------------

    // Fade effect
    Timer fadeTimer;
    CRGB fadeStartColor = CRGB::Black;
    CRGB fadeEndColor = CRGB::Black;
    uint32_t fadeDuration = 0;
    bool fadeActive = false;

    // Expanding effect
    Timer expandTimer;
    CRGB expandColor = CRGB::Black;
    CRGB expandBackgroundColor = CRGB::Black;
    uint32_t expandDuration = 0;
    bool expandActive = false;

//---------------------------------------------------

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

// Event renderers
void renderEventKill();
void renderEventAssist();
void renderEventDeath();
void renderEventGameEnd();
void renderEventDowned();
void renderEventRevived();
void renderEventFullShield();
void renderEventFullHealth();

void checkSerialInput();

// Helpers
void fillStripColor(const CRGB& color);
bool fadeColor(const CRGB& startColor, const CRGB& endColor, uint32_t durationMs, bool forceRestart = false);

void logMsg(const char* msg)
{
#if DEBUG_SERIAL
    Serial.println(msg);
#endif
}


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
    if (Serial.available() == 0) return;

    uint8_t command = Serial.read();

    if (command == 'S')
    {
        logMsg("CMD S received");

        // WARTEN bis zweites Byte da ist
        unsigned long startTime = millis();
        while (Serial.available() < 1 && (millis() - startTime) < 100) {}

        if (Serial.available() == 0)
        {
            logMsg("CMD S missing stage byte");
            return;
        }

        uint8_t stageValue = Serial.read();
        if (stageValue >= NUM_STAGES)
        {
            logMsg("CMD S invalid stage");
            return;
        }

        currentStage = static_cast<Stages>(stageValue);

        Serial.write('A');
        Serial.write('S');
        Serial.write(stageValue);

        logMsg("Stage set");
        return;
    }

    if (command == 'E')
    {
        logMsg("CMD E received");

        // WARTEN bis zweites Byte da ist (HIER WAR DER FEHLER!)
        unsigned long startTime = millis();
        while (Serial.available() < 1 && (millis() - startTime) < 100) {}

        if (Serial.available() == 0)
        {
            logMsg("CMD E missing event byte");
            return;
        }

        uint8_t eventValue = Serial.read();
        if (eventValue >= NUM_EVENTS)
        {
            logMsg("CMD E invalid event");
            return;
        }

        activeEvent = static_cast<Events>(eventValue);
        eventActive = true;
        eventTimer.start();
        eventAnimationStep = 0;
        eventBrightness = 0;

        logMsg("Event triggered");
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

        case e_gameEnd:
            renderEventGameEnd();
            break;

        case e_downed:
            renderEventDowned();
            break;

        case e_revived:
            renderEventRevived();
            break;

        case e_fullShield:
            renderEventFullShield();
            break;

        case e_fullHealth:
            renderEventFullHealth();
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
    // Initialize or reset every 500ms
    if (!battleBusInitialized || battleBusTimer.elapsed(500))
    {
        battleBusTimer.start();
        battleBusInitialized = true;

        // Generate random colors for each LED
        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            // 20% chance for yellow, 80% for blue tones
            if (random(100) < 20)
            {
                // Yellow
                battleBusColors[i] = CRGB(255, 255, 0);
            }
            else
            {
                // Random blue tones
                uint8_t blueShade = random(150, 255);
                battleBusColors[i] = CRGB(0, random(50, 150), blueShade);
            }
        }
    }

    // Calculate brightness fade (from 100% to 0% over 500ms)
    uint32_t elapsed = battleBusTimer.elapsedStart();
    float progress = (float)elapsed / 500.0f;  // 0.0 to 1.0
    uint8_t brightness = 255 - (uint8_t)(255 * progress);  // 255 to 0

    // Apply colors with fading brightness
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        Hardware::leds[i] = battleBusColors[i];
        Hardware::leds[i].nscale8(brightness);  // Scale brightness
    }
}

void renderFreefall(uint16_t functionValue)
{
    // Initialize brightness array
    if (!freefallInitialized)
    {
        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            freefallLedBrightness[i] = random(50, 150);  // Random starting brightness
        }
        freefallInitialized = true;
        freefallTimer.start();
    }

    // Every 100ms, update sparkles (slower than battlebus)
    if (freefallTimer.elapsed(100))
    {
        freefallTimer.start();

        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            // Random chance to trigger new sparkle (10% chance)
            if (random(100) < 10 && freefallLedBrightness[i] < 200)
            {
                freefallLedBrightness[i] = 255;  // Full brightness!
            }
            else
            {
                // Fade down slowly
                if (freefallLedBrightness[i] > 30)
                {
                    freefallLedBrightness[i] -= random(3, 8);  // Slower fade
                }
                else
                {
                    freefallLedBrightness[i] = random(20, 40);  // Keep minimal brightness
                }
            }
        }
    }

    // Apply color with individual brightness
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        Hardware::leds[i] = freefallColor;
        Hardware::leds[i].nscale8(freefallLedBrightness[i]);
    }
}

void renderInGame(uint16_t functionValue)
{
    fillStripColor(defaultColor);
}

//------------------------------------------------------------------------------
// Event Renderers
//------------------------------------------------------------------------------


void renderEventKill()
{
    // Phase 1: Fade to green (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::Green, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;  // Next phase
        }
    }
    // Phase 2: Fade back (2000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::Green, defaultColor, 2000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventAssist()
{
    // Phase 1: Fade to lightgreen (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::LightGreen, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;  // Next phase
        }
    }
    // Phase 2: Fade back (1000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::LightGreen, defaultColor, 1000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventDeath() // only in team mode
{
    // Phase 1: Fade to red (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::Red, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;  // Next phase
        }
    }
    // Phase 2: Fade back (2000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::Red, defaultColor, 2000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventGameEnd()
{
    // Phase 1: Expand red from center (2000ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = expandFromCenter(CRGB::Red, CRGB::Black, 2000, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;
        }
    }
    // Phase 2: Fade to default color (4000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::Red, defaultColor, 4000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventDowned()
{
    // Phase 1: Fade to yellow (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::Yellow, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;
        }
    }
    // Phase 2: Fade back to default (1000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::Yellow, defaultColor, 1000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventRevived()
{
    // Phase 1: Expand light blue from center (2000ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = expandFromCenter(CRGB::LightBlue, CRGB::Black, 2000, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;
        }
    }
    // Phase 2: Fade to default color (4000ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::LightBlue, defaultColor, 4000, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventFullShield()
{
    // Phase 1: Fade to light blue (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::LightBlue, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;
        }
    }
    // Phase 2: Fade back to default (500ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::LightBlue, defaultColor, 500, true);
        if (phase2Done)
        {
            eventActive = false;
        }
    }
}

void renderEventFullHealth()
{
    // Phase 1: Fade to green (250ms)
    if (eventAnimationStep == 0)
    {
        bool phase1Done = fadeColor(defaultColor, CRGB::Green, 250, true);
        if (phase1Done)
        {
            eventAnimationStep = 1;  // Next phase
        }
    }
    // Phase 2: Fade back (500ms)
    else if (eventAnimationStep == 1)
    {
        bool phase2Done = fadeColor(CRGB::Green, defaultColor, 500, true);
        if (phase2Done)
        {
            eventActive = false;
        }
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


// Fade between two colors over time (non-blocking)
// Returns: true if fade is complete, false if still fading
// forceRestart: if true, interrupts current fade and starts new one
bool fadeColor(const CRGB& startColor, const CRGB& endColor, uint32_t durationMs, bool forceRestart = false)
{
    // Check if parameters changed (new fade requested while one is running)
    bool parametersChanged = (startColor != fadeStartColor ||
                              endColor != fadeEndColor ||
                              durationMs != fadeDuration);

    // Start new fade if: not active, force restart, or parameters changed
    if (!fadeActive || forceRestart || parametersChanged)
    {
        fadeStartColor = startColor;
        fadeEndColor = endColor;
        fadeDuration = durationMs;
        fadeActive = true;
        fadeTimer.start();
    }

    uint32_t elapsed = fadeTimer.elapsedStart();

    // Fade complete?
    if (elapsed >= fadeDuration)
    {
        fillStripColor(fadeEndColor);
        fadeActive = false;
        fadeTimer.stop();
        return true;  // Done!
    }

    // Calculate progress (0.0 to 1.0)
    float progress = (float)elapsed / (float)fadeDuration;

    // Linear interpolation between colors
    uint8_t r = fadeStartColor.r + (fadeEndColor.r - fadeStartColor.r) * progress;
    uint8_t g = fadeStartColor.g + (fadeEndColor.g - fadeStartColor.g) * progress;
    uint8_t b = fadeStartColor.b + (fadeEndColor.b - fadeStartColor.b) * progress;

    CRGB currentColor = CRGB(r, g, b);
    fillStripColor(currentColor);

    return false;  // Still fading
}



// Expand a color from center to edges
// Returns: true if expansion is complete, false if still expanding
bool expandFromCenter(const CRGB& color, const CRGB& backgroundColor, uint32_t durationMs, bool forceRestart = false)
{
    // Check if parameters changed
    bool parametersChanged = (color != expandColor ||
                              backgroundColor != expandBackgroundColor ||
                              durationMs != expandDuration);

    // Start new expansion if needed
    if (!expandActive || forceRestart || parametersChanged)
    {
        expandColor = color;
        expandBackgroundColor = backgroundColor;
        expandDuration = durationMs;
        expandActive = true;
        expandTimer.start();
    }

    uint32_t elapsed = expandTimer.elapsedStart();

    // Expansion complete?
    if (elapsed >= expandDuration)
    {
        fillStripColor(expandColor);
        expandActive = false;
        expandTimer.stop();
        return true;  // Done!
    }

    // Calculate progress (0.0 to 1.0)
    float progress = (float)elapsed / (float)expandDuration;

    // Calculate how many LEDs from center should be lit
    uint8_t center = NUM_LEDS / 2;
    uint8_t radius = (uint8_t)(center * progress);

    // Fill strip with background color first
    fillStripColor(expandBackgroundColor);

    // Light up LEDs expanding from center
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        int8_t distanceFromCenter = abs((int8_t)i - (int8_t)center);

        if (distanceFromCenter <= radius)
        {
            Hardware::leds[i] = expandColor;
        }
    }

    return false;  // Still expanding
}