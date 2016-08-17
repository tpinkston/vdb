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
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_siman_pdus.h"

// ----------------------------------------------------------------------------
vdb::acknowledge_pdu_t::acknowledge_pdu_t(void) :
    acknowledge_flag(0),
    response_flag(0),
    request_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::acknowledge_pdu_t::~acknowledge_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::acknowledge_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    acknowledge_flag = 0;
    response_flag = 0;
    request_id = 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::acknowledge_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::acknowledge_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::acknowledge_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::acknowledge_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl << prefix << "acknowledge_flag "
           << enumerations::print(ENUM_ACK_ACKNOWLEDGE_FLAG, acknowledge_flag)
           << std::endl << prefix << "acknowledge_flag "
           << enumerations::print(ENUM_ACK_RESPONSE_FLAG, response_flag)
           << std::endl << prefix << "request_id "
           << request_id << " (0x" << hexadecimal::str(request_id) << ")"
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::acknowledge_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator
           << "->" << recipient << std::endl
           << "  " << (ack_acknowledge_flag_e)acknowledge_flag
           << "  " << (ack_response_flag_e)response_flag << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::acknowledge_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(acknowledge_flag);
    stream.read(response_flag);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdb::acknowledge_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(acknowledge_flag);
    stream.write(response_flag);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdb::create_entity_pdu_t::create_entity_pdu_t(void) : request_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::create_entity_pdu_t::~create_entity_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::create_entity_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::create_entity_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::create_entity_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::create_entity_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::create_entity_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl << prefix << "request_id "
           << request_id << " (0x" << hexadecimal::str(request_id) << ")"
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::create_entity_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::create_entity_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdb::create_entity_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdb::remove_entity_pdu_t::remove_entity_pdu_t(void) : request_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::remove_entity_pdu_t::~remove_entity_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::remove_entity_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::remove_entity_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::remove_entity_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::remove_entity_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::remove_entity_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl << prefix << "request_id "
           << request_id << " (0x" << hexadecimal::str(request_id) << ")"
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::remove_entity_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::remove_entity_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdb::remove_entity_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdb::start_resume_pdu_t::start_resume_pdu_t(void) : request_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::start_resume_pdu_t::~start_resume_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::start_resume_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    real_time.clear();
    simulation_time.clear();
    request_id = 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::start_resume_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::start_resume_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::start_resume_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::start_resume_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl
           << prefix << "real_time " << real_time << std::endl
           << prefix << "simulation_time " << simulation_time << std::endl
           << prefix << "request_id " << request_id
           << " (0x" << hexadecimal::str(request_id) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::start_resume_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl
           << "  " << real_time << "  " << simulation_time << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::start_resume_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(real_time);
    stream.read(simulation_time);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdb::start_resume_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(real_time);
    stream.write(simulation_time);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdb::stop_freeze_pdu_t::stop_freeze_pdu_t(void) :
    reason(0),
    behavior(0),
    padding(0),
    request_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::stop_freeze_pdu_t::~stop_freeze_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::stop_freeze_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    real_time.clear();
    reason = 0;
    behavior = 0;
    padding = 0;
    request_id = 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::stop_freeze_pdu_t::get_request_id(void) const
{
    return &request_id;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::stop_freeze_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::stop_freeze_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::stop_freeze_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl
           << prefix << "real_time " << real_time << std::endl
           << prefix << "reason "
           << enumerations::print(ENUM_SF_REASON_CODES, reason) << std::endl
           << prefix << "behavior "
           << enumerations::print(ENUM_FROZEN_BEHAVIOR, behavior) << std::endl
           << prefix << "request_id " << request_id << " (0x"
           << hexadecimal::str(request_id) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::stop_freeze_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl
           << "  " << real_time << std::endl
           << "  " << (sf_reason_codes_e)reason
           << "  " << (frozen_behavior_e)behavior << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::stop_freeze_pdu_t::read(byte_stream &stream)
{
    stream.read(originator);
    stream.read(recipient);
    stream.read(real_time);
    stream.read(reason);
    stream.read(behavior);
    stream.read(padding);
    stream.read(request_id);
}

// ----------------------------------------------------------------------------
void vdb::stop_freeze_pdu_t::write(byte_stream &stream) const
{
    stream.write(originator);
    stream.write(recipient);
    stream.write(real_time);
    stream.write(reason);
    stream.write(behavior);
    stream.write(padding);
    stream.write(request_id);
}

// ----------------------------------------------------------------------------
vdb::comment_pdu_t::comment_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::comment_pdu_t::~comment_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::comment_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
bool vdb::comment_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::comment_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    fixed_records.clear();
    variable_records.clear();
}

// ----------------------------------------------------------------------------
void vdb::comment_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl
           << prefix << "fixed_datum_records "
           << fixed_records.records.size() << std::endl;

    fixed_records.print("comment.", stream);

    stream << prefix << "variable_datum_records "
           << variable_records.records.size() << std::endl;

    variable_records.print("comment.", stream);
}

