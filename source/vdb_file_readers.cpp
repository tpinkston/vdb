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

#include "vdb_file_readers.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_time.h"

// ----------------------------------------------------------------------------
vdb::file_reader_t::file_reader_t(const std::string &filename) :
    filename(filename),
    error_condition(false)
{

}

// ----------------------------------------------------------------------------
vdb::file_reader_t::~file_reader_t(void)
{

}

// ----------------------------------------------------------------------------
// Starts the parsing of the file.  Input is a function pointer for
// callback handler for PDUs read from the file.  Callback function
// should return false when file parsing need not continue (true
// to keep parsing).
//
// This method returns true if no errors occurred during parsing.
//
bool vdb::file_reader_t::parse(bool (*callback)(const pdu_data_t &))
{
    if (callback)
    {
        const uint64_t
            start_time = time::get_system();
        pdu_data_t
            data;
        bool
            parsing = true;

        LOG_VERBOSE("Parsing file content...");

        while(parsing and good() and next_entry(data))
        {
            if (data.has_pdu())
            {
                parsing = callback(data);
            }
        }

        LOG_VERBOSE(
            "Parsing completed in %d milliseconds...",
            (time::get_system() - start_time));
    }
    else
    {
        LOG_ERROR("Parsing callback function is null!");
        error_condition = true;
    }

    return good();
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::standard_reader_t(const std::string &filename) :
    file_reader_t(filename)
{
    if (not stream.read_file(filename))
    {
        error_condition = true;
    }
    else
    {
        header.read(stream);

        if (stream.error() || header.invalid_title())
        {
            std::cerr << options::get_terminal_command()
                      << ": invalid capture file: " << filename << std::endl;

            error_condition = true;
        }
    }
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::~standard_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::standard_reader_t::next_entry(pdu_data_t &data)
{
    bool
        valid_entry = false;

    if (good() and (stream.get_length_remaining() > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Reading PDU data at index %d of %d...",
            stream.get_index(),
            stream.get_length());

        data.read(stream);

        if (stream.error())
        {
            error_condition = true;
        }
        else
        {
            valid_entry = true;
        }
    }

    return valid_entry;
}

// ----------------------------------------------------------------------------
vdb::pcap_reader_t::pcap_reader_t(const std::string &filename) :
    file_reader_t(filename),
    pcap_ptr(0),
    index_counter(0)
{
    char
        error_buffer[PCAP_ERRBUF_SIZE];

    std::memset(error_buffer, 0, PCAP_ERRBUF_SIZE);

    pcap_ptr = pcap_open_offline(filename.c_str(), error_buffer);

    if (pcap_ptr)
    {
        LOG_VERBOSE("Opened PCAP file: %s", filename.c_str());
    }
    else
    {
        std::cerr << options::get_terminal_command()
                  << ": " << error_buffer << std::endl;

        error_condition = true;
    }
}

// ----------------------------------------------------------------------------
vdb::pcap_reader_t::~pcap_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::pcap_reader_t::next_entry(pdu_data_t &data)
{
    bool
        valid_entry = false;

    if (good())
    {
        pcap_packet_header_t
            *header_ptr;
        const uint8_t
            *data_ptr;

        valid_entry = (pcap_next_ex(pcap_ptr, &header_ptr, &data_ptr) > -1);

        if (valid_entry)
        {
            byte_stream
                stream(data_ptr, header_ptr->caplen);

            valid_entry = read_pcap_entry(
                *header_ptr,
                stream,
                data);
        }
    }

    return valid_entry;
}

// ----------------------------------------------------------------------------
bool vdb::pcap_reader_t::read_pcap_entry(
    const pcap_packet_header_t &header,
    byte_stream &stream,
    pdu_data_t &data)
{
    pcap_ethernet_t
        ethernet;
    ip_header_t
        *ip_header_ptr = 0;
    udp_header_t
        udp_header;
    bool
        valid_entry = false;

    // Clear PDU data
    //
    data.clear();

    ethernet.read(stream);

    if (not stream.error())
    {
        if (options::flag(OPT_EXTRA_VERBOSE))
        {
            ethernet.print("pcap.ethernet.", std::cout);
        }

        if (ethernet.type == ETHERTYPE_IP)
        {
            ip_header_ptr = new ipv4_header_t();
            ip_header_ptr->read(stream);
        }
        else if (ethernet.type == ETHERTYPE_IPV6)
        {
            ip_header_ptr = new ipv6_header_t();
            ip_header_ptr->read(stream);
        }
        else
        {
            LOG_WARNING("Unexpected ethernet type: %d", ethernet.type)
        }

        if (ip_header_ptr and not stream.error())
        {
            if (options::flag(OPT_EXTRA_VERBOSE) and not stream.error())
            {
                ip_header_ptr->print("ip_header.", std::cout);
            }

            // The first entry in PCAP files written by the SE Core Gateway
            // are not PDUs but some other data related to the BRPS.  The
            // observable difference is that the protocol value will be -1.
            //
            if (ip_header_ptr->get_protocol() and
                (*ip_header_ptr->get_protocol() < 0))
            {
                // Let entry go through as valid but with empty PDU data.
                //
                valid_entry = true;
            }
            else
            {
                udp_header.read(stream);

                if (not stream.error())
                {
                    if (options::flag(OPT_EXTRA_VERBOSE))
                    {
                        udp_header.print("udp_header.", std::cout);
                    }

                    data.set_index(index_counter++);
                    data.set_time(time::get_system(header.ts));

                    // Remaining bytes in the stream are the PDU itself...
                    //
                    data.set_pdu_buffer(
                        (uint8_t *)(stream.get_buffer() + stream.get_index()),
                        stream.get_length_remaining());

                    if (ethernet.type == ETHERTYPE_IP)
                    {
                        inet_address_t
                            address;

                        std::memcpy(
                            &address,
                            ip_header_ptr->get_source_address(),
                            ip_header_ptr->get_address_size());

                        data.set_source(address, udp_header.destination_port);
                    }
                    else if (ethernet.type == ETHERTYPE_IPV6)
                    {
                        inet6_address_t
                            address;

                        std::memcpy(
                            &address,
                            ip_header_ptr->get_source_address(),
                            ip_header_ptr->get_address_size());

                        data.set_source(address, udp_header.destination_port);
                    }

                    valid_entry = true;
                }
            }
        }
    }

    if (ip_header_ptr)
    {
        delete ip_header_ptr;
        ip_header_ptr = 0;
    }

    return valid_entry;
}
