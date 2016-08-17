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

#ifndef VDB_SERVICES_H
#define VDB_SERVICES_H

#include "vdb_color.h"
#include "vdb_enums.h"
#include "vdb_ids.h"

namespace vdb
{
    uint32_t get_padding_length(uint32_t length, uint32_t boundary = 8);

    color::color_e get_color(force_id_e);

    pdu_type_e *get_pdu_type(const std::string &string);
    pdu_family_e *get_pdu_family(const std::string &string);

    bool get_id(
        const std::string &site,
        const std::string &application,
        const std::string &identity,
        id_t &id
    );
}

#endif