// ----------------------------------------------------------------------------
void vdb::comment_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::comment_pdu_t::length(void) const
{
    return (BASE_LENGTH + fixed_records.length() + variable_records.length());
}

// ----------------------------------------------------------------------------
void vdb::comment_pdu_t::read(byte_stream &stream)
{
    uint32_t
        fixed_count = 0,
        variable_count = 0;

    clear();

    stream.read(originator);
    stream.read(recipient);
    stream.read(fixed_count);
    stream.read(variable_count);

    LOG_EXTRA_VERBOSE(
        "Restoring %d fixed and %d variable records...",
        fixed_count,
        variable_count);

    fixed_records.read(stream, fixed_count);
    variable_records.read(stream, variable_count);
}

// ----------------------------------------------------------------------------
void vdb::comment_pdu_t::write(byte_stream &stream) const
{
    const uint32_t
        fixed_count = fixed_records.records.size(),
        variable_count = variable_records.records.size();

    stream.write(originator);
    stream.write(recipient);
    stream.write(fixed_count);
    stream.write(variable_count);

    fixed_records.write(stream);
    variable_records.write(stream);
}

// ----------------------------------------------------------------------------
vdb::abstract_siman_pdu_t::abstract_siman_pdu_t(void) :
    value_1(0),
    value_2(0)
{

}

// ----------------------------------------------------------------------------
vdb::abstract_siman_pdu_t::~abstract_siman_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::abstract_siman_pdu_t::get_initator(void) const
{
    return &originator;
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::set_lengths(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::abstract_siman_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        originator.matches(*id_ptr) or
        recipient.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::clear(void)
{
    originator.clear();
    recipient.clear();
    value_1 = 0;
    value_2 = 0;
    fixed_records.clear();
    variable_records.clear();
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "originator ";

    originator.print_full(stream);

    stream << std::endl << prefix << "recipient ";

    recipient.print_full(stream);

    stream << std::endl;

    print_values(prefix, stream);

    stream << prefix << "fixed_datum_records "
           << fixed_records.records.size() << std::endl;

    fixed_records.print(prefix, stream);

    stream << prefix << "variable_datum_records "
           << variable_records.records.size() << std::endl;

    variable_records.print(prefix, stream);
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << originator << "->" << recipient << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::abstract_siman_pdu_t::length(void) const
{
    return (BASE_LENGTH + fixed_records.length() + variable_records.length());
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::read(byte_stream &stream)
{
    uint32_t
        fixed_count = 0,
        variable_count = 0;

    clear();

    stream.read(originator);
    stream.read(recipient);
    stream.read(value_1);
    stream.read(value_2);
    stream.read(fixed_count);
    stream.read(variable_count);

    LOG_EXTRA_VERBOSE(
        "Restoring %d fixed and %d variable records...",
        fixed_count,
        variable_count);

    fixed_records.read(stream, fixed_count);
    variable_records.read(stream, variable_count);
}

// ----------------------------------------------------------------------------
void vdb::abstract_siman_pdu_t::write(byte_stream &stream) const
{
    const uint32_t
        fixed_count = fixed_records.records.size(),
        variable_count = variable_records.records.size();

    stream.write(originator);
    stream.write(recipient);
    stream.write(value_1);
    stream.write(value_2);
    stream.write(fixed_count);
    stream.write(variable_count);

    fixed_records.write(stream);
    variable_records.write(stream);
}

// ----------------------------------------------------------------------------
void vdb::action_request_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "action_id "
           << enumerations::print(ENUM_ACTREQ_ACTION_IDS, value_2)
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::action_request_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    stream << (actreq_action_ids_e)value_2 << std::endl;

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::action_response_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "action_id "
           << enumerations::print(ENUM_ACTREQ_ACTION_IDS, value_2)
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::action_response_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    stream << (actres_req_status_e)value_2 << std::endl;

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::data_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "padding " << value_2 << " (0x"
           << hexadecimal::str(value_2) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::data_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::data_query_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "time_interval " << value_2 << " (0x"
           << hexadecimal::str(value_2) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::data_query_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::set_data_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "padding " << value_2 << " (0x"
           << hexadecimal::str(value_2) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::set_data_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
void vdb::event_report_pdu_t::print_values(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "request_id " << value_1 << " (0x"
           << hexadecimal::str(value_1) << ")" << std::endl
           << prefix << "padding " << value_2 << " (0x"
           << hexadecimal::str(value_2) << ")" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::event_report_pdu_t::print_summary(std::ostream &stream) const
{
    abstract_siman_pdu_t::print_summary(stream);

    stream << "  " << (event_type_e)value_1 << std::endl;

    fixed_records.print_summary(stream);
    variable_records.print_summary(stream);
}
