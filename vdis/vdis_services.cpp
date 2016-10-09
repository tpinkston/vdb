
#include "vdis_data_types.h"
#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const uint64_t
        MILLIS_PER_SECOND = 1000,
        MILLIS_PER_MINUTE = (MILLIS_PER_SECOND * 60),
        MILLIS_PER_HOUR = (MILLIS_PER_MINUTE * 60);
    bool
        swapping = false;
    bool
        color_enabled = true;
}

vdis::entity_marking::marking_map_t
    vdis::entity_marking::markings;

// ----------------------------------------------------------------------------
time_struct_t *vdis::get_local_time()
{
    time_t
        seconds = std::time(0);
    time_struct_t
        *local_ptr = std::localtime(&seconds);

    local_ptr->tm_year += 1900;

    return local_ptr;
}

// ----------------------------------------------------------------------------
string_t vdis::time_to_string(const time_struct_t *ts_ptr)
{
    std::ostringstream
        stream;
    const time_struct_t
        *local_ptr = ts_ptr;

    if (not local_ptr)
    {
        local_ptr = get_local_time();
    }

    stream << std::setfill('0')
           << std::setw(2) << local_ptr->tm_mon << "-"
           << std::setw(2) << local_ptr->tm_mday << "-"
           << std::setw(4) << local_ptr->tm_year << " "
           << std::setw(2) << local_ptr->tm_hour << ":"
           << std::setw(2) << local_ptr->tm_min << ":"
           << std::setw(2) << local_ptr->tm_sec;

    return stream.str();
}

// ----------------------------------------------------------------------------
uint64_t vdis::get_system_time(void)
{
    time_value_t
        time;
    time_zone_t
        zone;
    uint64_t
        milliseconds = 0;

    if (gettimeofday(&time, &zone) == 0)
    {
        milliseconds = (uint64_t)(time.tv_sec * 1000);
        milliseconds += ((uint64_t)(time.tv_usec) / 1000);
    }

    return milliseconds;
}

// ----------------------------------------------------------------------------
string_t vdis::time_to_string(uint64_t time)
{
    string_t
        time_string;
    time_t
        seconds = (time / 1000);

    time_string = string_t(std::ctime(&seconds));

    // Remove trailing new line character.
    //
    time_string = time_string.substr(0, (time_string.length() - 1));

    return time_string;
}

// ----------------------------------------------------------------------------
uint64_t vdis::get_system_time(const time_value_t &value)
{
    uint64_t
        milliseconds = (value.tv_sec * 1000LL);

    milliseconds += (uint64_t)((float64_t)value.tv_usec / 1000.0);

    return milliseconds;
}

// ----------------------------------------------------------------------------
time_value_t vdis::get_time_value(uint64_t time)
{
    time_value_t
        time_value;

    time_value.tv_sec = (time / 1000LL);
    time_value.tv_usec = (time - (time_value.tv_sec * 1000LL)) * 1000;

    return time_value;
}

// ----------------------------------------------------------------------------
void vdis::parse_time(
    uint64_t system_time,
    uint32_t &hours,
    uint32_t &minutes,
    uint32_t &seconds,
    uint32_t &milliseconds)
{
    hours = (system_time / MILLIS_PER_HOUR);
    system_time -= (uint64_t)hours * MILLIS_PER_HOUR;

    minutes = (system_time / MILLIS_PER_MINUTE);
    system_time -= (uint64_t)minutes * MILLIS_PER_MINUTE;

    seconds = (system_time / MILLIS_PER_SECOND);
    system_time -= (uint64_t)seconds * MILLIS_PER_SECOND;

    milliseconds = system_time;
}

