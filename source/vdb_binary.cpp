// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#include "vdb_binary.h"
#include "vdb_logger.h"

namespace
{
    union long_union
    {
        uint32_t int32[];
        uint64_t number;
    };

    const uint32_t SET_MASK[] =
    {
        0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7,
        0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F,
        0xFFFFFEFF, 0xFFFFFDFF, 0xFFFFFBFF, 0xFFFFF7FF,
        0xFFFFEFFF, 0xFFFFDFFF, 0xFFFFBFFF, 0xFFFF7FFF,
        0xFFFEFFFF, 0xFFFDFFFF, 0xFFFBFFFF, 0xFFF7FFFF,
        0xFFEFFFFF, 0xFFDFFFFF, 0xFFBFFFFF, 0xFF7FFFFF,
        0xFEFFFFFF, 0xFDFFFFFF, 0xFBFFFFFF, 0xF7FFFFFF,
        0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, 0x7FFFFFFF
    };

    // ------------------------------------------------------------------------
    bool check_index(uint16_t index, uint16_t minimum, uint16_t maximum)
    {
        if ((index < minimum) || (index > maximum))
        {
            LOG_ERROR(
                "check_index(): index out of range: %s, min: %s, max: %s",
                index,
                minimum,
                maximum);

            return false;
        }

        return true;
    }

    // ------------------------------------------------------------------------
    bool check_value(uint32_t value, uint32_t maximum)
    {
        if ((value < 0) || (value > maximum))
        {
            LOG_ERROR(
                "check_value(): value out of range: %s, max: %s",
                value,
                maximum);

            return false;
        }

        return true;
    }
}

