// =============================================================================
// Utils | GPIO Handler
// =============================================================================

#include "gpioHandler.h"

bool GPIO::getPort(uint8_t port)
{
    return digitalRead(port);
}

uint16_t GPIO::getAnalogPort(uint8_t port)
{
    return analogRead(port);
}

void GPIO::setPort(uint8_t port, bool value)
{
    digitalWrite(port, value);
}

void GPIO::setAnalogPort(uint8_t port, uint8_t value)
{
    analogWrite(port, value);
}