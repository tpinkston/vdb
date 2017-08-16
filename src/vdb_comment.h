#ifndef VDB_COMMENT_H
#define VDB_COMMENT_H

#include "vdb_command.h"

namespace vdb
{
    class standard_reader_t;

    // ------------------------------------------------------------------------
    class comment_t : public command_t
    {
      public:

        typedef enum { ADD, REMOVE, PRINT } action_e;

        comment_t(
            const std::string &command,
            const std::vector<std::string> &arguments
        );

        virtual ~comment_t(void);

        virtual int run(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

      protected:

        int add(void);
        int remove(void);
        int remove_comment(standard_reader_t &reader, uint32_t number);
        int remove_all_comments(standard_reader_t &reader);
        int print(void);

        action_e
            action;
        uint32_t
            deletion;
    };
}

#endif
