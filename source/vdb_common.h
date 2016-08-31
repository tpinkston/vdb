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

#ifndef VDB_COMMON_H
#define VDB_COMMON_H

// C++ headers
#include <algorithm>
#include <bitset>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// C headers
#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// PCAP headers
#include <pcap.h>
#include <ethertype.h>

#ifdef __CYGWIN__
#include <cygwin/socket.h>
#else
#include <sys/socket.h>
#endif

typedef float  float32_t;
typedef double float64_t;

typedef std::set<int32_t> int32_set_t;
typedef std::set<uint32_t> uint32_set_t;

namespace vdb
{
    // ------------------------------------------------------------------------
    // Endian-ness and byte swapping

    bool is_little_endian(void);

    bool byteswapping(void);
    void set_byteswapping(void);
    void force_byteswapping(bool value);

    void byteswap(int16_t &value, bool force = false);
    void byteswap(uint16_t &value, bool force = false);
    void byteswap(int32_t &value, bool force = false);
    void byteswap(uint32_t &value, bool force = false);
    void byteswap(int64_t &value, bool force = false);
    void byteswap(uint64_t &value, bool force = false);

    // ------------------------------------------------------------------------
    // Conversion between floating point bits to integer bits

    float32_t get_float32(uint32_t i);
    float64_t get_float64(uint64_t i);
    uint32_t get_int32(float32_t f);
    uint64_t get_int64(float64_t f);

    // ------------------------------------------------------------------------
    // Pauses currently running thread for the specified time.
    //
    void pause(uint32_t milliseconds);

    // ------------------------------------------------------------------------
    // Returns true if user confirms action
    //
    bool user_confirmation(const char *prompt_ptr = 0);

    // ------------------------------------------------------------------------
    inline bool is_little_endian(void)
    {
        #if (__BYTE_ORDER == __LITTLE_ENDIAN)
            return true;
        #else
            return false;
        #endif
    }

    // ------------------------------------------------------------------------
    inline float32_t get_float32(uint32_t value)
    {
        return *(reinterpret_cast<float32_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline float64_t get_float64(uint64_t value)
    {
        return *(reinterpret_cast<float64_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline uint32_t get_int32(float32_t value)
    {
        return *(reinterpret_cast<uint32_t *>(&value));
    }

    // ------------------------------------------------------------------------
    inline uint64_t get_int64(float64_t value)
    {
        return *(reinterpret_cast<uint64_t *>(&value));
    }
}

#endif
