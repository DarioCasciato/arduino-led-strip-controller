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
    uint16_t rawValue = analogRead(_pin); // Read the raw analog value
    uint16_t rectifiedValue;

    if(rawValue < 256){
        // If the value is below the 256, flip it
        rectifiedValue = (256 - (rawValue - 256));
    } else {
        // If the value is above the 256, keep it as it is
        rectifiedValue = rawValue;
    }

    // Subtract the 256 to center around 0V
    uint16_t centeredValue = rectifiedValue - 256;

    // Ensure the volume is within 0 to 1023
    uint16_t volumeValue = constrain(centeredValue, 0, 1023);

    return volumeValue;
}
