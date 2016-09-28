#include <cstdio>

#include "vdis_byte_stream.h"
#include "vdis_datum_records.h"
#include "vdis_entity_appearance.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"
#include "vdis_vp_records.h"

namespace
{
    const uint32_t BASE_PDU_SIZE[] =
    {
        0,                              // 0,PDU_TYPE_OTHER
        ENTITY_STATE_PDU_SIZE,          // 1,PDU_TYPE_ENTITY_STATE
        FIRE_PDU_SIZE,                  // 2,PDU_TYPE_FIRE
        DETONATION_PDU_SIZE,            // 3,PDU_TYPE_DETONATION
        COLLISION_PDU_SIZE,             // 4,PDU_TYPE_COLLISION
        0, // 5,PDU_TYPE_SERVICE_REQ
        0, // 6,PDU_TYPE_RESUPPLY_OFFER
        0, // 7,PDU_TYPE_RESUPPLY_RECEIVED
        0, // 8,PDU_TYPE_RESUPPLY_CANCEL
        0, // 9,PDU_TYPE_REPAIR_COMPLETE
        0, // 10,PDU_TYPE_REPAIR_RESPONSE
        CREATE_ENTITY_PDU_SIZE,         // 11,PDU_TYPE_CREATE_ENTITY
        REMOVE_ENTITY_PDU_SIZE,         // 12,PDU_TYPE_REMOVE_ENTITY
        START_RESUME_PDU_SIZE,          // 13,PDU_TYPE_START_RESUME
        STOP_FREEZE_PDU_SIZE,           // 14,PDU_TYPE_STOP_FREEZE
        ACKNOWLEDGE_PDU_SIZE,           // 15,PDU_TYPE_ACKNOWLEDGE
        ACTION_REQUEST_PDU_SIZE,        // 16,PDU_TYPE_ACTION_REQUEST
        ACTION_RESPONSE_PDU_SIZE,       // 17,PDU_TYPE_ACTION_RESPONSE
        DATA_QUERY_PDU_SIZE,            // 18,PDU_TYPE_DATA_QUERY
        SET_DATA_PDU_SIZE,              // 19,PDU_TYPE_SET_DATA
        DATA_QUERY_PDU_SIZE,            // 20,PDU_TYPE_DATA
        EVENT_REPORT_PDU_SIZE,          // 21,PDU_TYPE_EVENT_REPORT
        COMMENT_PDU_SIZE,               // 22,PDU_TYPE_COMMENT
        EM_EMISSION_PDU_SIZE,           // 23,PDU_TYPE_EM_EMISSION
        DESIGNATOR_PDU_SIZE,            // 24,PDU_TYPE_DESIGNATOR
        TRANSMITTER_PDU_SIZE,           // 25,PDU_TYPE_TRANSMITTER
        SIGNAL_PDU_SIZE,                // 26,PDU_TYPE_SIGNAL
        RECEIVER_PDU_SIZE,              // 27,PDU_TYPE_RECEIVER
        IFF_PDU_SIZE,                   // 28,PDU_TYPE_IFF
        0, // 29,PDU_TYPE_UNDERWATER_ACOUSTIC
        0, // 30,PDU_TYPE_SEES
        0, // 31,PDU_TYPE_INTERCOM_SIGNAL
        0, // 32,PDU_TYPE_INTERCOM_CONTROL
        0, // 33,PDU_TYPE_AGGREGATE_STATE
        0, // 34,PDU_TYPE_ISGROUPOF
        0, // 35,PDU_TYPE_TRANSFER_OWNERSHIP
        0, // 36,PDU_TYPE_ISPARTOF
        0, // 37,PDU_TYPE_MINEFIELD_STATE
        0, // 38,PDU_TYPE_MINEFIELD_QUERY
        0, // 39,PDU_TYPE_MINEFIELD_DATA
        0, // 40,PDU_TYPE_MINEFIELD_RESPONSE_NAK
        ENVIRONMENTAL_PROCESS_PDU_SIZE, // 41,PDU_TYPE_ENVIRONMENTAL_PROCESS
        0, // 42,PDU_TYPE_GRIDDED_DATA
        POINT_OBJECT_STATE_PDU_SIZE,    // 43,PDU_TYPE_POINT_OBJECT_STATE
        LINEAR_OBJECT_STATE_PDU_SIZE,   // 44,PDU_TYPE_LINEAR_OBJECT_STATE
        AREAL_OBJECT_STATE_PDU_SIZE,    // 45,PDU_TYPE_AREAL_OBJECT_STATE
        0, // 46,PDU_TYPE_TSPI
        0, // 47,PDU_TYPE_APPEARANCE
        0, // 48,PDU_TYPE_ARTICULATED_PARTS
        0, // 49,PDU_TYPE_LE_FIRE
        0, // 50,PDU_TYPE_LE_DETONATION
        0, // 51,PDU_TYPE_CREATE_ENTITY_R
        0, // 52,PDU_TYPE_REMOVE_ENTITY_R
        0, // 53,PDU_TYPE_START_RESUME_R
        0, // 54,PDU_TYPE_STOP_FREEZE_R
        0, // 55,PDU_TYPE_ACKNOWLEDGE_R
        0, // 56,PDU_TYPE_ACTION_REQUEST_R
        0, // 57,PDU_TYPE_ACTION_RESPONSE_R
        0, // 58,PDU_TYPE_DATA_QUERY_R
        0, // 59,PDU_TYPE_SET_DATA_R
        0, // 60,PDU_TYPE_DATA_R
        0, // 61,PDU_TYPE_EVENT_REPORT_R
        0, // 62,PDU_TYPE_COMMENT_R
        0, // 63,PDU_TYPE_RECORD_R
        0, // 64,PDU_TYPE_SET_RECORD_R
        0, // 65,PDU_TYPE_RECORD_QUERY_R
        0, // 66,PDU_TYPE_COLLISION_ELASTIC
        0, // 67,PDU_TYPE_ENTITY_STATE_UPDATE
        0, // 68,PDU_TYPE_DE_FIRE
        0, // 69,PDU_TYPE_ENTITY_DAMAGE_STATUS
        0, // 70,PDU_TYPE_INFO_OPS_ACTION
        0, // 71,PDU_TYPE_INFO_OPS_REPORT
        0, // 72,PDU_TYPE_ATTRIBUTE
        0, // 73,PDU_TYPE_UNDEFINED
        0, // 74,PDU_TYPE_UNDEFINED
        0, // 75,PDU_TYPE_UNDEFINED
        0, // 76,PDU_TYPE_UNDEFINED
        0, // 77,PDU_TYPE_UNDEFINED
        0, // 78,PDU_TYPE_UNDEFINED
        0, // 79,PDU_TYPE_UNDEFINED
        0, // 80,PDU_TYPE_UNDEFINED
        0, // 81,PDU_TYPE_UNDEFINED
        0, // 82,PDU_TYPE_UNDEFINED
        0, // 83,PDU_TYPE_UNDEFINED
        0, // 84,PDU_TYPE_UNDEFINED
        0, // 85,PDU_TYPE_UNDEFINED
        0, // 86,PDU_TYPE_UNDEFINED
        0, // 87,PDU_TYPE_UNDEFINED
        0, // 88,PDU_TYPE_UNDEFINED
        0, // 89,PDU_TYPE_UNDEFINED
        0, // 90,PDU_TYPE_UNDEFINED
        0, // 91,PDU_TYPE_UNDEFINED
        0, // 92,PDU_TYPE_UNDEFINED
        0, // 93,PDU_TYPE_UNDEFINED
        0, // 94,PDU_TYPE_UNDEFINED
        0, // 95,PDU_TYPE_UNDEFINED
        0, // 96,PDU_TYPE_UNDEFINED
        0, // 97,PDU_TYPE_UNDEFINED
        0, // 98,PDU_TYPE_UNDEFINED
        0, // 99,PDU_TYPE_UNDEFINED
        0, // 100,PDU_TYPE_UNDEFINED
        0, // 101,PDU_TYPE_UNDEFINED
        0, // 102,PDU_TYPE_UNDEFINED
        0, // 103,PDU_TYPE_UNDEFINED
        0, // 104,PDU_TYPE_UNDEFINED
        0, // 105,PDU_TYPE_UNDEFINED
        0, // 106,PDU_TYPE_UNDEFINED
        0, // 107,PDU_TYPE_UNDEFINED
        0, // 108,PDU_TYPE_UNDEFINED
        0, // 109,PDU_TYPE_UNDEFINED
        0, // 110,PDU_TYPE_UNDEFINED
        0, // 111,PDU_TYPE_UNDEFINED
        0, // 112,PDU_TYPE_UNDEFINED
        0, // 113,PDU_TYPE_UNDEFINED
        0, // 114,PDU_TYPE_UNDEFINED
        0, // 115,PDU_TYPE_UNDEFINED
        0, // 116,PDU_TYPE_UNDEFINED
        0, // 117,PDU_TYPE_UNDEFINED
        0, // 118,PDU_TYPE_UNDEFINED
        0, // 119,PDU_TYPE_UNDEFINED
        0, // 120,PDU_TYPE_UNDEFINED
        0, // 121,PDU_TYPE_UNDEFINED
        0, // 122,PDU_TYPE_UNDEFINED
        0, // 123,PDU_TYPE_UNDEFINED
        0, // 124,PDU_TYPE_UNDEFINED
        0, // 125,PDU_TYPE_UNDEFINED
        0, // 126,PDU_TYPE_UNDEFINED
        0, // 127,PDU_TYPE_UNDEFINED
        0, // 128,PDU_TYPE_UNDEFINED
        0, // 129,PDU_TYPE_UNDEFINED
        0, // 130,PDU_TYPE_UNDEFINED
        0, // 131,PDU_TYPE_UNDEFINED
        0, // 132,PDU_TYPE_UNDEFINED
        0, // 133,PDU_TYPE_UNDEFINED
        0, // 134,PDU_TYPE_UNDEFINED
        0, // 135,PDU_TYPE_UNDEFINED
        0, // 136,PDU_TYPE_UNDEFINED
        0, // 137,PDU_TYPE_UNDEFINED
        0, // 138,PDU_TYPE_UNDEFINED
        0, // 139,PDU_TYPE_UNDEFINED
        0, // 140,PDU_TYPE_UNDEFINED
        0, // 141,PDU_TYPE_UNDEFINED
        0, // 142,PDU_TYPE_UNDEFINED
        0, // 143,PDU_TYPE_UNDEFINED
        0, // 144,PDU_TYPE_UNDEFINED
        0, // 145,PDU_TYPE_UNDEFINED
        0, // 146,PDU_TYPE_UNDEFINED
        0, // 147,PDU_TYPE_UNDEFINED
        0, // 148,PDU_TYPE_UNDEFINED
        0, // 149,PDU_TYPE_UNDEFINED
        0, // 150,PDU_TYPE_UNDEFINED
        0, // 151,PDU_TYPE_UNDEFINED
        0, // 152,PDU_TYPE_UNDEFINED
        0, // 153,PDU_TYPE_UNDEFINED
        0, // 154,PDU_TYPE_UNDEFINED
        0, // 155,PDU_TYPE_UNDEFINED
        0, // 156,PDU_TYPE_UNDEFINED
        0, // 157,PDU_TYPE_UNDEFINED
        0, // 158,PDU_TYPE_UNDEFINED
        0, // 159,PDU_TYPE_UNDEFINED
        0, // 160,PDU_TYPE_UNDEFINED
        0, // 161,PDU_TYPE_UNDEFINED
        0, // 162,PDU_TYPE_UNDEFINED
        0, // 163,PDU_TYPE_UNDEFINED
        0, // 164,PDU_TYPE_UNDEFINED
        0, // 165,PDU_TYPE_UNDEFINED
        0, // 166,PDU_TYPE_UNDEFINED
        0, // 167,PDU_TYPE_UNDEFINED
        0, // 168,PDU_TYPE_UNDEFINED
        0, // 169,PDU_TYPE_UNDEFINED
        0, // 170,PDU_TYPE_UNDEFINED
        0, // 171,PDU_TYPE_UNDEFINED
        0, // 172,PDU_TYPE_UNDEFINED
        0, // 173,PDU_TYPE_UNDEFINED
        0, // 174,PDU_TYPE_UNDEFINED
        0, // 175,PDU_TYPE_UNDEFINED
        0, // 176,PDU_TYPE_UNDEFINED
        0, // 177,PDU_TYPE_UNDEFINED
        0, // 178,PDU_TYPE_UNDEFINED
        0, // 179,PDU_TYPE_UNDEFINED
        0, // 180,PDU_TYPE_UNDEFINED
        0, // 181,PDU_TYPE_UNDEFINED
        0, // 182,PDU_TYPE_UNDEFINED
        0, // 183,PDU_TYPE_UNDEFINED
        0, // 184,PDU_TYPE_UNDEFINED
        0, // 185,PDU_TYPE_UNDEFINED
        0, // 186,PDU_TYPE_UNDEFINED
        0, // 187,PDU_TYPE_UNDEFINED
        0, // 188,PDU_TYPE_UNDEFINED
        0, // 189,PDU_TYPE_UNDEFINED
        0, // 190,PDU_TYPE_UNDEFINED
        0, // 191,PDU_TYPE_UNDEFINED
        0, // 192,PDU_TYPE_UNDEFINED
        0, // 193,PDU_TYPE_UNDEFINED
        0, // 194,PDU_TYPE_UNDEFINED
        0, // 195,PDU_TYPE_UNDEFINED
        0, // 196,PDU_TYPE_UNDEFINED
        0, // 197,PDU_TYPE_UNDEFINED
        0, // 198,PDU_TYPE_UNDEFINED
        0, // 199,PDU_TYPE_UNDEFINED
        0, // 200,PDU_TYPE_APPLICATION_CTRL
        0, // 201,PDU_TYPE_UNDEFINED
        0, // 202,PDU_TYPE_UNDEFINED
        0, // 203,PDU_TYPE_UNDEFINED
        0, // 204,PDU_TYPE_UNDEFINED
        0, // 205,PDU_TYPE_UNDEFINED
        0, // 206,PDU_TYPE_UNDEFINED
        0, // 207,PDU_TYPE_UNDEFINED
        0, // 208,PDU_TYPE_UNDEFINED
        0, // 209,PDU_TYPE_UNDEFINED
        0, // 210,PDU_TYPE_UNDEFINED
        0, // 211,PDU_TYPE_UNDEFINED
        0, // 212,PDU_TYPE_UNDEFINED
        0, // 213,PDU_TYPE_UNDEFINED
        0, // 214,PDU_TYPE_UNDEFINED
        0, // 215,PDU_TYPE_UNDEFINED
        0, // 216,PDU_TYPE_UNDEFINED
        0, // 217,PDU_TYPE_UNDEFINED
        0, // 218,PDU_TYPE_UNDEFINED
        0, // 219,PDU_TYPE_UNDEFINED
        0, // 220,PDU_TYPE_UNDEFINED
        0, // 221,PDU_TYPE_UNDEFINED
        0, // 222,PDU_TYPE_UNDEFINED
        0, // 223,PDU_TYPE_UNDEFINED
        0, // 224,PDU_TYPE_UNDEFINED
        0, // 225,PDU_TYPE_UNDEFINED
        0, // 226,PDU_TYPE_UNDEFINED
        0, // 227,PDU_TYPE_UNDEFINED
        0, // 228,PDU_TYPE_UNDEFINED
        0, // 229,PDU_TYPE_UNDEFINED
        0, // 230,PDU_TYPE_UNDEFINED
        0, // 231,PDU_TYPE_UNDEFINED
        0, // 232,PDU_TYPE_UNDEFINED
        0, // 233,PDU_TYPE_UNDEFINED
        0, // 234,PDU_TYPE_UNDEFINED
        0, // 235,PDU_TYPE_UNDEFINED
        0, // 236,PDU_TYPE_UNDEFINED
        0, // 237,PDU_TYPE_UNDEFINED
        0, // 238,PDU_TYPE_UNDEFINED
        0, // 239,PDU_TYPE_UNDEFINED
        0, // 240,PDU_TYPE_UNDEFINED
        0, // 241,PDU_TYPE_UNDEFINED
        0, // 242,PDU_TYPE_UNDEFINED
        0, // 243,PDU_TYPE_UNDEFINED
        0, // 244,PDU_TYPE_UNDEFINED
        0, // 245,PDU_TYPE_UNDEFINED
        0, // 246,PDU_TYPE_UNDEFINED
        0, // 247,PDU_TYPE_UNDEFINED
        0, // 248,PDU_TYPE_UNDEFINED
        0, // 249,PDU_TYPE_UNDEFINED
        0, // 250,PDU_TYPE_UNDEFINED
        0, // 251,PDU_TYPE_UNDEFINED
        0, // 252,PDU_TYPE_UNDEFINED
        0, // 253,PDU_TYPE_UNDEFINED
        0, // 254,PDU_TYPE_UNDEFINED
        0  // 255,PDU_TYPE_UNDEFINED
    };
}

