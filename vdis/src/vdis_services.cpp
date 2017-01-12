
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

    const float64_t
        PI = 3.1415926535897932384626433,
        HALF_PI = (PI / 2.0),
        NEGATIVE_HALF_PI = (-PI / 2.0),
        DEGREES_PER_RADIAN = 57.2957795130823208768,
        RADIANS_PER_DEGREE = 0.01745329251994329576;

    // Constants based on WGS 84 Ellipsoid.
    const float64_t
        WGS84_A = 6378137.0,
        WGS84_A2 = (WGS84_A * WGS84_A),
        WGS84_A_HALF = (WGS84_A / 2.0),
        WGS84_B = (0.9999944354799 / 4.0),
        WGS84_C = (1.0 / 298.257223563),
        WGS84_D = 0.100225438677758E+01,
        WGS84_E = -0.393246903633930E-04,
        WGS84_F = 0.241216653453483E+12,
        WGS84_G = 0.133733602228679E+14,
        WGS84_H = 0.984537701867943E+00,
        WGS84_J = (WGS84_A * (1.0 - WGS84_C)),
        WGS84_J2 = (WGS84_J * WGS84_J),
        WGS84_J2_54 = (54.0 * WGS84_J2),
        WGS84_L = (WGS84_C * (2.0 - WGS84_C)), // (F)
        WGS84_L1 = (WGS84_L - 1.0),
        WGS84_L2 = (WGS84_L * WGS84_L),
        WGS84_L2_2 = (2.0 * WGS84_L2),
        WGS84_L4TH = (0.25 * WGS84_L),
        WGS84_L_SQRT = std::sqrt(WGS84_L),
        WGS84_L_SQRT_HALF = (std::sqrt(WGS84_L) / 2.0),
        WGS84_P = (WGS84_J2 / WGS84_A),
        WGS84_Q = (WGS84_A2 - WGS84_J2),
        WGS84_R = (WGS84_Q / WGS84_J2),
        WGS84_LQ = (WGS84_L * WGS84_Q),
        WGS84_THIRD = (1.0 / 3.0),
        WGS84_AR1 = std::pow((WGS84_A + 50005.0), 2.0),
        WGS84_AR2 = (WGS84_AR1 / std::pow((WGS84_J + 50005.0), 2.0)),
        WGS84_BR1 = std::pow((WGS84_A - 10005.0), 2.0),
        WGS84_BR2 = (WGS84_BR1 / std::pow((WGS84_J - 10005.0), 2.0));

    vdis::geodetic_conversion
        geodetic_conversion_ptr = 0;
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
void vdis::convert(const location24_t &gcc, geodetic_location_t &gdc)
{
    gdc.clear();

    if (geodetic_conversion_ptr)
    {
        geodetic_conversion_ptr(gcc, gdc);
    }
    else
    {
        bool
            complete = false;

        if (gcc.x == 0.0)
        {
            if (gcc.y > 0.0)
            {
                gdc.latitude = HALF_PI;
            }
            else if (gcc.y < 0.0)
            {
                gdc.longitude  = NEGATIVE_HALF_PI;
            }
            else if (gcc.z > 0)
            {
                gdc.latitude = HALF_PI;
                gdc.longitude  = 0.0;
                gdc.elevation = gcc.z;
                complete = true;
            }
            else if (gcc.z < 0.0)
            {
                gdc.latitude = NEGATIVE_HALF_PI;
                gdc.longitude  = 0.0;
                gdc.elevation = gcc.z;
                complete = true;
            }
            else
            {
                gdc.latitude = 0.0;
                gdc.longitude  = 0.0;
                gdc.elevation = 0.0;
                complete = true;
            }
        }

        if (not complete)
        {
            const float64_t
                w2 = ((gcc.x * gcc.x) + (gcc.y * gcc.y)),
                w = std::sqrt(w2),
                z2 = (gcc.z * gcc.z);

            const float64_t
                ar2 = (w2 + (WGS84_AR2 * z2)),
                br2 = (w2 + (WGS84_BR2 * z2));

            if ((br2 > WGS84_BR1) and (ar2 < WGS84_AR1))
            {
                float64_t
                    w2EF = (WGS84_E * w2 + WGS84_F),
                    w2GH = (WGS84_G + w2 * WGS84_H + z2),
                    top = gcc.z * (WGS84_D + w2EF / w2GH),
                    top2 = (top * top),
                    top2w2 = (top2 + w2),
                    top2w2sqrt = std::sqrt(top2w2),
                    r = (top2 / top2w2),
                    n = (0.25 - (WGS84_L4TH * r)),
                    nb = (n + WGS84_B),
                    a = WGS84_A / (nb + n / nb);

                gdc.latitude = std::atan(top / w);
                gdc.longitude  = std::atan2(gcc.y, gcc.x);

                if (r < 0.50)
                {
                    gdc.elevation = (top2w2sqrt - a);
                }
                else
                {
                    gdc.elevation = gcc.z / (top / top2w2sqrt) + (WGS84_L1 * a);
                }
            }
            else
            {
                float64_t
                    oz2 = (WGS84_J2_54 * z2),
                    mz2 = (w2 - (WGS84_L1 * z2) - WGS84_LQ),
                    a = (oz2 / (mz2 * mz2)),
                    s0 = (WGS84_L2 * w2 * a / mz2),
                    s1 = (s0 * (s0 + 2.0)),
                    s2 = (1.0 + s0 + std::sqrt(s1)),
                    s3 = std::pow(s2, WGS84_THIRD),
                    p = a / (3.0 * std::pow((s3 + (1.0 / s3) + 1.0), 2)),
                    pT = (1.0 + (WGS84_L2_2 * p)),
                    pTsqrt = std::sqrt(pT),
                    r0 = (pTsqrt * (1.0 + pTsqrt)),
                    r0w2 = (-p * (2.0 * (1.0 - WGS84_L) * z2 / r0 + w2)),
                    r1 = (1.0 + (1.0 / pTsqrt)),
                    r2 = (r0w2 / WGS84_A2),
                    r3 = 0.0;

                if ((r1 + r2) > 0.0)
                {
                    r3 = WGS84_A * std::sqrt(.50 * (r1 + r2));
                }

                r3 -= (p * WGS84_L * w / (1.0 + pTsqrt));

                float64_t
                    r3L = (r3 * WGS84_L),
                    r3Lz2 = (std::pow((w - r3L), 2.0) + z2),
                    v = std::sqrt(r3Lz2 - (WGS84_L * z2)),
                    zo = (WGS84_P * gcc.z / v);

                gdc.latitude = std::atan((gcc.z + (WGS84_R * zo)) / w);
                gdc.longitude  = std::atan2(gcc.y, gcc.x);
                gdc.elevation = std::sqrt(r3Lz2) * (1.0 - WGS84_P / v);
            }

            gdc.latitude *= DEGREES_PER_RADIAN;
            gdc.longitude *= DEGREES_PER_RADIAN;
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::set_geodetic_conversion(geodetic_conversion f)
{
    geodetic_conversion_ptr = f;
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
