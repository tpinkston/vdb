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

#include "vdb_string.h"

namespace
{
    // ------------------------------------------------------------------------
    bool is_whitespace(char ch)
    {
        switch(ch)
        {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                return true;
        }

        return false;
    }
}

// ----------------------------------------------------------------------------
std::string vdb::to_lowercase(const std::string &string)
{
    std::string
        copy = string;

    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

    return copy;
}

// ----------------------------------------------------------------------------
std::string vdb::to_uppercase(const std::string &string)
{
    std::string
        copy = string;

    std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);

    return copy;
}

// ----------------------------------------------------------------------------
std::string vdb::trim(const char *buffer)
{
    if (buffer)
    {
        std::string
            string(buffer);

        return trim(string);
    }

    return std::string();
}

// ----------------------------------------------------------------------------
std::string vdb::trim(const std::string &string)
{
    std::string
        trimmed(string);
    std::string::size_type
        i = 0;

    // Strip off leading whitespace.
    //
    while(not trimmed.empty() and is_whitespace(trimmed[i]))
    {
        trimmed.erase(0, 1);
    }

    i = (trimmed.length() - 1);

    // Strip off trailing whitespace.
    //
    while(not trimmed.empty() and is_whitespace(trimmed[i]))
    {
        trimmed.erase(i, 1);

        if (not trimmed.empty())
        {
            i = (trimmed.length() - 1);
        }
    }

    return trimmed;
}

// ----------------------------------------------------------------------------
bool vdb::starts_with(
    const std::string &string,
    const std::string &prefix)
{
    const std::string::size_type
        string_length = string.length(),
        prefix_length = prefix.length();
    bool
        result = false;

    if (prefix_length <= string_length)
    {
        std::string::size_type
            i = 0;

        result = true;

        while(result and (i < prefix_length))
        {
            if (prefix[i] == string[i])
            {
                ++i;
            }
            else
            {
                result = false;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::ends_with(
    const std::string &string,
    const std::string &suffix)
{
    const int string_length = string.length();
    const int suffix_length = suffix.length();
    bool result = false;

    if (suffix_length <= string_length)
    {
        result = (suffix == string.substr(
            (string_length - suffix_length),
            suffix_length));
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::contains(
    const std::string &string,
    const std::string &substring)
{
    return (string.find(substring, 0) != std::string::npos);
}

// ----------------------------------------------------------------------------
bool vdb::contains_ignore_case(
    const std::string &string,
    const std::string &substring)
{
    return contains(to_lowercase(string), to_lowercase(substring));
}

// ----------------------------------------------------------------------------
size_t vdb::tokenize(const std::string &string, std::list<std::string> &tokens)
{
    std::istringstream
        stream(string);
    std::string
        token;

    tokens.clear();

    while(stream.good())
    {
        stream >> token;

        if (not token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens.size();
}

// ----------------------------------------------------------------------------
size_t vdb::tokenize(
    const std::string &string,
    std::vector<std::string> &tokens)
{
    std::istringstream
        stream(string);
    std::string
        token;

    tokens.clear();

    while(stream.good())
    {
        stream >> token;

        if (not token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens.size();
}

// ----------------------------------------------------------------------------
size_t vdb::tokenize_csv(
    const std::string &string,
    std::list<std::string> &tokens)
{
    std::string
        modified_string = string;
    bool
        in_single_quote = false,
        in_double_quote = false;

    for(std::string::size_type i = 0; i < modified_string.length(); ++i)
    {
        switch(modified_string[i])
        {
            case '\'':
                in_single_quote = not in_single_quote;
                break;
            case '"':
                in_double_quote = not in_double_quote;
                break;
            case ',':
                if (not in_single_quote and not in_double_quote)
                {
                    modified_string[i] = ' ';
                }
                break;
        }
    }

    return tokenize(modified_string, tokens);
}

// ----------------------------------------------------------------------------
size_t vdb::tokenize_csv(
    const std::string &string,
    std::vector<std::string> &tokens)
{
    std::string
        modified_string = string;
    bool
        in_single_quote = false,
        in_double_quote = false;

    for(std::string::size_type i = 0; i < modified_string.length(); ++i)
    {
        switch(modified_string[i])
        {
            case '\'':
                in_single_quote = not in_single_quote;
                break;
            case '"':
                in_double_quote = not in_double_quote;
                break;
            case ',':
                if (not in_single_quote and not in_double_quote)
                {
                    modified_string[i] = ' ';
                }
                break;
        }
    }

    return tokenize(modified_string, tokens);
}

// ----------------------------------------------------------------------------
void vdb::parse_entry(
    const std::string &line,
    std::string &key,
    std::string &value)
{
    std::string::size_type i = line.find_first_of('=');

    if (i == std::string::npos)
    {
        // The whole line is the key.
        //
        key = line;
        value.clear();
    }
    else
    {
        key = line.substr(0, i);
        value = line.substr(i + 1, line.length());

        trim(key);
        trim(value);
    }
}

// ----------------------------------------------------------------------------
bool vdb::is_integer(const std::string &string)
{
    std::string
        trimmed = trim(string);

    if (trimmed.empty())
    {
        return false;
    }

    if (starts_with(trimmed, "-"))
    {
        trimmed = trimmed.substr(1);
    }

    for(std::string::size_type i = 0; i < trimmed.length(); ++i)
    {
        switch(trimmed[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                // Good, do nothing
                break;
            default:
                return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_int8(const std::string &string, int8_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_uint8(const std::string &string, uint8_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_int16(const std::string &string, int16_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_uint16(const std::string &string, uint16_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_int32(const std::string &string, int32_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_uint32(const std::string &string, uint32_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_int64(const std::string &string, int64_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtoll(string_value.c_str(), NULL, 10);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_uint64(const std::string &string, uint64_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtoll(string_value.c_str(), NULL, 10);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_float32(const std::string &string, float32_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtof(string_value.c_str(), NULL);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::string_to_float64(const std::string &string, float64_t &value)
{
    std::string
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtold(string_value.c_str(), NULL);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
std::string vdb::float_to_string(float32_t f, uint32_t min, uint32_t max)
{
    std::ostringstream
        stream;

    stream << std::fixed << std::setprecision(max) << f;

    std::string
        string = stream.str();
    std::string::size_type
        point = string.find('.', 0);

    if (point != std::string::npos)
    {
        std::string::size_type
            size = string.size();

        while((string[size - 1]) == '0' and ((size - point - 1) > min))
        {
            string = string.substr(0, (size - 1));
            size = string.size();
        }
    }

    return string;
}

// -----------------------------------------------------------------------------
std::string vdb::float_to_string(float64_t f, uint32_t min, uint32_t max)
{
    std::ostringstream
        stream;

    stream << std::fixed << std::setprecision(max) << f;

    std::string
        string = stream.str();
    std::string::size_type
        point = string.find('.', 0);

    if (point != std::string::npos)
    {
        std::string::size_type
            size = string.size();

        while((string[size - 1]) == '0' and ((size - point - 1) > min))
        {
            string = string.substr(0, (size - 1));
            size = string.size();
        }
    }

    return string;
}
