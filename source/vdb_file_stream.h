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

#ifndef VDB_FILE_STREAM_H
#define VDB_FILE_STREAM_H

#include "vdb_byte_stream.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class file_stream : public byte_stream
    {
      public:

        file_stream(void);
        file_stream(uint32_t length);
        file_stream(const file_stream &copy);
        virtual ~file_stream(void);

        virtual void read_file(const std::string &filename);
        virtual void write_file(const std::string &filename);
    };
}

#endif
