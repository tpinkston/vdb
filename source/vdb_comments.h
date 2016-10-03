#ifndef VDB_COMMENTS_H
#define VDB_COMMENTS_H

#include "vdb_system.h"

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
