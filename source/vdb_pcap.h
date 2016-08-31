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

#ifndef VDB_PCAP_H
#define VDB_PCAP_H

#include "vdb_network.h"
#include "vdb_object.h"

namespace vdb
{
    typedef struct pcap_pkthdr
        pcap_packet_header_t;

    // ------------------------------------------------------------------------
    class pcap_ethernet_t : public record_t
    {
      public:

        pcap_ethernet_t(void);
        virtual ~pcap_ethernet_t(void);

        uint16_t ip_version(void) const;

        virtual void clear(void);

        virtual uint32_t length(void) const { return sizeof(*this); }

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        mac_address_t
            destination,
            source;
        uint16_t
            type;
    };
}

#endif
