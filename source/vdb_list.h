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

#ifndef VDB_LIST_H
#define VDB_LIST_H

#include "vdb_command.h"
#include "vdb_file_handler.h"

namespace vdb
{
    class list : public file_handler, public command
    {
      public:

        list(void);
        virtual ~list(void);

        virtual int run(void);

      protected:

        virtual void process_pdu_data(const pdu_data_t &data);

        virtual bool passes_data_filters(const pdu_data_t &data);

        const std::string
            *address_ptr;
        uint32_t
            pdus_listed,
            pdus_filtered_out;
    };
}

#endif
