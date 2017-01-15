#ifndef VDB_COMMENT_H
#define VDB_COMMENT_H

#include "vdb_system.h"

namespace vdb
{
    class standard_reader_t;

    // ------------------------------------------------------------------------
    class comment_t
    {
      public:

        typedef enum { ADD, REMOVE, PRINT } action_e;

        comment_t(void) : action(PRINT), deletion(0) { }
        ~comment_t(void) { }

        int run(void);

        action_e
            action;
        uint32_t
            deletion;

      private:

        int add(void);
        int remove(void);
        int remove_comment(standard_reader_t &reader, uint32_t number);
        int remove_all_comments(standard_reader_t &reader);
        int print(void);
    };
}

#endif
