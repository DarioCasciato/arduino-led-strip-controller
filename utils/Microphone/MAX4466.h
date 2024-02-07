// =============================================================================
// Utils | MAX4466
// =============================================================================


#ifndef MAX4466_h
#define MAX4466_h

#include <Arduino.h>

class MAX4466
{
private:
    uint8_t _pin; // Analog pin where the MAX4466 output is connected

public:
    // Constructor
    MAX4466(uint8_t pin);

    // Get the raw analog reading from the microphone
    uint16_t getRaw();

    // Get the volume level from the microphone
    uint16_t getVolume();
};

#endif // MAX4466_h
