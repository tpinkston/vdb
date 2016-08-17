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

#include "vdb_hexadecimal.h"
#include "vdb_string.h"

namespace
{
    const std::string
        SPACER = "   ";

    // ------------------------------------------------------------------------
    template<typename T>
    std::string get_hexadecimal(T t, unsigned size)
    {
        std::ostringstream
            stream;

        stream << std::hex << uint64_t(t);

        std::string
            s = stream.str();

        if ((s == "0") or (s.size() <= size))
        {
            return vdb::to_uppercase(s);
        }
        else
        {
            return vdb::to_uppercase(s.substr((s.size() - size), size));
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
std::string vdb::hexadecimal::str(int8_t i) {

    return fill(get_hexadecimal(i, 2), 2);
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(uint8_t i) {

    return str(int8_t(i));
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(int16_t i, bool separated) {

    std::string
        s = fill(get_hexadecimal(i, 4), 4);

    if (separated) {

        s.insert(2, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(uint16_t i, bool separated) {

    return str(int16_t(i), separated);
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(int32_t i, bool separated) {

    std::string
        s = fill(get_hexadecimal(i, 8), 8);

    if (separated) {

        s.insert(6, 1, '-');
        s.insert(4, 1, '-');
        s.insert(2, 1, '-');
    }

    return s;
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(uint32_t i, bool separated) {

    return str(int32_t(i), separated);
}

// ----------------------------------------------------------------------------
std::string vdb::hexadecimal::str(int64_t i, bool separated) {

    std::string
        s = fill(get_hexadecimal(i, 16), 16);

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
std::string vdb::hexadecimal::str(uint64_t i, bool separated) {

    return str(int64_t(i), separated);
}

// ----------------------------------------------------------------------------
int8_t vdb::hexadecimal::parse_int8(const std::string &string) {

    return std::strtoll(string.c_str(), NULL, 16);
}

// ----------------------------------------------------------------------------
int16_t vdb::hexadecimal::parse_int16(const std::string &string) {

    return std::strtoll(string.c_str(), NULL, 16);
}

// ----------------------------------------------------------------------------
int32_t vdb::hexadecimal::parse_int32(const std::string &string) {

    return std::strtoll(string.c_str(), NULL, 16);
}

// ----------------------------------------------------------------------------
int64_t vdb::hexadecimal::parse_int64(const std::string &string) {

    int64_t
        value = 0;

    // The 'strtoll' function is a little funny with strings with length 16
    // for negative numbers (bit 63 is set) so use a divide and conquer
    // approach to such strings.
    //
    if (string.length() < 16)
    {
        value = std::strtoll(string.c_str(), NULL, 16);
    }
    else
    {
        std::string
            string_63_32 = string.substr(0, 8),
            string_32_01 = string.substr(8, (string.length() - 8));
        int64_t
            value_63_32 = parse_int32(string_63_32),
            value_32_01 = parse_int32(string_32_01);

        value = (value_63_32 << 32);
        value |= (0x00000000FFFFFFFF & value_32_01);
    }

    return value;
}

// ----------------------------------------------------------------------------
void vdb::hexadecimal::stream(
    const uint8_t *array,
    uint32_t length,
    uint32_t byte_width,
    std::ostream &ostream)
{
    stream(
        std::string(),
        array,
        length,
        byte_width,
        ostream);
}

// ----------------------------------------------------------------------------
void vdb::hexadecimal::stream(
    const std::string &prefix,
    const uint8_t *array,
    uint32_t length,
    uint32_t byte_width,
    std::ostream &ostream)
{
    uint32_t
        count;

    for(uint32_t i = 0; i < length;)
    {
        std::string
            header = str(i);
        std::ostringstream
            ascii,
            hexadecimal;

        for(uint32_t j = 0; (j < byte_width); ++i, ++j)
        {
            if (i < length)
            {
                hexadecimal << str(array[i]);
            }
            else
            {
                hexadecimal << "  ";
            }

            if ((j % byte_width) != (byte_width - 1))
            {
                hexadecimal << ' ';
            }

            if (i >= length)
            {
                ascii << ' ';
            }
            else if ((array[i] > 31) && (array[i] < 127))
            {
                ascii << char(array[i]);
            }
            else
            {
                ascii << '.';
            }
        }

        count = ((byte_width * 3) - 1);

        while(hexadecimal.str().size() < count)
        {
            hexadecimal << ' ';
        }

        ostream << (prefix.empty() ? (header + SPACER) : prefix)
                << hexadecimal.str() << SPACER << ascii.str() << std::endl;
    }
}
