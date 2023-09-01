// =============================================================================
// Arduino-LED-Strip-Controller | Mode-Fire
// =============================================================================

#include "strip_modes.h"
#include "hardware.h"
#include "Timer.h"
#include <FastLED.h>

namespace {
  Timer fireEffectTimer;
  int currentLED = 0;
  uint32_t delayTime = 0;  // This will store the random delay time
}

void Mode::fire(uint16_t functionValue)
{
  // If the timer has elapsed, reset and set a new random time.
  if (fireEffectTimer.elapsed(delayTime)) {
    fireEffectTimer.start();
    delayTime = random(40, 125);
    currentLED = 0;  // Reset LED index
  }

  // Initialize the fire effect timer if it hasn't been started yet.
  if (fireEffectTimer.elapsedStart() == 0) {
    fireEffectTimer.start();
    delayTime = random(40, 125);
  }

  // Only update one LED at a time based on the currentLED index
  if (currentLED < NUM_LEDS) {
    int r = 255;
    int g = 96;
    int b = 12;
    short flicker = random(8, 40);

    short r1 = r - flicker;
    short g1 = g - flicker;
    short b1 = b - flicker;

    if (g1 < 0) g1 = 0;
    if (r1 < 0) r1 = 0;
    if (b1 < 0) b1 = 0;

    Hardware::leds[currentLED] = CRGB(r1, g1, b1);
    currentLED++;  // Move to the next LED for the next cycle
  }
}