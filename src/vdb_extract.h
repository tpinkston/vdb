#ifndef VDB_EXTRACT_H
#define VDB_EXTRACT_H

#include "vdb_command.h"
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
    class extract_t : public file_read_command_t
    {
      public:

        extract_t(
            const std::string &command,
            const std::vector<std::string> &arguments
        );

        virtual ~extract_t(void);

        virtual int run(void);

      protected:

        bool verify_command_arguments(void);

        bool process_pdu_data(const pdu_data_t &);

        void write_to_output_file(const pdu_data_t &, const vdis::pdu_t &);

        std::string
            output_file;
        int64_t
            index;
        bool
            pdu_found;
    };
}

#endif
