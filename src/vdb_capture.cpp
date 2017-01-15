#include "vdb_associations.h"
#include "vdb_capture.h"
#include "vdb_capture_help.h"
#include "vdb_common.h"
#include "vdb_entities.h"
#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_filter.h"
#include "vdb_fires.h"
#include "vdb_lasers.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_scan.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

namespace
{
    vdb::capture_t
        capture;
}

bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
void signal_handler(int value)
{
    capture.capturing = false;
}

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-capture", argc, argv);
    int
        result = 1;

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
    options.add(OPTION_HELP);
    options.add(OPTION_VERBOSE);
    options.add(OPTION_VERSION);
    options.add(vdb::option_t("scan", 'S', true));
    options.add(vdb::option_t("scanall", 'A', false));

    options.set_callback(*option_callback);

    if (options.parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_help();
            result = 0;
        }
        else
        {
            result = capture.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'A')
    {
        vdb::scan::scan_all();
    }
    else if (option.short_option == 'S')
    {
        success = vdb::scan::parse(value);

        if (not success)
        {
            std::cerr << "vdb-capture: invalid scan parameters: "
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
int vdb::capture_t::run(void)
{
    int result = 1;

    // Port argument (1st) required, file argument (2nd) optional
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb-capture: missing port argument" << std::endl;
    }
    else if (options::command_arguments.size() > 2)
    {
        std::cerr << "vdb-capture: too many arguments" << std::endl;
    }
    else if (not vdis::to_int32(options::command_arguments[0], port))
    {
        std::cerr << "vdb-capture: invalid port argument '"
                  << options::command_arguments[0] << "'" << std::endl;
    }
    else
    {
        capturing = true;

        // Was filename provided?  If so check overwrite and open it for
        // output.
        //
        if (options::options::command_arguments.size() > 1)
        {
            std::string
                filename = options::command_arguments[1];
            struct stat
                file_stat;

            LOG_VERBOSE("Checking file '%s'...", filename.c_str());

            if (stat(filename.c_str(), &file_stat) == 0)
            {
                std::cout << "Overwrite '" << filename << "'? ";
                capturing = user_confirmation();
            }

            if (capturing)
            {
                file_ptr = new file_writer_t(filename);

                // Ensure that file was successfully opened.
                //
                capturing = file_ptr->get_file();
            }
        }

        if (capturing)
        {
            vdis::set_byteswapping();
            vdis::enumerations::load();
            vdis::entity_types::load();
            vdis::object_types::load();

            open_socket();
            register_signal();
            start();
            print_stats(std::cout);
            result = 0;
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::capture_t::open_socket(void)
{
    const char
        *address_ptr = 0;

    if (not vdis::network_options::address.empty())
    {
        address_ptr = vdis::network_options::address.c_str();
    }

    socket_ptr = new vdis::receive_socket_t(
        vdis::network_options::ipv6,
        port,
        address_ptr);
}

// ----------------------------------------------------------------------------
// Register handler for the interrupt signal.
//
void vdb::capture_t::register_signal(void)
{
    struct sigaction
        action;

    std::memset(&action, 0, sizeof(action));

    action.sa_handler = signal_handler;

    sigaction(SIGINT, &action, NULL);
}

// ----------------------------------------------------------------------------
void vdb::capture_t::start(void)
{
    uint32_t
        index_counter = 0;
    pdu_data_t
        data;
    vdis::byte_buffer_t
        pdu_buffer;
    vdis::pdu_t
        *pdu_ptr = 0;

    capturing = true;

    while(capturing)
    {
        int32_t bytes_read = socket_ptr->receive(pdu_buffer);

        if (bytes_read > 0)
        {
            if (socket_ptr->socket_ipv6)
            {
                data.set_source(
                    socket_ptr->address_ipv6(),
                    socket_ptr->socket_port);
            }
            else
            {
                data.set_source(
                    socket_ptr->address_ipv4(),
                    socket_ptr->socket_port);
            }

            if (logger::is_enabled(logger::VERBOSE))
            {
                LOG_VERBOSE(
                    "Received %d bytes from %s...",
                    bytes_read,
                    data.get_source().c_str(),
                    port);
            }

            data.set_time(vdis::get_system_time());
            data.set_pdu_buffer(pdu_buffer.buffer(), bytes_read);

            bytes_received += (uint64_t)bytes_read;
            pdus_received += 1U;

            if (filter::filter_by_header(data) and
                filter::filter_by_metadata(data))
            {
                pdu_ptr = data.generate_pdu();

                if (pdu_ptr)
                {
                    if (filter::filter_by_content(*pdu_ptr))
                    {
                        data.set_index(index_counter++);

                        bytes_accepted += data.get_pdu_length();
                        pdus_accepted += 1U;

                        LOG_VERBOSE(
                            "Accepted %d bytes from %s...",
                            data.get_pdu_length(),
                            data.get_source().c_str());

                        process_pdu(data, *pdu_ptr);
                    }

                    delete pdu_ptr;
                    pdu_ptr = 0;
                }
            }

            data.clear();
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::capture_t::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (file_ptr)
    {
        file_ptr->write_pdu_data(data);
    }

    if (scan::scanning)
    {
        scan::process_pdu(data, pdu);

        if (scan::associations)
        {
            associations::process_pdu(data, pdu);
        }

        if (scan::lasers)
        {
            lasers::process_pdu(data, pdu);
        }

        if (scan::fires)
        {
            fires::process_pdu(data, pdu);
        }

        if (scan::collisions)
        {
            // TODO scan_collisions
        }

        if (scan::objects)
        {
            // TODO scan_objects
        }
    }
    else
    {
        print::print_pdu(data, pdu, std::cout);
    }
}

// ----------------------------------------------------------------------------
void vdb::capture_t::print_stats(std::ostream &stream)
{
    stream << std::endl
           << "Summary for port " << port << ": " << std::endl
           << "PDUs received: " << pdus_received
           << " (" << bytes_received << " bytes)" << std::endl
           << "PDUs accepted: " << pdus_accepted
           << " (" << bytes_accepted << " bytes)" << std::endl;
}
