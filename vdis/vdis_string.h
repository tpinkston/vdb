#ifndef VDIS_STRING_H
#define VDIS_STRING_H

#include <cstring>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "vdis_data_types.h"
#include "vdis_services.h"

namespace vdis
{
    std::string to_lowercase(const std::string &string);
    std::string to_uppercase(const std::string &string);

    std::string trim(const char *buffer);
    std::string trim(const std::string &string);

    bool starts_with(const std::string &string, const std::string &prefix);
    bool ends_with(const std::string &string, const std::string &suffix);

    bool contains(
        const std::string &string,
        const std::string &substring,
        bool ignore_case = false
    );

    // Splits 'string' into separate strings, using whitespace as delimiter.
    //
    size_t tokenize(const std::string &, std::list<std::string> &);
    size_t tokenize(const std::string &, std::vector<std::string> &);

    // Splits 'string' into separate strings, using a comma as delimiter.
    //
    size_t tokenize_csv(const std::string &, std::list<std::string> &);
    size_t tokenize_csv(const std::string &, std::vector<std::string> &);

    // Parses string in form "key=value" and returns the key/value
    //
    void parse_entry(
        const std::string &line,
        std::string &key,
        std::string &value
    );

    bool is_integer(const std::string &string);

    // Conversion from string to primitive types
    //
    bool to_int8(const std::string &string, int8_t &value);
    bool to_int16(const std::string &string, int16_t &value);
    bool to_int32(const std::string &string, int32_t &value);
    bool to_int64(const std::string &string, int64_t &value);
    bool to_uint8(const std::string &string, uint8_t &value);
    bool to_uint16(const std::string &string, uint16_t &value);
    bool to_uint32(const std::string &string, uint32_t &value);
    bool to_uint64(const std::string &string, uint64_t &value);
    bool to_float32(const std::string &string, float32_t &value);
    bool to_float64(const std::string &string, float64_t &value);

    // Floating point to string
    //
    std::string to_string(float32_t f, uint32_t min = 1, uint32_t max = 6);
    std::string to_string(float64_t f, uint32_t min = 1, uint32_t max = 6);

    // ------------------------------------------------------------------------
    // Zeros out memory allocated to pointer up to given size.
    //
    inline void clear_memory(void *value, uint32_t size)
    {
        std::memset(value, 0, size);
    }

    // ------------------------------------------------------------------------
    // Zeros out memory allocated to the object up to its size (sizeof(T)).
    //
    template<typename T>
    inline void clear_memory(T &t)
    {
        std::memset(&t, 0, sizeof(T));
    }

    // ------------------------------------------------------------------------
    // Use stream output operator to convert object to a string.
    //
    template<typename T>
    inline std::string to_string(const T &t)
    {
        std::ostringstream
            stream;

        stream << t;

        return stream.str();
    }
}

#endif
