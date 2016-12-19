#include "vdb_file_reader.h"
#include "vdb_options.h"

#include "vdis_logger.h"
#include "vdis_network.h"

// ----------------------------------------------------------------------------
vdb::file_reader_t::file_reader_t(const string_t &filename) :
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
            start_time = vdis::get_system_time();
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
            (vdis::get_system_time() - start_time));
    }
    else
    {
        LOG_ERROR("Parsing callback function is null!");
        error_condition = true;
    }

    return good();
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::standard_reader_t(const string_t &filename) :
    file_reader_t(filename)
{
    if (not stream.read_file(filename))
    {
        error_condition = true;
    }
    else
    {
        header.read(stream);

        if (header.invalid_title() or stream.error())
        {
            std::cerr << "vdb: invalid capture file: " << filename
                      << std::endl;

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

    if (good() and (stream.remaining_length() > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Reading PDU data at index %d of %d...",
            stream.index(),
            stream.length());

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
vdb::pdu_reader_t::pdu_reader_t(const string_t &filename, uint32_t interval) :
    file_reader_t(filename),
    interval_milliseconds(interval),
    index(0)
{
    if (options::ipv6)
    {
        vdis::set_address("::1", 0, address_ipv6);
    }
    else
    {
        bool broadcast = false;

        vdis::set_address("127.0.0.1", 0, address_ipv4, broadcast);
    }
}

// ----------------------------------------------------------------------------
vdb::pdu_reader_t::~pdu_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::pdu_reader_t::next_entry(pdu_data_t &data)
{
    file_stream
        stream;
    bool
        success = false;

    if (stream.read_file(filename))
    {
        if (index == 0)
        {
            data.set_time(vdis::get_system_time());
        }
        else
        {
            data.set_time(vdis::get_system_time() + interval_milliseconds);
        }

        data.set_index(index++);
        data.set_pdu_buffer(stream.buffer(), stream.length());

        if (options::ipv6)
        {
            data.set_source(address_ipv6, 0);
        }
        else
        {
            data.set_source(address_ipv4, 0);
        }

        success = true;
    }

    return success;
}

// ----------------------------------------------------------------------------
vdb::pcap_reader_t::pcap_reader_t(const string_t &filename) :
    file_reader_t(filename)
{
//    char
//        error_buffer[PCAP_ERRBUF_SIZE];
//
//    pcap_ptr = 0;
//    index_counter = 0;
//
//    std::memset(error_buffer, 0, PCAP_ERRBUF_SIZE);
//
//    pcap_ptr = pcap_open_offline(filename.c_str(), error_buffer);
//
//    if (pcap_ptr)
//    {
//        LOG_VERBOSE("Opened PCAP file: %s", filename.c_str());
//    }
//    else
//    {
//        std::cerr << "vdb: " << error_buffer << std::endl;
//
//        error_condition = true;
//    }
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

//    if (good())
//    {
//        pcap_packet_header_t
//            *header_ptr = 0;
//        const uint8_t
//            *data_ptr = 0;
//
//        valid_entry = (pcap_next_ex(pcap_ptr, &header_ptr, &data_ptr) > -1);
//
//        if (valid_entry)
//        {
//            vdis::byte_stream_t
//                stream(data_ptr, header_ptr->caplen);
//
//            valid_entry = read_pcap_entry(
//                *header_ptr,
//                stream,
//                data);
//        }
//    }

    return valid_entry;
}

// ----------------------------------------------------------------------------
//bool vdb::pcap_reader_t::read_pcap_entry(
//    const pcap_packet_header_t &header,
//    vdis::byte_stream_t &stream,
//    pdu_data_t &data)
//{
//    uint16_t
//        ethernet_type = 0,
//        source_port = 0,
//        destination_port = 0;
//    int8_t
//        protocol = 0;
//    vdis::socket_address_ipv4_t
//        address_ipv4;
//    vdis::socket_address_ipv6_t
//        address_ipv6;
//    void
//        *address_ptr = 0;
//    bool
//        valid_entry = false;
//
//    // Clear PDU data
//    //
//    data.clear();
//
//    // First 12 bytes are the destination and source MAC addresses (6 bytes
//    // each), skip them and read unsigned 16-bit ethernet type...
//    //
//    stream.skip(12);
//    stream.read(ethernet_type);
//
//    if (stream.ready())
//    {
//        LOG_EXTRA_VERBOSE("Ethernet type is 0x%04X...", ethernet_type)
//
//        // Next is either the IPv4 or IPv6 header:
//        // struct iphdr (20 bytes)
//        // struct ip6_hdr (40 bytes)
//        //
//        if (ethernet_type == ETHERTYPE_IP)
//        {
//            address_ptr = &address_ipv4;
//
//            // Skip 9 bytes then read the 8-bit protocol value
//            //
//            stream.skip(9);
//            stream.read(protocol);
//
//            // Skip another 2 bytes bytes then read the 4-byte source IP
//            // address, then skip another 4 bytes for the destination IP
//            //
//            stream.skip(2);
//            stream.read(&address_ipv4.sin_addr, sizeof(address_ipv4));
//            stream.skip(4);
//        }
//        else if (ethernet_type == ETHERTYPE_IPV6)
//        {
//            address_ptr = &address_ipv6;
//
//            // Skip 8 bytes bytes then read the 16-byte source IP address,
//            // then skip another 16 bytes for the destination IP
//            //
//            stream.skip(8);
//            stream.read(&address_ipv6.sin6_addr, sizeof(address_ipv6));
//            stream.skip(16);
//        }
//        else
//        {
//            LOG_WARNING("Unexpected ethernet type: 0x%04X", ethernet_type)
//        }
//
//        if (address_ptr and stream.ready())
//        {
//            if (logger::is_enabled(logger::EXTRA_VERBOSE))
//            {
//                if (address_ptr == &address_ipv4)
//                {
//                    LOG_EXTRA_VERBOSE(
//                        "Source IPv4 address is %s...",
//                        vdis::get_address(address_ipv4.sin_addr).c_str());
//                }
//                else
//                {
//                    LOG_EXTRA_VERBOSE(
//                        "Source IPv6 address is %s...",
//                        vdis::get_address(address_ipv6.sin6_addr).c_str());
//                }
//            }
//
//            // The first entry in PCAP files written by the SE Core Gateway
//            // are not PDUs but some other data related to the BRPS.  The
//            // observable difference is that the protocol value will be -1.
//            //
//            if (protocol < 0)
//            {
//                // Let entry go through as valid but with empty PDU data.
//                //
//                valid_entry = true;
//            }
//            else
//            {
//                // Read the the source and destination port from the UPD
//                // header record.  Then skip the remaining 4 bytes.
//                //
//                stream.read(source_port);
//                stream.read(destination_port);
//                stream.skip(4);
//
//                if (stream.ready())
//                {
//                    LOG_EXTRA_VERBOSE(
//                        "Source port %d...",
//                        source_port);
//                    LOG_EXTRA_VERBOSE(
//                        "Destination port %d...",
//                        destination_port);
//
//                    data.set_index(index_counter++);
//                    data.set_time(vdis::get_system_time(header.ts));
//
//                    // Remaining bytes in the stream are the PDU itself...
//                    //
//                    data.set_pdu_buffer(
//                        (uint8_t *)(stream.buffer() + stream.index()),
//                        stream.remaining_length());
//
//                    if (ethernet_type == ETHERTYPE_IP)
//                    {
//                        data.set_source(address_ipv4, destination_port);
//                    }
//                    else
//                    {
//                        data.set_source(address_ipv6, destination_port);
//                    }
//
//                    valid_entry = true;
//                }
//            }
//        }
//    }
//
//    return valid_entry;
//}
