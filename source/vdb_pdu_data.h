#ifndef VDB_PDU_DATA_H
#define VDB_PDU_DATA_H

#include <cstring>

#include "vdis_network.h"

namespace vdis
{
    class byte_stream_t;
    class pdu_t;
}

namespace vdb
{
    // 16-bit status record
    //
    // Bits  Description
    // ----  -----------
    // 15-1  Unused
    //    0  PDU data is from PCAP file
    //
    typedef uint16_t
        pdu_data_status_t;

    class pdu_data_t
    {
      public:

        pdu_data_t(void);
        pdu_data_t(const pdu_data_t &copy);
        ~pdu_data_t(void);

        bool has_pdu(void) const { return (pdu_length > 0); }

        uint64_t get_time(void) const { return time; }
        void set_time(uint64_t value) { time = value; }

        uint32_t get_index(void) const { return index; }
        void set_index(uint32_t value) { index = value; }

        const uint8_t *get_address_buffer(void) const;

        const std::string &get_hostname(void) const { return hostname; }
        uint16_t get_hostname_length(void) const;

        void set_source(
            const vdis::socket_address_ipv4_t &source_address,
            uint16_t socket_port
        );

        void set_source(
            const vdis::socket_address_ipv6_t &source_address,
            uint16_t socket_port
        );

        std::string get_source(void) const;

        void set_pdu_buffer(const uint8_t *buffer, uint32_t length);

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
        vdis::pdu_t *generate_pdu(void) const;

        // Clears all PDU data on this object.
        //
        void clear(void);

        void print(const std::string &prefix, std::ostream &stream) const;

        uint32_t length(void) const;

        void read(vdis::byte_stream_t &stream);
        void write(vdis::byte_stream_t &stream) const;

        static const uint32_t
            HEADER =            0x50445544,     // "PDUD" in ASCII (4 bytes)
            ADDRESS_LENGTH =    0x10,           // IPv4 and IPv6 (16 bytes)
            PDU_BUFFER_LENGTH = 0x800,          // 2 KB
            BASE_LENGTH =       0x30;           // 48 bytes

      protected:

        uint32_t                header;                         // 4 bytes
        uint32_t                index;                          // 4 bytes
        uint64_t                time;                           // 8 bytes
        uint32_t                padding;                        // 4 bytes
        pdu_data_status_t       status;                         // 2 bytes
        std::string             hostname;                       // 2+ bytes
        uint8_t                 address[ADDRESS_LENGTH];        // 16 bytes
        uint16_t                port;                           // 2 bytes
        uint16_t                ip_version;                     // 2 bytes
        uint32_t                pdu_length;                     // 4 bytes
        uint8_t                 pdu_buffer[PDU_BUFFER_LENGTH];  // Variable
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
inline void vdb::pdu_data_t::set_pdu_length(uint32_t length)
{
    pdu_length = length;
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(std::ostream &out, const vdb::pdu_data_t &data)
{
    data.print(std::string(), out);

    return out;
}

#endif
