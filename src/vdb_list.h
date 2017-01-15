#ifndef VDB_LIST_H
#define VDB_LIST_H

#include "vdb_file_reader.h"

namespace vdb
{
    class file_writer_t;
    class pdu_data_t;

    // Lists PDUs in a capture file and optionally outputs PDUs to a separate
    // capture file which is useful when filtering.
    //
    class list_t : file_read_callback_t
    {
      public:

        list_t(void) :
            file_ptr(0),
            output_index(0),
            pdus_listed(0),
            pdus_filtered_out(0)
        {

        }

        ~list_t(void)
        {
            if (file_ptr)
            {
                delete file_ptr;
                file_ptr = 0;
            }
        }

        int run(void);

        std::string output_file;

      protected:

        bool process_pdu_data(const pdu_data_t &data);

        file_writer_t
            *file_ptr;
        uint32_t
            output_index,
            pdus_listed,
            pdus_filtered_out;
    };
}

#endif
