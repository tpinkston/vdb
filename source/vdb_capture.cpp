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

#include "vdb_capture.h"
#include "vdb_logger.h"
#include "vdb_network.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_pdu.h"
#include "vdb_print.h"
#include "vdb_string.h"

namespace
{
    vdb::capture
        *instance_ptr = 0;
}

// ----------------------------------------------------------------------------
vdb::capture::capture(void) :
    socket_ptr(0),
    port(0),
    bytes_received(0),
    bytes_accepted(0),
    pdus_received(0),
    pdus_accepted(0),
    capturing(true)
{
    if (instance_ptr)
    {
        LOG_ERROR("Already instantiated!");
        exit(2);
    }

    instance_ptr = this;
}

// ----------------------------------------------------------------------------
vdb::capture::~capture(void)
{
    instance_ptr = 0;
}

// ----------------------------------------------------------------------------
int vdb::capture::run(void)
{
    int result = 0;

    // Port argument (1st) required, file argument (2nd) optional
    //
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " capture: missing port argument" << std::endl;

        result = 1;
    }
    else if (options::get_command_arguments().size() > 2)
    {
        std::cerr << options::get_terminal_command()
                  << " capture: too many arguments" << std::endl;

        result = 1;
    }
    else if (not string_to_int32(*options::get_command_argument(0), port))
    {
        std::cerr << options::get_terminal_command()
                  << " capture: invalid port argument '"
                  << *options::get_command_argument(0) << "'" << std::endl;

        result = 1;
    }
    else
    {
        // Was filename provided?  If so check overwrite and open it for
        // output.
        //
        if (options::get_command_argument(1))
        {
            struct stat
                file_stat;

            filename = *options::get_command_argument(1);

            LOG_VERBOSE("Checking file '%s'...", filename.c_str());

            if (stat(filename.c_str(), &file_stat) == 0)
            {
                std::cout << "Overwrite '" << filename << "'? ";

                capturing = user_confirmation();
            }

            if (capturing)
            {
                capturing = open_output_file();
            }
        }

        if (capturing)
        {
            open_socket();
            register_signal();
            start();
            print_stats(std::cout);
            close_socket();
            close_output_file();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::capture::open_socket(void)
{
    const char
        *address_ptr = 0;

    if (options::string(OPT_NET_ADDRESS))
    {
        address_ptr = options::string(OPT_NET_ADDRESS)->c_str();
    }

    socket_ptr = new capture_socket_t(
        address_ptr,
        port,
        options::flag(OPT_IPV6));
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
    pdu_t
        *pdu_ptr = 0;

    capturing = true;

    while(capturing)
    {
        socket_ptr->receive(data);

        if (data.get_pdu_length() > 0)
        {
            bytes_received += (uint64_t)data.get_pdu_length();
            pdus_received += 1U;

            if (passes_data_filters(data))
            {
                pdu_ptr = data.generate_pdu();

                if (pdu_ptr)
                {
                    if (passes_pdu_filters(*pdu_ptr))
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
    if (instance_ptr)
    {
        instance_ptr->capturing = false;
    }
}
