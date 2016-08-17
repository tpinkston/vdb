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

#include "vdb_command.h"
#include "vdb_file_handler.h"

namespace vdb
{
    class capture_socket_t;
    class pdu_data_t;

    class capture : public file_handler, public command
    {
      public:

        capture(void);
        virtual ~capture(void);

        virtual int run(void);

      protected:

        void open_socket(void);
        void close_socket(void);

        void register_signal(void);

        void start(void);

        void print_stats(std::ostream &stream);

        static void signal_handler(int value);

        capture_socket_t
            *socket_ptr;
        int32_t
            port;
        uint32_t
            bytes_received,
            bytes_accepted,
            pdus_received,
            pdus_accepted;
        bool
            capturing;
    };
}

#endif