bool vdis::pdu_t::ignore_invalid_headers = false;

// ----------------------------------------------------------------------------
vdis::pdu_t::pdu_t(byte_stream_t &stream) : base_ptr(0)
{
    LOG_EXTRA_VERBOSE(
        "Constructing PDU with stream at index: %d/%d)",
        stream.index(),
        stream.length());

    // Verify that stream has enough bytes for PDU header
    //
    if (stream.remaining_length() < sizeof(pdu_header_t))
    {
        LOG_ERROR(
            "Stream too short for PDU: %d bytes",
            stream.remaining_length());
    }
    else if (validate_header(stream) || ignore_invalid_headers)
    {
        const pdu_type_e
            type = (pdu_type_e)stream[2];
        const uint32_t
            base_size = BASE_PDU_SIZE[type];

        // Verify that stream has enough bytes for base PDU
        //
        if (stream.remaining_length() < base_size)
        {
            LOG_ERROR(
                "Stream too short for PDU type %s: %d bytes",
                enumerations::get_name(ENUM_PDU_TYPE, type).c_str(),
                stream.remaining_length());
        }
        else switch(type)
        {
            case PDU_TYPE_ENTITY_STATE:
            {
                LOG_EXTRA_VERBOSE("Creating new entity_state_pdu_t...");
                base_ptr = new entity_state_pdu_t;
              break;
            }
            case PDU_TYPE_FIRE:
            {
                LOG_EXTRA_VERBOSE("Creating new fire_pdu_t...");
                base_ptr = new fire_pdu_t;
                break;
            }
            case PDU_TYPE_DETONATION:
            {
                LOG_EXTRA_VERBOSE("Creating new detonation_pdu_t...");
                base_ptr = new detonation_pdu_t;
                break;
            }
            default:
            {
                base_ptr = new default_pdu_t;
                LOG_WARNING(
                    "PDU type not handled: %s",
                    enumerations::get_name(ENUM_PDU_TYPE, type).c_str());
            }
        }

        if (base_ptr)
        {
            base_ptr->read(stream);
        }

        if (base_ptr and (stream.remaining_length() > 0))
        {
            LOG_WARNING(
                "Unread bytes (%d) in stream after reading PDU: %s",
                stream.remaining_length(),
                enumerations::get_name(ENUM_PDU_TYPE, type).c_str());
        }

        if (base_ptr and (base_ptr->header.length != stream.index()))
        {
            LOG_WARNING(
                "PDU length (%d) does not match bytes read (%d) from stream "
                "after reading PDU: %s",
                base_ptr->header.length,
                stream.index(),
                enumerations::get_name(ENUM_PDU_TYPE, type).c_str());
        }
    }
}

