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

#ifndef VDB_CAPTURE_H
#define VDB_CAPTURE_H

#include "vdb_common.h"

namespace vdb
{
    class capture_socket_t;
    class pdu_data_t;

    class capture
    {
      public:

        static int capture_pdus(void);

      private:

        static void open_socket(void);
        static void close_socket(void);

        static bool open_output_file(const std::string &filename);
        static bool close_output_file(const std::string &filename);

        static void write_pdu_data(const pdu_data_t &data);

        static void register_signal(void);

        static void start(void);

        static void print_stats(std::ostream &stream);

        static void signal_handler(int value);

        static FILE
            *file_ptr;
        static capture_socket_t
            *socket_ptr;
        static int32_t
            port;
        static uint32_t
            bytes_received,
            bytes_accepted,
            pdus_received,
            pdus_accepted;
        static bool
            capturing;
    };
}

#endif
