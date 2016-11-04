#ifndef VDB_OUTPUT_FILE_H
#define VDB_OUTPUT_FILE_H

#include "vdb_system.h"

namespace vdb
{
    class pdu_data_t;

    class output_file_t
    {
      public:

        output_file_t(
            const std::string &filename,
            const std::string *comment_ptr = 0
        );

        ~output_file_t(void);

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