// ----------------------------------------------------------------------------
std::string vdb::binary::str(int8_t i)
{
    std::string
        s("00000000");

    for(unsigned j = 0; j < 8; ++j)
    {
        if (i & (1 << j))
        {
            s[7 - j] = '1';
        }
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdb::binary::str(uint8_t i)
{
    return str(int8_t(i));
}

// ----------------------------------------------------------------------------
std::string vdb::binary::str(int16_t i, bool separated)
{
    std::string
        s("0000000000000000");

    for(unsigned j = 0; j < 16; ++j) {

        if (i & (1 << j)) {

            s[15 - j] = '1';
        }
    }

    if (separated) {

        s.insert(8, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdb::binary::str(uint16_t i, bool separated)
{
    return str(int16_t(i), separated);
}

// ----------------------------------------------------------------------------
std::string vdb::binary::str(int32_t i, bool separated)
{
    std::string
        s("00000000000000000000000000000000");

    for(unsigned j = 0; j < 32; ++j)
    {
        if (i & (1 << j))
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
std::string vdb::binary::str(uint32_t i, bool separated)
{
    return str(int32_t(i), separated);
}


// ----------------------------------------------------------------------------
std::string vdb::binary::str(int64_t i, bool separated)
{
    std::string
        s("0000000000000000000000000000000000000000000000000000000000000000");

    long_union
        bits64;

    bits64.number = i;

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
std::string vdb::binary::str(uint64_t i, bool separated)
{
    return str(int64_t(i), separated);
}

// ----------------------------------------------------------------------------
int8_t vdb::binary::parse_int8(const std::string &string)
{
    return std::strtoll(string.c_str(), NULL, 2);
}

// ----------------------------------------------------------------------------
int16_t vdb::binary::parse_int16(const std::string &string)
{
    return std::strtoll(string.c_str(), NULL, 2);
}

// ----------------------------------------------------------------------------
int32_t vdb::binary::parse_int32(const std::string &string)
{
    return std::strtoll(string.c_str(), NULL, 2);
}

// ----------------------------------------------------------------------------
int64_t vdb::binary::parse_int64(const std::string &string)
{
    return std::strtoll(string.c_str(), NULL, 2);
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_1_bit_8(uint16_t index, uint8_t bits)
{
    if (check_index(index, 0, 7))
    {
        return get_1_bit_32(index, (int32_t(bits) & 0xFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_1_bit_16(uint16_t index, uint16_t bits)
{
    if (check_index(index, 0, 15))
    {
        return get_1_bit_32(index, (int32_t(bits) & 0xFFFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_1_bit_32(uint16_t index, uint32_t bits)
{
    if (check_index(index, 0, 31))
    {
        return ((bits >> index) & 0x1);
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_2_bits_8(uint16_t index, uint8_t bits)
{
    if (check_index(index, 1, 7))
    {
        return get_2_bits_32(index, (int32_t(bits) & 0xFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_2_bits_16(uint16_t index, uint16_t bits)
{
    if (check_index(index, 1, 15))
    {
        return get_2_bits_32(index, (int32_t(bits) & 0xFFFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_2_bits_32(uint16_t index, uint32_t bits)
{
    if (check_index(index, 1, 31))
    {
        return ((bits >> (index - 1)) & 0x3);
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_3_bits_8(uint16_t index, uint8_t bits)
{
    if (check_index(index, 2, 7))
    {
        return get_3_bits_32(index, (int32_t(bits) & 0xFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_3_bits_16(uint16_t index, uint16_t bits)
{
    if (check_index(index, 2, 15))
    {
        return get_3_bits_32(index, (int32_t(bits) & 0xFFFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_3_bits_32(uint16_t index, uint32_t bits)
{
    if (check_index(index, 2, 31))
    {
        return ((bits >> (index - 2)) & 0x7);
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_4_bits_8(uint16_t index, uint8_t bits)
{
    if (check_index(index, 3, 7))
    {
        return get_4_bits_32(index, (int32_t(bits) & 0xFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_4_bits_16(uint16_t index, uint16_t bits)
{
    if (check_index(index, 3, 15))
    {
        return get_4_bits_32(index, (int32_t(bits) & 0xFFFF));
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::get_4_bits_32(uint16_t index, uint32_t bits)
{
    if (check_index(index, 3, 31))
    {
        return ((bits >> (index - 3)) & 0xF);
    }

    return 0;
}

// ----------------------------------------------------------------------------
uint8_t vdb::binary::set_1_bit_8(
    uint16_t index,
    uint8_t bits,
    uint32_t value)
{
    if (check_index(index, 0, 7))
    {
        return uint8_t(set_1_bit_32(index, (bits & 0xFF), value) & 0xFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint16_t vdb::binary::set_1_bit_16(
    uint16_t index,
    uint16_t bits,
    uint32_t value)
{
    if (check_index(index, 0, 15))
    {
        return uint16_t(set_1_bit_32(index, (bits & 0xFFFF), value) & 0xFFFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::set_1_bit_32(
    uint16_t index,
    uint32_t bits,
    uint32_t value)
{
    if (check_value(value, 1) and check_index(index, 0, 31))
    {
        if (value == 0)
        {
            return (bits & SET_MASK[index]);
        }
        else
        {
            return (bits | (1 << index));
        }
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint8_t vdb::binary::set_2_bits_8(
    uint16_t index,
    uint8_t bits,
    uint32_t value)
{
    if (check_index(index, 1, 7))
    {
        return uint8_t(set_2_bits_32(index, (bits & 0xFF), value) & 0xFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint16_t vdb::binary::set_2_bits_16(
    uint16_t index,
    uint16_t bits,
    uint32_t value)
{
    if (check_index(index, 1, 15))
    {
        return uint16_t(set_2_bits_32(index, (bits & 0xFFFF), value) & 0xFFFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::set_2_bits_32(
    uint16_t index,
    uint32_t bits,
    uint32_t value)
{
    if (check_value(value, 3) and check_index(index, 1, 31))
    {
        bits = set_1_bit_32(index, bits, get_1_bit_32(1, value));
        bits = set_1_bit_32((index - 1), bits, get_1_bit_32(0, value));
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint8_t vdb::binary::set_3_bits_8(
    uint16_t index,
    uint8_t bits,
    uint32_t value)
{
    if (check_index(index, 2, 7))
    {
        return uint8_t(set_3_bits_32(index, (bits & 0xFF), value) & 0xFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint16_t vdb::binary::set_3_bits_16(
    uint16_t index,
    uint16_t bits,
    uint32_t value)
{
    if (check_index(index, 2, 15))
    {
        return uint16_t(set_3_bits_32(index, (bits & 0xFFFF), value) & 0xFFFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::set_3_bits_32(
    uint16_t index,
    uint32_t bits,
    uint32_t value)
{
    if (check_value(value, 7) and check_index(index, 2, 31))
    {
        bits = set_1_bit_32(index, bits, get_1_bit_32(2, value));
        bits = set_1_bit_32((index - 1), bits, get_1_bit_32(1, value));
        bits = set_1_bit_32((index - 2), bits, get_1_bit_32(0, value));
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint8_t vdb::binary::set_4_bits_8(
    uint16_t index,
    uint8_t bits,
    uint32_t value)
{
    if (check_index(index, 3, 7))
    {
        return uint8_t(set_4_bits_32(index, (bits & 0xFF), value) & 0xFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint16_t vdb::binary::set_4_bits_16(
    uint16_t index,
    uint16_t bits,
    uint32_t value)
{
    if (check_index(index, 3, 15))
    {
        return uint16_t(set_4_bits_32(index, (bits & 0xFFFF), value) & 0xFFFF);
    }

    return bits;
}

// ----------------------------------------------------------------------------
uint32_t vdb::binary::set_4_bits_32(
    uint16_t index,
    uint32_t bits,
    uint32_t value)
{
    if (check_value(value, 15) and check_index(index, 3, 31)) {

        bits = set_1_bit_32(index, bits, get_1_bit_32(3, value));
        bits = set_1_bit_32((index - 1), bits, get_1_bit_32(2, value));
        bits = set_1_bit_32((index - 2), bits, get_1_bit_32(1, value));
        bits = set_1_bit_32((index - 3), bits, get_1_bit_32(0, value));
    }

    return bits;
}
