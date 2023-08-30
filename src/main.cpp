// =============================================================================
// Arduino-LED-Strip-Controller | Main
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "EdgeDetection.h"
#include "configurations.h"
#include "Potentiometer.h"
#include "state.h"

void refreshData();

//------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);

  Hardware::init();
}

void loop()
{
  for (;;)
  {
    refreshData();  // refresh port values

    State::stateDriver();
  }
}

//------------------------------------------------------------------------------

void refreshData()
{
  Hardware::updateHardware();
  EdgeDetection::updateEdges();
}