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
    e_win,
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
void renderEventWin();

void checkSerialInput();

// Helpers
void fillStripColor(const CRGB& color);
bool fadeColor(const CRGB& startColor, const CRGB& endColor, uint32_t durationMs, bool forceRestart = false);
bool expandFromCenter(const CRGB& color, const CRGB& backgroundColor, uint32_t durationMs, bool forceRestart = false);

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

        case e_win:
            renderEventWin();
            break;

        default:
            break;
    }
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

    // Calculate brightness fade (from 100% to 50% over 500ms)
    uint32_t elapsed = battleBusTimer.elapsedStart();
    float progress = (float)elapsed / 500.0f;  // 0.0 to 1.0
    if (progress > 1.0f) progress = 1.0f; // clamp
    const uint8_t startBrightness = 255;
    const uint8_t endBrightness = 70; // ~50%
    uint8_t brightness = startBrightness - (uint8_t)((startBrightness - endBrightness) * progress);  // 255 to 128

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
    if (freefallTimer.elapsed(50))
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
        if(fadeColor(defaultColor, CRGB::Green, 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back (4000ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB::Green, defaultColor, 4000)) eventActive = false;
        return;
    }
}

void renderEventAssist()
{
    // Phase 1: Fade to dark green (250ms)
    if (eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::DarkGreen, 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back (1000ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB::DarkGreen, defaultColor, 1000)) eventActive = false;
        return;
    }
}

void renderEventDeath() // only in team mode
{
    // Phase 1: Fade to red (250ms)
    if (eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::Red, 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back (4000ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB::Red, defaultColor, 4000)) eventActive = false;
        return;
    }
}

void renderEventGameEnd()
{
    if(eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::Black, 200)) eventAnimationStep = 1;
        return;
    }

    // Phase 1: Expand red from center (4000ms)
    if (eventAnimationStep == 1)
    {
        if(expandFromCenter(CRGB::Red, CRGB::Black, 1000)) eventAnimationStep = 2;
        return;
    }
    // Phase 2: Fade to default color (4000ms)
    else if (eventAnimationStep == 2)
    {
        if(fadeColor(CRGB::Red, defaultColor, 6000)) eventActive = false;
        return;
    }
}

void renderEventDowned()
{
    // Phase 1: Fade to yellow (250ms)
    if (eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::OrangeRed, 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back to default (1000ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB::OrangeRed, defaultColor, 3000)) eventActive = false;
        return;
    }
}

void renderEventRevived()
{
    if(eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::Black, 200)) eventAnimationStep = 1;
        return;
    }

    // Phase 1: Expand light blue from center (4000ms)
    if (eventAnimationStep == 1)
    {
        if(expandFromCenter(CRGB(0x00, 0xc3, 0xff), CRGB::Black, 1000)) eventAnimationStep = 2;
        return;
    }
    // Phase 2: Fade to default color (4000ms)
    else if (eventAnimationStep == 2)
    {
        if(fadeColor(CRGB(0x00, 0xc3, 0xff), defaultColor, 4000)) eventActive = false;
        return;
    }
}

void renderEventFullShield()
{
    // Phase 1: Fade to light blue (250ms)
    if (eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB(0x00, 0xc3, 0xff), 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back to default (500ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB(0x00, 0xc3, 0xff), defaultColor, 1000)) eventActive = false;
        return;
    }
}

void renderEventFullHealth()
{
    // Phase 1: Fade to green (250ms)
    if (eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::Green, 100)) eventAnimationStep = 1;
        return;
    }
    // Phase 2: Fade back (500ms)
    else if (eventAnimationStep == 1)
    {
        if(fadeColor(CRGB::Green, defaultColor, 1000)) eventActive = false;
        return;
    }
}

