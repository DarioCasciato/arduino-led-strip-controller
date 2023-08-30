// =============================================================================
// Utils | GPIO Handler
// =============================================================================

#include "gpioHandler.h"

bool GPIO::getPort(uint8_t port)
{
    return digitalRead((uint8_t)port);
}

uint16_t GPIO::getAnalogPort(uint8_t port)
{
    return analogRead((uint8_t)port);
}

void GPIO::setPort(uint8_t port, bool value)
{
    digitalWrite((uint8_t)port, value);
}

void GPIO::setAnalogPort(uint8_t port, uint8_t value)
{
    analogWrite((uint8_t)port, value);
}