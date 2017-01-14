#include "vdb_associations.h"
#include "vdb_capture.h"
#include "vdb_common.h"
#include "vdb_entities.h"
#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_file_writer.h"
#include "vdb_filter.h"
#include "vdb_fires.h"
#include "vdb_lasers.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_version.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

vdb::file_writer_t
    *vdb::capture::file_ptr = 0;
vdis::receive_socket_t
    *vdb::capture::socket_ptr = 0;
int32_t
    vdb::capture::port = 0;
uint32_t
    vdb::capture::bytes_received = 0,
    vdb::capture::bytes_accepted = 0,
    vdb::capture::pdus_received = 0,
    vdb::capture::pdus_accepted = 0;
bool
    vdb::capture::capturing = false;

bool option_callback(const vdb::option_t &option, const char *value);

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    vdb::options_t options("vdb-capture", argc, argv);

    options.add(vdb::option_t("version",     'V', false));
    options.add(vdb::option_t("help",        'h', false));
    options.add(vdb::option_t("ipv6",        '6', false));
    options.add(vdb::option_t("address",     'a', true));
    options.add(vdb::option_t("interface",   'N', true));
    options.add(vdb::option_t("extra",       'e', false));
    options.add(vdb::option_t("extract",     'x', false));
    options.add(vdb::option_t("dump",        'd', false));
    options.add(vdb::option_t("color",       'c', false));
    options.add(vdb::option_t("errors",      'E', false));
    options.add(vdb::option_t("warnings",    'W', false));
    options.add(vdb::option_t("verbose",     'v', false));
    options.add(vdb::option_t("hostname",    'o', true));
    options.add(vdb::option_t("xhostname",   'O', true));
    options.add(vdb::option_t("exercise",    'y', true));
    options.add(vdb::option_t("xexercise",   'Y', true));
    options.add(vdb::option_t("type",        't', true));
    options.add(vdb::option_t("xtype",       'T', true));
    options.add(vdb::option_t("family",      'f', true));
    options.add(vdb::option_t("xfamily",     'F', true));
    options.add(vdb::option_t("id",          'i', true));
    options.add(vdb::option_t("xid",         'I', true));

    options.set_callback(*option_callback);

    if (options.parse())
    {
        std::cout << "done" << std::endl;
    }

    return 0;
}

// ----------------------------------------------------------------------------
bool option_callback(const vdb::option_t &option, const char *value)
{
    std::cout << option.long_option << "=" << (value ? value : "null") << std::endl;
    return true;
}

// ----------------------------------------------------------------------------
int vdb::capture::capture_pdus(void)
{
    int
        result = 0;

    // Port argument (1st) required, file argument (2nd) optional
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb capture: missing port argument" << std::endl;

        result = 1;
    }
    else if (options::command_arguments.size() > 2)
    {
        std::cerr << "vdb capture: too many arguments" << std::endl;

        result = 1;
    }
    else if (not vdis::to_int32(options::command_arguments[0], port))
    {
        std::cerr << "vdb capture: invalid port argument '"
                  << options::command_arguments[0] << "'" << std::endl;

        result = 1;
    }
    else
    {
        std::string
            filename;

        capturing = true;

        // Was filename provided?  If so check overwrite and open it for
        // output.
        //
        if (options::options::command_arguments.size() > 1)
        {
            struct stat
                file_stat;

            filename = options::command_arguments[1];

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
            open_socket();
            register_signal();
            start();
            print_stats(std::cout);
            close_socket();

            // Close output file.
            //
            if (file_ptr)
            {
                delete file_ptr;
                file_ptr = 0;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::capture::open_socket(void)
{
    const char
        *address_ptr = 0;

    if (not options::network_address.empty())
    {
        address_ptr = options::network_address.c_str();
    }

    socket_ptr = new vdis::receive_socket_t(
        options::ipv6,
        port,
        address_ptr);
}

// ----------------------------------------------------------------------------
void vdb::capture::close_socket(void)
{
    // Actual closing of the socket is handled in destructor
    //
    delete socket_ptr;
    socket_ptr = 0;
}

// ----------------------------------------------------------------------------
// Register handler for the interrupt signal.
//
void vdb::capture::register_signal(void)
{
    struct sigaction
        action;

    std::memset(&action, 0, sizeof(action));

    action.sa_handler = signal_handler;

    sigaction(SIGINT, &action, NULL);
}

// ----------------------------------------------------------------------------
void vdb::capture::start(void)
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
void vdb::capture::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (file_ptr)
    {
        file_ptr->write_pdu_data(data);
    }

    if (not options::scanning)
    {
        print::print_pdu(data, pdu, std::cout);
    }
    else
    {
        if (options::scan_entities)
        {
            entities::process_pdu(data, pdu);
        }

        if (options::scan_associations)
        {
            associations::process_pdu(data, pdu);
        }

        if (options::scan_lasers)
        {
            lasers::process_pdu(data, pdu);
        }

        if (options::scan_fires)
        {
            fires::process_pdu(data, pdu);
        }

        if (options::scan_collisions)
        {
            // TODO scan_collisions
        }

        if (options::scan_objects)
        {
            // TODO scan_objects
        }
   }
}

// ----------------------------------------------------------------------------
void vdb::capture::print_stats(std::ostream &stream)
{
    stream << std::endl
           << "Summary for port " << port << ": " << std::endl
           << "PDUs received: " << pdus_received
           << " (" << bytes_received << " bytes)" << std::endl
           << "PDUs accepted: " << pdus_accepted
           << " (" << bytes_accepted << " bytes)" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::capture::signal_handler(int value)
{
    capturing = false;
}