void renderEventWin()
{
    if(eventAnimationStep == 0)
    {
        if(fadeColor(defaultColor, CRGB::Black, 200)) eventAnimationStep = 1;
        return;
    }

    // Phase 1 to 3: same intro
    if (eventAnimationStep == 1)
    {
        if (expandFromCenter(CRGB::Blue, CRGB::Black, 800)) eventAnimationStep = 2;
        return;
    }

    if (eventAnimationStep == 2)
    {
        if (expandFromCenter(CRGB::Yellow, CRGB::Blue, 800)) eventAnimationStep = 3;
        return;
    }

    if (eventAnimationStep == 3)
    {
        if (expandFromCenter(CRGB::Blue, CRGB::Yellow, 800)) eventAnimationStep = 4;
        return;
    }

    if (eventAnimationStep == 4)
    {
        if (expandFromCenter(CRGB::Yellow, CRGB::Blue, 800)) eventAnimationStep = 5;
        return;
    }

    if (eventAnimationStep == 5)
    {
        if (fadeColor(CRGB::Yellow, CRGB::Black, 800)) eventAnimationStep = 6;
        return;
    }

    // Phase 4: simple fireworks (10s)
    if (eventAnimationStep == 6)
    {
        static Timer fwTimer;
        static Timer fwUpdate;
        static bool fwInit = false;

        static bool rocketActive = false;
        static int16_t rocketPos = 0;
        static int16_t rocketVel = 0;
        static CRGB rocketColor = CRGB::White;

        static uint8_t sparkLife[NUM_LEDS];   // brightness per LED for explosion
        static CRGB sparkColor[NUM_LEDS];     // color per LED for explosion

        if (!fwInit)
        {
            fwInit = true;
            fwTimer.start();
            fwUpdate.start();

            for (uint16_t i = 0; i < NUM_LEDS; i++)
            {
                sparkLife[i] = 0;
                sparkColor[i] = CRGB::Black;
            }

            rocketActive = true;
            rocketPos = 0;
            rocketVel = random(3, 7); // speed
            rocketColor = CHSV(random8(), 255, 255);
        }

        if (fwUpdate.elapsed(25))
        {
            fwUpdate.start();

            // global fade
            for (uint16_t i = 0; i < NUM_LEDS; i++)
            {
                Hardware::leds[i].fadeToBlackBy(35);
            }

            // rocket rising
            if (rocketActive)
            {
                rocketPos += rocketVel;
                rocketVel -= 1; // gravity

                if (rocketPos < 0) rocketPos = 0;
                if (rocketPos > (int16_t)NUM_LEDS - 1) rocketPos = (int16_t)NUM_LEDS - 1;

                Hardware::leds[rocketPos] += rocketColor;

                // explode near top or when slowing down
                if (rocketVel <= 0 || rocketPos >= (int16_t)NUM_LEDS - 2)
                {
                    rocketActive = false;

                    // create explosion: set life for random positions around rocketPos
                    for (uint8_t k = 0; k < 25; k++)
                    {
                        int16_t p = rocketPos + random(-10, 11);
                        if (p < 0 || p >= (int16_t)NUM_LEDS) continue;

                        sparkLife[p] = random(120, 255);
                        sparkColor[p] = rocketColor;

                        // little color variation
                        sparkColor[p].nscale8(random(160, 255));
                    }
                }
            }

            // sparks: decay and render
            bool anySpark = false;
            for (uint16_t i = 0; i < NUM_LEDS; i++)
            {
                if (sparkLife[i] == 0) continue;

                anySpark = true;

                CRGB c = sparkColor[i];
                c.nscale8(sparkLife[i]);
                Hardware::leds[i] += c;

                // decay
                sparkLife[i] = (sparkLife[i] > 12) ? (sparkLife[i] - 12) : 0;

                // optional tiny drift
                if (sparkLife[i] > 0 && random8() < 30)
                {
                    int16_t j = (random8() < 128) ? (int16_t)i - 1 : (int16_t)i + 1;
                    if (j >= 0 && j < (int16_t)NUM_LEDS && sparkLife[j] < sparkLife[i])
                    {
                        sparkLife[j] = sparkLife[i] - 5;
                        sparkColor[j] = sparkColor[i];
                    }
                }
            }

            // start a new rocket when sparks are gone
            if (!rocketActive && !anySpark)
            {
                rocketActive = true;
                rocketPos = 0;
                rocketVel = random(3, 10);
                rocketColor = CHSV(random8(), 255, 255);
            }

            // if you dont call show() globally, enable this:
            // FastLED.show();
        }

        if (fwTimer.elapsedStart() >= 15000)
        {
            eventAnimationStep = 7;
            fwTimer.stop();
            fwInit = false;
        }
        return;
    }

    // Phase 5: back to default
    if (eventAnimationStep == 7)
    {
        if (fadeColor(CRGB::Black, defaultColor, 2000))
        {
            eventActive = false;
            currentStage = st_lobby; // back to lobby
        }
        return;
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
bool fadeColor(const CRGB& startColor, const CRGB& endColor, uint32_t durationMs, bool forceRestart)
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
bool expandFromCenter(const CRGB& color, const CRGB& backgroundColor, uint32_t durationMs, bool forceRestart)
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