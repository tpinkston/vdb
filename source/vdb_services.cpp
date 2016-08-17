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

#include "vdb_logger.h"
#include "vdb_services.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
// Returns the number of bytes needed to pad data with the specified
// length to a certain word boundary (default 8 bytes).
//
uint32_t vdb::get_padding_length(uint32_t length, uint32_t boundary)
{
    uint32_t
        length_data = length,
        length_padding = 0;

    while(length_data % boundary)
    {
        length_data += 1;
        length_padding++;
    }

    return length_padding;
}

// ----------------------------------------------------------------------------
// Returns the following colors based on force identification:
//
// friendly:    bold_green
// hostile:     bold_red
// neutral:     bold_yellow
//
color::color_e vdb::get_color(force_id_e value)
{
    switch(value)
    {
        case FORCE_ID_FRIENDLY:
        case FORCE_ID_FRIENDLY_2:
        case FORCE_ID_FRIENDLY_3:
        case FORCE_ID_FRIENDLY_4:
        case FORCE_ID_FRIENDLY_5:
        case FORCE_ID_FRIENDLY_6:
        case FORCE_ID_FRIENDLY_7:
        case FORCE_ID_FRIENDLY_8:
        case FORCE_ID_FRIENDLY_9:
        case FORCE_ID_FRIENDLY_10:
            return color::blue;
        case FORCE_ID_NEUTRAL:
        case FORCE_ID_NEUTRAL_2:
        case FORCE_ID_NEUTRAL_3:
        case FORCE_ID_NEUTRAL_4:
        case FORCE_ID_NEUTRAL_5:
        case FORCE_ID_NEUTRAL_6:
        case FORCE_ID_NEUTRAL_7:
        case FORCE_ID_NEUTRAL_8:
        case FORCE_ID_NEUTRAL_9:
        case FORCE_ID_NEUTRAL_10:
            return color::yellow;
        case FORCE_ID_OPPOSING:
        case FORCE_ID_OPPOSING_2:
        case FORCE_ID_OPPOSING_3:
        case FORCE_ID_OPPOSING_4:
        case FORCE_ID_OPPOSING_5:
        case FORCE_ID_OPPOSING_6:
        case FORCE_ID_OPPOSING_7:
        case FORCE_ID_OPPOSING_8:
        case FORCE_ID_OPPOSING_9:
        case FORCE_ID_OPPOSING_10:
            return color::red;
        default:
            return color::white;
    }
}

// ----------------------------------------------------------------------------
// Returns enumeration value based on string, if valid pointer is
// returned the caller is required to deallocate.  If null is returned
// then no match was made.
//
vdb::pdu_type_e *vdb::get_pdu_type(const std::string &string)
{
    pdu_type_e
        *type_ptr = 0;

    LOG_ERROR("Not implemented!");

    return type_ptr;
}

// ----------------------------------------------------------------------------
// Returns enumeration value based on string, if valid pointer is
// returned the caller is required to deallocate.  If null is returned
// then no match was made.
//
vdb::pdu_family_e *vdb::get_pdu_family(const std::string &string)
{
    pdu_family_e
        *family_ptr = 0;

    LOG_ERROR("Not implemented!");

    return family_ptr;
}

// ----------------------------------------------------------------------------
// Returns true if a valid entity ID could be parsed from the given
// string and if so, sets the ID in the output parameter.
//
bool vdb::get_id(
    const std::string &site,
    const std::string &application,
    const std::string &identity,
    id_t &id)
{
    int
        site_value = 0,
        application_value = 0,
        identity_value = 0;
    bool
        success = false;

    if (string_to_int32(site, site_value) and
        string_to_int32(application, application_value) and
        string_to_int32(identity, identity_value))
    {
        if ((site_value > -1) and (site_value < 65536) and
            (application_value > -1) and (application_value < 65536) and
            (identity_value > -1) and (identity_value < 65536))
        {
            id.site = (uint16_t)site_value;
            id.application = (uint16_t)application_value;
            id.identity = (uint16_t)identity_value;
            success = true;
        }
    }

    return success;
}
