#ifndef VDB_FILE_TYPES_H
#define VDB_FILE_TYPES_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class byte_stream_t;
}

namespace vdb
{
    class file_header_comment_t;
    class file_header_record_t;

    // ------------------------------------------------------------------------
    // Class defining parameters for the file header that starts all files
    // written and read by this program.
    //
    class file_header_t
    {
      public:

        file_header_t(void);
        ~file_header_t(void);

        std::ostream &print_comment(
            const string_t &prefix,
            uint32_t index,
            std::ostream &stream
        ) const;

        bool invalid_title(void) const;

        void print(const string_t &prefix, std::ostream &) const;

        void clear(void);

        uint32_t length(void) const;
        uint32_t comments_length(void) const;

        void read(vdis::byte_stream_t &stream);
        void write(vdis::byte_stream_t &stream) const;

        uint8_t
            title[10];
        uint16_t
            major_version,
            minor_version;
        uint16_t
            status; // bit flags
        uint64_t
            time_created;
        uint32_t
            padding;
        std::vector<file_header_record_t>
            records;
        std::vector<file_header_comment_t>
            comments;

      protected:

        static const uint32_t
            BASE_RECORD_SIZE;
    };

    // ------------------------------------------------------------------------
    class file_header_comment_t
    {
      public:

        file_header_comment_t(void);
        file_header_comment_t(const file_header_comment_t &copy);
        ~file_header_comment_t(void);

        void print(const string_t &prefix, std::ostream &) const;

        void clear(void);

        uint32_t length(void) const;

        void read(vdis::byte_stream_t &stream);
        void write(vdis::byte_stream_t &stream) const;

        uint64_t
            time;
        string_t
            value;

      protected:

        static const uint32_t
            BASE_RECORD_SIZE,
            BOUNDARY_SIZE;
    };

    // ------------------------------------------------------------------------
    class file_header_record_t
    {
      public:

        file_header_record_t(void);
        file_header_record_t(const file_header_record_t &copy);
        ~file_header_record_t(void);

        void print(const string_t &prefix, std::ostream &) const;

        void clear(void);

        uint32_t length(void) const { return RECORD_SIZE; }

        void read(vdis::byte_stream_t &stream);
        void write(vdis::byte_stream_t &stream) const;

        uint32_t
            type,
            status; // bit flags
        uint64_t
            data;

      protected:

        static const uint32_t
            RECORD_SIZE;
    };
}

// ----------------------------------------------------------------------------
inline vdb::file_header_t::file_header_t(void) :
    title({ 'v', 'd', 'b', 'c', 'a', 'p', 't', 'u', 'r', 'e' }),
    major_version(0),
    minor_version(0),
    status(0),
    time_created(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_t::~file_header_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_comment_t::file_header_comment_t(void) : time(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_comment_t::file_header_comment_t(
    const file_header_comment_t &copy
) :
    time(copy.time),
    value(copy.value)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_comment_t::~file_header_comment_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_record_t::file_header_record_t(void) :
    type(0),
    status(0),
    data(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_record_t::file_header_record_t(
    const file_header_record_t &copy
) :
    type(copy.type),
    status(copy.status),
    data(copy.data)
{

}

// ----------------------------------------------------------------------------
inline vdb::file_header_record_t::~file_header_record_t(void)
{
    clear();
}

#endif
