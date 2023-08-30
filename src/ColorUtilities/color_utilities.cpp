// =============================================================================
// Arduino-LED-Strip-Controller | Color-Utilities
// =============================================================================

#include "color_utilities.h"
#include "Adafruit_NeoPixel.h"


uint32_t Color::getColorValue(uint16_t inputValue)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;


    if (inputValue <= 42.5)
    {
        red = 255;
        green = inputValue * 6;
        blue = 0;
    }

    if ((inputValue > 42.5) && (inputValue <= 85))
    {
        red = 255 - (inputValue - 43) * 6;
        green = 255;
        blue = 0;
    }

    if ((inputValue > 85) && (inputValue <= 127.5))
    {
        red = 0;
        green = 255;
        blue = (inputValue - 85) * 6;
    }

    if ((inputValue > 127.5) && (inputValue <= 170))
    {
        red = 0;
        green = 255 - (inputValue - 127.5) * 6;
        blue = 255;
    }

    if ((inputValue > 170) && (inputValue <= 212.5))
    {
        red = (inputValue - 170) * 6;
        green = 0;
        blue = 255;
    }

    if ((inputValue > 212.5) && (inputValue <= 255))
    {
        red = 255;
        green = 0;
        blue = 255 - (inputValue - 212.5) * 6;
    }

    return Adafruit_NeoPixel::Color(red, green, blue);
}