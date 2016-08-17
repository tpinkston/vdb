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

#ifndef VDB_HEXADECIMAL_H
#define VDB_HEXADECIMAL_H

#include "vdb_common.h"

namespace vdb
{
    class hexadecimal
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

        static void stream(
            const uint8_t *array,
            uint32_t length,
            uint32_t byte_width,
            std::ostream &stream
        );

        static void stream(
            const std::string &prefix,
            const uint8_t *array,
            uint32_t length,
            uint32_t byte_width,
            std::ostream &stream
        );
    };
}

#endif
