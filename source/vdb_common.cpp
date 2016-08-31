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

#include "vdb_common.h"
#include "vdb_logger.h"
#include "vdb_string.h"

namespace
{
    bool perform_byteswapping = false;
}

// ----------------------------------------------------------------------------
void vdb::pause(uint32_t milliseconds)
{
    uint32_t
        microseconds = (milliseconds * 1000);

    LOG_VERBOSE("Sleeping for %d microseconds...", microseconds);

    usleep(microseconds);
}

// ----------------------------------------------------------------------------
bool vdb::byteswapping(void)
{
    return perform_byteswapping;
}

// ----------------------------------------------------------------------------
void vdb::set_byteswapping(void)
{
    perform_byteswapping = not is_little_endian();

    LOG_VERBOSE(
        "Byteswapping turned %s...",
        (perform_byteswapping ? "on" : "off"));
}

// ----------------------------------------------------------------------------
void vdb::force_byteswapping(bool value)
{
    perform_byteswapping = value;

    LOG_VERBOSE(
        "Forcing byteswapping %s...",
        (perform_byteswapping ? "on" : "off"));
}

// ----------------------------------------------------------------------------
void vdb::byteswap(int16_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        value = ((value << 8) |
                ((value >> 8) &0xFF));
    }
}

// ----------------------------------------------------------------------------
void vdb::byteswap(uint16_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        byteswap(*reinterpret_cast<int16_t *>(&value), force);
    }
}

// ----------------------------------------------------------------------------
void vdb::byteswap(int32_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        value = ((value << 24) |
                ((value << 8) & 0xFF0000) |
                ((value >> 8) & 0xFF00) |
                ((value >> 24) & 0xFF));
    }
}

// ----------------------------------------------------------------------------
void vdb::byteswap(uint32_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        byteswap(*reinterpret_cast<int32_t *>(&value), force);
    }
}

// ----------------------------------------------------------------------------
void vdb::byteswap(int64_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        value = ((value << 56) |
                ((value << 40) & 0xFF000000000000) |
                ((value << 24) & 0xFF0000000000) |
                ((value << 8) & 0xFF00000000) |
                ((value >> 8) & 0xFF000000) |
                ((value >> 24) & 0xFF0000) |
                ((value >> 40) & 0xFF00) |
                ((value >> 56) & 0xFF));
    }
}

// ----------------------------------------------------------------------------
void vdb::byteswap(uint64_t &value, bool force)
{
    if (perform_byteswapping or force)
    {
        byteswap(*reinterpret_cast<int64_t *>(&value), force);
    }
}

// ----------------------------------------------------------------------------
bool vdb::user_confirmation(const char *prompt_ptr)
{
    const int
        input_length = 1024;
    std::string
        string_input;
    char
        input[input_length];

    if (prompt_ptr)
    {
        std::cout << prompt_ptr;
    }

    std::cin.getline(input, input_length);

    string_input = to_lowercase(trim(input));

    return starts_with(string_input, "y");
}
