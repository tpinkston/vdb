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

#include "vdb_byte_stream.h"
#include "vdb_binary.h"
#include "vdb_datum.h"
#include "vdb_emission_pdus.h"
#include "vdb_entity_info_pdus.h"
#include "vdb_enums.h"
#include "vdb_environment_pdus.h"
#include "vdb_experimental_pdus.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_pdu_data.h"
#include "vdb_pdu.h"
#include "vdb_siman_pdus.h"
#include "vdb_radio_pdus.h"
#include "vdb_warfare_pdus.h"

// ----------------------------------------------------------------------------
vdb::pdu_t::pdu_t(void) :
    header_protocol(0),
    header_exercise(0),
    header_type(0),
    header_family(0),
    header_length(0),
    header_status(0),
    header_padding(0),
    content_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::pdu_t::~pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::pdu_t::clear(void)
{
    header_protocol = 0;
    header_exercise = 0;
    header_type = 0;
    header_family = 0;
    header_length = 0;
    header_status = 0;
    header_padding = 0;

    if (content_ptr)
    {
        delete content_ptr;
        content_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::pdu_t::print(const std::string &prefix, std::ostream &stream) const
{
    stream << prefix << "header.protocol "
           << enumerations::print(ENUM_PROTOCOL_VERSION, header_protocol)
           << std::endl
           << prefix << "header.exercise " << (int)header_exercise
           << std::endl
           << prefix << "header.type "
           << enumerations::print(ENUM_PDU_TYPE, header_type)
           << std::endl
           << prefix << "header.family "
           << enumerations::print(ENUM_PDU_FAMILY, header_family)
           << std::endl
           << prefix << "header.timestamp "
           << header_timestamp
           << " [" << hexadecimal::str(header_timestamp.value) << "]"
           << std::endl
           << prefix << "header.length " << (int)header_length
           << std::endl
           << prefix << "header.status " << binary::str(header_status)
           << std::endl
           << prefix << "header.padding " << binary::str(header_padding)
           << std::endl;

    if (content_ptr)
    {
        content_ptr->print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::pdu_t::length(void) const
{
    return (HEADER_LENGTH + (content_ptr ? content_ptr->length() : 0));
}

// ----------------------------------------------------------------------------
// Reads just the first 12 bytes of the PDU (the header)
//
void vdb::pdu_t::read(byte_stream &stream)
{
    clear();

    stream.read(header_protocol);
    stream.read(header_exercise);
    stream.read(header_type);
    stream.read(header_family);
    stream.read(header_timestamp);
    stream.read(header_length);
    stream.read(header_status);
    stream.read(header_padding);

    LOG_EXTRA_VERBOSE(
        "PDU header, protocol %d, exercise %d, type %d, family %d, "
        "length %d, status %d...",
        (int)header_protocol,
        (int)header_exercise,
        (int)header_type,
        (int)header_family,
        (int)header_length,
        (int)header_protocol,
        (int)header_status);

    switch((pdu_type_e)header_type)
    {
        case PDU_TYPE_ENTITY_STATE:
            content_ptr = new entity_state_pdu_t();
            break;
        case PDU_TYPE_FIRE:
            content_ptr = new fire_pdu_t();
            break;
        case PDU_TYPE_DETONATION:
            content_ptr = new detonation_pdu_t();
            break;
        case PDU_TYPE_COLLISION:
            content_ptr = new collision_pdu_t();
            break;
        case PDU_TYPE_CREATE_ENTITY:
            content_ptr = new create_entity_pdu_t();
            break;
        case PDU_TYPE_REMOVE_ENTITY:
            content_ptr = new remove_entity_pdu_t();
            break;
        case PDU_TYPE_START_RESUME:
            content_ptr = new start_resume_pdu_t();
            break;
        case PDU_TYPE_STOP_FREEZE:
            content_ptr = new stop_freeze_pdu_t();
            break;
        case PDU_TYPE_ACKNOWLEDGE:
            content_ptr = new acknowledge_pdu_t();
            break;
        case PDU_TYPE_ACTION_REQUEST:
            content_ptr = new action_request_pdu_t();
            break;
        case PDU_TYPE_ACTION_RESPONSE:
            content_ptr = new action_response_pdu_t();
            break;
        case PDU_TYPE_DATA_QUERY:
            content_ptr = new data_query_pdu_t();
            break;
        case PDU_TYPE_SET_DATA:
            content_ptr = new set_data_pdu_t();
            break;
        case PDU_TYPE_DATA:
            content_ptr = new data_pdu_t();
            break;
        case PDU_TYPE_EVENT_REPORT:
            content_ptr = new event_report_pdu_t();
            break;
        case PDU_TYPE_COMMENT:
            content_ptr = new comment_pdu_t();
            break;
        case PDU_TYPE_EM_EMISSION:
            content_ptr = new em_emission_pdu_t();
            break;
        case PDU_TYPE_DESIGNATOR:
            content_ptr = new designator_pdu_t();
            break;
        case PDU_TYPE_TRANSMITTER:
            content_ptr = new transmitter_pdu_t();
            break;
        case PDU_TYPE_SIGNAL:
            content_ptr = new signal_pdu_t();
            break;
        case PDU_TYPE_RECEIVER:
            content_ptr = new receiver_pdu_t();
            break;
        case PDU_TYPE_IFF:
            content_ptr = new iff_pdu_t();
            break;
//        case PDU_TYPE_MINEFIELD_STATE:
////TODO            content_ptr = new minefield_state_pdu_t();
//            content_ptr = new default_pdu_t();
//            break;
//        case PDU_TYPE_MINEFIELD_QUERY:
////TODO            content_ptr = new minefield_query_pdu_t();
//            content_ptr = new default_pdu_t();
//            break;
//        case PDU_TYPE_MINEFIELD_DATA:
////TODO            content_ptr = new minefield_data_pdu_t();
//            content_ptr = new default_pdu_t();
//            break;
//        case PDU_TYPE_MINEFIELD_RESPONSE_NAK:
////TODO            content_ptr = new minefield_response_nack_pdu_t();
//            content_ptr = new default_pdu_t();
//            break;
        case PDU_TYPE_ENVIRONMENTAL_PROCESS:
            content_ptr = new environmental_process_pdu_t();
            break;
        case PDU_TYPE_POINT_OBJECT_STATE:
            content_ptr = new point_object_state_pdu_t();
            break;
        case PDU_TYPE_LINEAR_OBJECT_STATE:
            content_ptr = new linear_object_state_pdu_t();
            break;
        case PDU_TYPE_AREAL_OBJECT_STATE:
            content_ptr = new areal_object_state_pdu_t();
            break;
        case PDU_TYPE_ENTITY_STATE_UPDATE:
            content_ptr = new entity_state_update_pdu_t();
            break;
        case PDU_TYPE_APPLICATION_CTRL:
            content_ptr = new application_control_pdu_t();
            break;
        default:
            content_ptr = new default_pdu_t(header_length - HEADER_LENGTH);
    }

    content_ptr->read(stream);

    const uint32_t
        content_length = content_ptr->length();

    if ((header_length - HEADER_LENGTH) != content_length)
    {
        LOG_WARNING(
            "PDU length discrepancy: %d/%d",
            content_length,
            header_length);
    }
}

// ----------------------------------------------------------------------------
// Writes just the first 12 bytes of the PDU (the header)
//
void vdb::pdu_t::write(byte_stream &stream) const
{
    header_length = length();

    stream.write(header_protocol);
    stream.write(header_exercise);
    stream.write(header_type);
    stream.write(header_family);
    stream.write(header_timestamp);
    stream.write(header_length);
    stream.write(header_status);
    stream.write(header_padding);

    if (content_ptr)
    {
        stream.write(*content_ptr);
    }
}

// ----------------------------------------------------------------------------
vdb::default_pdu_t::default_pdu_t(uint32_t length) : data(length)
{

}

// ----------------------------------------------------------------------------
vdb::default_pdu_t::~default_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::default_pdu_t::clear(void)
{
    data.clear();
}

// ----------------------------------------------------------------------------
void vdb::default_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  (no decoding)" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::default_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    data.print((prefix + "content."), stream);
}

// ----------------------------------------------------------------------------
void vdb::default_pdu_t::read(byte_stream &stream)
{
    // The 'length' parameter on 'data' member should be set to the number
    // of bytes to be read before this call (via constructor or manually).
    //
    data.read(stream);
}

// ----------------------------------------------------------------------------
void vdb::default_pdu_t::write(byte_stream &stream) const
{
    data.write(stream);
}
