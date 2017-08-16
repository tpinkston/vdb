#include "vdb_common.h"
#include "vdb_filter.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_playback.h"
#include "vdb_print.h"

#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_network.h"
#include "vdis_object_types.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

std::list<vdb::playback_t *>
    vdb::playback_t::instances;

// ----------------------------------------------------------------------------
vdb::playback_t::playback_t(
    const std::string &command,
    const std::vector<std::string> &arguments
) :
    file_read_command_t(command, arguments),
    socket_ptr(0),
    port(0),
    bytes_sent(0),
    pdus_sent(0),
    pdu_interval(0),
    capture_start_time(0),
    playback_start_time(0),
    playing_back(false)
{
    instances.push_back(this);

    options.add(vdb::option_t("pdu", 'P', true));
    options.add(OPTION_ADDRESS);
    options.add(OPTION_INTERFACE);
    options.add(OPTION_IPV6);
    options.add(OPTION_EXTRACT);
    options.add(OPTION_DUMP);
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
}

// ----------------------------------------------------------------------------
vdb::playback_t::~playback_t(void)
{
    instances.remove(this);

    if (socket_ptr)
    {
        delete socket_ptr;
        socket_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
int vdb::playback_t::run(void)
{
    int result = 1;

    // Port argument (1st) required, file argument (2nd) required
    //
    if (options::command_arguments.empty())
    {
        LOG_FATAL("Missing arguments");
    }
    else if (options::command_arguments.size() < 2)
    {
        LOG_FATAL("Too few arguments");
    }
    else if (options::command_arguments.size() > 2)
    {
        LOG_FATAL("Too many arguments");
    }
    else if (not vdis::to_int32(options::command_arguments[0], port))
    {
        LOG_FATAL(
            "Invalid port: %s",
            options::command_arguments[0].c_str());
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

        if (reader_ptr->good())
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
bool vdb::playback_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'P')
    {
        success = vdis::to_uint64(value, pdu_interval);

        if (not success)
        {
            LOG_FATAL("Invalid PDU interval: %s", value.c_str());
        }
    }
    else
    {
        result = false;
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::playback_t::process_pdu_data(const pdu_data_t &data)
{
    bool past_end = false;

    if (debug_enabled())
    {
        CONSOLE_OUT("DEBUG: processing PDU #%d...", data.get_index());
    }

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
void vdb::playback_t::signal_handler(int value)
{
    std::list<playback_t *>::iterator
        itor = instances.begin();

    if (debug_enabled())
    {
        CONSOLE_OUT("DEBUG: caught signal...");
    }

    while(itor != instances.end())
    {
        (*itor)->playing_back = false;

        ++itor;
    }
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

    if (debug_enabled())
    {
        CONSOLE_OUT("DEBUG: signal registered...");
    }
}

// ----------------------------------------------------------------------------
void vdb::playback_t::print_stats(std::ostream &stream)
{
    stream << std::endl
           << "PDUs sent: " << std::fixed << pdus_sent
           << " (" << std::fixed << bytes_sent << " bytes)" << std::endl;
}
