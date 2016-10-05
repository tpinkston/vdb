#include "vdb_capture.h"
#include "vdb_common.h"
#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_filter.h"
#include "vdb_network.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_version.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

FILE
    *vdb::capture::file_ptr = 0;
vdb::capture_socket_t
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
        string_t
            filename;

        capturing = true;

        // Was filename provided?  If so check overwrite and open it for
        // output.
        //
        if (options::options::command_arguments.size() < 2)
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
                capturing = open_output_file(filename);
            }
        }

        if (capturing)
        {
            open_socket();
            register_signal();
            start();
            print_stats(std::cout);
            close_socket();
            close_output_file(filename);
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

    socket_ptr = new capture_socket_t(
        address_ptr,
        port,
        options::use_ipv6);
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
bool vdb::capture::open_output_file(const string_t &filename)
{
    file_header_t
        header;
    file_stream
        stream;
    bool
        success = false;

    // First save header to the output capture file.
    //
    header.major_version = VDB_VERSION_MAJOR;
    header.minor_version = VDB_VERSION_MINOR;
    header.time_created = vdis::get_system_time();

    stream.reset(header.length());
    stream.write(header);
    stream.write_file(filename);

    // Reopen the file for manually writing PDU data, appending to the header
    // just written ('a' for append, 'b' for binary).
    //
    file_ptr = fopen(filename.c_str(), "ab");

    if (file_ptr)
    {
        std::cout << "Opened output file: " << filename << std::endl;
        success = true;
    }
    else
    {
        perror("fopen");
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::capture::close_output_file(const string_t &filename)
{
    bool
        success = false;

    if (file_ptr)
    {
        if (fclose(file_ptr) == 0)
        {
            std::cout << "Closed output file: " << filename << std::endl;
            success = true;
        }
        else
        {
            perror("fclose");
        }

        file_ptr = 0;
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdb::capture::write_pdu_data(const pdu_data_t &data)
{
    if (file_ptr)
    {
        const uint32_t
            size = data.length();
        vdis::byte_stream_t
            stream(size);

        LOG_VERBOSE("Writing %d bytes to file...", size);

        data.write(stream);

        if (not stream.error())
        {
            uint32_t bytes_written = fwrite(
                stream.buffer(),
                1,
                stream.length(),
                file_ptr);

            if (bytes_written != size)
            {
                LOG_ERROR(
                    "File write failed (%d out of %d bytes)",
                    size,
                    stream.length());
            }
        }
    }
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
    vdis::pdu_t
        *pdu_ptr = 0;

    capturing = true;

    while(capturing)
    {
        socket_ptr->receive(data);

        if (data.get_pdu_length() > 0)
        {
            bytes_received += (uint64_t)data.get_pdu_length();
            pdus_received += 1U;

            if (filter::filter_by_header(data))
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

                        write_pdu_data(data);
                        print::print_pdu(data, *pdu_ptr, std::cout);
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
