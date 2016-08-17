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

#include "vdb_common.h"
#include "vdb_logger.h"
#include "vdb_network.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_pdu.h"
#include "vdb_playback.h"
#include "vdb_print.h"
#include "vdb_string.h"

namespace
{
    vdb::playback
        *instance_ptr = 0;
}

// ----------------------------------------------------------------------------
vdb::playback::playback(void) :
    socket_ptr(0),
    port(0),
    bytes_sent(0),
    pdus_sent(0),
    capture_start_time(0),
    playback_start_time(0),
    started(false),
    playing_back(true)
{
    if (instance_ptr)
    {
        LOG_ERROR("Already instantiated!");
        exit(2);
    }

    instance_ptr = this;
}

// ----------------------------------------------------------------------------
vdb::playback::~playback(void)
{
    instance_ptr = 0;
}

// ----------------------------------------------------------------------------
int vdb::playback::run(void)
{
    int result = 0;

    // Port argument (1st) required, file argument (2nd) required
    //
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " playback: missing playback arguments" << std::endl;

        result = 1;
    }
    else if (options::get_command_arguments().size() < 2)
    {
        std::cerr << options::get_terminal_command()
                  << " playback: too few arguments" << std::endl;

        result = 1;
    }
    else if (options::get_command_arguments().size() > 2)
    {
        std::cerr << options::get_terminal_command()
                  << " playback: too many arguments" << std::endl;

        result = 1;
    }
    else if (not string_to_int32(*options::get_command_argument(0), port))
    {
        std::cerr << options::get_terminal_command()
                  << " playback: invalid port argument '"
                  << *options::get_command_argument(0) << "'" << std::endl;

        result = 1;
    }
    else
    {
        LOG_EXTRA_VERBOSE("Starting playback...");

        filename = *options::get_command_argument(1);

        open_socket();
        register_signal();
        read_content();
        parse_content();
        print_stats(std::cout);
        close_socket();
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::playback::open_socket(void)
{
    const char
        *address_ptr = 0;

    if (options::string(OPT_NET_ADDRESS))
    {
        address_ptr = options::string(OPT_NET_ADDRESS)->c_str();
    }

    socket_ptr = new playback_socket_t(
        address_ptr,
        port,
        options::flag(OPT_IPV6));
}

// ----------------------------------------------------------------------------
void vdb::playback::close_socket(void)
{
    // Actual closing of the socket is handled in destructor
    //
    delete socket_ptr;
    socket_ptr = 0;
}

// ----------------------------------------------------------------------------
void vdb::playback::process_pdu_data(const pdu_data_t &pdu_data)
{
    bool
        past_end = false;

    if (options::pdu_index_in_range(pdu_data.get_index(), past_end))
    {
        if (passes_data_filters(pdu_data))
        {
            const pdu_t
                *pdu_ptr = pdu_data.generate_pdu();

            if (pdu_ptr)
            {
                if (passes_pdu_filters(*pdu_ptr))
                {
                    send_pdu(pdu_data, *pdu_ptr);
                }

                delete pdu_ptr;
                pdu_ptr = 0;
            }
        }
    }

    if (past_end or not playing_back)
    {
        // Puts index at the end of the buffer to terminate any
        // more processing.
        //
        stream.reset_index(stream.get_length());
    }
}

// ----------------------------------------------------------------------------
void vdb::playback::send_pdu(const pdu_data_t &pdu_data, const pdu_t &pdu)
{
    if (not started)
    {
        // First PDU to send marks the start times.
        //
        capture_start_time = pdu_data.get_time();
        playback_start_time = time::get_system();
    }
    else
    {
        const uint64_t
            capture_time_elapsed = (pdu_data.get_time() - capture_start_time),
            playback_time_elapsed = (time::get_system() - playback_start_time);
        int32_t
            delay_time = (int32_t)(capture_time_elapsed - playback_time_elapsed);

        LOG_EXTRA_VERBOSE(
            "Time elapsed: capture = %d, playback = %d...",
            capture_time_elapsed,
            playback_time_elapsed);
        LOG_VERBOSE("Delay time is %d...", delay_time);

        if (delay_time > 0)
        {
            pause((uint32_t)delay_time);
        }
    }

    bytes_sent += pdu_data.get_pdu_length();
    pdus_sent += 1;

    print::print_pdu(pdu_data, pdu, std::cout);

    socket_ptr->send(pdu_data);

    started = true;
}

// ----------------------------------------------------------------------------
// Register handler for the interrupt signal.
//
void vdb::playback::register_signal(void)
{
    struct sigaction
        action;

    std::memset(&action, 0, sizeof(action));

    action.sa_handler = signal_handler;

    sigaction(SIGINT, &action, NULL);
}

// ----------------------------------------------------------------------------
void vdb::playback::print_stats(std::ostream &stream)
{
    stream << std::endl
           << "PDUs sent: " << std::fixed << pdus_sent
           << " (" << std::fixed << bytes_sent << " bytes)" << std::endl;
}


// ----------------------------------------------------------------------------
void vdb::playback::signal_handler(int value)
{
    if (instance_ptr)
    {
        instance_ptr->playing_back = false;
    }
}
