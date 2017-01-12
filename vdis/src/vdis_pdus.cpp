#include "vdis_appearance.h"
#include "vdis_byte_stream.h"
#include "vdis_datum_records.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_standard_variable_records.h"
#include "vdis_string.h"
#include "vdis_variable_parameter_records.h"

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
        APPLICATION_CONTROL_PDU_SIZE,   // 200,PDU_TYPE_APPLICATION_CTRL
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

bool
    vdis::pdu_t::validate_headers = false;
bool
    vdis::entity_state_pdu_t::set_entity_markings = true;

// ----------------------------------------------------------------------------
vdis::pdu_t::pdu_t(byte_stream_t &stream) : base_ptr(0)
{
    uint32_t
        base_size = 0,
        start_index = stream.index(),
        bytes_read = 0;

    LOG_EXTRA_VERBOSE(
        "Constructing PDU with stream at index: %d/%d",
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
    else
    {
        const pdu_type_e type = (pdu_type_e)stream[2];
        base_size = BASE_PDU_SIZE[type];

        if (validate_headers and not validate_header(stream))
        {
            return;
        }

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
            case PDU_TYPE_COLLISION:
            {
                LOG_EXTRA_VERBOSE("Creating new collision_pdu_t...");
                base_ptr = new collision_pdu_t;
                break;
            }
            case PDU_TYPE_CREATE_ENTITY:
            {
                LOG_EXTRA_VERBOSE("Creating new create_entity_pdu_t...");
                base_ptr = new create_entity_pdu_t;
                break;
            }
            case PDU_TYPE_REMOVE_ENTITY:
            {
                LOG_EXTRA_VERBOSE("Creating new remove_entity_pdu_t...");
                base_ptr = new remove_entity_pdu_t;
                break;
            }
            case PDU_TYPE_START_RESUME:
            {
                LOG_EXTRA_VERBOSE("Creating new start_resume_pdu_t...");
                base_ptr = new start_resume_pdu_t;
                break;
            }
            case PDU_TYPE_STOP_FREEZE:
            {
                LOG_EXTRA_VERBOSE("Creating new stop_freeze_pdu_t...");
                base_ptr = new stop_freeze_pdu_t;
                break;
            }
            case PDU_TYPE_ACKNOWLEDGE:
            {
                LOG_EXTRA_VERBOSE("Creating new acknowledge_pdu_t...");
                base_ptr = new acknowledge_pdu_t;
                break;
            }
            case PDU_TYPE_ACTION_REQUEST:
            {
                LOG_EXTRA_VERBOSE("Creating new action_request_pdu_t...");
                base_ptr = new action_request_pdu_t;
                break;
            }
            case PDU_TYPE_ACTION_RESPONSE:
            {
                LOG_EXTRA_VERBOSE("Creating new action_response_pdu_t...");
                base_ptr = new action_response_pdu_t;
                break;
            }
            case PDU_TYPE_DATA_QUERY:
            {
                LOG_EXTRA_VERBOSE("Creating new data_query_pdu_t...");
                base_ptr = new data_query_pdu_t;
                break;
            }
            case PDU_TYPE_SET_DATA:
            {
                LOG_EXTRA_VERBOSE("Creating new set_data_pdu_t...");
                base_ptr = new set_data_pdu_t;
                break;
            }
            case PDU_TYPE_DATA:
            {
                LOG_EXTRA_VERBOSE("Creating new data_pdu_t...");
                base_ptr = new data_pdu_t;
                break;
            }
            case PDU_TYPE_EVENT_REPORT:
            {
                LOG_EXTRA_VERBOSE("Creating new event_report_pdu_t...");
                base_ptr = new event_report_pdu_t;
                break;
            }
            case PDU_TYPE_COMMENT:
            {
                LOG_EXTRA_VERBOSE("Creating new comment_pdu_t...");
                base_ptr = new comment_pdu_t;
                break;
            }
            case PDU_TYPE_EM_EMISSION:
            {
                LOG_EXTRA_VERBOSE("Creating new em_emission_pdu_t...");
                base_ptr = new em_emission_pdu_t;
                break;
            }
            case PDU_TYPE_DESIGNATOR:
            {
                LOG_EXTRA_VERBOSE("Creating new designator_pdu_t...");
                base_ptr = new designator_pdu_t;
                break;
            }
            case PDU_TYPE_TRANSMITTER:
            {
                LOG_EXTRA_VERBOSE("Creating new transmitter_pdu_t...");
                base_ptr = new transmitter_pdu_t;
                break;
            }
            case PDU_TYPE_SIGNAL:
            {
                LOG_EXTRA_VERBOSE("Creating new signal_pdu_t...");
                base_ptr = new signal_pdu_t;
                break;
            }
            case PDU_TYPE_RECEIVER:
            {
                LOG_EXTRA_VERBOSE("Creating new receiver_pdu_t...");
                base_ptr = new receiver_pdu_t;
                break;
            }
            case PDU_TYPE_IFF:
            {
                LOG_EXTRA_VERBOSE("Creating new iff_pdu_t...");
                base_ptr = new iff_pdu_t;
                break;
            }
//            case PDU_TYPE_MINEFIELD_STATE: TODO
//            {
//                LOG_EXTRA_VERBOSE("Creating new minefield_state_pdu_t...");
//                base_ptr = new minefield_state_pdu_t;
//                break;
//            }
//            case PDU_TYPE_MINEFIELD_QUERY: TODO
//            {
//                LOG_EXTRA_VERBOSE("Creating new minefield_query_pdu_t...");
//                base_ptr = new minefield_query_pdu_t;
//                break;
//            }
//            case PDU_TYPE_MINEFIELD_DATA: TODO
//            {
//                LOG_EXTRA_VERBOSE("Creating new minefield_data_pdu_t...");
//                base_ptr = new minefield_data_pdu_t;
//                break;
//            }
//            case PDU_TYPE_MINEFIELD_RESPONSE_NAK: TODO
//            {
//                LOG_EXTRA_VERBOSE("Creating new minefield_response_nack_pdu_t...");
//                base_ptr = new minefield_response_nack_pdu_t;
//                break;
//            }
            case PDU_TYPE_ENVIRONMENTAL_PROCESS:
            {
                LOG_EXTRA_VERBOSE("Creating new environmental_process_pdu_t...");
                base_ptr = new environmental_process_pdu_t;
                break;
            }
            case PDU_TYPE_POINT_OBJECT_STATE:
            {
                LOG_EXTRA_VERBOSE("Creating new point_object_state_pdu_t...");
                base_ptr = new point_object_state_pdu_t;
                break;
            }
            case PDU_TYPE_LINEAR_OBJECT_STATE:
            {
                LOG_EXTRA_VERBOSE("Creating new linear_object_state_pdu_t...");
                base_ptr = new linear_object_state_pdu_t;
                break;
            }
            case PDU_TYPE_AREAL_OBJECT_STATE:
            {
                LOG_EXTRA_VERBOSE("Creating new areal_object_state_pdu_t...");
                base_ptr = new areal_object_state_pdu_t;
                break;
            }
            case PDU_TYPE_APPLICATION_CTRL:
            {
                LOG_EXTRA_VERBOSE("Creating new application_control_pdu_t...");
                base_ptr = new application_control_pdu_t;
                break;
            }
            default:
            {
                base_ptr = new default_pdu_t(type);
                LOG_WARNING(
                    "No base class for PDU type: %s",
                    enumerations::get_name(ENUM_PDU_TYPE, type).c_str());
            }
        }

        if (base_ptr)
        {
            base_ptr->read(stream);

            // Compare number of bytes read with PDU length in PDU header
            //
            bytes_read = (stream.index() - start_index);

            if (base_ptr->header.length != bytes_read)
            {
                LOG_WARNING(
                    "PDU length (%d) does not match bytes read (%d) "
                    "after reading PDU: %s",
                    base_ptr->header.length,
                    bytes_read,
                    enumerations::get_name(ENUM_PDU_TYPE, type).c_str());
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::pdu_t::clear(void)
{
    LOG_EXTRA_VERBOSE("Clearing PDU with base: %p", base_ptr);

    if (base_ptr)
    {
        base_ptr->clear();

        delete base_ptr;
        base_ptr = 0;
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
        valid_type_family = false;

    valid_protocol = (protocol == PROTOCOL_VERSION_PTCL_VER_IEEE_1278_1_2012);

    if (not valid_protocol)
    {
        LOG_WARNING(
            "Invalid protocol: %s",
            enumerations::get_name(ENUM_PROTOCOL_VERSION, protocol).c_str());
    }

    valid_type_family = (get_family(type) != family);

    if (not valid_type_family)
    {
        LOG_WARNING(
            "Invalid PDU type/family: %s/%s",
            enumerations::get_name(ENUM_PDU_TYPE, type).c_str(),
            enumerations::get_name(ENUM_PDU_FAMILY, family).c_str());
    }

    return (valid_protocol and valid_type_family);
}

// ----------------------------------------------------------------------------
void vdis::default_pdu_t::print(std::ostream &out) const
{
    header.print("pdu.header.", out);
    content.print("pdu.content.", out);
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
    record_count = 0;
    type.clear();
    alternate_type.clear();
    velocity.clear();
    location.clear();
    orientation.clear();
    appearance = 0;
    dead_reckoning.clear();
    marking.clear();
    capabilities.clear();
    records = 0;
}

// ----------------------------------------------------------------------------
vdis::entity_state_pdu_t::~entity_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::clear(void)
{
    for(uint32_t i = 0; (records and (i < record_count)); ++i)
    {
        delete records[i];
        records[i] = 0;
    }

    if (records)
    {
        delete[] records;
    }

    pdu_base_t::clear();
    id.clear();
    force = 0;
    record_count = 0;
    type.clear();
    alternate_type.clear();
    velocity.clear();
    location.clear();
    orientation.clear();
    appearance = 0;
    dead_reckoning.clear();
    marking.clear();
    capabilities.clear();
    records = 0;
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "entity_state.";
    geodetic_location_t
        gdc;

    convert(location, gdc);

    header.print((prefix + "header."), out);

    out << prefix << "id " << id << std::endl
        << prefix << "force " << (force_id_e)force << std::endl
        << prefix << "marking " << marking << std::endl
        << prefix << "type " << type
        << " '" << type.description() << "'" << std::endl
        << prefix << "alternate_type " << alternate_type
        << " '" << alternate_type.description() << "'" << std::endl
        << prefix << "velocity(vector) " << velocity << std::endl
        << prefix << "velocity(m/s) "
        << to_string(velocity.length(), 1, 2) << std::endl
        << prefix << "location(GCC) " << location << std::endl
        << prefix << "location(GDC) " << gdc << std::endl
        << prefix << "orientation " << orientation << std::endl
        << prefix << "appearance.value "
        << to_bin_string(appearance, true) << std::endl;

    vdis::print_appearance(prefix, type, appearance, out);

    dead_reckoning.print(prefix, out);
    capabilities.print(prefix, out);

    out << prefix << "records.count " << (int)record_count << std::endl;

    variable_parameter_content_t::using_type(&type);

    for(uint32_t i = 0; i < record_count; ++i)
    {
        const variable_parameter_record_t *record_ptr = record(i);

        if (record_ptr)
        {
            record_ptr->print(
                (prefix + "records[" + to_string(i) + "]."),
                out);
        }
    }

    variable_parameter_content_t::using_type(0);
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    id.read(stream);
    stream.read(force);
    stream.read(record_count);
    type.read(stream);
    alternate_type.read(stream);
    velocity.read(stream);
    location.read(stream);
    orientation.read(stream);
    stream.read(appearance);
    dead_reckoning.read(stream);
    marking.read(stream);
    capabilities.read(stream);
    records = read_variable_parameter_records(stream, record_count);

    if (set_entity_markings)
    {
        entity_marking::set(id, marking);
    }
}

// ----------------------------------------------------------------------------
void vdis::entity_state_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    id.write(stream);
    stream.write(force);
    stream.write(record_count);
    type.write(stream);
    alternate_type.write(stream);
    velocity.write(stream);
    location.write(stream);
    orientation.write(stream);
    stream.write(appearance);
    dead_reckoning.write(stream);
    marking.write(stream);
    capabilities.write(stream);

    if ((record_count > 0) and not records)
    {
        LOG_ERROR(
            "Null VP record array with length %d for entity %s!",
            record_count,
            to_string(id).c_str());
    }
    else if ((record_count == 0) and records)
    {
        LOG_ERROR(
            "Non-null VP record array with length 0 for entity %s!",
            to_string(id).c_str());
    }
    else if ((record_count > 0) and records)
    {
        for(uint8_t i = 0; i < record_count; ++i)
        {
            if (records[i])
            {
                records[i]->write(stream);
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
    pdu_base_t::clear();
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
    const string_t
        prefix = "fire.";

    header.print((prefix + "header."), out);

    out << prefix << "shooter " << entity_marking(shooter) << std::endl
        << prefix << "target " << entity_marking(target) << std::endl
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
    record_count = 0;
    padding = 0;
    records = 0;
}

// ----------------------------------------------------------------------------
vdis::detonation_pdu_t::~detonation_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::clear(void)
{
    for(uint32_t i = 0; (records and (i < record_count)); ++i)
    {
        delete records[i];
        records[i] = 0;
    }

    if (records)
    {
        delete[] records;
    }

    pdu_base_t::clear();
    shooter.clear();
    target.clear();
    munition.clear();
    event.clear();
    velocity.clear();
    world_location.clear();
    burst_descriptor.clear();
    entity_location.clear();
    result = 0;
    record_count = 0;
    padding = 0;
    records = 0;
}

// ----------------------------------------------------------------------------
void vdis::detonation_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "detonation.";

    header.print((prefix + "header."), out);

    out << prefix << "shooter " << entity_marking(shooter) << std::endl
        << prefix << "target " << entity_marking(target) << std::endl
        << prefix << "munition " << munition << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "velocity " << velocity << std::endl
        << prefix << "world_location " << world_location << std::endl;

    burst_descriptor.print((prefix + "burst_descriptor."), out);

    out << prefix << "entity_location " << entity_location << std::endl
        << prefix << "result " << (detonation_result_e)result << std::endl
        << prefix << prefix << "records.count "
        << (int)record_count << std::endl;

    variable_parameter_content_t::using_type(0);

    for(uint32_t i = 0; i < record_count; ++i)
    {
        const variable_parameter_record_t *record_ptr = record(i);

        if (record_ptr)
        {
            record_ptr->print(
                (prefix + "records[" + to_string(i) + "]."),
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
    stream.read(record_count);
    stream.read(padding);
    records = read_variable_parameter_records(stream, record_count);
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
    stream.write(record_count);
    stream.write(padding);

    if ((record_count > 0) and not records)
    {
        LOG_ERROR(
            "Null VP record array with length %d for shooter %s!",
            record_count,
            to_string(shooter).c_str());
    }
    else if ((record_count == 0) and records)
    {
        LOG_ERROR(
            "Non-null VP record array with length 0 for shooter %s!",
            to_string(shooter).c_str());
    }
    else if ((record_count > 0) and records)
    {
        for(uint8_t i = 0; i < record_count; ++i)
        {
            if (records[i])
            {
                records[i]->write(stream);
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
vdis::collision_pdu_t::collision_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::collision_pdu_t::clear(void)
{
    pdu_base_t::clear();
    issuing_entity.clear();
    colliding_entity.clear();
    event.clear();
    collision_type = 0;
    padding = 0;
    velocity.clear();
    mass = 0;
    location.clear();
}

// ----------------------------------------------------------------------------
void vdis::collision_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "collision.";

    header.print((prefix + "header."), out);

    out << prefix << "issuing_entity "
        << entity_marking(issuing_entity) << std::endl
        << prefix << "colliding_entity "
        << entity_marking(colliding_entity) << std::endl
        << prefix << "collision_type "
        << (collision_e)collision_type << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "padding(8 bits) " << to_bin_string(padding) << std::endl
        << prefix << "velocity " << velocity << std::endl
        << prefix << "mass " << to_string(mass, 1, 3) << std::endl
        << prefix << "location " << location << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::collision_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    issuing_entity.read(stream);
    colliding_entity.read(stream);
    event.read(stream);
    stream.read(collision_type);
    stream.read(padding);
    velocity.read(stream);
    stream.read(mass);
    location.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::collision_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    issuing_entity.write(stream);
    colliding_entity.write(stream);
    event.write(stream);
    stream.write(collision_type);
    stream.write(padding);
    velocity.write(stream);
    stream.write(mass);
    location.write(stream);
}

// ----------------------------------------------------------------------------
vdis::create_entity_pdu_t::create_entity_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::create_entity_pdu_t::clear(void)
{
    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::create_entity_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "create_entity.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::create_entity_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdis::create_entity_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdis::remove_entity_pdu_t::remove_entity_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::remove_entity_pdu_t::clear(void)
{
    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::remove_entity_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "remove_entity.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::remove_entity_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdis::remove_entity_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdis::start_resume_pdu_t::start_resume_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::start_resume_pdu_t::clear(void)
{
    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    real_time.clear();
    simulation_time.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::start_resume_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "start_resume.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "real_time " << real_time << std::endl
        << prefix << "simulation_time " << simulation_time << std::endl
        << prefix << "request_id " << request_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::start_resume_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    real_time.read(stream);
    simulation_time.read(stream);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdis::start_resume_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    real_time.write(stream);
    simulation_time.write(stream);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdis::stop_freeze_pdu_t::stop_freeze_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::stop_freeze_pdu_t::clear(void)
{
    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    real_time.clear();
    reason = 0;
    behavior = 0;
    padding = 0;
    request_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::stop_freeze_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "stop_freeze.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "real_time " << real_time << std::endl
        << prefix << "reason " << (sf_reason_codes_e)reason << std::endl
        << prefix << "behavior " << (frozen_behavior_e)behavior << std::endl
        << prefix << "padding " << to_bin_string(padding) << std::endl
        << prefix << "request_id " << request_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stop_freeze_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    real_time.read(stream);
    stream.read(reason);
    stream.read(behavior);
    stream.read(padding);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdis::stop_freeze_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    real_time.write(stream);
    stream.write(reason);
    stream.write(behavior);
    stream.write(padding);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdis::acknowledge_pdu_t::acknowledge_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::acknowledge_pdu_t::clear(void)
{
    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    acknowledge_flag = 0;
    response_flag = 0;
    request_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::acknowledge_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "acknowledge.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "acknowledge_flag "
        << (ack_acknowledge_flag_e)acknowledge_flag << std::endl
        << prefix << "response_flag "
        << (ack_response_flag_e)response_flag << std::endl
        << prefix << "request_id " << request_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::acknowledge_pdu_t::read(byte_stream_t &stream)
{
    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(acknowledge_flag);
    stream.read(response_flag);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdis::acknowledge_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(acknowledge_flag);
    stream.write(response_flag);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdis::abstract_siman_pdu_t::abstract_siman_pdu_t(void) :
    fixed_count(0),
    variable_count(0),
    fixed_records(0),
    variable_records(0)
{

}

// ----------------------------------------------------------------------------
void vdis::abstract_siman_pdu_t::clear(void)
{
    if (fixed_records and (fixed_count > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Clearing abstract_siman_pdu_t fixed records: %d@%p...",
            fixed_count,
            fixed_records);

        for(uint32_t i = 0; i < fixed_count; ++i)
        {
            LOG_EXTRA_VERBOSE(
                "Deleting fixed record %d/%d@%p...",
                (i + 1),
                fixed_count,
                fixed_records[i]);

            delete fixed_records[i];
            fixed_records[i] = 0;
        }

        delete[] fixed_records;
    }

    if (variable_records and (variable_count > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Clearing abstract_siman_pdu_t variable records: %d@%p...",
            variable_count,
            variable_records);

        for(uint32_t i = 0; i < variable_count; ++i)
        {
            LOG_EXTRA_VERBOSE(
                "Deleting variable record %d/%d@%p...",
                (i + 1),
                variable_count,
                variable_records[i]);

            delete variable_records[i];
            variable_records[i] = 0;
        }

        delete[] variable_records;
    }

    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    fixed_count = 0;
    variable_count = 0;
    fixed_records = 0;
    variable_records = 0;
}

// ----------------------------------------------------------------------------
void vdis::abstract_siman_pdu_t::print_records(
    const std::string &prefix,
    std::ostream &out) const
{
	out << prefix << "fixed_datums.count " << fixed_count << std::endl;

    for(uint32_t i = 0; (fixed_records and (i < fixed_count)); ++i)
    {
    	fixed_records[i]->print(
    		(prefix + "fixed_datums[" + to_string(i) + "]."),
			out);
    }

	out << prefix << "variable_datums.count " << variable_count << std::endl;

    for(uint32_t i = 0; (variable_records and (i < variable_count)); ++i)
    {
    	variable_records[i]->print(
    		(prefix + "variable_datums[" + to_string(i) + "]."),
			out);
    }
}

// ----------------------------------------------------------------------------
void vdis::abstract_siman_pdu_t::read_records(byte_stream_t &stream)
{
    stream.read(fixed_count);
    stream.read(variable_count);

    fixed_records = read_fixed_datum_records(stream, fixed_count);
    variable_records = read_variable_datum_records(stream, variable_count);
}

// ----------------------------------------------------------------------------
void vdis::abstract_siman_pdu_t::write_records(byte_stream_t &stream)
{
    stream.write(fixed_count);
    stream.write(variable_count);

    if (not write_fixed_datum_records(
                stream,
                fixed_records,
                fixed_count))
    {
        LOG_ERROR(
            "Error writing fixed records for originator %s!",
            to_string(originator).c_str());
    }

    if (not write_variable_datum_records(
                stream,
                variable_records,
                variable_count))
    {
        LOG_ERROR(
            "Error writing variable records for originator %s!",
            to_string(originator).c_str());
    }
}

// ----------------------------------------------------------------------------
vdis::action_request_pdu_t::action_request_pdu_t(void) :
    request_id(0),
    action_id(0)
{

}

// ----------------------------------------------------------------------------
vdis::action_request_pdu_t::~action_request_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::action_request_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    request_id = 0;
    action_id = 0;
}

// ----------------------------------------------------------------------------
void vdis::action_request_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "action_request.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl
        << prefix << "action_id "
        << (actreq_action_ids_e)action_id << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::action_request_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
    stream.read(action_id);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::action_request_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
    stream.write(action_id);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::action_response_pdu_t::action_response_pdu_t(void) :
    request_id(0),
    request_status(0)
{

}

// ----------------------------------------------------------------------------
vdis::action_response_pdu_t::~action_response_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::action_response_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    request_id = 0;
    request_status = 0;
}

// ----------------------------------------------------------------------------
void vdis::action_response_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "action_response.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl
        << prefix << "request_status "
        << (actres_req_status_e)request_status << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::action_response_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
    stream.read(request_status);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::action_response_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
    stream.write(request_status);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::data_query_pdu_t::data_query_pdu_t(void) :
    request_id(0),
    time_interval(0)
{

}

// ----------------------------------------------------------------------------
vdis::data_query_pdu_t::~data_query_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::data_query_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    request_id = 0;
    time_interval = 0;
}

// ----------------------------------------------------------------------------
void vdis::data_query_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "data_query.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl
        << prefix << "time_interval " << time_interval << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::data_query_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
    stream.read(time_interval);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::data_query_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
    stream.write(time_interval);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::set_data_pdu_t::set_data_pdu_t(void) :
    request_id(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdis::set_data_pdu_t::~set_data_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::set_data_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    request_id = 0;
    padding = 0;
}

// ----------------------------------------------------------------------------
void vdis::set_data_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "set_data.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl
        << prefix << "padding(4 bytes) "
        << to_hex_string(padding, true) << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::set_data_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
    stream.read(padding);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::set_data_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
    stream.write(padding);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::data_pdu_t::data_pdu_t(void) :
    request_id(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdis::data_pdu_t::~data_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::data_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    request_id = 0;
    padding = 0;
}

// ----------------------------------------------------------------------------
void vdis::data_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "data.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "request_id " << request_id << std::endl
        << prefix << "padding(4 bytes) "
        << to_hex_string(padding, true) << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::data_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(request_id);
    stream.read(padding);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::data_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(request_id);
    stream.write(padding);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::event_report_pdu_t::event_report_pdu_t(void) :
    event_type(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdis::event_report_pdu_t::~event_report_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::event_report_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();

    event_type = 0;
    padding = 0;
}

// ----------------------------------------------------------------------------
void vdis::event_report_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "event_report.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl
        << prefix << "event_type " << event_type << std::endl
        << prefix << "padding(4 bytes) "
        << to_hex_string(padding, true) << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::event_report_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(event_type);
    stream.read(padding);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::event_report_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(event_type);
    stream.write(padding);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::comment_pdu_t::comment_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
vdis::comment_pdu_t::~comment_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::comment_pdu_t::clear(void)
{
    abstract_siman_pdu_t::clear();
}

// ----------------------------------------------------------------------------
void vdis::comment_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "event_report.";

    header.print((prefix + "header."), out);

    out << prefix << "originator " << entity_marking(originator) << std::endl
        << prefix << "recipient " << entity_marking(recipient) << std::endl;

    print_records(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::comment_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);

    read_records(stream);
}

// ----------------------------------------------------------------------------
void vdis::comment_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);

    write_records(stream);
}

// ----------------------------------------------------------------------------
vdis::em_emission_pdu_t::em_emission_pdu_t(void) :
    update(0),
    system_count(0),
    padding(0),
    systems(0)
{

}


// ----------------------------------------------------------------------------
vdis::em_emission_pdu_t::~em_emission_pdu_t(void)
{
    clear();
}


// ----------------------------------------------------------------------------
void vdis::em_emission_pdu_t::clear(void)
{
    for(uint8_t i = 0; systems and (i < system_count); ++i)
    {
        if (systems[i])
        {
            delete systems[i];
            systems[i] = 0;
        }
    }

    if (systems)
    {
        delete[] systems;
    }

    pdu_base_t::clear();
    emitting_entity.clear();
    event.clear();
    update = 0;
    system_count = 0;
    padding = 0;
    systems = 0;
}


// ----------------------------------------------------------------------------
void vdis::em_emission_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "em_emission.";

    header.print((prefix + "header."), out);

    out << prefix << "emitting_entity "
        << entity_marking(emitting_entity) << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "update " << (int)update << std::endl
        << prefix << "system_count " << (int)system_count << std::endl
        << prefix << "padding(16 bits) " << to_bin_string(padding) << std::endl;

    for(uint16_t i = 0; (systems and (i < system_count)); ++i)
    {
        if (systems[i])
        {
            systems[i]->print(
                (prefix + "systems[" + to_string(i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::em_emission_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    emitting_entity.read(stream);
    event.read(stream);
    stream.read(update);
    stream.read(system_count);
    stream.read(padding);

    if (system_count > 0)
    {
        systems = new emitter_system_t*[system_count];

        for(uint16_t i = 0; (i < system_count) and stream.ready(); ++i)
        {
            systems[i] = new emitter_system_t;
            systems[i]->read(stream);
        }
    }
}


// ----------------------------------------------------------------------------
void vdis::em_emission_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    emitting_entity.write(stream);
    event.write(stream);
    stream.write(update);
    stream.write(system_count);
    stream.write(padding);

    for(uint16_t i = 0; (i < system_count) and systems; ++i)
    {
        systems[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
vdis::designator_pdu_t::designator_pdu_t(void) :
    spot_type(0),
    system_name(0),
    code(0),
    power(0),
    wavelength(0),
    algorithm(0),
    flash_rate(0),
    system_number(0),
    function(0)
{

}

// ----------------------------------------------------------------------------
vdis::designator_pdu_t::~designator_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdis::designator_pdu_t::clear(void)
{
    pdu_base_t::clear();
    designating_id.clear();
    spot_type = 0;
    system_name = 0;
    designated_id.clear();
    code = 0;
    power = 0;
    wavelength = 0;
    spot_offset.clear();
    spot_location.clear();
    algorithm = 0;
    flash_rate = 0;
    system_number = 0;
    function = 0;
    beam_offset.clear();
}

// ----------------------------------------------------------------------------
void vdis::designator_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "designator.";

    header.print((prefix + "header."), out);

    out << prefix << "designating_id "
        << entity_marking(designating_id) << std::endl
        << prefix << "designated_id "
        << entity_marking(designated_id) << std::endl
        << prefix << "spot_type " << spot_type_enum() << std::endl
        << prefix << "system_name " << system_name_enum() << std::endl
        << prefix << "code " << (int)code << std::endl
        << prefix << "power(watts) " << to_string(power) << std::endl
        << prefix << "wavelength(microns) " << to_string(wavelength) << std::endl
        << prefix << "spot_offset " << spot_offset << std::endl
        << prefix << "spot_location " << spot_location << std::endl
        << prefix << "algorithm " << (dead_reckoning_e)algorithm << std::endl
        << prefix << "flash_rate " << (int)flash_rate << std::endl
        << prefix << "system_number " << (int)system_number << std::endl
        << prefix << "function " << function_enum() << std::endl
        << prefix << "beam_offset " << beam_offset << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::designator_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    designating_id.read(stream);
    stream.read(spot_type);
    stream.read(system_name);
    designated_id.read(stream);
    stream.read(code);
    stream.read(power);
    stream.read(wavelength);
    spot_offset.read(stream);
    spot_location.read(stream);
    stream.read(algorithm);
    stream.read(flash_rate);
    stream.read(system_number);
    stream.read(function);
    beam_offset.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::designator_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    designating_id.write(stream);
    stream.write(spot_type);
    stream.write(system_name);
    designated_id.write(stream);
    stream.write(code);
    stream.write(power);
    stream.write(wavelength);
    spot_offset.write(stream);
    spot_location.write(stream);
    stream.write(algorithm);
    stream.write(flash_rate);
    stream.write(system_number);
    stream.write(function);
    beam_offset.write(stream);
}

// ----------------------------------------------------------------------------
vdis::transmitter_pdu_t::transmitter_pdu_t(void) :
    radio_id(0),
    transmit_state(0),
    input_source(0),
    transmitter_parameters(0),
    antenna_pattern_type(0),
    antenna_pattern_length(0),
    frequency(0),
    bandwidth(0),
    power(0),
    crypto_system(0),
    crypto_key(0),
    modulation_parameter_length(0),
    padding8(0),
    padding16(0),
    antenna_patterns(0),
    modulation_parameters(0)
{

}

// ----------------------------------------------------------------------------
vdis::transmitter_pdu_t::~transmitter_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::transmitter_pdu_t::clear(void)
{
    if (antenna_patterns)
    {
        delete[] antenna_patterns;
    }

    if (modulation_parameters)
    {
        delete[] modulation_parameters;
    }

    pdu_base_t::clear();
    entity_id.clear();
    radio_id = 0;
    radio_type.clear();
    transmit_state = 0;
    input_source = 0;
    transmitter_parameters = 0;
    antenna_location.clear();
    relative_antenna_location.clear();
    antenna_pattern_type = 0;
    antenna_pattern_length = 0;
    frequency = 0;
    bandwidth = 0;
    power = 0;
    modulation_type.clear();
    crypto_system = 0;
    crypto_key = 0;
    modulation_parameter_length = 0;
    padding8 = 0;
    padding16 = 0;
    antenna_patterns = 0;
    modulation_parameters = 0;
}

// ----------------------------------------------------------------------------
void vdis::transmitter_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "transmitter.";
    byte_buffer_t
        antenna_pattern_bytes(
            antenna_patterns,
            antenna_pattern_length,
            false),
        modulation_parameter_bytes(
            modulation_parameters,
            modulation_parameter_length,
            false);

    header.print((prefix + "header."), out);

    out << prefix << "entity_id " << entity_marking(entity_id) << std::endl
        << prefix << "radio_id " << (int)radio_id << std::endl
        << prefix << "radio_type " << radio_type
        << " '" << radio_type.description() << "'" << std::endl
        << prefix << "transmit_state "
        << (transmit_state_e)transmit_state << std::endl
        << prefix << "input_source "
        << (input_source_e)input_source << std::endl
        << prefix << "transmitter_parameters "
        << to_bin_string(transmitter_parameters, true) << std::endl
        << prefix << "antenna_location "
        << antenna_location << std::endl
        << prefix << "relative_antenna_location "
        << relative_antenna_location << std::endl
        << prefix << "antenna_pattern_type "
        << (antenna_pattern_type_e)antenna_pattern_type << std::endl
        << prefix << "frequency(Hz) " << (int)frequency << std::endl
        << prefix << "bandwidth(Hz) " << to_string(bandwidth) << std::endl
        << prefix << "power(dBm) " << to_string(power) << std::endl;

    modulation_type.print((prefix + "modulation_type."), out);

    out << prefix << "crypto_system "
        << (crypto_sys_e)crypto_system << std::endl
        << prefix << "crypto_key "
        << to_bin_string(crypto_key) << std::endl
        << prefix << "padding(8 bits) "
        << to_bin_string(padding8) << std::endl
        << prefix << "padding(16 bits) "
        << to_bin_string(padding16) << std::endl
        << prefix << "antenna_patterns.length "
        << (int)antenna_pattern_length << std::endl;

    antenna_pattern_bytes.print((prefix + "antenna_patterns."), out);

    out << prefix << "modulation_parameters.length "
        << (int)modulation_parameter_length << std::endl;

    modulation_parameter_bytes.print((prefix + "modulation_parameters."), out);
}

// ----------------------------------------------------------------------------
void vdis::transmitter_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    entity_id.read(stream);
    stream.read(radio_id);
    radio_type.read(stream);
    stream.read(transmit_state);
    stream.read(input_source);
    stream.read(transmitter_parameters);
    antenna_location.read(stream);
    relative_antenna_location.read(stream);
    stream.read(antenna_pattern_type);
    stream.read(antenna_pattern_length);
    stream.read(frequency);
    stream.read(bandwidth);
    stream.read(power);
    modulation_type.read(stream);
    stream.read(crypto_system);
    stream.read(crypto_key);
    stream.read(modulation_parameter_length);
    stream.read(padding8);
    stream.read(padding16);

    if (antenna_pattern_length > 0)
    {
        antenna_patterns = new uint8_t[antenna_pattern_length];

        stream.read(
            antenna_patterns,
            (uint32_t)antenna_pattern_length);
    }

    if (modulation_parameter_length > 0)
    {
        modulation_parameters = new uint8_t[modulation_parameter_length];

        stream.read(
            modulation_parameters,
            (uint32_t)modulation_parameter_length);
    }
}

// ----------------------------------------------------------------------------
void vdis::transmitter_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    entity_id.write(stream);
    stream.write(radio_id);
    radio_type.write(stream);
    stream.write(transmit_state);
    stream.write(input_source);
    stream.write(transmitter_parameters);
    antenna_location.write(stream);
    relative_antenna_location.write(stream);
    stream.write(antenna_pattern_type);
    stream.write(antenna_pattern_length);
    stream.write(frequency);
    stream.write(bandwidth);
    stream.write(power);
    modulation_type.write(stream);
    stream.write(crypto_system);
    stream.write(crypto_key);
    stream.write(modulation_parameter_length);
    stream.write(padding8);
    stream.write(padding16);

    if (antenna_patterns and (antenna_pattern_length > 0))
    {
        stream.write(
            antenna_patterns,
            (uint32_t)antenna_pattern_length);
    }

    if (modulation_parameters and (modulation_parameter_length > 0))
    {
        stream.write(
            modulation_parameters,
            (uint32_t)modulation_parameter_length);
    }
}

// ----------------------------------------------------------------------------
vdis::signal_pdu_t::signal_pdu_t(void) :
    radio_id(0),
    tdl_type(0),
    sample_rate(0),
    data_length(0),
    samples(0),
    data(0)
{
    encoding.clear();
}

// ----------------------------------------------------------------------------
vdis::signal_pdu_t::~signal_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::signal_pdu_t::clear(void)
{
    if (data)
    {
        delete[] data;
    }

    pdu_base_t::clear();
    entity_id.clear();
    radio_id = 0;
    encoding.clear();
    tdl_type = 0;
    sample_rate = 0;
    data_length = 0;
    samples = 0;
    data = 0;
    padding.clear();
}

// ----------------------------------------------------------------------------
void vdis::signal_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "signal.";
    byte_buffer_t
        data_bytes(data, data_length, false);

    header.print((prefix + "header."), out);

    out << prefix << "entity_id " << entity_marking(entity_id) << std::endl
        << prefix << "radio_id " << (int)radio_id << std::endl;

    encoding.print(prefix, out);

    out << prefix << "tdl_type " << (tdl_type_e)tdl_type << std::endl
        << prefix << "sample_rate " << sample_rate << std::endl
        << prefix << "samples " << (int)samples << std::endl
        << prefix << "data.length " << (int)data_length << std::endl;

    data_bytes.print((prefix + "data."), out);

    if (padding.length() > 0)
    {
        padding.print((prefix + "padding."), out);
    }
}

// ----------------------------------------------------------------------------
void vdis::signal_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    entity_id.read(stream);
    stream.read(radio_id);
    encoding.read(stream);
    stream.read(tdl_type);
    stream.read(sample_rate);
    stream.read(data_length);
    stream.read(samples);

    if (data_length > 0)
    {
        // Data length will be in bits...
        //
        if ((data_length % 8) > 0)
        {
            LOG_ERROR("Signal data length not a factor of 8: %d", data_length);
        }

        data_length = (data_length / 8);

        data = new uint8_t[data_length];
        stream.read(data, (uint32_t)data_length);

        uint32_t temp_padding_length = padding_length(data_length, 4);

        if (temp_padding_length > 0)
        {
            padding.read(stream, temp_padding_length);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::signal_pdu_t::write(byte_stream_t &stream)
{
    const uint16_t data_length_bits = (data_length * 8);

    header.write(stream);
    entity_id.write(stream);
    stream.write(radio_id);
    encoding.write(stream);
    stream.write(tdl_type);
    stream.write(sample_rate);
    stream.write(data_length_bits);
    stream.write(samples);

    if (data and (data_length > 0))
    {
        stream.write(data, (uint32_t)data_length);
    }

    if (padding.length() > 0)
    {
        padding.write(stream);
    }
}

// ----------------------------------------------------------------------------
vdis::receiver_pdu_t::receiver_pdu_t(void) :
    radio_id(0),
    receiver_state(0),
    padding(0),
    power(0),
    transmitter_radio(0)
{

}

// ----------------------------------------------------------------------------
vdis::receiver_pdu_t::~receiver_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::receiver_pdu_t::clear(void)
{
    pdu_base_t::clear();
    entity_id.clear();
    radio_id = 0;
    receiver_state = 0;
    padding = 0;
    power = 0;
    transmitter_entity.clear();
    transmitter_radio = 0;
}

// ----------------------------------------------------------------------------
void vdis::receiver_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "receiver.";

    header.print((prefix + "header."), out);

    out << prefix << "receiver_entity "
        << entity_marking(entity_id) << std::endl
        << prefix << "receiver_state "
        << (receiver_e)receiver_state << std::endl
        << prefix << "radio_id "
        << (int)radio_id << std::endl
        << prefix << "padding(16 bits) "
        << to_bin_string(padding, true) << std::endl
        << prefix << "power(dBm) "
        << to_string(power) << std::endl
        << prefix << "transmitter_entity "
        << entity_marking(transmitter_entity) << std::endl
        << prefix << "transmitter_radio "
        << (int)transmitter_radio << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::receiver_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    entity_id.read(stream);
    stream.read(radio_id);
    stream.read(receiver_state);
    stream.read(padding);
    stream.read(power);
    transmitter_entity.read(stream);
    stream.read(transmitter_radio);
}

// ----------------------------------------------------------------------------
void vdis::receiver_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    entity_id.write(stream);
    stream.write(radio_id);
    stream.write(receiver_state);
    stream.write(padding);
    stream.write(power);
    transmitter_entity.write(stream);
    stream.write(transmitter_radio);
}

// ----------------------------------------------------------------------------
vdis::iff_pdu_t::iff_pdu_t(void) : layer2(0)
{

}

// ----------------------------------------------------------------------------
vdis::iff_pdu_t::~iff_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::iff_pdu_t::clear(void)
{
    pdu_base_t::clear();
    layer1.clear();

    if (layer2)
    {
        delete layer2;
        layer2 = 0;
    }

    extra_layers.clear();
}

// ----------------------------------------------------------------------------
void vdis::iff_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "iff.";

    header.print("iff.header.", out);
    layer1.print("iff.layer1.", out);

    if (layer2)
    {
        layer2->print("iff.layer2.", out);
    }
}

// ----------------------------------------------------------------------------
void vdis::iff_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    layer1.read(stream);

    // TODO: read layers 2,3,...
}

// ----------------------------------------------------------------------------
void vdis::iff_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    layer1.write(stream);

    // TODO: read layers 2,3,...
}

// ----------------------------------------------------------------------------
vdis::environmental_process_pdu_t::environmental_process_pdu_t(void) :
    model_type(0),
    status(0),
    record_count(0),
    sequence_number(0),
    records(0)
{

}

// ----------------------------------------------------------------------------
vdis::environmental_process_pdu_t::~environmental_process_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::environmental_process_pdu_t::clear(void)
{
    for(uint16_t i = 0; records and (i < record_count); ++i)
    {
        if (records[i])
        {
            delete records[i];
            records[i] = 0;
        }
    }

    if (records)
    {
        delete[] records;
        records = 0;
    }

    pdu_base_t::clear();
    process_id.clear();
    environment_type.clear();
    model_type = 0;
    status = 0;
    record_count = 0;
    sequence_number = 0;
}

// ----------------------------------------------------------------------------
void vdis::environmental_process_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "environmental_process.";

    header.print((prefix + "header."), out);

    out << prefix << "process_id " << process_id << std::endl
        << prefix << "environment_type " << environment_type
        << " " << environment_type.description() << std::endl
        << prefix << "model_type " << (int)model_type << std::endl
        << prefix << "status " << (int)status << std::endl
        << prefix << "sequence_number " << (int)sequence_number << std::endl
        << prefix << "records.count " << (int)record_count << std::endl;

    for(uint16_t i = 0; records and (i < record_count); ++i)
    {
        if (records[i])
        {
            records[i]->print(
                (prefix + "records[" + to_string((int)i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::environmental_process_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    process_id.read(stream);
    environment_type.read(stream);
    stream.read(model_type);
    stream.read(status);
    stream.read(record_count);
    stream.read(sequence_number);

    if (record_count > 0)
    {
        records = new environment_record_t*[record_count];

        for(uint16_t i = 0; i < record_count; ++i)
        {
            records[i] = new environment_record_t;
            records[i]->read(stream);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::environmental_process_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    process_id.write(stream);
    environment_type.write(stream);
    stream.write(model_type);
    stream.write(status);
    stream.write(record_count);
    stream.write(sequence_number);

    for(uint16_t i = 0; records and (i < record_count); ++i)
    {
        if (records[i])
        {
            records[i]->write(stream);
        }
    }
}

// ----------------------------------------------------------------------------
vdis::point_object_state_pdu_t::point_object_state_pdu_t(void) :
    update(0),
    force_id(0),
    specific_appearance(0),
    padding16(0),
    padding32(0)
{
    modifications.clear();
    generic_appearance.clear();
}

// ----------------------------------------------------------------------------
vdis::point_object_state_pdu_t::~point_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::point_object_state_pdu_t::clear(void)
{
    pdu_base_t::clear();
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    modifications.clear();
    object_type.clear();
    location.clear();
    orientation.clear();
    specific_appearance = 0;
    generic_appearance.clear();
    padding16 = 0;
    requestor_id.clear();
    receiver_id.clear();
    padding32 = 0;
}

// ----------------------------------------------------------------------------
void vdis::point_object_state_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "point_object_state.";
    geodetic_location_t
        geodetic_location;

    convert(location, geodetic_location);

    header.print((prefix + "header."), out);

    out << prefix << "object_id " << object_id << std::endl
        << prefix << "referenced_object_id "
        << referenced_object_id << std::endl
        << prefix << "update " << (int)update << std::endl
        << prefix << "force_id " << (force_id_e)force_id << std::endl
        << prefix << "modifications "
        << to_bin_string(modifications.value) << std::endl;

    modifications.print(prefix, out);

    out << prefix << "object_type " << object_type << " "
        << object_type.description(OBJECT_GEOMETRY_POINT) << std::endl
        << prefix << "location.gcc " << location << std::endl
        << prefix << "location.gdc " << geodetic_location << std::endl
        << prefix << "orientation " << orientation << std::endl
        << prefix << "specific_appearance "
        << to_bin_string(specific_appearance, true) << std::endl;

    switch(object_type.get() & 0xFFFFFF00U)
    {
        case 0x01010100U: // Abatis (1.1.1.*)
        case 0x01010200U: // Log Cribs (1.1.2.*)
        case 0x01020100U: // Vehicle Defilades (1.2.1.*)
        case 0x01020200U: // Infantry Fighting Positions (1.2.2.*)
        {
            point_appearance_v2_t point_appearance;
            point_appearance.value = specific_appearance;
            point_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
        case 0x00050100U: // Smoke Ground Bursts (0.5.1.*)
        case 0x00050200U: // Smoke Air Bursts (0.5.2.*)
        {
            point_appearance_v3_t point_appearance;
            point_appearance.value = specific_appearance;
            point_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
        case 0x01010300U: // Craters (1.1.3.*)
        {
            point_appearance_v4_t point_appearance;
            point_appearance.value = specific_appearance;
            point_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
        case 0x01040300U: // Ribbon Bridges (1.4.3.*)
        {
            point_appearance_v5_t point_appearance;
            point_appearance.value = specific_appearance;
            point_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
    }

    generic_appearance.print(prefix, out);

    out << prefix << "padding16 " << to_hex_string(padding16) << std::endl
        << prefix << "requestor_id " << requestor_id << std::endl
        << prefix << "receiver_id " << receiver_id << std::endl
        << prefix << "padding32 " << to_hex_string(padding32) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::point_object_state_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    object_id.read(stream);
    referenced_object_id.read(stream);
    stream.read(update);
    stream.read(force_id);
    modifications.read(stream);
    object_type.read(stream);
    location.read(stream);
    orientation.read(stream);
    stream.read(specific_appearance);
    stream.read(generic_appearance);
    stream.read(padding16);
    requestor_id.read(stream);
    receiver_id.read(stream);
    stream.read(padding32);
}

// ----------------------------------------------------------------------------
void vdis::point_object_state_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    object_id.write(stream);
    referenced_object_id.write(stream);
    stream.write(update);
    stream.write(force_id);
    modifications.write(stream);
    object_type.write(stream);
    location.write(stream);
    orientation.write(stream);
    stream.write(specific_appearance);
    stream.write(generic_appearance);
    stream.write(padding16);
    requestor_id.write(stream);
    receiver_id.write(stream);
    stream.write(padding32);
}

// ----------------------------------------------------------------------------
vdis::linear_object_state_pdu_t::linear_object_state_pdu_t(void) :
    update(0),
    force_id(0),
    segment_count(0),
    segments(0)
{

}

// ----------------------------------------------------------------------------
vdis::linear_object_state_pdu_t::~linear_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::linear_object_state_pdu_t::clear(void)
{
    for(uint8_t i = 0; segments and (i < segment_count); ++i)
    {
         if (segments[i])
         {
             delete segments[i];
             segments[i] = 0;
         }
    }

    if (segments)
    {
        delete[] segments;
        segments = 0;
    }

    pdu_base_t::clear();
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    segment_count = 0;
    requestor_id.clear();
    receiver_id.clear();
    object_type.clear();
}

// ----------------------------------------------------------------------------
void vdis::linear_object_state_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "linear_object_state.";

    header.print((prefix + "header."), out);

    out << prefix << "object_id " << object_id << std::endl
        << prefix << "referenced_object_id "
        << referenced_object_id << std::endl
        << prefix << "update " << (int)update << std::endl
        << prefix << "force_id " << (force_id_e)force_id << std::endl
        << prefix << "requestor_id " << requestor_id << std::endl
        << prefix << "receiver_id " << receiver_id << std::endl
        << prefix << "object_type " << object_type << " "
        << object_type.description(OBJECT_GEOMETRY_LINEAR) << std::endl
        << prefix << "segments.count " << (int)segment_count << std::endl;

    linear_segment_t::using_type(&object_type);

    for(uint8_t i = 0; segments and (i < segment_count); ++i)
    {
        if (segments[i])
        {
            segments[i]->print(
                (prefix + "segments[" + to_string((int)i) + "]."),
                out);
        }
    }

    linear_segment_t::using_type(0);
}

// ----------------------------------------------------------------------------
void vdis::linear_object_state_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    object_id.read(stream);
    referenced_object_id.read(stream);
    stream.read(update);
    stream.read(force_id);
    stream.read(segment_count);
    requestor_id.read(stream);
    receiver_id.read(stream);
    object_type.read(stream);

    if (segment_count > 0)
    {
        segments = new linear_segment_t*[segment_count];

        for(uint8_t i = 0; i < segment_count; ++i)
        {
            segments[i] = new linear_segment_t;
            segments[i]->read(stream);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::linear_object_state_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    object_id.write(stream);
    referenced_object_id.write(stream);
    stream.write(update);
    stream.write(force_id);
    stream.write(segment_count);
    requestor_id.write(stream);
    receiver_id.write(stream);
    object_type.write(stream);

    for(uint8_t i = 0; segments and (i < segment_count); ++i)
    {
        segments[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
vdis::areal_object_state_pdu_t::areal_object_state_pdu_t(void) :
    update(0),
    force_id(0),
    specific_appearance(0),
    point_count(0),
    points(0)
{
    modifications.clear();
}

// ----------------------------------------------------------------------------
vdis::areal_object_state_pdu_t::~areal_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::areal_object_state_pdu_t::clear(void)
{
    for(uint16_t i = 0; points and (i < point_count); ++i)
    {
         if (points[i])
         {
             delete points[i];
             points[i] = 0;
         }
    }

    if (points)
    {
        delete[] points;
        points = 0;
    }

    pdu_base_t::clear();
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    modifications.clear();
    object_type.clear();
    specific_appearance = 0;
    generic_appearance.clear();
    point_count = 0;
    requestor_id.clear();
    receiver_id.clear();
}

// ----------------------------------------------------------------------------
void vdis::areal_object_state_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "areal_object_state.";
    geodetic_location_t
        geodetic_location;

    header.print((prefix + "header."), out);

    out << prefix << "object_id " << object_id << std::endl
        << prefix << "referenced_object_id "
        << referenced_object_id << std::endl
        << prefix << "update " << (int)update << std::endl
        << prefix << "force_id " << (force_id_e)force_id << std::endl
        << prefix << "modifications "
        << to_bin_string(modifications.value) << std::endl;

    modifications.print(prefix, out);

    out << prefix << "object_type " << object_type << " "
        << object_type.description(OBJECT_GEOMETRY_AREAL) << std::endl
        << prefix << "specific_appearance "
        << to_bin_string(specific_appearance, true) << std::endl;

    switch(object_type.get() & 0xFFFFFF00U)
    {
        case 0x00010100U: // Minefields (0.1.1.*)
        {
            areal_appearance_v1_t areal_appearance;
            areal_appearance.value = specific_appearance;
            areal_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
    }

    generic_appearance.print(prefix, out);

    out << prefix << "requestor_id " << requestor_id << std::endl
        << prefix << "receiver_id " << receiver_id << std::endl
        << prefix << "points.count " << (int)point_count << std::endl;

    for(uint8_t i = 0; points and (i < point_count); ++i)
    {
        if (points[i])
        {
            convert(*points[i], geodetic_location);

            out << prefix << "points[" << (int)i << "].gcc "
                << *points[i] << std::endl
                << prefix << "points[" << (int)i << "].gdc "
                << geodetic_location << std::endl;
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::areal_object_state_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    object_id.read(stream);
    referenced_object_id.read(stream);
    stream.read(update);
    stream.read(force_id);
    modifications.read(stream);
    object_type.read(stream);
    stream.read(specific_appearance);
    generic_appearance.read(stream);
    stream.read(point_count);
    requestor_id.read(stream);
    receiver_id.read(stream);

    if (point_count > 0)
    {
        points = new location24_t*[point_count];

        for(uint8_t i = 0; i < point_count; ++i)
        {
            points[i] = new location24_t;
            points[i]->read(stream);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::areal_object_state_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    object_id.write(stream);
    referenced_object_id.write(stream);
    stream.write(update);
    stream.write(force_id);
    modifications.write(stream);
    object_type.write(stream);
    stream.write(specific_appearance);
    generic_appearance.write(stream);
    stream.write(point_count);
    requestor_id.write(stream);
    receiver_id.write(stream);

    for(uint8_t i = 0; points and (i < point_count); ++i)
    {
        points[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
vdis::application_control_pdu_t::application_control_pdu_t(void)
{
    header.clear();
    originator.clear();
    recipient.clear();
    reliability_service = 0;
    time_interval = 0;
    control_type = 0;
    padding = 0;
    originator_type = 0;
    recipient_type = 0;
    request_id = 0;
    total_parts = 0;
    current_part = 0;
    record_count = 0;
    records = 0;
}

// ----------------------------------------------------------------------------
vdis::application_control_pdu_t::~application_control_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::application_control_pdu_t::clear(void)
{
    for(uint16_t i = 0; (records and (i < record_count)); ++i)
    {
        delete records[i];
        records[i] = 0;
    }

    if (records)
    {
        delete[] records;
    }

    pdu_base_t::clear();
    originator.clear();
    recipient.clear();
    reliability_service = 0;
    time_interval = 0;
    control_type = 0;
    padding = 0;
    originator_type = 0;
    recipient_type = 0;
    request_id = 0;
    total_parts = 0;
    current_part = 0;
    record_count = 0;
    records = 0;
}

// ----------------------------------------------------------------------------
void vdis::application_control_pdu_t::print(std::ostream &out) const
{
    const string_t
        prefix = "application_control.";

    header.print((prefix + "header."), out);

    out << prefix << "originator "
        << entity_marking(originator) << std::endl
        << prefix << "recipient "
        << entity_marking(recipient) << std::endl
        << prefix << "reliability_service "
        << (int)reliability_service << std::endl
        << prefix << "time_interval "
        << (int)time_interval << std::endl
        << prefix << "control_type "
        << control_type_enum() << std::endl
        << prefix << "padding "
        << to_bin_string(padding) << std::endl
        << prefix << "originator_type "
        << originator_type_enum() << std::endl
        << prefix << "recipient_type "
        << recipient_type_enum() << std::endl
        << prefix << "request_id "
        << request_id << std::endl
        << prefix << "total_parts "
        << (int)total_parts << std::endl
        << prefix << "current_part "
        << (int)current_part << std::endl
        << prefix << "records.count "
        << (int)record_count << std::endl;

    for(uint16_t i = 0; (records and (i < record_count)); ++i)
    {
        if (records[i])
        {
            records[i]->print(
                (prefix + "records[" + to_string(i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::application_control_pdu_t::read(byte_stream_t &stream)
{
    clear();

    header.read(stream);
    originator.read(stream);
    recipient.read(stream);
    stream.read(reliability_service);
    stream.read(time_interval);
    stream.read(control_type);
    stream.read(padding);
    stream.read(originator_type);
    stream.read(recipient_type);
    stream.read(request_id);
    stream.read(total_parts);
    stream.read(current_part);
    stream.read(record_count);

    records = read_standard_variable_records(stream, record_count);
}

// ----------------------------------------------------------------------------
void vdis::application_control_pdu_t::write(byte_stream_t &stream)
{
    header.write(stream);
    originator.write(stream);
    recipient.write(stream);
    stream.write(reliability_service);
    stream.write(time_interval);
    stream.write(control_type);
    stream.write(padding);
    stream.write(originator_type);
    stream.write(recipient_type);
    stream.write(request_id);
    stream.write(total_parts);
    stream.write(current_part);
    stream.write(record_count);

    if ((record_count > 0) and not records)
    {
        LOG_ERROR(
            "Null VP record array with length %d for originator %s!",
            record_count,
            to_string(originator).c_str());
    }
    else if ((record_count == 0) and records)
    {
        LOG_ERROR(
            "Non-null VP record array with length 0 for originator %s!",
            to_string(originator).c_str());
    }
    else if ((record_count > 0) and records)
    {
        for(uint8_t i = 0; i < record_count; ++i)
        {
            if (records[i])
            {
                records[i]->write(stream);
            }
            else
            {
                LOG_ERROR(
                    "Null VP record at index %d for originator %s!",
                    i,
                    to_string(originator).c_str());
            }
        }
    }
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        pdu.base()->print(out);
    }
    else
    {
        out << "No PDU base object available..." << std::endl;
    }

    return out;
}
