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

#ifndef VDB_NETWORK_H
#define VDB_NETWORK_H

#include "vdb_common.h"
#include "vdb_network_types.h"
#include "vdb_pdu_data.h"

namespace vdb
{
    typedef std::map<uint32_t, std::string>
        ipv4_address_cache_t;

    // ------------------------------------------------------------------------
    class network
    {
      public:

        static int32_t open_socket(bool ipv6);

        static void set_address(
            const char *address,
            int32_t port,
            inet_socket_address_t &socket_address,
            bool &broadcast
        );

        static void set_address(
            const char *address,
            inet_address_t &inet_address,
            bool &broadcast,
            bool fail_on_error = true
        );

        static void set_address(
            const char *address,
            int32_t port,
            inet6_socket_address_t &socket_address
        );

        static void set_address(
            const char *address,
            inet6_address_t &inet6_address,
            bool fail_on_error = true
        );

        // Returns IP address as string (e.g. '192.168.1.1')
        //
        // family:  is AF_INET or AF_INET6
        // address: pointer to 'inet_address_t' or 'inet6_address_t'
        //
        static std::string get_address(int family, const void *address_ptr);

        static bool get_hostname(const inet_socket_address_t &, std::string &);
        static bool get_hostname(const inet6_socket_address_t &, std::string &);

        static const char
            *ANY,
            *LOOPBACK,
            *BROADCAST;

      private:

        static bool query_hostname(
            const inet_socket_address_t &,
            std::string &
        );

        static bool query_hostname(
            const inet6_socket_address_t &,
            std::string &
        );

        static ipv4_address_cache_t
            ipv4_address_cache;
    };

    // ------------------------------------------------------------------------
    class socket_base_t
    {
      public:

        int32_t get_port(void) const;

      protected:

        socket_base_t(const char *address, int32_t port, bool ipv6);
        virtual ~socket_base_t();

        void set_socket_option(int32_t level, int32_t option, int32_t value);
        void bind_socket(void);
        void close_socket(void);

        const char
            *address;
        const int32_t
            port;
        const bool
            ipv6;
        const int32_t
            descriptor;

        static const int32_t
            ON,
            OFF;
    };

    // ------------------------------------------------------------------------
    class capture_socket_t : public socket_base_t
    {
      public:

        capture_socket_t(const char *address, int32_t port, bool ipv6);
        virtual ~capture_socket_t(void);

        void receive(pdu_data_t &data);

      private:

        inet_socket_address_t
            address_ipv4;
        inet6_socket_address_t
            address_ipv6;
        socklen_t
            address_size;
    };

    // ------------------------------------------------------------------------
    class playback_socket_t : public socket_base_t
    {
      public:

        playback_socket_t(const char *address, int32_t port, bool ipv6);
        virtual ~playback_socket_t(void);

        void send(const pdu_data_t &data);

      protected:

        inet_socket_address_t
            address_ipv4;
        inet6_socket_address_t
            address_ipv6;
        void
            *address_ptr;
        size_t
            address_size;
    };
}

// ----------------------------------------------------------------------------
inline int32_t vdb::socket_base_t::get_port(void) const
{
    return port;
}

#endif
