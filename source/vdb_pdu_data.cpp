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

#include "vdb_byte_stream.h"
#include "vdb_common.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_network.h"
#include "vdb_options.h"
#include "vdb_pdu.h"
#include "vdb_pdu_data.h"
#include "vdb_services.h"
#include "vdb_string.h"
#include "vdb_time.h"

// ----------------------------------------------------------------------------
// Returns buffer containing IPv4 or IPv6 address structure.
//
const uint8_t *vdb::pdu_data_t::get_address_buffer(void) const
{
    return address;
}

// ----------------------------------------------------------------------------
// Returns length of hostname as saved to and restored from file (with
// 8-byte boundaries).
//
uint16_t vdb::pdu_data_t::get_hostname_length(void) const
{
    return (hostname.length() + get_padding_length(hostname.length(), 4));
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::set_source(
    const inet_socket_address_t &socket_address,
    uint16_t socket_port)
{
    LOG_VERBOSE("Setting address with IPv4 address...");

    port = socket_port;
    ip_version = 4;
    hostname.clear();
    network::get_hostname(socket_address, hostname);

    std::memset(address, 0, ADDRESS_LENGTH);
    std::memcpy(address, &socket_address.sin_addr, sizeof(inet_address_t));
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::set_source(
    const inet6_socket_address_t &socket_address,
    uint16_t socket_port)
{
    LOG_VERBOSE("Setting address with IPv6 address...");

    port = socket_port;
    ip_version = 6;
    hostname.clear();
    network::get_hostname(socket_address, hostname);

    std::memset(address, 0, ADDRESS_LENGTH);
    std::memcpy(address, &socket_address.sin6_addr, sizeof(inet6_address_t));
}

// ----------------------------------------------------------------------------
// Returns the source as string (include address, port and hostname
// if available).
//
std::string vdb::pdu_data_t::get_source(void) const
{
    std::ostringstream
        stream;

    if (ip_version == 4)
    {
        stream << network::get_address(AF_INET, address);
    }
    else if (ip_version == 6)
    {
        stream << network::get_address(AF_INET6, address);
    }
    else
    {
        stream << "unknown";
    }

    if (hostname.length() > 0)
    {
        stream << "[" << hostname << "]";
    }

    if (port > 0)
    {
        stream << ":" << (int)port;
    }

    return stream.str();
}

// ----------------------------------------------------------------------------
vdb::pdu_t *vdb::pdu_data_t::generate_pdu(void) const
{
    // PDU type is always the 3rd bytes in the PDU buffer.
    //
    const pdu_type_e
        pdu_type = (pdu_type_e)get_pdu_type();
    const std::string
        pdu_type_string = vdb::enumerations::get_name(ENUM_PDU_TYPE, pdu_type);

    // Create serialized buffer and restore the PDU...
    //
    byte_stream
        stream(pdu_buffer, pdu_length);

    // Create the PDU object...
    //
    pdu_t
        *pdu_ptr = new pdu_t();

    LOG_EXTRA_VERBOSE(
        "Reading PDU type %d at index %d with %d bytes...",
        (int)pdu_type,
        index,
        pdu_length);

    pdu_ptr->read(stream);

    // Check to see of the amount of bytes read from the buffer does not
    // match the PDU length.  Buffer index should be at the end of the
    // buffer.
    //
    if (stream.error())
    {
        LOG_ERROR(
            "Read error for PDU %d, type: %s",
            index,
            pdu_type_string.c_str());
    }
    else if (stream.get_index() != stream.get_length())
    {
        std::string
            pdu_type_string = vdb::enumerations::get_name(
                ENUM_PDU_TYPE,
                pdu_type);

        LOG_WARNING(
            "%s[index %d]: read %d bytes out of %d",
            pdu_type_string.c_str(),
            index,
            stream.get_index(),
            stream.get_length());
    }

    return pdu_ptr;
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::clear(void)
{
    header = HEADER;
    index = 0;
    time = 0;
    padding = 0;
    status = 0;
    port = 0;
    ip_version = 0;
    pdu_length = 0;

    hostname.clear();

    std::memset(address, 0, ADDRESS_LENGTH);
    std::memset(pdu_buffer, 0, PDU_BUFFER_LENGTH);
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "PDU " << index
           << " received from " << get_source()
           << " on " << time::to_string(time) << std::endl;
}

// ----------------------------------------------------------------------------
// Returns length in bytes of all PDU data in its entirety (includes
// optional hostname and PDU contents).
//
uint32_t vdb::pdu_data_t::length(void) const
{
    return (BASE_LENGTH + get_hostname_length() + pdu_length);
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::read(byte_stream &stream)
{
    const uint32_t
        start_index = stream.get_index();
    uint16_t
        hostname_length = 0;

    clear();

    stream.read(header);
    stream.read(index);
    stream.read(time);
    stream.read(padding);
    stream.read(status);
    stream.read(hostname_length);

    if (hostname_length > 0)
    {
        char
            *hostname_buffer = new char[hostname_length + 1];

        std::memset(hostname_buffer, 0, (hostname_length + 1));

        stream.read(hostname_buffer, hostname_length);

        hostname = hostname_buffer;

        delete[] hostname_buffer;
        hostname_buffer = 0;
    }

    stream.read(address, ADDRESS_LENGTH);
    stream.read(port);
    stream.read(ip_version);
    stream.read(pdu_length);
    stream.read(pdu_buffer, pdu_length);

    if (options::flag(OPT_EXTRA_VERBOSE))
    {
        hexadecimal::stream(
            "DEBUG_PDU_DATA ",
            pdu_buffer,
            pdu_length,
            16,
            std::cout);
    }

    if (options::flag(OPT_EXTRA_VERBOSE))
    {
        LOG_EXTRA_VERBOSE(
            "Read %d bytes for PDU data starting at index %d\n"
            "  hostname length: %d\n"
            "  PDU index: %d\n"
            "  PDU length: %d",
            (stream.get_index() - start_index),
            start_index,
            hostname_length,
            index,
            pdu_length);
    }
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::write(byte_stream &stream) const
{
    const uint32_t
        start_index = stream.get_index();
    const uint16_t
        hostname_length = get_hostname_length();

    stream.write(header);
    stream.write(index);
    stream.write(time);
    stream.write(padding);
    stream.write(status);
    stream.write(hostname_length);

    for(uint16_t i = 0; i < hostname_length; ++i)
    {
        stream.write((int8_t)((i < hostname.length()) ? hostname[i] : 0));
    }

    stream.write(address, ADDRESS_LENGTH);
    stream.write(port);
    stream.write(ip_version);
    stream.write(pdu_length);
    stream.write(pdu_buffer, pdu_length);

    if (options::flag(OPT_EXTRA_VERBOSE))
    {
        LOG_EXTRA_VERBOSE(
            "Wrote %d bytes in byte data starting at index %d (%d hostname)...",
            (stream.get_index() - start_index),
            start_index,
            hostname_length);
    }
}
