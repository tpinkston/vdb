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

#ifndef VDB_FILE_HANDLER_H
#define VDB_FILE_HANDLER_H

#include "vdb_common.h"
#include "vdb_file_stream.h"
#include "vdb_file_types.h"

namespace vdb
{
    class pdu_t;
    class pdu_data_t;

    // ------------------------------------------------------------------------
    // Handler for VDB capture files.  On construction the file is opened for
    // reading, the file contents read into the 'buffer' and the file header
    // restored from the 'buffer' and verified.
    //
    class file_handler
    {
      protected:

        file_handler(void);
        virtual ~file_handler(void);

        virtual bool open_output_file(void);
        virtual bool close_output_file(void);

        // Writes PDU data to output file
        //
        virtual void write_pdu_data(const pdu_data_t &data);

        // Reads input file into 'buffer' and restores 'header'
        //
        virtual bool read_content(void);

        // Parses input file content.
        //
        virtual void parse_content(void);

        // Callback for content parsing, does nothing by default, re-implement
        // if needed.
        //
        virtual void process_pdu_data(const pdu_data_t &data);

        // Data filters can be checked with just the data without having
        // to restore the PDU.
        //
        virtual bool passes_data_filters(const pdu_data_t &data);

        // Checking PDU filters requires the 'pdu_t' object be restored.
        //
        virtual bool passes_pdu_filters(const pdu_t &pdu);

        std::string
            filename;
        FILE
            *file_ptr;
        file_stream
            stream;
        file_header_t
            header;
    };
}

#endif
