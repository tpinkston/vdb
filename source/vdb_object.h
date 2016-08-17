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

#ifndef VDB_OBJECT_H
#define VDB_OBJECT_H

#include "vdb_common.h"

namespace vdb
{
    class byte_stream;

    // ------------------------------------------------------------------------
    class object_t
    {
      public:

        object_t(void) { }
        virtual ~object_t(void) { }

        // Clear all attributes and restores object to default state.
        //
        virtual void clear(void) = 0;

        // Returns object as human-readable string.
        //
        virtual std::string str(void) const;

        // Returns object as human-readable, C-style string.
        //
        virtual const char *c_str(void) const;

        // Streams object attributes to output stream.
        //
        virtual void print(const std::string &prefix, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    class record_t : public object_t
    {
      public:

        record_t(void) { }
        virtual ~record_t(void) { }

        // Returns length in bytes of record.
        //
        virtual uint32_t length(void) const = 0;

        // Read and write operations.
        //
        virtual void read(byte_stream &stream) = 0;
        virtual void write(byte_stream &stream) const = 0;
    };

    // ------------------------------------------------------------------------
    inline std::string vdb::object_t::str(void) const
    {
        std::ostringstream
            stream;

        print(std::string(), stream);

        return stream.str();
    }

    // ------------------------------------------------------------------------
    inline const char *vdb::object_t::c_str(void) const
    {
        return str().c_str();
    }

    // ------------------------------------------------------------------------
    inline void vdb::object_t::print(
        const std::string &prefix,
        std::ostream &stream) const
    {
        stream << prefix << "[OUTPUT_NOT_AVAILABLE]";
    }
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &stream,
    const vdb::object_t &object)
{
    object.print(std::string(), stream);
    return stream;
}

#endif
