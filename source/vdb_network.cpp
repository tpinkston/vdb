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

#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_network.h"

#include "vdb_byte_stream.h"
#include "vdb_options.h"
#include "vdb_string.h"
#include "vdb_time.h"

namespace
{
    const uint32_t
        HOSTNAME_MAX_SIZE = 512;
}

namespace vdb
{
    const char
        *network::ANY = "any",
        *network::LOOPBACK = "loopback",
        *network::BROADCAST = "broadcast";
    ipv4_address_cache_t
        network::ipv4_address_cache;
    const int32_t
        socket_base_t::ON = 0x01,
        socket_base_t::OFF = 0x00;
}

// ----------------------------------------------------------------------------
int32_t vdb::network::open_socket(bool ipv6)
{
    if (ipv6)
    {
        return socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
    }
    else
    {
        return socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    }
}

// ----------------------------------------------------------------------------
void vdb::network::set_address(
    const char *address,
    int32_t port,
    inet_socket_address_t &socket_address,
    bool &broadcast)
{
    std::memset(&socket_address, 0, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);

    LOG_VERBOSE("Setting address: %s...", (address ? address : ANY));

    if (address)
    {
        set_address(address, socket_address.sin_addr, broadcast);
    }
    else
    {
        // Default to 'ANY'
        //
        socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
}

// ----------------------------------------------------------------------------
void vdb::network::set_address(
    const char *address,
    inet_address_t &inet_address,
    bool &broadcast,
    bool fail_on_error)
{
    const std::string
        address_string = to_lowercase(address);

    std::memset(&inet_address, 0, sizeof(inet_address));

    if (address_string == ANY)
    {
        inet_address.s_addr = htonl(INADDR_ANY);
    }
    else if (address_string == BROADCAST)
    {
        inet_address.s_addr = htonl(INADDR_BROADCAST);
        broadcast = true;
    }
    else if (address_string == LOOPBACK)
    {
        inet_address.s_addr = htonl(INADDR_LOOPBACK);
    }
    else
    {
        int result = inet_pton(
            AF_INET,
            address,
            &inet_address);

        if (result == -1)
        {
            std::cerr << options::get_terminal_command()
                      << ": inet_pton: " <<  strerror(errno) << std::endl;

            if (fail_on_error)
            {
                exit(1);
            }
        }
        else if (result == 0)
        {
            std::cerr << options::get_terminal_command()
                      << ": invalid address: " << address << std::endl;

            if (fail_on_error)
            {
                exit(1);
            }
        }

        broadcast = ends_with(address_string, ".255");

        if (options::flag(OPT_VERBOSE))
        {
            LOG_VERBOSE("Converted %s to IPv4 address:", address);

            hexadecimal::stream(
                "IPv4: ",
                (const uint8_t *)&inet_address,
                sizeof(inet_address_t),
                sizeof(inet_address_t),
                std::cout);
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::network::set_address(
    const char *address,
    int32_t port,
    inet6_socket_address_t &socket_address)
{
    std::memset(&socket_address, 0, sizeof(socket_address));

    socket_address.sin6_family = AF_INET6;
    socket_address.sin6_port = htons(port);

    if (address)
    {
        set_address(address, socket_address.sin6_addr);
    }
    else
    {
        std::memcpy(
            &socket_address.sin6_addr.s6_addr,
            &in6addr_any,
            sizeof(in6addr_any));
    }
}

// ----------------------------------------------------------------------------
void vdb::network::set_address(
    const char *address,
    inet6_address_t &inet6_address,
    bool fail_on_error)
{
    const std::string
        address_string = to_lowercase(address);

    if (address_string == ANY)
    {
        std::memcpy(
            &inet6_address.s6_addr,
            &in6addr_any,
            sizeof(in6addr_any));
    }
    else if (address_string == LOOPBACK)
    {
        std::memcpy(
            &inet6_address.s6_addr,
            &in6addr_loopback,
            sizeof(in6addr_loopback));
    }
    else
    {
        int result = inet_pton(
            AF_INET6,
            address,
            &inet6_address.s6_addr);

        if (result == -1)
        {
            std::cerr << options::get_terminal_command()
                      << ": inet_pton: " <<  strerror(errno) << std::endl;

            if (fail_on_error)
            {
                exit(1);
            }
        }
        else if (result == 0)
        {
            std::cerr << options::get_terminal_command()
                      << ": invalid address: " << address << std::endl;

            if (fail_on_error)
            {
                exit(1);
            }
        }

        if (options::flag(OPT_VERBOSE))
        {
            LOG_VERBOSE("Converted %s to IPv6 address:", address);

            hexadecimal::stream(
                "IPv6: ",
                (const uint8_t *)&inet6_address,
                sizeof(inet6_address_t),
                sizeof(inet6_address_t),
                std::cout);
        }
    }
}

// ----------------------------------------------------------------------------
std::string vdb::network::get_address(int family, const void *address_ptr)
{
    std::string
        source;
    char
        buffer[64];

    std::memset(buffer, 0, 64);

    if (family == AF_INET)
    {
        inet_address_t
            address_ipv4;

        std::memset(&address_ipv4, 0, sizeof(address_ipv4));

        std::memcpy(
            &address_ipv4,
            address_ptr,
            sizeof(address_ipv4));

        const char *result = inet_ntop(
            AF_INET,
            &address_ipv4,
            buffer,
            INET_ADDRSTRLEN);

        if (result)
        {
            source = buffer;
        }
    }
    else if (family == AF_INET6)
    {
        inet6_address_t
            address_ipv6;

        std::memset(&address_ipv6, 0, sizeof(address_ipv6));

        std::memcpy(
            &address_ipv6,
            address_ptr,
            sizeof(address_ipv6));

        const char *result = inet_ntop(
            AF_INET6,
            &address_ipv6,
            buffer,
            INET6_ADDRSTRLEN);

        if (result)
        {
            source = buffer;
        }
    }
    else
    {
        source = "unknown";
    }

    return source;
}

// ----------------------------------------------------------------------------
bool vdb::network::get_hostname(
    const inet_socket_address_t &address,
    std::string &host)
{
    const uint32_t
        ip = address.sin_addr.s_addr;
    ipv4_address_cache_t::const_iterator
        itor;
    bool
        success = true;

    // The call to 'query_hostname' is expensive, check for a cached hostname
    // for this IP address first.
    //
    itor = ipv4_address_cache.find(ip);

    if (itor != ipv4_address_cache.end())
    {
        host = itor->second;
    }
    else
    {
        success = query_hostname(address, host);

        if (success)
        {
            ipv4_address_cache[ip] = host;
        }
        else
        {
            ipv4_address_cache[ip] = "unknown";
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::network::query_hostname(
    const inet_socket_address_t &address,
    std::string &host)
{
    char
        hostname[HOSTNAME_MAX_SIZE],
        service[HOSTNAME_MAX_SIZE];
    bool
        success = false;
    int
        result = 0;

    std::memset(hostname, 0, HOSTNAME_MAX_SIZE);
    std::memset(service, 0, HOSTNAME_MAX_SIZE);

    result = getnameinfo(
        (sockaddr*)&address,
        sizeof(inet_socket_address_t),
        hostname, HOSTNAME_MAX_SIZE,
        service, HOSTNAME_MAX_SIZE,
        0);

    if (result == 0)
    {
        host = hostname;
        success = true;
    }
    else
    {
        perror("getnameinfo(IPv4)");
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::network::get_hostname(
    const inet6_socket_address_t &address,
    std::string &host)
{
    // TODO: Cache hostname from IPv6 address
    //
    return query_hostname(address, host);
}

// ----------------------------------------------------------------------------
bool vdb::network::query_hostname(
    const inet6_socket_address_t &address,
    std::string &host)
{
    char
        hostname[HOSTNAME_MAX_SIZE],
        service[HOSTNAME_MAX_SIZE];
    bool
        success = false;
    int
        result = 0;

    std::memset(hostname, 0, HOSTNAME_MAX_SIZE);
    std::memset(service, 0, HOSTNAME_MAX_SIZE);

    result = getnameinfo(
        (sockaddr*)&address,
        sizeof(inet6_socket_address_t),
        hostname, HOSTNAME_MAX_SIZE,
        service, HOSTNAME_MAX_SIZE,
        0);

    if (result == 0)
    {
        host = hostname;
        success = true;
    }
    else
    {
        perror("getnameinfo(IPv6)");
    }

    return success;
}

// ----------------------------------------------------------------------------
vdb::socket_base_t::socket_base_t(
    const char *address,
    int32_t port,
    bool ipv6
) :
    address(address),
    port(port),
    ipv6(ipv6),
    descriptor(network::open_socket(ipv6))
{
    if (descriptor == -1)
    {
        perror("socket");
        exit(1);
    }

    set_socket_option(SOL_SOCKET, SO_REUSEADDR, ON);

    LOG_VERBOSE(
        "Opened %s socket %d on %s:%d...",
        (ipv6 ? "IPv6" : "IPv4"),
        descriptor,
        (address ? address : "default"),
        port);
}

// ----------------------------------------------------------------------------
vdb::socket_base_t::~socket_base_t(void)
{
    close_socket();
}

// ----------------------------------------------------------------------------
void vdb::socket_base_t::set_socket_option(
    int32_t level,
    int32_t option,
    int32_t value) {

    int32_t result = setsockopt(
        descriptor,
        level,
        option,
        &value,
        sizeof(int32_t));

    if (result < 0)
    {
        perror("setsockopt");
        exit(1);
    }
}

// ----------------------------------------------------------------------------
void vdb::socket_base_t::bind_socket(void)
{
    int32_t
        result = -1;

    if (options::flag(OPT_IPV6))
    {
        inet6_socket_address_t
            bind_address;

        network::set_address(address, port, bind_address);

        result = bind(
            descriptor,
            (sockaddr *)&bind_address,
            sizeof(inet6_socket_address_t));

        if (result < 0)
        {
            perror("bind(ipv6)");
            exit(1);
        }
    }
    else
    {
        inet_socket_address_t
            bind_address;
        bool
            broadcast = false;

        network::set_address(address, port, bind_address, broadcast);

        result = bind(
            descriptor,
            (struct sockaddr *)&bind_address,
            sizeof(sockaddr_in));

        if (result < 0)
        {
            perror("bind(ipv4)");
            exit(1);
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::socket_base_t::close_socket(void)
{
    if (descriptor > 0)
    {
        LOG_VERBOSE("Closing socket %d on port %d...", descriptor, port);

        if (close(descriptor) != 0)
        {
            perror("close");
        }
    }
}

// ----------------------------------------------------------------------------
vdb::capture_socket_t::capture_socket_t(
    const char *address,
    int32_t port,
    bool ipv6
) :
    socket_base_t(address, port, ipv6),
    address_size(0)
{
    bind_socket();
}

// ----------------------------------------------------------------------------
vdb::capture_socket_t::~capture_socket_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::capture_socket_t::receive(pdu_data_t &data)
{
    void
        *address_ptr = 0;

    if (ipv6)
    {
        address_ptr = &address_ipv6;
        address_size = sizeof(address_ipv6);
    }
    else
    {
        address_ptr = &address_ipv4;
        address_size = sizeof(address_ipv4);
    }

    std::memset(address_ptr, 0, address_size);

    LOG_VERBOSE("Listening on socket...");

    // Returns negative number on error, cannot be unsigned
    //
    int result = recvfrom(
        descriptor,
        (void *)data.update_pdu_buffer(),
        pdu_data_t::PDU_BUFFER_LENGTH,
        0,
        (socket_address_t *)address_ptr,
        &address_size);

    if (result < 1)
    {
        LOG_VERBOSE("recvfrom() returned %d...", result);
    }
    else
    {
        data.set_time(time::get_system());
        data.set_pdu_length(result);

        if (ipv6)
        {
            data.set_source(address_ipv6, (uint16_t)port);
        }
        else
        {
            data.set_source(address_ipv4, (uint16_t)port);
        }

        LOG_VERBOSE(
            "Received %d bytes from %s...",
            result,
            data.get_source().c_str(),
            port);
    }
}

// ----------------------------------------------------------------------------
vdb::playback_socket_t::playback_socket_t(
    const char *address,
    int32_t port,
    bool ipv6
) :
    socket_base_t(address, port, ipv6),
    address_ptr(0),
    address_size(0)
{
    bool
        broadcast = false;

    std::memset((void *)&address_ipv4, 0, sizeof(inet_socket_address_t));
    std::memset((void *)&address_ipv6, 0, sizeof(inet6_socket_address_t));

    if (ipv6)
    {
        network::set_address(address, port, address_ipv6);

        address_ptr = &(address_ipv6);
        address_size = sizeof(inet6_socket_address_t);
    }
    else
    {
        network::set_address(address, port, address_ipv4, broadcast);

        address_ptr = &(address_ipv4);
        address_size = sizeof(inet_socket_address_t);
    }

    if (broadcast)
    {
        LOG_VERBOSE("Enabling broadcast...");
        set_socket_option(SOL_SOCKET, SO_BROADCAST, ON);
    }
}

// ----------------------------------------------------------------------------
vdb::playback_socket_t::~playback_socket_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::playback_socket_t::send(const pdu_data_t &data)
{
    int
        flags = 0;

    int result = sendto(
        descriptor,
        (void *)data.get_pdu_buffer(),
        data.get_pdu_length(),
        flags,
        (socket_address_t *)address_ptr,
        address_size);

    if (result == -1)
    {
        perror("sendto");
        exit(1);
    }
    else if (((unsigned)result) != data.get_pdu_length())
    {
        LOG_WARNING(
            "Socket %d of %d bytes sent!",
            result,
            data.get_pdu_length());
    }
}
