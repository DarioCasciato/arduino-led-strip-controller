// =============================================================================
// Arduino-LED-Strip-Controller | Main
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "EdgeDetection.h"
#include "configurations.h"
#include "Potentiometer.h"
#include "state.h"
#include "modes/strip_modes.h"

void refreshData();
void bootUpSequence();

//------------------------------------------------------------------------------

void setup()
{
    Serial.begin(9600);

    Hardware::init();

    bootUpSequence();
}

void loop()
{
    for (;;)
    {
        refreshData(); // refresh port values

        State::stateDriver();
    }
}

//------------------------------------------------------------------------------

void refreshData()
{
    Hardware::updateHardware();
    EdgeDetection::updateEdges();
}

void bootUpSequence()
{
    const uint16_t functionPotValue = Hardware::potFunction.get();
    const uint8_t currentBrightness = map(Hardware::potBrightness.get(),
                                          0, 1024, 1, 255);

    Mode::white(functionPotValue);

    for (uint8_t i = 0; i < currentBrightness; i++)
    {
        FastLED.setBrightness(i);
        FastLED.show();
        delay(8);
    }
}