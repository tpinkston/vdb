#ifndef VDB_PLAYBACK_H
#define VDB_PLAYBACK_H

#include "vdb_file_reader.h"

namespace vdis
{
    class pdu_t;
    class send_socket_t;
}

namespace vdb
{
    class playback_t : file_read_callback_t
    {
      public:

        playback_t(void) :
            playing_back(false),
            socket_ptr(0),
            port(0),
            bytes_sent(0),
            pdus_sent(0),
            pdu_interval(0),
            capture_start_time(0),
            playback_start_time(0)
        {

        }

        ~playback_t(void)
        {
            if (socket_ptr)
            {
                delete socket_ptr;
                socket_ptr = 0;
            }
        }

        void set_pdu_interval(uint64_t value)
        {
            pdu_interval = value;
        }

        int run(void);

        bool playing_back;

      private:

        void open_socket(void);

        bool process_pdu_data(const pdu_data_t &);

        void send_pdu(const pdu_data_t &, const vdis::pdu_t &);

        void register_signal(void);

        void print_stats(std::ostream &);

        vdis::send_socket_t
            *socket_ptr;
        int32_t
            port;
        uint32_t
            bytes_sent,
            pdus_sent;
        uint64_t
            pdu_interval,           // milliseconds
            capture_start_time,     // milliseconds
            playback_start_time;    // milliseconds
    };
}

#endif
