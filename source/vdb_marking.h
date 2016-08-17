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

#ifndef VDB_MARKING_H
#define VDB_MARKING_H

#include "vdb_object.h"

#define ENTITY_MARKING_CHARACTERS 11

namespace vdb
{
    class entity_marking_t : public record_t
    {
      public:

        entity_marking_t(void);
        entity_marking_t(const entity_marking_t &copy);
        virtual ~entity_marking_t(void);

        std::string hex_characters(void) const;

        std::string ascii_characters(void) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        virtual entity_marking_t &operator=(const entity_marking_t &);

        virtual bool operator==(const entity_marking_t &) const;
        virtual bool operator!=(const entity_marking_t &) const;
        virtual bool operator<(const entity_marking_t &) const;
        virtual bool operator>(const entity_marking_t &) const;

        uint8_t
            character_set;
        char
            characters[ENTITY_MARKING_CHARACTERS];

        static const uint32_t
            LENGTH;
    };
}

#endif
