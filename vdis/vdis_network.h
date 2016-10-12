#ifndef VDIS_NETWORK_H
#define VDIS_NETWORK_H

#include "vdis_system.h"

namespace vdis
{
    class byte_buffer_t;

    typedef struct sockaddr         socket_address_t;
    typedef struct sockaddr_in      socket_address_ipv4_t;
    typedef struct sockaddr_in6     socket_address_ipv6_t;
    typedef struct in_addr          address_ipv4_t;
    typedef struct in6_addr         address_ipv6_t;
    typedef struct hostent          host_t;
    typedef struct ifreq            interface_request_t;

    int32_t open_socket(bool ipv6);

    void set_address(
        const char *address,
        int32_t port,
        socket_address_ipv4_t &socket_address,
        bool &broadcast
    );

    void set_address(
        const char *address,
        address_ipv4_t &inet_address,
        bool &broadcast,
        bool exit_on_error = false
    );

    void set_address(
        const char *address,
        int32_t port,
        socket_address_ipv6_t &socket_address
    );

    void set_address(
        const char *address,
        address_ipv6_t &inet6_address,
        bool exit_on_error = false
    );

    std::string get_address(const address_ipv4_t &);
    std::string get_address(const address_ipv6_t &);

    std::string query_hostname(const socket_address_ipv4_t &);
    std::string query_hostname(const socket_address_ipv6_t &);

    bool query_hostname(const socket_address_ipv4_t &, std::string &);
    bool query_hostname(const socket_address_ipv6_t &, std::string &);

    // ------------------------------------------------------------------------
    class socket_base_t
    {
      public:

        const char             *socket_address;
        const bool              socket_ipv6;
        const int16_t           socket_port;
        const int32_t           socket_descriptor;
        uint32_t                socket_error;

        inline const socket_address_ipv4_t &address_ipv4(void) const
        {
            return socket_address_ipv4;
        }

        inline const socket_address_ipv6_t &address_ipv6(void) const
        {
            return socket_address_ipv6;
        }

        inline socklen_t address_size(void) const
        {
            return socket_address_size;
        }

      protected:

        socket_base_t(const char *address_ptr, int16_t port, bool ipv6);
        virtual ~socket_base_t(void);

        void set_socket_option(int32_t level, int32_t option, int32_t value);
        void bind_socket(void);

        socket_address_ipv4_t
            socket_address_ipv4;
        socket_address_ipv6_t
            socket_address_ipv6;
        socklen_t
            socket_address_size;
        void
            *socket_address_ptr;

        const static uint32_t
            BUFFER_LENGTH = 0x600,
            ERROR_OPEN = 0x10,
            ERROR_OPTION = 0x20,
            ERROR_BIND = 0x30,
            ERROR_SOCKET_CLOSE = 0x100;
    };

    // ------------------------------------------------------------------------
    class send_socket_t : public socket_base_t
    {
      public:

        send_socket_t(bool ipv6, int16_t port, const char *address_ptr = 0);
        virtual ~send_socket_t(void);

        int32_t send(const byte_buffer_t &buffer);
    };

    // ------------------------------------------------------------------------
    class receive_socket_t : public socket_base_t
    {
      public:

        receive_socket_t(bool ipv6, int16_t port, const char *address_ptr = 0);
        virtual ~receive_socket_t(void);

        int32_t receive(byte_buffer_t &buffer);
    };
}

#endif
