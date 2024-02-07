// =============================================================================
// Utils | MAX4466
// =============================================================================


#include "MAX4466.h"

// Constructor implementation
MAX4466::MAX4466(uint8_t pin) : _pin(pin)
{
    pinMode(_pin, INPUT);
}

uint16_t MAX4466::getRaw()
{
    return analogRead(_pin);
}

uint16_t MAX4466::getVolume()
{
    uint16_t raw = getRaw();
    uint16_t volume = abs(raw - 512); // Assuming 512 is the midpoint for 0-1023 range
    return map(volume, 0, 512, 0, 100); // Map the volume to a more useful range (e.g., 0-100)
}
