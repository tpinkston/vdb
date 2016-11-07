#ifndef VDB_FILE_READER_H
#define VDB_FILE_READER_H

#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_system.h"

namespace vdb
{
    class pdu_data_t;

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
    // Reader for PCAP format capture files
    //
    class pcap_reader_t : public file_reader_t
    {
      public:

        pcap_reader_t(const string_t &filename);
        virtual ~pcap_reader_t(void);

      protected:

        virtual bool next_entry(pdu_data_t &data);

      private:

//        bool read_pcap_entry(
//            const pcap_packet_header_t &header,
//            vdis::byte_stream_t &stream,
//            pdu_data_t &data
//        );
//
//        pcap_t
//            *pcap_ptr;
//        uint32_t
//            index_counter;
    };
}

#endif
