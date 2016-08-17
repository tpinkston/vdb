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

#ifndef VDB_COMMAND_H
#define VDB_COMMAND_H

#include "vdb_logger.h"

namespace vdb
{
    typedef enum
    {
        USER_COMMAND_NONE,
        USER_COMMAND_CAPTURE,
        USER_COMMAND_PLAYBACK,
        USER_COMMAND_LIST,
        USER_COMMAND_QUERY,
        USER_COMMAND_COMMENT,
        USER_COMMAND_UNCOMMENT,
        USER_COMMAND_ENUMS,
        USER_COMMAND_ENTITIES,
        USER_COMMAND_OBJECTS,
        USER_COMMAND_TEST

    } user_command_e;

    class command
    {
      public:

        command(void);
        virtual ~command(void);

        virtual int run(void) = 0;
    };
}

// ----------------------------------------------------------------------------
inline vdb::command::command(void)
{
    LOG_VERBOSE("Constructing command...");
}

// ----------------------------------------------------------------------------
inline vdb::command::~command(void)
{
    LOG_VERBOSE("Destructing command...");
}

#endif