// ----------------------------------------------------------------------------
vdis::pdu_t::~pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::pdu_t::clear(void)
{
    if (base_ptr)
    {
        base_ptr->clear();

        delete base_ptr;
        base_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
bool vdis::pdu_t::is_ignoring_invalid_headers(void)
{
    return ignore_invalid_headers;
}

// ----------------------------------------------------------------------------
void vdis::pdu_t::set_ignoring_invalid_headers(bool value)
{
    if (ignore_invalid_headers != value)
    {
        LOG_VERBOSE(
            "Ignoring invalid headers: %s",
            (value ? "true " : "false"));

        ignore_invalid_headers = value;
    }
}

// ----------------------------------------------------------------------------
bool vdis::pdu_t::validate_header(byte_buffer_t &buffer)
{
    const protocol_version_e
        protocol = (protocol_version_e)buffer[0];
    const pdu_type_e
        type = (pdu_type_e)buffer[2];
    const pdu_family_e
        family = (pdu_family_e)buffer[3];
    bool
        valid_protocol = false,
        valid_type = false,
        valid_family = false;

    valid_protocol = (protocol == PROTOCOL_VERSION_PTCL_VER_IEEE_1278_1_2012);

    if (not valid_protocol)
    {
        LOG_WARNING(
            "Invalid protocol: %s",
            enumerations::get_name(ENUM_PROTOCOL_VERSION, protocol).c_str());
    }

    switch(type)
    {
        case PDU_TYPE_ENTITY_STATE:
        case PDU_TYPE_COLLISION:
        case PDU_TYPE_COLLISION_ELASTIC:
        case PDU_TYPE_ENTITY_STATE_UPDATE:
        case PDU_TYPE_ATTRIBUTE:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_ENTITY_INFORMATION_INTERACTION);
            break;
        case PDU_TYPE_FIRE:
        case PDU_TYPE_DETONATION:
        case PDU_TYPE_DE_FIRE:
        case PDU_TYPE_ENTITY_DAMAGE_STATUS:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_WARFARE);
            break;
        case PDU_TYPE_SERVICE_REQ:
        case PDU_TYPE_RESUPPLY_OFFER:
        case PDU_TYPE_RESUPPLY_RECEIVED:
        case PDU_TYPE_RESUPPLY_CANCEL:
        case PDU_TYPE_REPAIR_COMPLETE:
        case PDU_TYPE_REPAIR_RESPONSE:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_LOGISTICS);
            break;
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
            valid_type = true;
            valid_family = (family == PDU_FAMILY_SIMULATION_MANAGEMENT);
            break;
        case PDU_TYPE_EM_EMISSION:
        case PDU_TYPE_DESIGNATOR:
        case PDU_TYPE_IFF:
        case PDU_TYPE_UNDERWATER_ACOUSTIC:
        case PDU_TYPE_SEES:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_DISTRIBUTED_EMISSION_REGENERATION);
            break;
        case PDU_TYPE_TRANSMITTER:
        case PDU_TYPE_SIGNAL:
        case PDU_TYPE_RECEIVER:
        case PDU_TYPE_INTERCOM_SIGNAL:
        case PDU_TYPE_INTERCOM_CONTROL:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_RADIO_COMMUNICATION);
            break;
        case PDU_TYPE_AGGREGATE_STATE:
        case PDU_TYPE_ISGROUPOF:
        case PDU_TYPE_TRANSFER_OWNERSHIP:
        case PDU_TYPE_ISPARTOF:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_ENTITY_MANAGEMENT);
            break;
        case PDU_TYPE_MINEFIELD_STATE:
        case PDU_TYPE_MINEFIELD_QUERY:
        case PDU_TYPE_MINEFIELD_DATA:
        case PDU_TYPE_MINEFIELD_RESPONSE_NAK:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_MINEFIELD);
            break;
        case PDU_TYPE_ENVIRONMENTAL_PROCESS:
        case PDU_TYPE_GRIDDED_DATA:
        case PDU_TYPE_POINT_OBJECT_STATE:
        case PDU_TYPE_LINEAR_OBJECT_STATE:
        case PDU_TYPE_AREAL_OBJECT_STATE:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_SYNTHETIC_ENVIRONMENT);
            break;
        case PDU_TYPE_TSPI:
        case PDU_TYPE_APPEARANCE:
        case PDU_TYPE_ARTICULATED_PARTS:
        case PDU_TYPE_LE_FIRE:
        case PDU_TYPE_LE_DETONATION:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_LIVE_ENTITY);
            break;
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
            valid_type = true;
            valid_family = (family == PDU_FAMILY_SIMULATION_MANAGEMENT_WITH_RELIABILITY);
            break;
        case PDU_TYPE_INFO_OPS_ACTION:
        case PDU_TYPE_INFO_OPS_REPORT:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_INFO_OPS);
            break;
        case PDU_TYPE_APPLICATION_CTRL:
            valid_type = true;
            valid_family = (family == PDU_FAMILY_EXPERIMENTAL_V_DIS);
            break;
        default:
            break;
    }

    if (not (valid_type and valid_family))
    {
        LOG_WARNING(
            "Invalid PDU type/family combination: %s/%s",
            enumerations::get_name(ENUM_PDU_TYPE, type).c_str(),
            enumerations::get_name(ENUM_PDU_FAMILY, family).c_str());
    }

    return (valid_protocol and valid_type and valid_family);
}

