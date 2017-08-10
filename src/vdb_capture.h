#ifndef VDB_CAPTURE_H
#define VDB_CAPTURE_H

#include "vdb_command.h"
#include "vdb_file_writer.h"

#include "vdis_network.h"

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    class capture_t : public command_t
    {
      public:

        capture_t(
            const std::string &command,
            const std::vector<std::string> &arguments
        );

        virtual ~capture_t(void);

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
        bool
            capturing;
        static std::list<capture_t *>
            instances;
    };
}

#endif
