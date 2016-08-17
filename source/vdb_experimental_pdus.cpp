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
#include "vdb_enums.h"
#include "vdb_experimental_pdus.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
vdb::application_control_pdu_t::application_control_pdu_t(void) :
    reliability_service(0),
    time_interval(0),
    control_type(0),
    padding(0),
    originating_application_type(0),
    receiving_application_type(0),
    request_id(0),
    total_parts(0),
    current_part(0)
{

}

// ----------------------------------------------------------------------------
vdb::application_control_pdu_t::application_control_pdu_t(
    const application_control_pdu_t &copy
) :
    reliability_service(copy.reliability_service),
    time_interval(copy.time_interval),
    control_type(copy.control_type),
    padding(copy.padding),
    originating_application_type(copy.originating_application_type),
    receiving_application_type(copy.receiving_application_type),
    request_id(copy.request_id),
    total_parts(copy.total_parts),
    current_part(copy.current_part),
    records(copy.records)
{

}

// ----------------------------------------------------------------------------
vdb::application_control_pdu_t::~application_control_pdu_t()
{

}

// ----------------------------------------------------------------------------
void vdb::application_control_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    reliability_service = 0;
    time_interval = 0;
    control_type = 0;
    padding = 0;
    originating_application_type = 0;
    receiving_application_type = 0;
    request_id = 0;
    total_parts = 0;
    current_part = 0;
    records.clear();
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::application_control_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::application_control_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::application_control_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        (originator == *id_ptr) or (recipient == *id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::application_control_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl
           << prefix << "reliability_service "
           << (int)reliability_service << std::endl
           << prefix << "control_type "
           << enumerations::print(ENUM_APP_CTRL_CONTROL_TYPE, control_type)
           << std::endl
           << prefix << "padding " << (int)padding << std::endl
           << prefix << "originating_application_type "
           << enumerations::print(
               ENUM_APP_CTRL_APPLICATION_TYPE,
               originating_application_type) << std::endl
           << prefix << "receiving_application_type "
           << enumerations::print(
               ENUM_APP_CTRL_APPLICATION_TYPE,
               receiving_application_type) << std::endl
           << prefix << "request_id " << (int)request_id << " (0x"
           << hexadecimal::str(request_id) << ")" << std::endl
           << prefix << "total_parts " << (int)total_parts << std::endl
           << prefix << "current_part " << (int)current_part << std::endl
           << prefix << "records " << records.records.size() << std::endl;

    records.print(prefix, stream);
}

// ----------------------------------------------------------------------------
void vdb::application_control_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << recipient
           << "  " << (app_ctrl_control_type_e)control_type << std::endl
           << "  " << (app_ctrl_application_type_e)originating_application_type
           << "->" << (app_ctrl_application_type_e)receiving_application_type
           << std::endl;

    records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::application_control_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(reliability_service);
    stream.read(time_interval);
    stream.read(control_type);
    stream.read(padding);
    stream.read(originating_application_type);
    stream.read(receiving_application_type);
    stream.read(request_id);
    stream.read(total_parts);
    stream.read(current_part);
    stream.read(records);
}

// ----------------------------------------------------------------------------
void vdb::application_control_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(reliability_service);
    stream.write(time_interval);
    stream.write(control_type);
    stream.write(padding);
    stream.write(originating_application_type);
    stream.write(receiving_application_type);
    stream.write(request_id);
    stream.write(total_parts);
    stream.write(current_part);
    stream.write(records);
}
