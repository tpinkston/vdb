#ifndef VDB_SUMMARIZE_H
#define VDB_SUMMARIZE_H

#include "vdb_file_reader.h"

#include "vdis_pdus.h"

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    // Extracts a single PDU (at index) from capture file and writes it to
    // an output file.
    //
    class extract_t : file_read_callback_t
    {
      public:

        extract_t(void) : index(-1), pdu_found(false) { }
        ~extract_t(void) { }

        int run(void);

        std::string
            output_file;
        int64_t
            index;

      protected:

        bool verify_command_arguments(void);

        bool process_pdu_data(const pdu_data_t &);

        void write_to_output_file(const pdu_data_t &, const vdis::pdu_t &);

        bool
            pdu_found;
    };
}

#endif
