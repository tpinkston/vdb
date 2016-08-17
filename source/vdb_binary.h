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

#ifndef VDB_BINARY_H
#define VDB_BINARY_H

#include "vdb_common.h"

namespace vdb
{
    class binary
    {
      public:

        static std::string str(int8_t i);
        static std::string str(uint8_t i);

        static std::string str(int16_t i, bool separated = false);
        static std::string str(uint16_t i, bool separated = false);

        static std::string str(int32_t i, bool separated = false);
        static std::string str(uint32_t i, bool separated = false);

        static std::string str(int64_t i, bool separated = false);
        static std::string str(uint64_t i, bool separated = false);

        static int8_t parse_int8(const std::string &string);
        static int16_t parse_int16(const std::string &string);
        static int32_t parse_int32(const std::string &string);
        static int64_t parse_int64(const std::string &string);

        static uint32_t get_1_bit_8(uint16_t i, uint8_t bits);
        static uint32_t get_1_bit_16(uint16_t i, uint16_t bits);
        static uint32_t get_1_bit_32(uint16_t i, uint32_t bits);

        static uint32_t get_2_bits_8(uint16_t i, uint8_t bits);
        static uint32_t get_2_bits_16(uint16_t i, uint16_t bits);
        static uint32_t get_2_bits_32(uint16_t i, uint32_t bits);

        static uint32_t get_3_bits_8(uint16_t i, uint8_t bits);
        static uint32_t get_3_bits_16(uint16_t i, uint16_t bits);
        static uint32_t get_3_bits_32(uint16_t i, uint32_t bits);

        static uint32_t get_4_bits_8(uint16_t i, uint8_t bits);
        static uint32_t get_4_bits_16(uint16_t i, uint16_t bits);
        static uint32_t get_4_bits_32(uint16_t i, uint32_t bits);

        static uint8_t set_1_bit_8(uint16_t i, uint8_t bits, uint32_t value);
        static uint16_t set_1_bit_16(uint16_t i, uint16_t bits, uint32_t value);
        static uint32_t set_1_bit_32(uint16_t i, uint32_t bits, uint32_t value);

        static uint8_t set_2_bits_8(uint16_t i, uint8_t bits, uint32_t value);
        static uint16_t set_2_bits_16(uint16_t i, uint16_t bits, uint32_t value);
        static uint32_t set_2_bits_32(uint16_t i, uint32_t bits, uint32_t value);

        static uint8_t set_3_bits_8(uint16_t i, uint8_t bits, uint32_t value);
        static uint16_t set_3_bits_16(uint16_t i, uint16_t bits, uint32_t value);
        static uint32_t set_3_bits_32(uint16_t i, uint32_t bits, uint32_t value);

        static uint8_t set_4_bits_8(uint16_t i, uint8_t bits, uint32_t value);
        static uint16_t set_4_bits_16(uint16_t i, uint16_t bits, uint32_t value);
        static uint32_t set_4_bits_32(uint16_t i, uint32_t bits, uint32_t value);
    };
}

#endif
