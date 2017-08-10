#ifndef VDB_COMMAND_H
#define VDB_COMMAND_H

#include "vdb_options.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    typedef enum
    {
        NOCOMMAND,
        CAPTURE,
        COMMENT,
        ENTITIES,
        ENUMS,
        EXTRACT,
        LIST,
        OBJECTS,
        PLAYBACK,
        SUMMARIZE
    } command_e;

    // ------------------------------------------------------------------------
    class command_t
    {
      public:

        command_t(
            const std::string &name,
            const std::vector<std::string> &arguments
        );

        virtual ~command_t(void);

        virtual int run(void) = 0;

        virtual bool parse_options(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

      protected:

        const std::string
            name;
        options_t
            options;

      private:

    };
}

#endif
