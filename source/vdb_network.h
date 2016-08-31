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
#include "vdb_hexadecimal.h"
#include "vdb_network_types.h"
#include "vdb_object.h"

namespace vdb
{
    class pdu_data_t;

    typedef struct sockaddr
        socket_address_t;
    typedef struct sockaddr_in
        inet_socket_address_t;
    typedef struct sockaddr_in6
        inet6_socket_address_t;

    typedef struct in_addr
        inet_address_t;
    typedef struct in6_addr
        inet6_address_t;

    typedef struct hostent
        host_entry_t;
    typedef struct ifreq
        interface_request_t;

    typedef struct { uint8_t address[IFHWADDRLEN]; } mac_address_t;

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

        static std::string str(const inet_address_t &);
        static std::string str(const inet6_address_t &);

        static bool get_hostname(const inet_address_t &, std::string &);
        static bool get_hostname(const inet6_address_t &, std::string &);

        static const char
            *ANY,
            *LOOPBACK,
            *BROADCAST;

      private:

        static bool query_hostname(
            const inet_address_t &,
            std::string &
        );

        static bool query_hostname(
            const inet6_address_t &,
            std::string &
        );

        static ipv4_address_cache_t
            ipv4_address_cache;
    };

    // ------------------------------------------------------------------------
    class socket_base_t
    {
      public:

        int32_t get_port(void) const { return port; }

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

    // ------------------------------------------------------------------------
    class ip_header_t : public record_t
    {
      public:

        ip_header_t(void) { }
        virtual ~ip_header_t(void) { }

        virtual const int8_t *get_protocol(void) const = 0;

        virtual const void *get_source_address(void) const = 0;
        virtual uint32_t get_address_size(void) const = 0;
    };

    // ------------------------------------------------------------------------
    class ipv4_header_t : public ip_header_t
    {
      public:

        ipv4_header_t(void);
        virtual ~ipv4_header_t(void);

        virtual const int8_t *get_protocol(void) const { return &protocol; }

        virtual const void *get_source_address(void) const;
        virtual uint32_t get_address_size(void) const;

        virtual void clear(void);

        virtual uint32_t length(void) const { return sizeof(*this); }

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            version,        // 4 bits
            header_length;  // 4 bits
        uint8_t
            service_type;
        uint16_t
            total_length,
            identification,
            fragment_offset_flags;
        uint8_t
            ttl;
        int8_t
            protocol;
        uint16_t
            header_checksum;
        inet_address_t
            source_address,
            destination_address;
    };

    // ------------------------------------------------------------------------
    class ipv6_header_t : public ip_header_t
    {
      public:

        ipv6_header_t(void);
        virtual ~ipv6_header_t(void);

        virtual const int8_t *get_protocol(void) const { return 0; }

        virtual const void *get_source_address(void) const;
        virtual uint32_t get_address_size(void) const;

        virtual void clear(void);

        virtual uint32_t length(void) const { return sizeof(*this); }

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            version,        //  4 bits
            traffic_class;  //  8 bits
        int32_t
            flow_label;     // 20 bits
        uint16_t
            payload_length;
        uint8_t
            next_header,
            hop_limit;
        inet6_address_t
            source_address,
            destination_address;
    };

    // ------------------------------------------------------------------------
    class udp_header_t : public record_t
    {
      public:

        udp_header_t(void);
        virtual ~udp_header_t(void);

        virtual void clear(void);

        virtual uint32_t length(void) const { return sizeof(*this); }

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            source_port,
            destination_port,
            payload_length,
            checksum;
    };

    // ------------------------------------------------------------------------
    inline const void *ipv4_header_t::get_source_address(void) const
    {
        return &source_address;
    }

    // ------------------------------------------------------------------------
    inline uint32_t ipv4_header_t::get_address_size(void) const
    {
        return sizeof(source_address);
    }

    // ------------------------------------------------------------------------
    inline const void *ipv6_header_t::get_source_address(void) const
    {
        return &source_address;
    }

    // ------------------------------------------------------------------------
    inline uint32_t ipv6_header_t::get_address_size(void) const
    {
        return sizeof(source_address);
    }
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &os,
    const vdb::inet_address_t &address)
{
    return (os << vdb::network::get_address(AF_INET, &address));
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &os,
    const vdb::inet6_address_t &address)
{
    return (os << vdb::network::get_address(AF_INET6, &address));
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &os,
    const vdb::mac_address_t &value)
{
    os  << vdb::hexadecimal::str(value.address[0]) << ":"
        << vdb::hexadecimal::str(value.address[1]) << ":"
        << vdb::hexadecimal::str(value.address[2]) << ":"
        << vdb::hexadecimal::str(value.address[3]) << ":"
        << vdb::hexadecimal::str(value.address[4]) << ":"
        << vdb::hexadecimal::str(value.address[5]);

    return os;
}

#endif
