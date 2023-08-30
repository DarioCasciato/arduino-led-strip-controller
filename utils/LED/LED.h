// =========================================
// Utils | LED
// =========================================

#include <Arduino.h>

#ifndef ARDUINO_OBJECTS_LED_
#define ARDUINO_OBJECTS_LED_

enum activeLevel
{
    LOW_ACTIVE,
    HIGH_ACTIVE
};
enum level
{
    OFF,
    ON
};

class LED
{
private:
    uint8_t _pin;
    activeLevel _activeLevel;

public:
    /// @brief LED constructor
    ///
    /// @param pin The pin number to which the LED is connected
    /// @param ActiveLevel The active level of the LED (LOW_ACTIVE or HIGH_ACTIVE)
    LED(uint8_t pin, activeLevel ActiveLevel = HIGH_ACTIVE);

    /// @brief Set the level of the LED
    ///
    /// @param setLevel The level to set the LED to (OFF or ON)
    void set(level setLevel);

    /// @brief Get the current level of the LED
    ///
    /// @return The current level of the LED (OFF or ON)
    bool get();

    /// @brief Toggle the state of the LED
    void toggle();

    /// @brief Turn on the LED
    void on();

    /// @brief Turn off the LED
    void off();
};

#endif // ARDUINO_OBJECTS_LED_