// ----------------------------------------------------------------------------
color::color_e color::get(vdis::force_id_e force)
{
    switch(force)
    {
        case vdis::FORCE_ID_FRIENDLY:
        case vdis::FORCE_ID_FRIENDLY_2:
        case vdis::FORCE_ID_FRIENDLY_3:
        case vdis::FORCE_ID_FRIENDLY_4:
        case vdis::FORCE_ID_FRIENDLY_5:
        case vdis::FORCE_ID_FRIENDLY_6:
        case vdis::FORCE_ID_FRIENDLY_7:
        case vdis::FORCE_ID_FRIENDLY_8:
        case vdis::FORCE_ID_FRIENDLY_9:
        case vdis::FORCE_ID_FRIENDLY_10:
            return blue;

        case vdis::FORCE_ID_OPPOSING:
        case vdis::FORCE_ID_OPPOSING_2:
        case vdis::FORCE_ID_OPPOSING_3:
        case vdis::FORCE_ID_OPPOSING_4:
        case vdis::FORCE_ID_OPPOSING_5:
        case vdis::FORCE_ID_OPPOSING_6:
        case vdis::FORCE_ID_OPPOSING_7:
        case vdis::FORCE_ID_OPPOSING_8:
        case vdis::FORCE_ID_OPPOSING_9:
        case vdis::FORCE_ID_OPPOSING_10:
            return red;

        case vdis::FORCE_ID_NEUTRAL:
        case vdis::FORCE_ID_NEUTRAL_2:
        case vdis::FORCE_ID_NEUTRAL_3:
        case vdis::FORCE_ID_NEUTRAL_4:
        case vdis::FORCE_ID_NEUTRAL_5:
        case vdis::FORCE_ID_NEUTRAL_6:
        case vdis::FORCE_ID_NEUTRAL_7:
        case vdis::FORCE_ID_NEUTRAL_8:
        case vdis::FORCE_ID_NEUTRAL_9:
        case vdis::FORCE_ID_NEUTRAL_10:
            return yellow;

        default:
            return none;
    }
}

