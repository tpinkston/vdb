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

#ifndef VDB_BITS_H
#define VDB_BITS_H

#include "vdb_common.h"

namespace vdb
{
    typedef struct
    {
        uint8_t v1:1; // Bit 0
        uint8_t v2:1; // Bit 1
        uint8_t v3:1; // Bit 2
        uint8_t v4:1; // Bit 3
        uint8_t v5:1; // Bit 4
        uint8_t v6:1; // Bit 5
        uint8_t v7:1; // Bit 6
        uint8_t v8:1; // Bit 7
    } record8x1_t;

    typedef struct
    {
        uint8_t v1:2; // Bits 0-1
        uint8_t v2:2; // Bits 2-3
        uint8_t v3:2; // Bits 4-5
        uint8_t v4:2; // Bits 6-7
    } record8x2_t;

    typedef struct
    {
        uint8_t v1:4; // Bits 0-3
        uint8_t v2:4; // Bits 4-7
    } record8x4_t;

    typedef union
    {
        record8x1_t s1;
        record8x2_t s2;
        record8x4_t s4;
        uint8_t value;
    } record8_t;

    typedef struct
    {
        uint8_t v1:1;  // Bit 0
        uint8_t v2:1;  // Bit 1
        uint8_t v3:1;  // Bit 2
        uint8_t v4:1;  // Bit 3
        uint8_t v5:1;  // Bit 4
        uint8_t v6:1;  // Bit 5
        uint8_t v7:1;  // Bit 6
        uint8_t v8:1;  // Bit 7
        uint8_t v9:1;  // Bit 8
        uint8_t v10:1; // Bit 9
        uint8_t v11:1; // Bit 10
        uint8_t v12:1; // Bit 11
        uint8_t v13:1; // Bit 12
        uint8_t v14:1; // Bit 13
        uint8_t v15:1; // Bit 14
        uint8_t v16:1; // Bit 15
    } record16x1_t;

    typedef struct
    {
        uint8_t v1:2; // Bits 0-1
        uint8_t v2:2; // Bits 2-3
        uint8_t v3:2; // Bits 4-5
        uint8_t v4:2; // Bits 6-7
        uint8_t v5:2; // Bits 8-9
        uint8_t v6:2; // Bits 10-11
        uint8_t v7:2; // Bits 12-13
        uint8_t v8:2; // Bits 14-15
    } record16x2_t;

    typedef struct
    {
        uint8_t v1:4; // Bits 0-3
        uint8_t v2:4; // Bits 4-7
        uint8_t v3:4; // Bits 8-11
        uint8_t v4:4; // Bits 12-15
    } record16x4_t;

    typedef union
    {
        record16x1_t s1;
        record16x2_t s2;
        record16x4_t s4;
        uint16_t value;
    } record16_t;
}

#endif
