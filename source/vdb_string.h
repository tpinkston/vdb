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

#ifndef VDB_STRING_H
#define VDB_STRING_H

#include "vdb_common.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Conversion to upper or lower case

    std::string to_lowercase(const std::string &string);
    std::string to_uppercase(const std::string &string);

    // ------------------------------------------------------------------------
    // Removes leading and trailing whitespace characters (spaces, tabs
    // and carriage return characters).

    std::string trim(const char *buffer);
    std::string trim(const std::string &string);

    // ------------------------------------------------------------------------
    // Miscellaneous string tests

    bool starts_with(const std::string &string, const std::string &prefix);
    bool ends_with(const std::string &string, const std::string &suffix);
    bool contains(const std::string &string, const std::string &substring);

    bool contains_ignore_case(
        const std::string &string,
        const std::string &substring
    );

    // ------------------------------------------------------------------------
    // Splits 'string' into separate strings, using whitespace as delimiter.

    size_t tokenize(
        const std::string &string,
        std::list<std::string> &tokens
    );

    size_t tokenize(
        const std::string &string,
        std::vector<std::string> &tokens
    );

    // ------------------------------------------------------------------------
    // Splits 'string' into separate strings, using a comma as delimiter.

    size_t tokenize_csv(
        const std::string &string,
        std::list<std::string> &tokens
    );

    size_t tokenize_csv(
        const std::string &string,
        std::vector<std::string> &tokens
    );

    // ------------------------------------------------------------------------
    // Parses string in form "key=value" and returns the key/value
    //
    void parse_entry(
        const std::string &line,
        std::string &key,
        std::string &value
    );

    // ------------------------------------------------------------------------
    // Converting from string to primitive types

    bool is_integer(const std::string &string);

    bool string_to_int8(const std::string &string, int8_t &value);
    bool string_to_uint8(const std::string &string, uint8_t &value);

    bool string_to_int16(const std::string &string, int16_t &value);
    bool string_to_uint16(const std::string &string, uint16_t &value);

    bool string_to_int32(const std::string &string, int32_t &value);
    bool string_to_uint32(const std::string &string, uint32_t &value);

    bool string_to_int64(const std::string &string, int64_t &value);
    bool string_to_uint64(const std::string &string, uint64_t &value);

    bool string_to_float32(const std::string &string, float32_t &value);
    bool string_to_float64(const std::string &string, float64_t &value);

    // ------------------------------------------------------------------------
    // Floating point to string

    std::string float_to_string(
        float32_t f,
        uint32_t min = 1,
        uint32_t max = 6
    );

    std::string float_to_string(
        float64_t f,
        uint32_t min = 1,
        uint32_t max = 6
    );

    // ------------------------------------------------------------------------
    // Generic to string
    //
    template<typename T>
    inline std::string generic_to_string(T t)
    {
        std::ostringstream
            stream;

        stream << t;

        return stream.str();
    }
}

#endif
