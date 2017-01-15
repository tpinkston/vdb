#ifndef VDB_CAPTURE_H
#define VDB_CAPTURE_H

#include "vdb_file_writer.h"

#include "vdis_network.h"

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    class capture_t
    {
      public:

        capture_t(void) :
            capturing(false),
            file_ptr(0),
            socket_ptr(0),
            port(0),
            bytes_received(0),
            bytes_accepted(0),
            pdus_received(0),
            pdus_accepted(0)
        {

        }

        ~capture_t(void)
        {
            if (file_ptr)
            {
                delete file_ptr;
                file_ptr = 0;
            }

            if (socket_ptr)
            {
                delete socket_ptr;
                socket_ptr = 0;
            }
        }

        int run(void);

        bool capturing;

      private:

        void open_socket(void);

        void register_signal(void);

        void start(void);

        void process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu);

        void print_stats(std::ostream &stream);

        file_writer_t
            *file_ptr;
        vdis::receive_socket_t
            *socket_ptr;
        int32_t
            port;
        uint32_t
            bytes_received,
            bytes_accepted,
            pdus_received,
            pdus_accepted;
    };
}

#endif
