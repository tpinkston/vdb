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

#ifndef VDB_COMMENTS_H
#define VDB_COMMENTS_H

#include "vdb_command.h"
#include "vdb_file_handler.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class comment : public file_handler, public command
    {
      public:

        comment(void);
        virtual ~comment(void);

        virtual int run(void);
    };

    // ------------------------------------------------------------------------
    class uncomment : public file_handler, public command
    {
      public:

        uncomment(void);
        virtual ~uncomment(void);

        virtual int run(void);

      protected:

        virtual void remove_comment(int32_t number);

        virtual void remove_all_comments(void);
    };
}

#endif
