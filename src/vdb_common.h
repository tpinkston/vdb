#ifndef VDB_COMMON_H
#define VDB_COMMON_H

#include "vdb_system.h"

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
}

#endif
