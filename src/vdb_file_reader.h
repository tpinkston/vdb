#ifndef VDB_FILE_READER_H
#define VDB_FILE_READER_H

#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_pdu_data.h"
#include "vdb_system.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Abstract reader for capture files
    //
    class file_reader_t
    {
      public:

        file_reader_t(const string_t &filename);
        virtual ~file_reader_t(void);

        const string_t &get_filename(void) const { return filename; }

        // Returns true if no errors have occurred.
        //
        bool good(void) const { return not error_condition; }

        // Returns true if error has occurred.
        //
        bool error(void) const { return error_condition; }

        bool parse(bool (*callback)(const pdu_data_t &));

      protected:

        // Reads the next entry from the file, returns false when there are
        // no more entries to be read (data will not valid when false is
        // returned).
        //
        virtual bool next_entry(pdu_data_t &data) = 0;

        const string_t
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

        standard_reader_t(const string_t &filename);
        virtual ~standard_reader_t(void);

        file_header_t
            header;
        file_stream
            stream;

      protected:

        virtual bool next_entry(pdu_data_t &data);
    };

    // ------------------------------------------------------------------------
    // Reader for file containing single PDU contents to be played back at
    // regular intervals
    //
    class pdu_reader_t : public file_reader_t
    {
      public:

        pdu_reader_t(const string_t &filename, uint32_t interval);
        virtual ~pdu_reader_t(void);

        const uint32_t
            interval_milliseconds;

      protected:

        virtual bool next_entry(pdu_data_t &data);

        vdis::socket_address_ipv4_t
            address_ipv4;
        vdis::socket_address_ipv6_t
            address_ipv6;
        uint32_t
            index;
    };
}

#endif
