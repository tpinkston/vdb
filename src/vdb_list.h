#ifndef VDB_LIST_H
#define VDB_LIST_H

#include "vdb_command.h"

namespace vdb
{
    class file_writer_t;
    class pdu_data_t;

    // Lists PDUs in a capture file and optionally outputs PDUs to a separate
    // capture file which is useful when filtering.
    //
    class list_t : public file_read_command_t
    {
      public:

        list_t(
            const std::string &command,
            const std::vector<std::string> &arguments
        );

        ~list_t(void);

        virtual int run(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

        virtual bool process_pdu_data(const pdu_data_t &data);

      protected:

        file_writer_t
            *file_ptr;
        std::string
            output_file;
        uint32_t
            output_index,
            pdus_listed,
            pdus_filtered_out;
    };
}

#endif
