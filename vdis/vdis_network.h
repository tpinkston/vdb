#ifndef VDIS_NETWORK_H
#define VDIS_NETWORK_H

#include "vdis_system.h"

namespace vdis
{
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

    bool query_hostname(const socket_address_ipv4_t &, std::string &);
    bool query_hostname(const socket_address_ipv6_t &, std::string &);

    // ------------------------------------------------------------------------
    class socket_base_t
    {
      public:

        const char             *socket_address;
        const int32_t           socket_port;
        const int32_t           socket_descriptor;
        const bool              socket_ipv6;
        uint32_t                socket_error;

      protected:

        socket_base_t(int32_t port, bool ipv6, const char *address_ptr = 0);
        virtual ~socket_base_t();

        void set_socket_option(int32_t level, int32_t option, int32_t value);
        void bind_socket(void);
        void close_socket(void);

        const static uint32_t
            ERROR_OPEN = 0x10,
            ERROR_OPTION = 0x20,
            ERROR_BIND = 0x30,
            ERROR_SOCKET_CLOSE = 0x100;
   };
}

#endif
