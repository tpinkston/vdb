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

#ifndef VDB_DATA_BUFFER_H
#define VDB_DATA_BUFFER_H

#include "vdb_object.h"

namespace vdb
{
    class data_buffer_t : public record_t
    {
      public:

        data_buffer_t(void);
        data_buffer_t(uint16_t length);
        data_buffer_t(uint16_t length, const uint8_t *data);
        data_buffer_t(const data_buffer_t &copy);
        virtual ~data_buffer_t(void);

        uint8_t operator[](uint32_t index) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            data_length;
        uint8_t
            *data_ptr;
    };

    // ------------------------------------------------------------------------
    inline uint32_t vdb::data_buffer_t::length(void) const
    {
        return data_length;
    }
}

#endif
