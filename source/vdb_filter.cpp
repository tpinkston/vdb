#include "vdb_filter.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_metadata(const pdu_data_t &data)
{
    bool pass = true;

    if (pass and not options::include_hostnames.empty())
    {
        const std::string
            &hostname = data.get_hostname();
        std::set<std::string>::const_iterator
            itor = options::include_hostnames.begin();

        pass = false;

        while((itor != options::include_hostnames.end()) and not pass)
        {
            pass = (vdis::to_lowercase(hostname) == vdis::to_lowercase(*itor));
            ++itor;
        }

        if (not pass)
        {
            LOG_VERBOSE("Filtered out PDU from host %s...", hostname.c_str());
        }
    }

    return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_header(const pdu_data_t &data)
{
    bool pass = true;

    if (pass and not options::include_exercises.empty())
    {
        pass = set_contains(
            options::include_exercises,
            data.get_pdu_exercise());
    }
    else if (pass and not options::exclude_exercises.empty())
    {
        pass = not set_contains(
            options::exclude_exercises,
            data.get_pdu_exercise());
    }

    if (pass and not options::include_types.empty())
    {
        pass = set_contains(
            options::include_types,
            data.get_pdu_type());
    }
    else if (pass and not options::exclude_types.empty())
    {
        pass = not set_contains(
            options::exclude_types,
            data.get_pdu_type());
    }

    if (pass and not options::include_families.empty())
    {
        pass = set_contains(
            options::include_families,
            data.get_pdu_family());
    }
    else if (pass and not options::exclude_families.empty())
    {
        pass = not set_contains(
            options::exclude_families,
            data.get_pdu_family());
    }

    if (logger::is_enabled(logger::EXTRA_VERBOSE) and not pass)
    {
        LOG_VERBOSE(
            "Filtered out PDU with exercise/type/family: %d/%s/%s",
            (int)data.get_pdu_exercise(),
            vdis::enumerations::get_name(
                ENUM_PDU_TYPE,
                data.get_pdu_type()).c_str(),
            vdis::enumerations::get_name(
                ENUM_PDU_FAMILY,
                data.get_pdu_family()).c_str());
    }

    return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_range(uint32_t index, bool &past_end)
{
    bool pass = true;

    if (not options::pdu_index_range.empty())
    {
        // Set 'past_end' to true if the provided index is beyond the
        // last index in the set.
        //
        past_end = (index > *options::pdu_index_range.rbegin());

        if (options::pdu_index_range.find(index) ==
            options::pdu_index_range.end())
        {
            LOG_VERBOSE("Filtered out PDU at index %d...", index);
            pass = false;
        }
    }

    return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::filter_by_content(const vdis::pdu_t &pdu)
{
    bool pass = true;

    if (pdu.base())
    {
        std::set<vdis::id_t>::const_iterator itor;

        if (not options::include_entity_ids.empty())
        {
            itor = options::include_entity_ids.begin();
            pass = false;

            while((itor != options::include_entity_ids.end()) and not pass)
            {
                if (pdu.base()->contains_id(*itor))
                {
                    pass = true;
                }

                ++itor;
            }
        }
        else if (not options::exclude_entity_ids.empty())
        {
            itor = options::exclude_entity_ids.begin();

            while(pass and (itor != options::exclude_entity_ids.end()))
            {
                if (pdu.base()->contains_id(*itor))
                {
                    pass = false;
                }
            }
        }
    }

    return pass;
}

// ----------------------------------------------------------------------------
bool vdb::filter::set_contains(const std::set<uint8_t> &set, uint8_t value)
{
    return (set.find(value) != set.end());
}
