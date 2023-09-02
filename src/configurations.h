// =============================================================================
// Arduino-LED-Strip-Controller | Configurations
// =============================================================================

#ifndef _LED_STRIP_CONTROLLER_CONFIGURATIONS_
#define _LED_STRIP_CONTROLLER_CONFIGURATIONS_

// LED Strip parameters
#define NUM_LEDS                    43

// button press ime for Timer acivation (in ms)
#define OFF_TIMER_START_DURATION    2500

// Time til shutdown                v   Defines time in minutes
const uint32_t SHUTDOWN_TIME =      30ULL * 1000ULL * 60ULL;

#endif // _LED_STRIP_CONTROLLER_CONFIGURATIONS_