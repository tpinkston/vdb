#include "vdis_byte_buffer.h"
#include "vdis_logger.h"
#include "vdis_network.h"
#include "vdis_string.h"

namespace
{
    uint32_t get_key(const vdis::address_ipv6_t &address)
    {
        uint32_t
            key = 9,            // arbitrary seed value
            multiplier = 13;    // arbitrary multiplier value

        for(uint32_t i = 0; i < 16; ++i)
        {
            key = key * multiplier + address.s6_addr[i];
        }

        return key;
    }

    typedef std::map<uint32_t, std::string>
        hostname_cache_t;

    const int32_t
        ON = 0x01,
        OFF = 0x00;
    const uint32_t
        HOSTNAME_MAX_SIZE = 512;
    const std::string
        ANY = "any",
        LOOPBACK = "loopback",
        BROADCAST = "broadcast";
    const std::string
        UNKNOWN_HOSTNAME = "unknown";
    hostname_cache_t
        ipv4_hostnames,
        ipv6_hostnames;
}

// ----------------------------------------------------------------------------
int32_t vdis::open_socket(bool ipv6)
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
void vdis::set_address(
    const char *address,
    int32_t port,
    socket_address_ipv4_t &socket_address,
    bool &broadcast)
{
    std::memset(&socket_address, 0, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);

    LOG_VERBOSE("Setting address: %s...", (address ? address : ANY.c_str()));

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
void vdis::set_address(
    const char *address,
    address_ipv4_t &inet_address,
    bool &broadcast,
    bool exit_on_error)
{
    const std::string
        address_string = to_lowercase(address ? address : ANY.c_str());

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
        int result = inet_pton(AF_INET, address, &inet_address);

        if (result == -1)
        {
            LOG_ERROR("inet_pton: '%s'", strerror(errno));

            if (exit_on_error)
            {
                exit(1);
            }
        }
        else if (result == 0)
        {
            LOG_ERROR("Invalid address '%s'", address);

            if (exit_on_error)
            {
                exit(1);
            }
        }

        broadcast = ends_with(address_string, ".255");

        if (logger::is_enabled(logger::VERBOSE))
        {
            byte_buffer_t
                temp_buffer(
                    (const uint8_t *)&inet_address,
                    sizeof(address_ipv4_t));

            LOG_VERBOSE("Converted %s to IPv4 address:", address);

            temp_buffer.print(std::string(), std::cout);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::set_address(
    const char *address,
    int32_t port,
    socket_address_ipv6_t &socket_address)
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
void vdis::set_address(
    const char *address,
    address_ipv6_t &inet6_address,
    bool exit_on_error)
{
    const std::string
        address_string = vdis::to_lowercase(address);

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
            LOG_ERROR("inet_pton: '%s'", strerror(errno));

            if (exit_on_error)
            {
                exit(1);
            }
        }
        else if (result == 0)
        {
            LOG_ERROR("Invalid address '%s'", address);

            if (exit_on_error)
            {
                exit(1);
            }
        }

        if (logger::is_enabled(logger::VERBOSE))
        {
            vdis::byte_buffer_t
                temp_buffer(
                    (const uint8_t *)&inet6_address,
                    sizeof(address_ipv6_t));

            LOG_VERBOSE("Converted %s to IPv6 address:", address);

            temp_buffer.print(std::string(), std::cout);
        }
    }
}

// ----------------------------------------------------------------------------
std::string vdis::get_address(const address_ipv4_t &address)
{
    char
        buffer[64];

    std::memset(buffer, 0, 64);

    const char *result = inet_ntop(
        AF_INET,
        &address,
        buffer,
        INET_ADDRSTRLEN);

    if (result)
    {
        return std::string(buffer);
    }
    else
    {
        return UNKNOWN_HOSTNAME;
    }
}

// ----------------------------------------------------------------------------
std::string vdis::get_address(const address_ipv6_t &address)
{
    char buffer[64];

    std::memset(buffer, 0, 64);

    const char *result = inet_ntop(
        AF_INET6,
        &address,
        buffer,
        INET6_ADDRSTRLEN);

    if (result)
    {
        return std::string(buffer);
    }
    else
    {
        return UNKNOWN_HOSTNAME;
    }
}

// ----------------------------------------------------------------------------
// Uses caching to store hostnames, cache gets checked first and if not
// cached a call is made to the overloaded 'query_hostname' that makes
// 'getnameinfo' system call.
//
std::string vdis::query_hostname(const socket_address_ipv4_t &address)
{
    const uint32_t
        key = address.sin_addr.s_addr;
    hostname_cache_t::const_iterator
        search_itor = ipv4_hostnames.find(key);
    std::string
        hostname;

    if (search_itor != ipv4_hostnames.end())
    {
        hostname = search_itor->second;
    }
    else
    {
        if (not query_hostname(address, hostname))
        {
            hostname = UNKNOWN_HOSTNAME;
        }

        ipv4_hostnames[key] = hostname;

        LOG_VERBOSE(
            "Caching IPv4 key = %d, hostname = '%s'",
            key,
            hostname.c_str());
    }

    return hostname;
}

// ----------------------------------------------------------------------------
// Uses caching to store hostnames, cache gets checked first and if not
// cached a call is made to the overloaded 'query_hostname' that makes
// 'getnameinfo' system call.
//
std::string vdis::query_hostname(const socket_address_ipv6_t &address)
{
    const uint32_t
        key = get_key(address.sin6_addr);
    hostname_cache_t::const_iterator
        search_itor = ipv4_hostnames.find(key);
    std::string
        hostname;

    if (search_itor != ipv4_hostnames.end())
    {
        hostname = search_itor->second;
    }
    else
    {
        if (not query_hostname(address, hostname))
        {
            hostname = UNKNOWN_HOSTNAME;
        }

        ipv4_hostnames[key] = hostname;

        LOG_VERBOSE(
            "Caching IPv6 key = %d, hostname = '%s'",
            key,
            hostname.c_str());
    }

    return hostname;
}

// ----------------------------------------------------------------------------
bool vdis::query_hostname(
    const socket_address_ipv4_t &address,
    std::string &host)
{
    char
        hostname[HOSTNAME_MAX_SIZE],
        service[HOSTNAME_MAX_SIZE];
    int
        result = 0;

    std::memset(hostname, 0, HOSTNAME_MAX_SIZE);
    std::memset(service, 0, HOSTNAME_MAX_SIZE);

    result = getnameinfo(
        (sockaddr*)&address,
        sizeof(socket_address_ipv4_t),
        hostname, HOSTNAME_MAX_SIZE,
        service, HOSTNAME_MAX_SIZE,
        0);

    if (result == 0)
    {
        host = hostname;
    }
    else
    {
        LOG_ERROR(
            "Name resolution failed for %s, family %d: (result %d = '%s')",
            get_address(address.sin_addr).c_str(),
            address.sin_family,
            result,
            gai_strerror(result));
    }

    return (result == 0);
}

// ----------------------------------------------------------------------------
bool vdis::query_hostname(
    const socket_address_ipv6_t &address,
    std::string &host)
{
    char
        hostname[HOSTNAME_MAX_SIZE],
        service[HOSTNAME_MAX_SIZE];
    int
        result = 0;

    std::memset(hostname, 0, HOSTNAME_MAX_SIZE);
    std::memset(service, 0, HOSTNAME_MAX_SIZE);

    result = getnameinfo(
        (sockaddr*)&address,
        sizeof(socket_address_ipv6_t),
        hostname, HOSTNAME_MAX_SIZE,
        service, HOSTNAME_MAX_SIZE,
        0);

    if (result == 0)
    {
        host = hostname;
    }
    else
    {
        LOG_ERROR(
            "Name resolution failed for %s, family %d: (result %d = '%s')",
            get_address(address.sin6_addr).c_str(),
            address.sin6_family,
            result,
            gai_strerror(result));
    }

    return (result == 0);
}

// ----------------------------------------------------------------------------
vdis::socket_base_t::socket_base_t(
    const char *address_ptr,
    int16_t port,
    bool ipv6
) :
    socket_address(address_ptr ? address_ptr : ANY.c_str()),
    socket_ipv6(ipv6),
    socket_port(port),
    socket_descriptor(open_socket(ipv6)),
    socket_error(0),
    socket_address_size(0),
    socket_address_ptr(0)
{
    std::memset(&socket_address_ipv4, 0, sizeof(socket_address_ipv4));
    std::memset(&socket_address_ipv6, 0, sizeof(socket_address_ipv6));

    socket_address_ptr = socket_ipv6 ?
        (void *)&socket_address_ipv6:
        (void *)&socket_address_ipv4;
    socket_address_size = socket_ipv6 ?
        sizeof(socket_address_ipv6):
        sizeof(socket_address_ipv4);

    if (socket_descriptor == -1)
    {
        LOG_ERROR(
            "Failed to open %s socket: %s",
            (socket_ipv6 ? "IPv6" : "IPv4"),
            std::strerror(errno));

        socket_error = ERROR_OPEN;
    }
    else
    {
        LOG_VERBOSE(
            "Opened %s socket %d on %s:%d...",
            (socket_ipv6 ? "IPv6" : "IPv4"),
            socket_descriptor,
            socket_address,
            socket_port);

        set_socket_option(SOL_SOCKET, SO_REUSEADDR, ON);
    }
}

// ----------------------------------------------------------------------------
vdis::socket_base_t::~socket_base_t(void)
{
    if (socket_descriptor > 0)
    {
        LOG_VERBOSE(
            "Closing socket %d on port %d...",
            socket_descriptor,
            socket_port);

        if (close(socket_descriptor) != 0)
        {
            LOG_ERROR(
                "Failed to close socket: %s",
                std::strerror(errno));

            socket_error = ERROR_BIND;
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::socket_base_t::set_socket_option(
    int32_t level,
    int32_t option,
    int32_t value) {

    int32_t result = setsockopt(
        socket_descriptor,
        level,
        option,
        &value,
        sizeof(int32_t));

    if (result != 0)
    {
        LOG_ERROR(
            "Failed to set option (level %d, option %d, value %d)"
            "on %s socket: %s",
            level,
            option,
            value,
            (socket_ipv6 ? "IPv6" : "IPv4"),
            std::strerror(errno));

        socket_error = ERROR_OPTION;
    }
}

// ----------------------------------------------------------------------------
void vdis::socket_base_t::bind_socket(void)
{
    if (socket_ipv6)
    {
        set_address(
            socket_address,
            socket_port,
            socket_address_ipv6);

        int32_t result = bind(
            socket_descriptor,
            (socket_address_t *)&socket_address_ipv6,
            sizeof(socket_address_ipv6));

        if (result < 0)
        {
            LOG_ERROR(
                "Failed to bind IPv6 socket (address '%s'): %s",
                socket_address,
                std::strerror(errno));

            socket_error = ERROR_BIND;
        }
    }
    else
    {
        bool broadcast = false;

        set_address(
            socket_address,
            socket_port,
            socket_address_ipv4,
            broadcast);

        LOG_VERBOSE(
            "Binding on address '%s' port %d...",
            get_address(socket_address_ipv4.sin_addr).c_str(),
            socket_port);

        int32_t result = bind(
            socket_descriptor,
            (socket_address_t *)&socket_address_ipv4,
            sizeof(socket_address_ipv4));

        if (result < 0)
        {
            LOG_ERROR(
                "Failed to bind IPv4 socket (address '%s'): %s",
                socket_address,
                std::strerror(errno));

            socket_error = ERROR_BIND;
        }
    }
}

// ----------------------------------------------------------------------------
vdis::send_socket_t::send_socket_t(
    bool ipv6,
    int16_t port,
    const char *address_ptr
) :
   socket_base_t(address_ptr ? address_ptr : BROADCAST.c_str(), port, ipv6)
{

}

// ----------------------------------------------------------------------------
vdis::send_socket_t::~send_socket_t(void)
{

}

// ----------------------------------------------------------------------------
int32_t vdis::send_socket_t::send(const byte_buffer_t &buffer)
{
    int
        flags = 0;

    int32_t result = sendto(
        socket_descriptor,
        (void *)buffer.buffer(),
        buffer.length(),
        flags,
        (const socket_address_t *)socket_address_ptr,
        socket_address_size);

    if (result == -1)
    {
        perror("sendto");
    }
    else if (((unsigned)result) != buffer.length())
    {
        LOG_WARNING(
            "sendto: %d of %d bytes sent!",
            result,
            buffer.length());
    }

    return result;
}

// ----------------------------------------------------------------------------
vdis::receive_socket_t::receive_socket_t(
    bool ipv6,
    int16_t port,
    const char *address_ptr
) :
    socket_base_t(address_ptr ? address_ptr : ANY.c_str(), port, ipv6)
{
    bind_socket();
}

// ----------------------------------------------------------------------------
vdis::receive_socket_t::~receive_socket_t(void)
{

}

// ----------------------------------------------------------------------------
int32_t vdis::receive_socket_t::receive(byte_buffer_t &buffer)
{
    std::memset(socket_address_ptr, 0, socket_address_size);

    LOG_EXTRA_VERBOSE("Listening on socket...");

    buffer.reset(BUFFER_LENGTH);

    int32_t result = recvfrom(
        socket_descriptor,
        (void *)buffer.update_buffer(),
        buffer.length(),
        0,
        (socket_address_t *)socket_address_ptr,
        &socket_address_size);

    if (result < 1)
    {
        LOG_EXTRA_VERBOSE("recvfrom() returned %d...", result);
    }

    return result;
}
