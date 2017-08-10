#ifndef VDB_COMMON_H
#define VDB_COMMON_H

#include "vdb_options.h"
#include "vdb_system.h"

// Debug bit masks (using bits 1 through 31)
//
#define DEBUG_OPTIONS           0x00000002U
#define DEBUG_NETWORK           0x00000004U
#define DEBUG_FILE_READER       0x00010000U

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
    // Get or set debug options
    //
    void set_debug(unsigned mask);
    bool debug_enabled(void);
    bool debug_enabled(unsigned mask);
}

#endif
