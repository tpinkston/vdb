#ifndef VDB_PLAYBACK_H
#define VDB_PLAYBACK_H

#include "vdb_system.h"

namespace vdis
{
    class pdu_t;
    class send_socket_t;
}

namespace vdb
{
    class playback
    {
      public:

        static int playback_pdus(void);

      private:

        static void open_socket(void);
        static void close_socket(void);

        static bool process_pdu_data(const pdu_data_t &);

        static void send_pdu(const pdu_data_t &, const vdis::pdu_t &);

        static void register_signal(void);

        static void print_stats(std::ostream &);

        static void signal_handler(int);

        static vdis::send_socket_t
            *socket_ptr;
        static int32_t
            port;
        static uint32_t
            bytes_sent,
            pdus_sent;
        static uint64_t
            capture_start_time,
            playback_start_time;
        static bool
            started,
            playing_back;
    };
}

#endif
