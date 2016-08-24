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

#ifndef VDB_FILE_READERS_H
#define VDB_FILE_READERS_H

#include "vdb_common.h"
#include "vdb_file_stream.h"
#include "vdb_file_types.h"

namespace vdb
{
    class pdu_data_t;

    // ------------------------------------------------------------------------
    // Abstract reader for capture files
    //
    class file_reader_t
    {
      public:

        file_reader_t(const std::string &filename);
        virtual ~file_reader_t(void);

        virtual bool read_header(void) = 0;

        virtual bool next_entry(pdu_data_t &data) = 0;

        const std::string &get_filename(void) const { return filename; }

        bool good(void) const { return not error_condition; }
        bool error(void) const { return error_condition; }

        bool parse(bool (*callback)(const pdu_data_t &));

      protected:

        const std::string
            filename;
        bool
            error_condition;
    };

    // ------------------------------------------------------------------------
    // Reader for standard format capture files
    //
    class standard_reader_t : public file_reader_t
    {
      public:

        standard_reader_t(const std::string &filename);
        virtual ~standard_reader_t(void);

        virtual bool read_header(void);

        virtual bool next_entry(pdu_data_t &data);

        file_header_t
            header;
        file_stream
            stream;
    };

    // ------------------------------------------------------------------------
    // Reader for PCAP format capture files
    //
    class pcap_reader_t : public file_reader_t
    {
      public:

        pcap_reader_t(const std::string &filename);
        virtual ~pcap_reader_t(void);

        virtual bool read_header(void);

        virtual bool next_entry(pdu_data_t &data);
    };
}

#endif
