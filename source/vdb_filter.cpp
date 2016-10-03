#include "vdb_filter.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_metadata(const pdu_data_t &data)
{
    const string_t
        *address_ptr = options::string(OPT_NET_ADDRESS);
    bool
         pass = true;

    if (pass and address_ptr)
    {
        const string_t
            source = data.get_source();

        if (not vdis::contains(source, *address_ptr, true))
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
                 vdis::enumerations::get_name(
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
                 vdis::enumerations::get_name(
                     ENUM_PDU_FAMILY,
                     data.get_pdu_family()).c_str());
         }
     }

     return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_content(const vdis::pdu_t &pdu)
{
    bool
        pass = true;

    if (pass and not options::entity_id_match(pdu))
    {
        pass = false;
    }

    return pass;
}
