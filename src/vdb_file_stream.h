#ifndef VDB_FILE_STREAM_H
#define VDB_FILE_STREAM_H

#include "vdis_byte_stream.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class file_stream : public vdis::byte_stream_t
    {
      public:

        file_stream(void);
        file_stream(uint32_t length);
        virtual ~file_stream(void);

        virtual bool read_file(const string_t &filename);
        virtual void write_file(const string_t &filename);

        inline bool ferror(void) const { return file_error; }

      protected:

        bool
            file_error;
    };
}

#endif
