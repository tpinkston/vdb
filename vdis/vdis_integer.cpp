#include "vdis_integer.h"
#include "vdis_string.h"

namespace
{
    union long_union
    {
        uint32_t int32[];
        uint64_t number;
    };

    // ------------------------------------------------------------------------
    template<typename T>
    std::string get_hexadecimal(T t, unsigned size)
    {
        std::ostringstream
            stream;

        stream << std::hex << (uint64_t)t;

        std::string
            s = stream.str();

        if ((s == "0") or (s.size() <= size))
        {
            return vdis::to_uppercase(s);
        }
        else
        {
            return vdis::to_uppercase(s.substr((s.size() - size), size));
        }
    }

    // ------------------------------------------------------------------------
    std::string fill(const std::string &s, unsigned length)
    {
        std::string
            copy = s;

        while(copy.size() < length)
        {
            copy = ("0" + copy);
        }

        return copy;
    }
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(int8_t value)
{
    std::string
        s("00000000");

    for(unsigned j = 0; j < 8; ++j)
    {
        if (value & (1 << j))
        {
            s[7 - j] = '1';
        }
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(uint8_t value)
{
    return to_bin_string((int8_t)value);
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(int16_t value, bool separated)
{
    std::string
        s("0000000000000000");

    for(unsigned j = 0; j < 16; ++j) {

        if (value & (1 << j)) {

            s[15 - j] = '1';
        }
    }

    if (separated) {

        s.insert(8, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(uint16_t value, bool separated)
{
    return to_bin_string((int16_t)value, separated);
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(int32_t value, bool separated)
{
    std::string
        s("00000000000000000000000000000000");

    for(unsigned j = 0; j < 32; ++j)
    {
        if (value & (1 << j))
        {
            s[31 - j] = '1';
        }
    }

    if (separated)
    {
        s.insert(24, 1, '-');
        s.insert(16, 1, '-');
        s.insert(8, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(uint32_t value, bool separated)
{
    return to_bin_string((int32_t)value, separated);
}


// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(int64_t value, bool separated)
{
    std::string
        s("0000000000000000000000000000000000000000000000000000000000000000");

    long_union
        bits64;

    bits64.number = value;

    for(unsigned j = 0; j < 32; ++j)
    {
        if (bits64.int32[0] & (1 << j))
        {
            s[63 - j] = '1';
        }
    }

    for(unsigned j = 32; j < 64; ++j)
    {
        if (bits64.int32[1] & (1 << (j - 32)))
        {
            s[63 - j] = '1';
        }
    }

    if (separated)
    {
        s.insert(56, 1, '-');
        s.insert(48, 1, '-');
        s.insert(40, 1, '-');
        s.insert(32, 1, '-');
        s.insert(24, 1, '-');
        s.insert(16, 1, '-');
        s.insert(8, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_bin_string(uint64_t value, bool separated)
{
    return to_bin_string((int64_t)value, separated);
}


// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(int8_t value) {

    return fill(get_hexadecimal(value, 2), 2);
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(uint8_t value) {

    return to_hex_string((int8_t)value);
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(int16_t value, bool separated) {

    std::string
        s = fill(get_hexadecimal(value, 4), 4);

    if (separated) {

        s.insert(2, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(uint16_t value, bool separated) {

    return to_hex_string((int16_t)value, separated);
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(int32_t value, bool separated) {

    std::string
        s = fill(get_hexadecimal(value, 8), 8);

    if (separated) {

        s.insert(6, 1, '-');
        s.insert(4, 1, '-');
        s.insert(2, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(uint32_t value, bool separated) {

    return to_hex_string((int32_t)value, separated);
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(int64_t value, bool separated) {

    std::string
        s = fill(get_hexadecimal(value, 16), 16);

    if (separated) {

        s.insert(14, 1, '-');
        s.insert(12, 1, '-');
        s.insert(10, 1, '-');
        s.insert(8, 1, '-');
        s.insert(6, 1, '-');
        s.insert(4, 1, '-');
        s.insert(2, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdis::to_hex_string(uint64_t i, bool separated) {

    return to_hex_string(int64_t(i), separated);
}
