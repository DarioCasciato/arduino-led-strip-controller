// =============================================================================
// Arduino-LED-Strip-Controller | Color-Utilities
// =============================================================================

#include <stdint.h>

namespace Color
{
    /// @brief Convert an input value to an RGB color for visualization.
    ///
    /// This function takes an input value and maps it to an RGB color suitable
    /// for visualization purposes. The mapping is done based on defined ranges
    /// of the input value, resulting in varying color gradients.
    ///
    /// @param inputValue The input value to convert (0 to 255).
    /// @return The corresponding RGB color value.
    uint32_t getColorValue(uint16_t inputValue);
} // namespace Color
