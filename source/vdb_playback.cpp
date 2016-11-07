#include "vdb_common.h"
#include "vdb_file_reader.h"
#include "vdb_filter.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_playback.h"
#include "vdb_print.h"

#include "vdis_logger.h"
#include "vdis_network.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

vdis::send_socket_t
    *vdb::playback::socket_ptr;
int32_t
    vdb::playback::port = 0;
uint32_t
    vdb::playback::bytes_sent = 0,
    vdb::playback::pdus_sent = 0;
uint64_t
    vdb::playback::capture_start_time = 0,
    vdb::playback::playback_start_time = 0;
bool
    vdb::playback::started = false,
    vdb::playback::playing_back = false;

// ----------------------------------------------------------------------------
int vdb::playback::playback_pdus(void)
{
    int result = 0;

    // Port argument (1st) required, file argument (2nd) required
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb playback: missing playback arguments" << std::endl;

        result = 1;
    }
    else if (options::command_arguments.size() < 2)
    {
        std::cerr << "vdb playback: too few arguments" << std::endl;

        result = 1;
    }
    else if (options::command_arguments.size() > 2)
    {
        std::cerr << "vdb playback: too many arguments" << std::endl;

        result = 1;
    }
    else if (not vdis::to_int32(options::command_arguments[0], port))
    {
        std::cerr << "vdb playback: invalid port argument '"
                  << options::command_arguments[0] << "'" << std::endl;

        result = 1;
    }
    else
    {
        const string_t
            filename = options::command_arguments[1];
        file_reader_t
            *reader_ptr = 0;

        LOG_EXTRA_VERBOSE("Starting playback...");

        reader_ptr = new standard_reader_t(filename);

        if (not reader_ptr->good())
        {
            result = 1;
        }
        else
        {
            open_socket();
            register_signal();

            playing_back = true;

            if (not reader_ptr->parse(process_pdu_data))
            {
                result = 1;
            }
            else
            {
                print_stats(std::cout);
            }

            close_socket();
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::playback::open_socket(void)
{
    const char
        *address_ptr = 0;

    // Default address for 'vdis::send_socket_t' is 'broadcast'
    //
    if (not options::network_address.empty())
    {
        address_ptr = options::network_address.c_str();
    }

    socket_ptr = new vdis::send_socket_t(
        options::ipv6,
        port,
        address_ptr);
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
bool vdb::playback::process_pdu_data(const pdu_data_t &data)
{
    bool
        past_end = false;

    if (filter::filter_by_range(data.get_index(), past_end))
    {
        if (filter::filter_by_header(data))
        {
            const vdis::pdu_t *pdu_ptr = data.generate_pdu();

            if (pdu_ptr)
            {
                if (filter::filter_by_content(*pdu_ptr))
                {
                    send_pdu(data, *pdu_ptr);
                }

                delete pdu_ptr;
                pdu_ptr = 0;
            }
        }
    }

    return (playing_back and not past_end);
}

// ----------------------------------------------------------------------------
void vdb::playback::send_pdu(
    const pdu_data_t &pdu_data,
    const vdis::pdu_t &pdu)
{
    vdis::byte_buffer_t pdu_buffer(
        pdu_data.get_pdu_buffer(),
        pdu_data.get_pdu_length(),
        false);

    if (not started)
    {
        // First PDU to send marks the start times.
        //
        capture_start_time = pdu_data.get_time();
        playback_start_time = vdis::get_system_time();
    }
    else
    {
        const uint64_t
            capture_elapsed = (pdu_data.get_time() - capture_start_time),
            playback_elapsed = (vdis::get_system_time() - playback_start_time);
        int32_t
            delay_time = (int32_t)(capture_elapsed - playback_elapsed);

        LOG_EXTRA_VERBOSE(
            "Time elapsed: capture = %d, playback = %d...",
            capture_elapsed,
            playback_elapsed);

        if (delay_time > 0)
        {
            pause((uint32_t)delay_time);
        }
    }

    bytes_sent += pdu_data.get_pdu_length();
    pdus_sent += 1;

    print::print_pdu(pdu_data, pdu, std::cout);

    socket_ptr->send(pdu_buffer);

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
    playing_back = false;
}
