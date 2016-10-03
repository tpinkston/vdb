#include "vdb_common.h"

#include "vdis_logger.h"
#include "vdis_string.h"

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
