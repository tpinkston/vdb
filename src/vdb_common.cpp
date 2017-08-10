#include "vdb_common.h"

#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_string.h"

namespace
{
    unsigned
        debug_value = 0;
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
bool vdb::user_confirmation(const char *prompt_ptr)
{
    const int
        input_length = 1024;
    string_t
        string_input;
    char
        input[input_length];

    if (prompt_ptr)
    {
        std::cout << prompt_ptr;
    }

    std::cin.getline(input, input_length);

    string_input = vdis::to_lowercase(vdis::trim(input));

    return vdis::starts_with(string_input, "y");
}

// ----------------------------------------------------------------------------
void vdb::set_debug(unsigned value)
{
    CONSOLE_OUT(
        "DEBUG: setting debug mask: %d [%s]",
        value,
        vdis::to_bin_string(value, true).c_str());

    debug_value = value;
}

// ----------------------------------------------------------------------------
bool vdb::debug_enabled(unsigned mask)
{
    const unsigned
        current_level = (debug_value & 0x0000000FU),
        requested_level = (mask & 0x0000000FU);

//    CONSOLE_OUT(
//        "current_level = %d, requested_level = %d",
//        current_level,
//        requested_level);

    if ((requested_level > 0) and (requested_level <= current_level))
    {
        const unsigned
            current_options = (debug_value & 0xFFFFFFF0U),
            requested_options = (mask & 0xFFFFFFF0U);

//        CONSOLE_OUT(
//            "current_options = %d, requested_options = %d",
//            current_options,
//            requested_options);

        return ((current_options & requested_options) == requested_options);
    }

    return false;
}
