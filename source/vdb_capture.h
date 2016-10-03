#ifndef VDB_CAPTURE_H
#define VDB_CAPTURE_H

#include "vdb_system.h"

#include "vdis_data_types.h"

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

        static bool open_output_file(const string_t &filename);
        static bool close_output_file(const string_t &filename);

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
