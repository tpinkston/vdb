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

#include "vdb_filter.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu.h"
#include "vdb_pdu_data.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_metadata(const pdu_data_t &data)
{
    const std::string
        *address_ptr = options::string(OPT_NET_ADDRESS);
    bool
         pass = true;

    if (pass and address_ptr)
    {
        const std::string
            source = data.get_source();

        if (not contains_ignore_case(source, *address_ptr))
        {
            LOG_VERBOSE("Filtered out PDU with source %s...", source.c_str());
            pass = false;
        }
    }

    return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_header(const pdu_data_t &data)
{
    bool
         pass = true;

     if (pass and options::provided(OPT_EXERCISE))
     {
         pass = (data.get_pdu_exercise() != *options::integer(OPT_EXERCISE));

         if (not pass)
         {
             LOG_VERBOSE(
                 "Filtered out PDU on exercise %d...",
                 data.get_pdu_exercise());
         }
     }

     if (pass and options::provided(OPT_PDU_TYPE))
     {
         pass = options::integer_in_set(OPT_PDU_TYPE, data.get_pdu_type());

         if (not pass)
         {
             LOG_VERBOSE(
                 "Filtered out PDU with type %s...",
                 enumerations::get_name(
                     ENUM_PDU_TYPE,
                     data.get_pdu_type()).c_str());
         }
     }

     if (pass and  options::provided(OPT_PDU_FAMILY))
     {
         pass =  options::integer_in_set(OPT_PDU_FAMILY, data.get_pdu_family());

         if (not pass)
         {
             LOG_VERBOSE(
                 "Filtered out PDU with family %s...",
                 enumerations::get_name(
                     ENUM_PDU_FAMILY,
                     data.get_pdu_family()).c_str());
         }
     }

     return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_content(const pdu_t &pdu)
{
    bool
        pass = true;

    if (pass and not options::entity_id_match(pdu))
    {
        pass = false;
    }

    return pass;
}
