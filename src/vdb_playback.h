#ifndef VDB_PLAYBACK_H
#define VDB_PLAYBACK_H

#include "vdb_command.h"
#include "vdb_file_reader.h"

namespace vdis
{
    class pdu_t;
    class send_socket_t;
}

namespace vdb
{
    class playback_t : public command_t, public file_read_callback_t
    {
      public:

        playback_t(
            const std::string &command,
            const std::vector<std::string> &arguments
        );

        virtual ~playback_t(void);

        virtual int run(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

      protected:

        static void signal_handler(int value);

      private:

        void open_socket(void);

        virtual bool process_pdu_data(const pdu_data_t &);

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
        bool
            playing_back;
        static std::list<playback_t *>
            instances;
    };
}

#endif
