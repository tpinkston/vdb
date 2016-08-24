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

#ifndef VDB_PLAYBACK_H
#define VDB_PLAYBACK_H

namespace vdb
{
    class pdu_t;
    class playback_socket_t;

    class playback
    {
      public:

        static int playback_pdus(void);

      private:

        static void open_socket(void);
        static void close_socket(void);

        static bool process_pdu_data(const pdu_data_t &data);

        static void send_pdu(const pdu_data_t &pdu_data, const pdu_t &pdu);

        static void register_signal(void);

        static void print_stats(std::ostream &stream);

        static void signal_handler(int value);

        static playback_socket_t
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
