#include "vdb_common.h"

#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_string.h"

namespace
{
    unsigned
        debug_mask = 0;
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
void vdb::set_debug(unsigned mask)
{
    CONSOLE_OUT(
        "DEBUG: setting debug mask: %d [%s]",
        mask,
        vdis::to_bin_string(mask, true).c_str());

    debug_mask = mask;
}

// ----------------------------------------------------------------------------
bool vdb::debug_enabled(void)
{
    // True if bit 0 is set.
    //
    return ((debug_mask & 0x00000001U) > 0);
}

// ----------------------------------------------------------------------------
bool vdb::debug_enabled(unsigned mask)
{
    if (debug_enabled())
    {
        const unsigned
            current_mask = (debug_mask & 0xFFFFFFFEU),
            requested_mask = (mask & 0xFFFFFFFEU);

//        CONSOLE_OUT(
//            "current_mask=%d & requested_mask=%d = %d",
//            current_mask,
//            requested_mask,
//            (current_mask & requested_mask));

        return ((unsigned)(current_mask & requested_mask) == requested_mask);
    }

    return false;
}