// ----------------------------------------------------------------------------
void color::set_enabled(bool value)
{
    color_enabled = value;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &stream, color::color_e c)
{
    if (isatty(STDOUT_FILENO) and color_enabled)
    {
        return (stream << "\e[" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &stream, color::bold_color_e c)
{
    if (isatty(STDOUT_FILENO) and color_enabled)
    {
        return (stream << "\e[;1;" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

// ----------------------------------------------------------------------------
bool vdis::byteswapping(void)
{
    return swapping;
}

// ----------------------------------------------------------------------------
void vdis::set_byteswapping(void)
{
    swapping = big_endian();
}

// ----------------------------------------------------------------------------
void vdis::force_byteswapping(bool value)
{
    swapping = value;
}

// ----------------------------------------------------------------------------
int16_t vdis::byteswap(int16_t value, bool force)
{
    uint16_t result = value;

    if (swapping or force)
    {
        result = (value << 8);
        result |= ((value >> 8) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint16_t vdis::byteswap(uint16_t value, bool force)
{
    uint16_t result = value;

    if (swapping or force)
    {
        result = (value << 8);
        result |= ((value >> 8) & 0xFF);
    }

    // LOG_EXTRA_VERBOSE("Byteswap uint16_t: %d -> %d", value, result);

    return result;
}

// ----------------------------------------------------------------------------
int32_t vdis::byteswap(int32_t value, bool force)
{
    int32_t result = value;

    if (swapping or force)
    {
        result = (value << 24);
        result |= ((value << 8) & 0xFF0000);
        result |= ((value >> 8) & 0xFF00);
        result |= ((value >> 24) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint32_t vdis::byteswap(uint32_t value, bool force)
{
    uint32_t result = value;

    if (swapping or force)
    {
        result = (value << 24);
        result |= ((value << 8) & 0xFF0000);
        result |= ((value >> 8) & 0xFF00);
        result |= ((value >> 24) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
int64_t vdis::byteswap(int64_t value, bool force)
{
    int64_t result = value;

    if (swapping or force)
    {
        result = (value << 56);
        result |= ((value << 40) & 0xFF000000000000);
        result |= ((value << 24) & 0xFF0000000000);
        result |= ((value << 8) & 0xFF00000000);
        result |= ((value >> 8) & 0xFF000000);
        result |= ((value >> 24) & 0xFF0000);
        result |= ((value >> 40) & 0xFF00);
        result |= ((value >> 56) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint64_t vdis::byteswap(uint64_t value, bool force)
{
    uint64_t result = value;

    if (swapping or force)
    {
        result = (value << 56);
        result |= ((value << 40) & 0xFF000000000000);
        result |= ((value << 24) & 0xFF0000000000);
        result |= ((value << 8) & 0xFF00000000);
        result |= ((value >> 8) & 0xFF000000);
        result |= ((value >> 24) & 0xFF0000);
        result |= ((value >> 40) & 0xFF00);
        result |= ((value >> 56) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
// Returns 'PDU_FAMILY_END' if PDU type is not valid
//
vdis::pdu_family_e vdis::get_family(pdu_type_e type)
{
    switch(type)
    {
        case PDU_TYPE_ENTITY_STATE:
        case PDU_TYPE_COLLISION:
        case PDU_TYPE_COLLISION_ELASTIC:
        case PDU_TYPE_ENTITY_STATE_UPDATE:
        case PDU_TYPE_ATTRIBUTE:
            return PDU_FAMILY_ENTITY_INFORMATION_INTERACTION;
        case PDU_TYPE_FIRE:
        case PDU_TYPE_DETONATION:
        case PDU_TYPE_DE_FIRE:
        case PDU_TYPE_ENTITY_DAMAGE_STATUS:
        	return PDU_FAMILY_WARFARE;
        case PDU_TYPE_SERVICE_REQ:
        case PDU_TYPE_RESUPPLY_OFFER:
        case PDU_TYPE_RESUPPLY_RECEIVED:
        case PDU_TYPE_RESUPPLY_CANCEL:
        case PDU_TYPE_REPAIR_COMPLETE:
        case PDU_TYPE_REPAIR_RESPONSE:
        	return PDU_FAMILY_LOGISTICS;
        case PDU_TYPE_CREATE_ENTITY:
        case PDU_TYPE_REMOVE_ENTITY:
        case PDU_TYPE_START_RESUME:
        case PDU_TYPE_STOP_FREEZE:
        case PDU_TYPE_ACKNOWLEDGE:
        case PDU_TYPE_ACTION_REQUEST:
        case PDU_TYPE_ACTION_RESPONSE:
        case PDU_TYPE_DATA_QUERY:
        case PDU_TYPE_SET_DATA:
        case PDU_TYPE_DATA:
        case PDU_TYPE_EVENT_REPORT:
        case PDU_TYPE_COMMENT:
        	return PDU_FAMILY_SIMULATION_MANAGEMENT;
        case PDU_TYPE_EM_EMISSION:
        case PDU_TYPE_DESIGNATOR:
        case PDU_TYPE_IFF:
        case PDU_TYPE_UNDERWATER_ACOUSTIC:
        case PDU_TYPE_SEES:
        	return PDU_FAMILY_DISTRIBUTED_EMISSION_REGENERATION;
        case PDU_TYPE_TRANSMITTER:
        case PDU_TYPE_SIGNAL:
        case PDU_TYPE_RECEIVER:
        case PDU_TYPE_INTERCOM_SIGNAL:
        case PDU_TYPE_INTERCOM_CONTROL:
        	return PDU_FAMILY_RADIO_COMMUNICATION;
        case PDU_TYPE_AGGREGATE_STATE:
        case PDU_TYPE_ISGROUPOF:
        case PDU_TYPE_TRANSFER_OWNERSHIP:
        case PDU_TYPE_ISPARTOF:
        	return PDU_FAMILY_ENTITY_MANAGEMENT;
        case PDU_TYPE_MINEFIELD_STATE:
        case PDU_TYPE_MINEFIELD_QUERY:
        case PDU_TYPE_MINEFIELD_DATA:
        case PDU_TYPE_MINEFIELD_RESPONSE_NAK:
        	return PDU_FAMILY_MINEFIELD;
        case PDU_TYPE_ENVIRONMENTAL_PROCESS:
        case PDU_TYPE_GRIDDED_DATA:
        case PDU_TYPE_POINT_OBJECT_STATE:
        case PDU_TYPE_LINEAR_OBJECT_STATE:
        case PDU_TYPE_AREAL_OBJECT_STATE:
        	return PDU_FAMILY_SYNTHETIC_ENVIRONMENT;
        case PDU_TYPE_TSPI:
        case PDU_TYPE_APPEARANCE:
        case PDU_TYPE_ARTICULATED_PARTS:
        case PDU_TYPE_LE_FIRE:
        case PDU_TYPE_LE_DETONATION:
        	return PDU_FAMILY_LIVE_ENTITY;
        case PDU_TYPE_CREATE_ENTITY_R:
        case PDU_TYPE_REMOVE_ENTITY_R:
        case PDU_TYPE_START_RESUME_R:
        case PDU_TYPE_STOP_FREEZE_R:
        case PDU_TYPE_ACKNOWLEDGE_R:
        case PDU_TYPE_ACTION_REQUEST_R:
        case PDU_TYPE_ACTION_RESPONSE_R:
        case PDU_TYPE_DATA_QUERY_R:
        case PDU_TYPE_SET_DATA_R:
        case PDU_TYPE_DATA_R:
        case PDU_TYPE_EVENT_REPORT_R:
        case PDU_TYPE_COMMENT_R:
        case PDU_TYPE_RECORD_R:
        case PDU_TYPE_SET_RECORD_R:
        case PDU_TYPE_RECORD_QUERY_R:
        	return PDU_FAMILY_SIMULATION_MANAGEMENT_WITH_RELIABILITY;
        case PDU_TYPE_INFO_OPS_ACTION:
        case PDU_TYPE_INFO_OPS_REPORT:
        	return PDU_FAMILY_INFO_OPS;
        case PDU_TYPE_APPLICATION_CTRL:
        	return PDU_FAMILY_EXPERIMENTAL_V_DIS;
        default:
        	return PDU_FAMILY_END;
    }
}

// ----------------------------------------------------------------------------
// Returns the number of bytes needed to pad data with the specified
// length to a certain word boundary (default 8 bytes).
//
uint32_t vdis::padding_length(uint32_t length, uint32_t boundary)
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
string_t vdis::entity_marking::get_marking(const id_t &id)
{
    const marking_t
        *marking_ptr = get(id);

    if (marking_ptr)
    {
        return marking_ptr->str();
    }
    else if (id.is_none())
    {
        return "[NO ENTITY]";
    }
    else if (id.is_all())
    {
        return "[ALL ENTITIES]";
    }
    else
    {
        return "[UNKNOWN]";
    }
}

// ----------------------------------------------------------------------------
const vdis::marking_t *vdis::entity_marking::get(const id_t &id)
{
    const marking_t
        *marking_ptr = 0;
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor != markings.end())
    {
        marking_ptr = &itor->second;
    }

    return marking_ptr;
}

// ----------------------------------------------------------------------------
void vdis::entity_marking::set(const id_t &id, const marking_t &marking)
{
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor == markings.end())
    {
        LOG_VERBOSE("Setting marking for entity %s", to_string(id).c_str());
        markings[id] = marking;
    }
    else if (marking != itor->second)
    {
        LOG_VERBOSE("Resetting marking for entity %s", to_string(id).c_str());
        markings[id] = marking;
    }
}

// ----------------------------------------------------------------------------
void vdis::entity_marking::unset(const id_t &id)
{
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor != markings.end())
    {
        LOG_VERBOSE("Unsetting marking for entity %s", to_string(id).c_str());
        markings.erase(id);
    }
}