// ----------------------------------------------------------------------------
void vdis::default_pdu_t::print(std::ostream &) const
{

}

// ----------------------------------------------------------------------------
void vdis::default_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    content.reset(stream.offset_buffer(), stream.remaining_length());

    stream.skip(stream.remaining_length());
}

// ----------------------------------------------------------------------------
void vdis::default_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);

    if (content.buffer())
    {
        stream.write(content.buffer(), content.length());
    }
}

// ----------------------------------------------------------------------------
vdis::entity_state_pdu_t::entity_state_pdu_t(void)
{
    header.clear();
    id.clear();
    force = 0;
    vp_record_count = 0;
    type.clear();
    alternate_type.clear();
    velocity.clear();
    location.clear();
    orientation.clear();
    appearance = 0;
    dead_reckoning.clear();
    marking.clear();
    capabilities.clear();
    vp_records = 0;
}

// ----------------------------------------------------------------------------
vdis::entity_state_pdu_t::~entity_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::clear(void)
{
    for(uint32_t i = 0; (vp_records and (i < vp_record_count)); ++i)
    {
        delete vp_records[i];
        vp_records[i] = 0;
    }

    if (vp_records)
    {
        delete[] vp_records;
    }

    header.clear();
    id.clear();
    force = 0;
    vp_record_count = 0;
    type.clear();
    alternate_type.clear();
    velocity.clear();
    location.clear();
    orientation.clear();
    appearance = 0;
    dead_reckoning.clear();
    marking.clear();
    capabilities.clear();
    vp_records = 0;
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::print(std::ostream &out) const
{
    const std::string
        prefix = "entity_state.";

    header.print((prefix + "header."), out);

    out << prefix << "id " << id << std::endl
        << prefix << "force " << (force_id_e)force << std::endl
        << prefix << "marking " << marking << std::endl
        << prefix << "type " << type.description() << std::endl
        << prefix << "alternate_type " << alternate_type.description() << std::endl
        << prefix << "velocity " << velocity << std::endl
        << prefix << "location " << location << std::endl
        << prefix << "orientation " << orientation << std::endl
        << prefix << "appearance.value "
        << to_bin_string(appearance, true) << std::endl;

    if (type.kind_enum() == vdis::ENT_KIND_LIFEFORMS)
    {
        vdis::lifeform_appearance_t
            entity_appearance;

        entity_appearance.value = appearance;
        entity_appearance.print(prefix, out);
    }
    else if (type.kind_enum() == vdis::ENT_KIND_PLATFORMS)
    {
        if (type.domain_enum() == vdis::DOMAIN_AIR)
        {
            vdis::air_platform_appearance_t
                entity_appearance;

            entity_appearance.value = appearance;
            entity_appearance.print(prefix, out);
        }
        else if (type.domain_enum() == vdis::DOMAIN_LAND)
        {
            vdis::land_platform_appearance_t
                entity_appearance;

            entity_appearance.value = appearance;
            entity_appearance.print(prefix, out);
        }
    }

    dead_reckoning.print(prefix, out);
    capabilities.print(prefix, out);

    out << prefix << "vp_records.count" << (int)vp_record_count << std::endl;

    vdis::vp_record_content_t::using_type(&type);

    for(uint32_t i = 0; i < vp_record_count; ++i)
    {
        const vdis::vp_record_t *record_ptr = vp_record(i);

        if (record_ptr)
        {
            record_ptr->print(
                (prefix + "vp_records[" + vdis::to_string(i) + "]."),
                out);
        }
    }

    vdis::vp_record_content_t::using_type(0);
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    id.read(stream);
    stream.read(force);
    stream.read(vp_record_count);
    type.read(stream);
    alternate_type.read(stream);
    velocity.read(stream);
    location.read(stream);
    orientation.read(stream);
    stream.read(appearance);
    dead_reckoning.read(stream);
    marking.read(stream);
    capabilities.read(stream);
    vp_records = vp_record_t::read(stream, vp_record_count);
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    id.write(stream);
    stream.write(force);
    stream.write(vp_record_count);
    type.write(stream);
    alternate_type.write(stream);
    velocity.write(stream);
    location.write(stream);
    orientation.write(stream);
    stream.write(appearance);
    dead_reckoning.write(stream);
    marking.write(stream);
    capabilities.write(stream);

    if ((vp_record_count > 0) and not vp_records)
    {
        LOG_ERROR(
            "Null VP record array with length %d for entity %s!",
            vp_record_count,
            to_string(id).c_str());
    }
    else if ((vp_record_count == 0) and vp_records)
    {
        LOG_ERROR(
            "Non-null VP record array with length 0 for entity %s!",
            to_string(id).c_str());
    }
    else if ((vp_record_count > 0) and vp_records)
    {
        for(uint8_t i = 0; i < vp_record_count; ++i)
        {
            if (vp_records[i])
            {
                vp_records[i]->write(stream);
            }
            else
            {
                LOG_ERROR(
                    "Null VP record at index %d for entity %s!",
                    i,
                    to_string(id).c_str());
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::fire_pdu_t::clear(void)
{
    header.clear();
    shooter.clear();
    target.clear();
    munition.clear();
    event.clear();
    fire_mission_index = 0;
    world_location.clear();
    burst_descriptor.clear();
    velocity.clear();
    range = 0.0f;
}

// ----------------------------------------------------------------------------
void vdis::fire_pdu_t::print(std::ostream &out) const
{
    const std::string
        prefix = "fire.";

    header.print((prefix + "header."), out);

    out << prefix << "shooter " << shooter << std::endl
        << prefix << "target " << target << std::endl
        << prefix << "munition " << munition << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "fire_mission_index " << fire_mission_index << std::endl
        << prefix << "world_location " << world_location << std::endl;

    burst_descriptor.print((prefix + "burst_descriptor."), out);

    out << prefix << "velocity " << velocity << std::endl
        << prefix << "range " << to_string(range, 1, 3) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::fire_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    shooter.read(stream);
    target.read(stream);
    munition.read(stream);
    event.read(stream);
    stream.read(fire_mission_index);
    world_location.read(stream);
    burst_descriptor.read(stream);
    velocity.read(stream);
    stream.read(range);
}

// ----------------------------------------------------------------------------
void vdis::fire_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    shooter.write(stream);
    target.write(stream);
    munition.write(stream);
    event.write(stream);
    stream.write(fire_mission_index);
    world_location.write(stream);
    burst_descriptor.write(stream);
    velocity.write(stream);
    stream.write(range);
}

// ----------------------------------------------------------------------------
vdis::detonation_pdu_t::detonation_pdu_t(void)
{
    header.clear();
    shooter.clear();
    target.clear();
    munition.clear();
    event.clear();
    velocity.clear();
    world_location.clear();
    burst_descriptor.clear();
    entity_location.clear();
    result = 0;
    vp_record_count = 0;
    padding = 0;
    vp_records = 0;
}

// ----------------------------------------------------------------------------
vdis::detonation_pdu_t::~detonation_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::clear(void)
{
    for(uint32_t i = 0; (vp_records and (i < vp_record_count)); ++i)
    {
        delete vp_records[i];
        vp_records[i] = 0;
    }

    if (vp_records)
    {
        delete[] vp_records;
    }

    header.clear();
    shooter.clear();
    target.clear();
    munition.clear();
    event.clear();
    velocity.clear();
    world_location.clear();
    burst_descriptor.clear();
    entity_location.clear();
    result = 0;
    vp_record_count = 0;
    padding = 0;
    vp_records = 0;
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::print(std::ostream &out) const
{
    const std::string
        prefix = "detonation.";

    header.print((prefix + "header."), out);

    out << prefix << "shooter " << shooter << std::endl
        << prefix << "target " << target << std::endl
        << prefix << "munition " << munition << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "velocity " << velocity << std::endl
        << prefix << "world_location " << world_location << std::endl;

    burst_descriptor.print((prefix + "burst_descriptor."), out);

    out << prefix << "entity_location " << entity_location << std::endl
        << prefix << "result " << (detonation_result_e)result << std::endl
        << prefix << prefix << "vp_records.count"
        << (int)vp_record_count << std::endl;

    vdis::vp_record_content_t::using_type(0);

    for(uint32_t i = 0; i < vp_record_count; ++i)
    {
        const vdis::vp_record_t *record_ptr = vp_record(i);

        if (record_ptr)
        {
            record_ptr->print(
                (prefix + "vp_records[" + vdis::to_string(i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    shooter.read(stream);
    target.read(stream);
    munition.read(stream);
    event.read(stream);
    velocity.read(stream);
    world_location.read(stream);
    burst_descriptor.read(stream);
    entity_location.read(stream);
    stream.read(result);
    stream.read(vp_record_count);
    stream.read(padding);
    vp_records = vp_record_t::read(stream, vp_record_count);
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    shooter.write(stream);
    target.write(stream);
    munition.write(stream);
    event.write(stream);
    velocity.write(stream);
    world_location.write(stream);
    burst_descriptor.write(stream);
    entity_location.write(stream);
    stream.write(result);
    stream.write(vp_record_count);
    stream.write(padding);

    if ((vp_record_count > 0) and not vp_records)
    {
        LOG_ERROR(
            "Null VP record array with length %d for shooter %s!",
            vp_record_count,
            to_string(shooter).c_str());
    }
    else if ((vp_record_count == 0) and vp_records)
    {
        LOG_ERROR(
            "Non-null VP record array with length 0 for shooter %s!",
            to_string(shooter).c_str());
    }
    else if ((vp_record_count > 0) and vp_records)
    {
        for(uint8_t i = 0; i < vp_record_count; ++i)
        {
            if (vp_records[i])
            {
                vp_records[i]->write(stream);
            }
            else
            {
                LOG_ERROR(
                    "Null VP record at index %d for shooter %s!",
                    i,
                    to_string(shooter).c_str());
            }
        }
    }
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::pdu_t &o)
{
    if (not o.base())
    {
        out << "NULL" << std::endl;
    }
    else
    {
        o.base()->print(out);
    }

    return out;
}
