// =============================================================================
// Utils | CRC16
// =============================================================================

#include <stdint.h>

#ifndef UTILS_CRC_16_
#define UTILS_CRC_16_


namespace Security::CRC16
{
    /// Calculate the CRC16 checksum for the given data.
    ///
    /// This function calculates the CRC16 checksum for the provided data
    /// of a specified length.
    ///
    /// @param data Pointer to the data for which to calculate the CRC16.
    /// @param length Length of the data in bytes.
    /// @return Calculated CRC16 checksum.
    uint16_t generate(void* data, uint8_t length);

    /// Compare a CRC16 checksum with the calculated checksum of the given data.
    ///
    /// This function compares a CRC16 checksum with the calculated CRC16
    /// checksum of the provided data of a specified length.
    ///
    /// @param crc16 The CRC16 checksum to compare against.
    /// @param data Pointer to the data to compare.
    /// @param length Length of the data in bytes.
    /// @return True if the CRC16 checksum matches the calculated checksum,
    ///         otherwise false.
    bool compare(uint16_t crc16, void* data, uint8_t length);
} // namespace Security::CRC16


#endif // UTILS_CRC_16_