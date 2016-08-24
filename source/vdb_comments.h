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

#include "vdb_common.h"

namespace vdb
{
    class standard_reader_t;

    // ------------------------------------------------------------------------
    class comments
    {
      public:

        static int add(void);
        static int remove(void);

      private:

        static int remove_comment(standard_reader_t &reader, int32_t number);

        static int remove_all_comments(standard_reader_t &reader);
    };
}

#endif
