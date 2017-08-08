#include "vdb_common.h"
#include "vdb_filter.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_playback.h"
#include "vdb_playback_help.h"
#include "vdb_print.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_network.h"
#include "vdis_object_types.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    vdb::playback_t
        playback;
}

bool playback_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
void signal_handler(int value)
{
    playback.playing_back = false;
}

// ----------------------------------------------------------------------------
int playback_main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-playback", argc, argv);
    int
        result = 1;

    options.add(vdb::option_t("pdu", 'P', true));
    options.add(OPTION_ADDRESS);
    options.add(OPTION_INTERFACE);
    options.add(OPTION_IPV6);
    options.add(OPTION_EXTRA);
    options.add(OPTION_EXTRACT);
    options.add(OPTION_DUMP);
    options.add(OPTION_COLOR);
    options.add(OPTION_ERRORS);
    options.add(OPTION_WARNINGS);
    options.add(OPTION_HOSTNAME);
    options.add(OPTION_XHOSTNAME);
    options.add(OPTION_EXERCISE);
    options.add(OPTION_XEXERCISE);
    options.add(OPTION_TYPE);
    options.add(OPTION_XTYPE);
    options.add(OPTION_FAMILY);
    options.add(OPTION_XFAMILY);
    options.add(OPTION_ID);
    options.add(OPTION_XID);
    options.add(OPTION_RANGE);
    options.add(OPTION_HELP);
    options.add(OPTION_VERBOSE);
    options.add(OPTION_VERSION);

    options.set_callback(*playback_option_callback);

    if (options.parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_playback_help();
            result = 0;
        }
        else
        {
            vdb::print::print_pdu_source_time = false;

            result = playback.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool playback_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    uint64_t
        interval = 0;
    bool
        result = true;

    if (option.short_option == 'P')
    {
        success = vdis::to_uint64(value, interval);

        if (success)
        {
            playback.set_pdu_interval(interval);
        }
        else
        {
            std::cerr << "vdb-playback: invalid value for PDU interval: "
                      << value << std::endl;
        }
    }
    else
    {
        result = false;
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::playback_t::run(void)
{
    int result = 1;

    // Port argument (1st) required, file argument (2nd) required
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb-playback: missing playback arguments" << std::endl;
    }
    else if (options::command_arguments.size() < 2)
    {
        std::cerr << "vdb-playback: too few arguments" << std::endl;
    }
    else if (options::command_arguments.size() > 2)
    {
        std::cerr << "vdb-playback: too many arguments" << std::endl;
    }
    else if (not vdis::to_int32(options::command_arguments[0], port))
    {
        std::cerr << "vdb-playback: invalid port argument '"
                  << options::command_arguments[0] << "'" << std::endl;
    }
    else
    {
        const string_t
            filename = options::command_arguments[1];
        file_reader_t
            *reader_ptr = 0;

        LOG_EXTRA_VERBOSE("Starting playback...");

        if (pdu_interval > 0)
        {
            reader_ptr = new pdu_reader_t(filename, pdu_interval);
        }
        else
        {
            reader_ptr = new standard_reader_t(filename);
        }

        if (not reader_ptr->good())
        {
            result = 1;
        }
        else
        {
            vdis::set_byteswapping();
            vdis::enumerations::load();
            vdis::entity_types::load();
            vdis::object_types::load();

            open_socket();
            register_signal();

            capture_start_time = 0;
            playback_start_time = 0;
            playing_back = true;

            if (reader_ptr->parse(this))
            {
                print_stats(std::cout);
                result = 0;
            }
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::playback_t::open_socket(void)
{
    const char
        *address_ptr = 0;

    // Default address for 'vdis::send_socket_t' is 'broadcast'
    //
    if (not vdis::network_options::address.empty())
    {
        address_ptr = vdis::network_options::address.c_str();
    }

    socket_ptr = new vdis::send_socket_t(
        vdis::network_options::ipv6,
        port,
        address_ptr);
}

// ----------------------------------------------------------------------------
bool vdb::playback_t::process_pdu_data(const pdu_data_t &data)
{
    bool
        past_end = false;

    if (filter::filter_by_range(data.get_index(), past_end))
    {
        if (filter::filter_by_header(data) and
            filter::filter_by_metadata(data))
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
void vdb::playback_t::send_pdu(
    const pdu_data_t &pdu_data,
    const vdis::pdu_t &pdu)
{
    vdis::byte_buffer_t pdu_buffer(
        pdu_data.get_pdu_buffer(),
        pdu_data.get_pdu_length(),
        false);

    if (capture_start_time == 0)
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

        // Pause and wait if needed before sending next PDU.
        //
        if (delay_time > 0)
        {
            pause((uint32_t)delay_time);
        }
    }

    bytes_sent += pdu_data.get_pdu_length();
    pdus_sent += 1;

    print::print_pdu(pdu_data, pdu, std::cout);

    socket_ptr->send(pdu_buffer);
}

// ----------------------------------------------------------------------------
// Register handler for the interrupt signal.
//
void vdb::playback_t::register_signal(void)
{
    struct sigaction
        action;

    std::memset(&action, 0, sizeof(action));

    action.sa_handler = signal_handler;

    sigaction(SIGINT, &action, NULL);
}

// ----------------------------------------------------------------------------
void vdb::playback_t::print_stats(std::ostream &stream)
{
    stream << std::endl
           << "PDUs sent: " << std::fixed << pdus_sent
           << " (" << std::fixed << bytes_sent << " bytes)" << std::endl;
}
