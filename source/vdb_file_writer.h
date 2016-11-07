#ifndef VDB_FILE_WRITER_H
#define VDB_FILE_WRITER_H

#include "vdb_system.h"

namespace vdb
{
    class pdu_data_t;

    class file_writer_t
    {
      public:

        file_writer_t(
            const std::string &filename,
            const std::string *comment_ptr = 0
        );

        ~file_writer_t(void);

        inline const FILE *get_file(void) { return file_ptr; }

        void write_pdu_data(const pdu_data_t &data);

        const std::string
            filename;

      protected:

        FILE
            *file_ptr;
    };
}

#endif
