#include "vdis_string.h"

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
string_t vdis::to_lowercase(const std::string &string)
{
    string_t
        copy = string;

    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

    return copy;
}

// ----------------------------------------------------------------------------
string_t vdis::to_uppercase(const std::string &string)
{
    string_t
        copy = string;

    std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);

    return copy;
}

// ----------------------------------------------------------------------------
string_t vdis::trim(const char *buffer)
{
    if (buffer)
    {
        string_t
            string(buffer);

        return trim(string);
    }

    return string_t();
}

// ----------------------------------------------------------------------------
string_t vdis::trim(const std::string &string)
{
    string_t
        trimmed(string);
    string_t::size_type
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
bool vdis::starts_with(
    const string_t &string,
    const string_t &prefix)
{
    const string_t::size_type
        string_length = string.length(),
        prefix_length = prefix.length();
    bool
        result = false;

    if (prefix_length <= string_length)
    {
        string_t::size_type
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
bool vdis::ends_with(
    const string_t &string,
    const string_t &suffix)
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
bool vdis::contains(
    const string_t &string,
    const string_t &substring,
    bool ignore_case)
{
    if (ignore_case)
    {
        return contains(to_lowercase(string), to_lowercase(substring), false);
    }
    else
    {
        return (string.find(substring, 0) != string_t::npos);
    }
}

// ----------------------------------------------------------------------------
size_t vdis::tokenize(
    const string_t &string,
    std::list<string_t> &tokens)
{
    std::istringstream
        stream(string);
    string_t
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
size_t vdis::tokenize(
    const string_t &string,
    std::vector<string_t> &tokens)
{
    std::istringstream
        stream(string);
    string_t
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
size_t vdis::tokenize_csv(
    const string_t &string,
    std::list<string_t> &tokens)
{
    string_t
        modified_string = string;
    bool
        in_single_quote = false,
        in_double_quote = false;

    for(string_t::size_type i = 0; i < modified_string.length(); ++i)
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
size_t vdis::tokenize_csv(
    const string_t &string,
    std::vector<string_t> &tokens)
{
    string_t
        modified_string = string;
    bool
        in_single_quote = false,
        in_double_quote = false;

    for(string_t::size_type i = 0; i < modified_string.length(); ++i)
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
void vdis::parse_entry(
    const string_t &line,
    string_t &key,
    string_t &value)
{
    string_t::size_type i = line.find_first_of('=');

    if (i == string_t::npos)
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
bool vdis::is_integer(const string_t &string)
{
    string_t
        trimmed = trim(string);

    if (trimmed.empty())
    {
        return false;
    }

    if (starts_with(trimmed, "-"))
    {
        trimmed = trimmed.substr(1);
    }

    for(string_t::size_type i = 0; i < trimmed.length(); ++i)
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
bool vdis::to_int8(const string_t &string, int8_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_uint8(const string_t &string, uint8_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_int16(const string_t &string, int16_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_uint16(const string_t &string, uint16_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_int32(const string_t &string, int32_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_uint32(const string_t &string, uint32_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::atoi(string_value.c_str());
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_int64(const string_t &string, int64_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtoll(string_value.c_str(), NULL, 10);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_uint64(const string_t &string, uint64_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtoll(string_value.c_str(), NULL, 10);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_float32(const string_t &string, float32_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtof(string_value.c_str(), NULL);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::to_float64(const string_t &string, float64_t &value)
{
    string_t
        string_value = trim(string);

    if (is_integer(string_value))
    {
        value = std::strtold(string_value.c_str(), NULL);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
string_t vdis::to_string(float32_t f, uint32_t min, uint32_t max)
{
    std::ostringstream
        stream;

    stream << std::fixed << std::setprecision(max) << f;

    string_t
        string = stream.str();
    string_t::size_type
        point = string.find('.', 0);

    if (point != string_t::npos)
    {
        string_t::size_type
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
string_t vdis::to_string(float64_t f, uint32_t min, uint32_t max)
{
    std::ostringstream
        stream;

    stream << std::fixed << std::setprecision(max) << f;

    string_t
        string = stream.str();
    string_t::size_type
        point = string.find('.', 0);

    if (point != string_t::npos)
    {
        string_t::size_type
            size = string.size();

        while((string[size - 1]) == '0' and ((size - point - 1) > min))
        {
            string = string.substr(0, (size - 1));
            size = string.size();
        }
    }

    return string;
}

