#ifndef VDB_COMMAND_H
#define VDB_COMMAND_H

#include "vdb_options.h"

namespace vdb
{
    class pdu_data_t;

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

    // ------------------------------------------------------------------------
    class file_read_command_t : public command_t
    {
      public:

        file_read_command_t(
            const std::string &name,
            const std::vector<std::string> &arguments
        );

        virtual ~file_read_command_t(void);

        virtual bool process_pdu_data(const pdu_data_t &) = 0;
    };
}

#endif
