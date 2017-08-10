#ifndef VDB_COMMON_H
#define VDB_COMMON_H

#include "vdb_options.h"
#include "vdb_system.h"

// Debug masks
//
#define DEBUG_OPTIONS           0x00000010U
#define DEBUG_NETWORK           0x00000020U

// Debug levels
//
#define DEBUG_LOW               1
#define DEBUG_MEDIUM            2
#define DEBUG_HIGH              3

namespace vdb
{
    // ------------------------------------------------------------------------
    // Pauses currently running thread for the specified time.
    //
    void pause(uint32_t milliseconds);

    // ------------------------------------------------------------------------
    // Returns true if user confirms action
    //
    bool user_confirmation(const char *prompt_ptr = 0);

    // ------------------------------------------------------------------------
    // Get or set debug level and options
    //
    void set_debug(unsigned level);
    bool debug_enabled(unsigned mask);
}

#endif
