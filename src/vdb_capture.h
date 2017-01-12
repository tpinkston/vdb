#ifndef VDB_CAPTURE_H
#define VDB_CAPTURE_H

#include "vdb_system.h"

#include "vdis_network.h"

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class file_writer_t;
    class pdu_data_t;

    class capture
    {
      public:

        static int capture_pdus(void);

      private:

        static void open_socket(void);
        static void close_socket(void);

        static void register_signal(void);

        static void start(void);

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu);

        static void print_stats(std::ostream &stream);

        static void signal_handler(int value);

        static file_writer_t
            *file_ptr;
        static vdis::receive_socket_t
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
