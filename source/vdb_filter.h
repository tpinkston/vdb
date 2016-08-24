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

#ifndef VDB_FILTER_H
#define VDB_FILTER_H

namespace vdb
{
    class pdu_t;
    class pdu_data_t;

    class filter
    {
      public:

        static bool filter_by_metadata(const pdu_data_t &data);

        static bool filter_by_header(const pdu_data_t &data);

        static bool filter_by_content(const pdu_t &pdu);
    };
}

#endif
