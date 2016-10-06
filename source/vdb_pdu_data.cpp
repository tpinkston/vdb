#include "vdb_network.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_byte_stream.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

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
    return (hostname.length() + vdis::padding_length(hostname.length(), 4));
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::set_source(
    const inet_socket_address_t &source_address,
    uint16_t socket_port)
{
    LOG_EXTRA_VERBOSE("Setting address with IPv4 address...");

    port = socket_port;
    ip_version = 4;
    hostname.clear();
    network::get_hostname(source_address, hostname);

    std::memset(address, 0, ADDRESS_LENGTH);
    std::memcpy(address, &source_address.sin_addr, sizeof(inet_address_t));
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::set_source(
    const inet6_socket_address_t &source_address,
    uint16_t socket_port)
{
    LOG_EXTRA_VERBOSE("Setting address with IPv6 address...");

    port = socket_port;
    ip_version = 6;
    hostname.clear();
    network::get_hostname(source_address, hostname);

    std::memset(address, 0, ADDRESS_LENGTH);
    std::memcpy(address, &source_address.sin6_addr, sizeof(inet6_address_t));
}

// ----------------------------------------------------------------------------
// Returns the source as string (include address, port and hostname
// if available).
//
string_t vdb::pdu_data_t::get_source(void) const
{
    std::ostringstream stream;

    if (ip_version == 4)
    {
        inet_address_t address4;

        std::memcpy(&address4, address, sizeof(inet_address_t));

        stream << network::get_address(address4);
    }
    else if (ip_version == 6)
    {
        inet_address_t address6;

        std::memcpy(&address6, address, sizeof(inet6_address_t));

        stream << network::get_address(address6);
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
vdis::pdu_t *vdb::pdu_data_t::generate_pdu(void) const
{
    // PDU type is always the 3rd bytes in the PDU buffer.
    //
    const vdis::pdu_type_e
        pdu_type = (vdis::pdu_type_e)get_pdu_type();
    const string_t
        pdu_type_string = vdis::enumerations::get_name(ENUM_PDU_TYPE, pdu_type);
    vdis::pdu_t
        *pdu_ptr = 0;

    // Create serialized buffer and restore the PDU...
    //
    vdis::byte_stream_t
        stream(pdu_buffer, pdu_length);

    LOG_EXTRA_VERBOSE(
        "Creating PDU type %d at index %d with %d bytes...",
        (int)pdu_type,
        index,
        pdu_length);

    // Create the PDU object...
    //
    pdu_ptr = new vdis::pdu_t(stream);

    // Check to see of the amount of bytes read from the buffer does not
    // match the PDU length.  Buffer index should be at the end of the
    // buffer.
    //
    if (stream.error())
    {
        LOG_ERROR(
            "Stream error for PDU %d, type: %s ("
            "index = %d, length = %d)",
            index,
            pdu_type_string.c_str(),
            stream.index(),
            stream.length());
    }
    else if (stream.index() != stream.length())
    {
        LOG_WARNING(
            "%s[index %d]: read %d bytes out of %d",
            pdu_type_string.c_str(),
            index,
            stream.index(),
            stream.length());
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
    const string_t &prefix,
    std::ostream &stream) const
{
    stream << prefix << "PDU " << index
           << " received from " << get_source()
           << " on " << vdis::time_to_string(time) << std::endl;
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
void vdb::pdu_data_t::read(vdis::byte_stream_t &stream)
{
    const uint32_t
        start_index = stream.index();
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

    if (logger::is_enabled(logger::EXTRA_VERBOSE))
    {
        vdis::byte_buffer_t
            temp_buffer(pdu_buffer, pdu_length, false);

        temp_buffer.print("DEBUG_PDU_DATA ", std::cout);
    }

    if (logger::is_enabled(logger::EXTRA_VERBOSE))
    {
        LOG_EXTRA_VERBOSE(
            "Read %d bytes for PDU data starting at index %d\n"
            "  hostname length: %d\n"
            "  PDU index: %d\n"
            "  PDU length: %d",
            (stream.index() - start_index),
            start_index,
            hostname_length,
            index,
            pdu_length);
    }
}

// ----------------------------------------------------------------------------
void vdb::pdu_data_t::write(vdis::byte_stream_t &stream) const
{
    const uint32_t
        start_index = stream.index();
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

    if (logger::is_enabled(logger::EXTRA_VERBOSE))
    {
        LOG_EXTRA_VERBOSE(
            "Wrote %d bytes in byte data starting at index %d (%d hostname)...",
            (stream.index() - start_index),
            start_index,
            hostname_length);
    }
}
