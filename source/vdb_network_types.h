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

#ifndef VDB_NETWORK_TYPES_H
#define VDB_NETWORK_TYPES_H

#include "vdb_common.h"

namespace vdb
{
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
}

#endif
