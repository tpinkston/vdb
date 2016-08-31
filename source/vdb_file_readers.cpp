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
    uint16_t
        ethernet_type = 0,
        source_port = 0,
        destination_port = 0;
    int8_t
        protocol = 0;
    inet_address_t
        address_ipv4;
    inet6_address_t
        address_ipv6;
    void
        *address_ptr = 0;
    bool
        valid_entry = false;

    // Clear PDU data
    //
    data.clear();

    // First 12 bytes are the destination and source MAC addresses (6 bytes
    // each), skip them and read unsigned 16-bit ethernet type...
    //
    stream.skip(12);
    stream.read(ethernet_type);

    if (not stream.error())
    {
        LOG_EXTRA_VERBOSE("Ethernet type is 0x%04X...", ethernet_type)

        // Next is either the IPv4 or IPv6 header:
        // struct iphdr (20 bytes)
        // struct ip6_hdr (40 bytes)
        //
        if (ethernet_type == ETHERTYPE_IP)
        {
            address_ptr = &address_ipv4;

            // Skip 9 bytes then read the 8-bit protocol value
            //
            stream.skip(9);
            stream.read(protocol);

            // Skip another 2 bytes bytes then read the 4-byte source IP
            // address, then skip another 4 bytes for the destination IP
            //
            stream.skip(2);
            stream.read(address_ptr, sizeof(inet_address_t));
            stream.skip(4);
        }
        else if (ethernet_type == ETHERTYPE_IPV6)
        {
            address_ptr = &address_ipv6;

            // Skip 8 bytes bytes then read the 16-byte source IP address,
            // then skip another 16 bytes for the destination IP
            //
            stream.skip(8);
            stream.read(address_ptr, sizeof(inet6_address_t));
            stream.skip(16);
        }
        else
        {
            LOG_WARNING("Unexpected ethernet type: 0x%04X", ethernet_type)
        }

        if (address_ptr and not stream.error())
        {
            if (options::flag(OPT_EXTRA_VERBOSE) and not stream.error())
            {
                if (address_ptr == &address_ipv4)
                {
                    LOG_EXTRA_VERBOSE(
                        "Source IPv4 address is %s...",
                        network::get_address(AF_INET, &address_ipv4).c_str());
                }
                else
                {
                    LOG_EXTRA_VERBOSE(
                        "Source IPv6 address is %s...",
                        network::get_address(AF_INET6, &address_ipv6).c_str());
                }
            }

            // The first entry in PCAP files written by the SE Core Gateway
            // are not PDUs but some other data related to the BRPS.  The
            // observable difference is that the protocol value will be -1.
            //
            if (protocol < 0)
            {
                // Let entry go through as valid but with empty PDU data.
                //
                valid_entry = true;
            }
            else
            {
                // Read the the source and destination port from the UPD
                // header record.  Then skip the remaining 4 bytes.
                //
                stream.read(source_port);
                stream.read(destination_port);
                stream.skip(4);

                if (not stream.error())
                {
                    LOG_EXTRA_VERBOSE(
                        "Source port %d...",
                        source_port);
                    LOG_EXTRA_VERBOSE(
                        "Destination port %d...",
                        destination_port);

                    data.set_index(index_counter++);
                    data.set_time(time::get_system(header.ts));

                    // Remaining bytes in the stream are the PDU itself...
                    //
                    data.set_pdu_buffer(
                        (uint8_t *)(stream.get_buffer() + stream.get_index()),
                        stream.get_length_remaining());

                    if (ethernet_type == ETHERTYPE_IP)
                    {
                        data.set_source(address_ipv4, destination_port);
                    }
                    else
                    {
                        data.set_source(address_ipv6, destination_port);
                    }

                    valid_entry = true;
                }
            }
        }
    }

    return valid_entry;
}
