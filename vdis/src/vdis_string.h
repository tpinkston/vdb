#ifndef VDIS_STRING_H
#define VDIS_STRING_H

#include "vdis_data_types.h"
#include "vdis_services.h"

namespace vdis
{
    string_t to_lowercase(const std::string &string);
    string_t to_uppercase(const std::string &string);

    string_t trim(const char *buffer);
    string_t trim(const std::string &string);

    bool starts_with(const string_t &string, const std::string &prefix);
    bool ends_with(const string_t &string, const std::string &suffix);

    bool contains(
        const string_t &string,
        const string_t &substring,
        bool ignore_case = false
    );

    // Splits 'string' into separate strings, using whitespace as delimiter.
    //
    size_t tokenize(const string_t &, std::list<std::string> &);
    size_t tokenize(const string_t &, std::vector<std::string> &);

    // Splits 'string' into separate strings, using a comma as delimiter.
    //
    size_t tokenize_csv(const string_t &, std::list<std::string> &);
    size_t tokenize_csv(const string_t &, std::vector<std::string> &);

    // Parses string in form "key=value" and returns the key/value
    //
    void parse_entry(
        const string_t &line,
        string_t &key,
        string_t &value
    );

    bool is_integer(const string_t &string);

    // Conversion from string to primitive types
    //
    bool to_int8(const string_t &string, int8_t &value);
    bool to_int16(const string_t &string, int16_t &value);
    bool to_int32(const string_t &string, int32_t &value);
    bool to_int64(const string_t &string, int64_t &value);
    bool to_uint8(const string_t &string, uint8_t &value);
    bool to_uint16(const string_t &string, uint16_t &value);
    bool to_uint32(const string_t &string, uint32_t &value);
    bool to_uint64(const string_t &string, uint64_t &value);
    bool to_float32(const string_t &string, float32_t &value);
    bool to_float64(const string_t &string, float64_t &value);

    // Floating point to string
    //
    string_t to_string(float32_t f, uint32_t min = 1, uint32_t max = 6);
    string_t to_string(float64_t f, uint32_t min = 1, uint32_t max = 6);

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
    inline string_t to_string(const T &t)
    {
        std::ostringstream
            stream;

        stream << t;

        return stream.str();
    }
}

#endif
