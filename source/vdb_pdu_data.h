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

#ifndef VDB_PDU_DATA_H
#define VDB_PDU_DATA_H

#include "vdb_common.h"
#include "vdb_network_types.h"
#include "vdb_object.h"

namespace vdb
{
    class byte_stream;
    class pdu_t;

    class pdu_data_t : public record_t
    {
      public:

        pdu_data_t(void);
        virtual ~pdu_data_t(void);

        uint64_t get_time(void) const { return time; }
        void set_time(uint64_t value) { time = value; }

        uint32_t get_index(void) const { return index; }
        void set_index(uint32_t value) { index = value; }

        const uint8_t *get_address_buffer(void) const;

        uint16_t get_hostname_length(void) const;

        void set_source(
            const inet_socket_address_t &socket_address,
            uint16_t socket_port
        );

        void set_source(
            const inet6_socket_address_t &socket_address,
            uint16_t socket_port
        );

        std::string get_source(void) const;

        void set_pdu_buffer(uint8_t *buffer, uint32_t length);

        const uint8_t *get_pdu_buffer(void) const { return pdu_buffer; }
        uint8_t *update_pdu_buffer(void) { return pdu_buffer; }

        uint32_t get_pdu_length(void) const { return pdu_length; }
        void set_pdu_length(uint32_t);

        uint8_t get_pdu_exercise(void) const { return pdu_buffer[1]; }
        uint8_t get_pdu_type(void) const { return pdu_buffer[2]; }
        uint8_t get_pdu_family(void) const { return pdu_buffer[3]; }

        // Generates new PDU object from data, it's the caller's responsibility
        // to free memory (delete).
        //
        pdu_t *generate_pdu(void) const;

        // Clears all PDU data on this object.
        //
        void clear(void);

        void print(const std::string &prefix, std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // "PDUD" in ASCII (4 bytes)
        static const uint32_t
            HEADER = 0x50445544;

        // Accommodate IPv4 and IPv6 addresses (16 bytes)
        static const uint32_t
            ADDRESS_LENGTH = 0x10;

        static const uint32_t
            PDU_BUFFER_LENGTH = 0x800; // 2 KB

        // In save/restore order:
        //
        // header                   4
        // index                    4
        // time                     8
        // padding                  4
        // status                   2
        // host name length         2
        // ------- (host name) ------
        // address                 16
        // port                     2
        // ip_version               2
        // pdu_length               4
        // ------ (PDU buffer) ------
        // ==========================
        // total                   48
        //
        static const uint32_t
            BASE_LENGTH = 0x30; // 48 bytes

      protected:

        uint32_t
            header,
            index;
        uint64_t
            time;
        uint32_t
            padding;
        uint16_t
            status; // bit flags
        std::string
            hostname;
        uint8_t
            address[ADDRESS_LENGTH];
        uint16_t
            port;
        uint16_t
            ip_version;
        uint32_t
            pdu_length;
        uint8_t
            pdu_buffer[PDU_BUFFER_LENGTH];
    };
}

// ----------------------------------------------------------------------------
inline vdb::pdu_data_t::pdu_data_t(void) :
    header(HEADER),
    index(0),
    time(0),
    padding(0),
    status(0),
    port(0),
    ip_version(0),
    pdu_length(0)
{
    std::memset(address, 0, ADDRESS_LENGTH);
    std::memset(pdu_buffer, 0, PDU_BUFFER_LENGTH);
}

// ----------------------------------------------------------------------------
inline vdb::pdu_data_t::~pdu_data_t(void)
{

}

// ----------------------------------------------------------------------------
inline void vdb::pdu_data_t::set_pdu_buffer(uint8_t *buffer, uint32_t length)
{
    std::memset(pdu_buffer, 0, PDU_BUFFER_LENGTH);
    std::memcpy(pdu_buffer, buffer, length);

    set_pdu_length(length);
}

// ----------------------------------------------------------------------------
inline void vdb::pdu_data_t::set_pdu_length(uint32_t length)
{
    pdu_length = length;
}

#endif
