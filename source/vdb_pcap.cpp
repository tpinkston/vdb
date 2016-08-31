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

#include "vdb_byte_stream.h"
#include "vdb_hexadecimal.h"
#include "vdb_network.h"
#include "vdb_pcap.h"

// ----------------------------------------------------------------------------
vdb::pcap_ethernet_t::pcap_ethernet_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
vdb::pcap_ethernet_t::~pcap_ethernet_t(void)
{

}

// ----------------------------------------------------------------------------
uint16_t vdb::pcap_ethernet_t::ip_version(void) const
{
    switch(type)
    {
        case ETHERTYPE_IP:
            return 4;
        case ETHERTYPE_IPV6:
            return 6;
    }

    return 0;
}

// ----------------------------------------------------------------------------
void vdb::pcap_ethernet_t::clear(void)
{
    std::memset(&destination, 0, sizeof(mac_address_t));
    std::memset(&source, 0, sizeof(mac_address_t));

    type = 0;
}

// ----------------------------------------------------------------------------
void vdb::pcap_ethernet_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "destination " << destination << std::endl
           << prefix << "source " << source << std::endl
           << prefix << "type " << type << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::pcap_ethernet_t::read(byte_stream &stream)
{
    stream.read(&destination, sizeof(mac_address_t));
    stream.read(&source, sizeof(mac_address_t));
    stream.read(type);
}

// ----------------------------------------------------------------------------
void vdb::pcap_ethernet_t::write(byte_stream &stream) const
{
    stream.write(&destination, sizeof(mac_address_t));
    stream.write(&source, sizeof(mac_address_t));
    stream.write(type);
}
