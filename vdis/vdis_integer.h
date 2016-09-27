#ifndef VDIS_INTEGER_H
#define VDIS_INTEGER_H

#include <string>

#include "vdis_data_types.h"

namespace vdis
{
    std::string to_bin_string(int8_t value);
    std::string to_bin_string(uint8_t value);

    std::string to_bin_string(int16_t value, bool separated = false);
    std::string to_bin_string(uint16_t value, bool separated = false);

    std::string to_bin_string(int32_t value, bool separated = false);
    std::string to_bin_string(uint32_t value, bool separated = false);

    std::string to_bin_string(int64_t value, bool separated = false);
    std::string to_bin_string(uint64_t value, bool separated = false);

    std::string to_hex_string(int8_t value);
    std::string to_hex_string(uint8_t value);

    std::string to_hex_string(int16_t value, bool separated = false);
    std::string to_hex_string(uint16_t value, bool separated = false);

    std::string to_hex_string(int32_t value, bool separated = false);
    std::string to_hex_string(uint32_t value, bool separated = false);

    std::string to_hex_string(int64_t value, bool separated = false);
    std::string to_hex_string(uint64_t value, bool separated = false);

    float32_t to_float32(uint32_t value);
    float64_t to_float64(uint64_t value);

    uint32_t to_int32(float32_t value);
    uint64_t to_int64(float64_t value);

    // ------------------------------------------------------------------------
    inline float32_t to_float32(uint32_t value)
    {
        return *(reinterpret_cast<float32_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline float64_t to_float64(uint64_t value)
    {
        return *(reinterpret_cast<float64_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline uint32_t to_int32(float32_t value)
    {
        return *(reinterpret_cast<uint32_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline uint64_t to_int64(float64_t value)
    {
        return *(reinterpret_cast<uint64_t *>(&value));
    }
}

#endif
