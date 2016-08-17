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

#include "vdb_binary.h"
#include "vdb_byte_stream.h"
#include "vdb_environment_pdus.h"
#include "vdb_hexadecimal.h"
#include "vdb_services.h"
#include "vdb_string.h"

namespace
{
    const std::string
        ENVIRONMENTAL_PROCESS = "environmental_process_pdu.",
        POINT_OBJECT_STATE = "point_object_state.",
        LINEAR_OBJECT_STATE = "linear_object_state.",
        AREAL_OBJECT_STATE = "areal_object_state.";
}

// ----------------------------------------------------------------------------
vdb::environment_record_t::environment_record_t(void) :
    type(0),
    index(0),
    padding(0),
    data(0)
{

}

// ----------------------------------------------------------------------------
vdb::environment_record_t::environment_record_t(
    const environment_record_t &copy
) :
    type(copy.type),
    index(copy.index),
    padding(copy.padding),
    data(copy.data)
{

}

// ----------------------------------------------------------------------------
vdb::environment_record_t::~environment_record_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::environment_record_t::clear(void)
{
    type = 0;
    index = 0;
    padding = 0;
    data.clear();
}

// ----------------------------------------------------------------------------
void vdb::environment_record_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "type ";

    if (enumerations::valid(ENUM_ENV_GEOMETRY_REC_TYPE, type))
    {
        stream << enumerations::print(ENUM_ENV_GEOMETRY_REC_TYPE, type)
               << std::endl;
    }
    else if (enumerations::valid(ENUM_ENV_STATE_REC_TYPE, type))
    {
        stream << enumerations::print(ENUM_ENV_STATE_REC_TYPE, type)
               << std::endl;
    }
    else if (enumerations::valid(ENUM_DATUM_IDS, type))
    {
        stream << enumerations::print(ENUM_DATUM_IDS, type)
               << std::endl;
    }
    else
    {
        stream << type << " (0x" << hexadecimal::str(type) << ")"
               << std::endl;
    }

    stream << prefix << "index " << (int)index << std::endl
           << prefix << "padding " << hexadecimal::str(padding)
           << std::endl;

    data.print(prefix, stream);
}

// ----------------------------------------------------------------------------
uint32_t vdb::environment_record_t::length(void) const
{
    return (BASE_LENGTH + data.length() + padding_length());
}

// ----------------------------------------------------------------------------
uint32_t vdb::environment_record_t::padding_length(void) const
{
    return get_padding_length(BASE_LENGTH + data.length());
}

// ----------------------------------------------------------------------------
void vdb::environment_record_t::read(byte_stream &stream)
{
    uint16_t
        record_length = 0;

    clear();

    stream.read(type);
    stream.read(record_length);
    stream.read(index);
    stream.read(padding);

    LOG_EXTRA_VERBOSE(
        "Environment record length %d bits = (%d bytes)",
        record_length,
        (record_length / 8));

    // Convert value in bits to value in bytes
    //
    data.data_length = ((record_length / 8) - 8);
    data.read(stream);
}

// ----------------------------------------------------------------------------
void vdb::environment_record_t::write(byte_stream &stream) const
{
    const uint16_t
        record_length = (length() * 8);

    stream.write(type);
    stream.write(record_length);
    stream.write(index);
    stream.write(padding);

    data.write(stream);
}

// ----------------------------------------------------------------------------
vdb::linear_segment_t::linear_segment_t(void) :
    number(0),
    modifications(0),
    generic_appearance(0),
    specific_appearance(0),
    segment_length(0),
    segment_width(0),
    segment_height(0),
    segment_depth(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::linear_segment_t::linear_segment_t(const linear_segment_t &copy) :
    number(copy.number),
    modifications(copy.modifications),
    generic_appearance(copy.generic_appearance),
    specific_appearance(copy.specific_appearance),
    location(copy.location),
    orientation(copy.orientation),
    segment_length(copy.segment_length),
    segment_width(copy.segment_width),
    segment_height(copy.segment_height),
    segment_depth(copy.segment_depth),
    padding(copy.padding)
{

}

// ----------------------------------------------------------------------------
vdb::linear_segment_t::~linear_segment_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::linear_segment_t::clear(void)
{
    number = 0;
    modifications = 0;
    generic_appearance = 0;
    specific_appearance = 0;
    location.clear();
    orientation.clear();
    segment_length = 0;
    segment_width = 0;
    segment_height = 0;
    segment_depth = 0;
    padding = 0;
}

// ----------------------------------------------------------------------------
void vdb::linear_segment_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "number " << (int)number << std::endl
           << prefix << "modifications " << (int)modifications << std::endl
           << prefix << "specific_appearance "
           << binary::str(specific_appearance, true) << std::endl
           << prefix << "generic_appearance "
           << binary::str(generic_appearance, true) << std::endl
           << prefix << "location.gcc "
           << location.str() << std::endl
           << prefix << "location.gdc "
           << location.geodetic_str() << std::endl
           << prefix << "orientation " << orientation.str() << std::endl
           << prefix << "segment_length " << (int)(segment_length) << std::endl
           << prefix << "segment_width " << (int)(segment_width) << std::endl
           << prefix << "segment_height " << (int)(segment_height) << std::endl
           << prefix << "segment_depth " << (int)(segment_depth) << std::endl
           << prefix << "padding " << (int)modifications << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::linear_segment_t::read(byte_stream &stream)
{
    stream.read(number);
    stream.read(modifications);
    stream.read(generic_appearance);
    stream.read(specific_appearance);
    stream.read(location);
    stream.read(orientation);
    stream.read(segment_length);
    stream.read(segment_width);
    stream.read(segment_height);
    stream.read(segment_depth);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdb::linear_segment_t::write(byte_stream &stream) const
{
    stream.write(number);
    stream.write(modifications);
    stream.write(generic_appearance);
    stream.write(specific_appearance);
    stream.write(location);
    stream.write(orientation);
    stream.write(segment_length);
    stream.write(segment_width);
    stream.write(segment_height);
    stream.write(segment_depth);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
vdb::environmental_process_pdu_t::environmental_process_pdu_t(void) :
    model_type(0),
    status(0),
    sequence_number(0)
{

}

// ----------------------------------------------------------------------------
vdb::environmental_process_pdu_t::~environmental_process_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::environmental_process_pdu_t::get_initator(void) const
{
    return &process_id;
}

// ----------------------------------------------------------------------------
bool vdb::environmental_process_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (process_id.matches(*id_ptr)) : false);
}

// ----------------------------------------------------------------------------
void vdb::environmental_process_pdu_t::clear(void)
{
    process_id.clear();
    environment_type.clear();
    model_type = 0;
    status = 0;
    sequence_number = 0;
    records.clear();
}

// ----------------------------------------------------------------------------
void vdb::environmental_process_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "process_id "
           << process_id << std::endl
           << prefix << "environment_type "
           << environment_type << std::endl
           << prefix << "model_type "
           << (int)model_type << std::endl
           << prefix << "status "
           << enumerations::print(ENUM_ENV_PROC_STATUS, status) << std::endl
           << prefix << "sequence_number "
           << (int)sequence_number << std::endl
           << prefix << "records "
           << records.size() << std::endl;

    for(uint32_t i = 0; i < records.size(); ++i)
    {
        records[i].print(
            (prefix + "record[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::environmental_process_pdu_t::print_summary(
    std::ostream &stream) const
{
    stream << "  " << (int)sequence_number << "  " << process_id << "  "
           << environment_type << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::environmental_process_pdu_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    for(uint32_t i = 0; i < records.size(); ++i)
    {
        record_length += records[i].length();
    }

    return record_length;
}

// ----------------------------------------------------------------------------
void vdb::environmental_process_pdu_t::read(byte_stream &stream)
{
    uint16_t
        record_count;

    clear();

    LOG_EXTRA_VERBOSE(
        "Restoring Environmental Process PDU at index %d...",
        stream.get_index());

    stream.read(process_id);
    stream.read(environment_type);
    stream.read(model_type);
    stream.read(status);
    stream.read(record_count);
    stream.read(sequence_number);

    for(uint16_t i = 0; i < record_count; ++i)
    {
        environment_record_t
            record;

        LOG_EXTRA_VERBOSE(
            "Restoring %d/%d environment records at index %d...",
            (i + 1),
            record_count,
            stream.get_index());

        record.read(stream);

        records.push_back(record);
    }
}

// ----------------------------------------------------------------------------
void vdb::environmental_process_pdu_t::write(byte_stream &stream) const
{
    // TODO: vdb::environmental_process_pdu_t::write
}

// ----------------------------------------------------------------------------
vdb::point_object_state_pdu_t::point_object_state_pdu_t(void) :
    update(0),
    force_id(0),
    modifications(0),
    specific_appearance(0),
    generic_appearance(0),
    padding16(0),
    padding32(0)
{

}

// ----------------------------------------------------------------------------
vdb::point_object_state_pdu_t::~point_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::point_object_state_pdu_t::get_initator(void) const
{
    return &object_id;
}

// ----------------------------------------------------------------------------
bool vdb::point_object_state_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (object_id.matches(*id_ptr)) : false);
}

// ----------------------------------------------------------------------------
void vdb::point_object_state_pdu_t::clear(void)
{
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    modifications = 0;
    object_type.clear();
    location.clear();
    orientation.clear();
    specific_appearance = 0;
    generic_appearance = 0;
    padding16 = 0;
    requestor_id.clear();
    receiver_id.clear();
    padding32 = 0;
}

// ----------------------------------------------------------------------------
void vdb::point_object_state_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    const domain_e
        domain = (domain_e)object_type.domain;
    const object_kind_e
        kind = (object_kind_e)object_type.kind;
    const object_geometry_e
        geometry = object_type.get_geometry();

    stream << prefix << "object_id "
           << object_id << std::endl
           << prefix << "referenced_object_id "
           << referenced_object_id << std::endl
           << prefix << "force_id "
           << enumerations::print(ENUM_FORCE_ID, force_id) << std::endl
           << prefix << "modifications "
           << (int)modifications << std::endl
           << prefix << "requestor_id "
           << requestor_id << std::endl
           << prefix << "receiver_id "
           << receiver_id << std::endl
           << prefix << "object_type "
           << object_type << std::endl
           << prefix << "object_type.domain "
           << enumerations::print(ENUM_DOMAIN, domain) << std::endl
           << prefix << "object_type.kind "
           << enumerations::print(ENUM_OBJECT_KIND, kind) << std::endl
           << prefix << "object_type.geometry "
           << enumerations::print(ENUM_OBJECT_GEOMETRY, geometry) << std::endl
           << prefix << "object_type.description '"
           << object_type.get_description() << "'" << std::endl
           << prefix << "location.gcc "
           << location.str() << std::endl
           << prefix << "location.gdc "
           << location.geodetic_str() << std::endl
           << prefix << "orientation "
           << orientation << std::endl
           << prefix << "specific_appearance "
           << binary::str(specific_appearance, true) << std::endl;

    object_state_appearance::print_specific(
        (prefix + "specific_appearance."),
        object_type,
        specific_appearance,
        stream);

    stream << prefix << "generic_appearance "
           << binary::str(generic_appearance, true) << std::endl;

    object_state_appearance::print_generic(
        (prefix + "generic_appearance."),
        generic_appearance,
        stream);

    stream << POINT_OBJECT_STATE << "padding16 "
           << hexadecimal::str(padding16, true) << std::endl
           << POINT_OBJECT_STATE << "padding32 "
           << hexadecimal::str(padding32, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::point_object_state_pdu_t::print_summary(
    std::ostream &stream) const
{
    stream << "  " << object_id << "  " << (force_id_e)force_id
           << std::endl
           << "  " << object_type << "  " << object_type.get_description()
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::point_object_state_pdu_t::read(byte_stream &stream)
{
    clear();

    stream.read(object_id);
    stream.read(referenced_object_id);
    stream.read(update);
    stream.read(force_id);
    stream.read(modifications);
    stream.read(object_type);
    stream.read(location);
    stream.read(orientation);
    stream.read(specific_appearance);
    stream.read(generic_appearance);
    stream.read(padding16);
    stream.read(requestor_id);
    stream.read(receiver_id);
    stream.read(padding32);
}

// ----------------------------------------------------------------------------
void vdb::point_object_state_pdu_t::write(byte_stream &stream) const
{
    stream.write(object_id);
    stream.write(referenced_object_id);
    stream.write(update);
    stream.write(force_id);
    stream.write(modifications);
    stream.write(object_type);
    stream.write(location);
    stream.write(orientation);
    stream.write(specific_appearance);
    stream.write(generic_appearance);
    stream.write(padding16);
    stream.write(requestor_id);
    stream.write(receiver_id);
    stream.write(padding32);
}

// ----------------------------------------------------------------------------
vdb::linear_object_state_pdu_t::linear_object_state_pdu_t(void) :
    update(0),
    force_id(0)
{

}

// ----------------------------------------------------------------------------
vdb::linear_object_state_pdu_t::~linear_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::linear_object_state_pdu_t::get_initator(void) const
{
    return &object_id;
}

// ----------------------------------------------------------------------------
bool vdb::linear_object_state_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (object_id.matches(*id_ptr)) : false);
}

// ----------------------------------------------------------------------------
void vdb::linear_object_state_pdu_t::clear(void)
{
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    requestor_id.clear();
    receiver_id.clear();
    object_type.clear();
    segments.clear();
}

// ----------------------------------------------------------------------------
void vdb::linear_object_state_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    const domain_e
        domain = (domain_e)object_type.domain;
    const object_kind_e
        kind = (object_kind_e)object_type.kind;
    const object_geometry_e
        geometry = object_type.get_geometry();

    stream << prefix << "object_id "
           << object_id << std::endl
           << prefix << "referenced_object_id "
           << referenced_object_id << std::endl
           << prefix << "force_id "
           << enumerations::print(ENUM_FORCE_ID, force_id) << std::endl
           << prefix << "update "
           << (int)update << std::endl
           << prefix << "requestor_id "
           << requestor_id << std::endl
           << prefix << "receiver_id "
           << receiver_id << std::endl
           << prefix << "object_type "
           << object_type << std::endl
           << prefix << "object_type.domain "
           << enumerations::print(ENUM_DOMAIN, domain) << std::endl
           << prefix << "object_type.kind "
           << enumerations::print(ENUM_OBJECT_KIND, kind) << std::endl
           << prefix << "object_type.geometry "
           << enumerations::print(ENUM_OBJECT_GEOMETRY, geometry) << std::endl
           << prefix << "object_type.description '"
           << object_type.get_description() << "'" << std::endl
           << prefix << "segments "
           << (int)segments.size() << std::endl;

    for(uint32_t i = 0; i < segments.size(); ++i)
    {
        segments[i].print(
            (prefix + "segment[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::linear_object_state_pdu_t::print_summary(
    std::ostream &stream) const
{
    stream << "  " << object_id << "  " << (force_id_e)force_id
           << std::endl
           << "  " << object_type << "  " << object_type.get_description()
           << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::linear_object_state_pdu_t::length(void) const
{
    return (BASE_LENGTH + (segments.size() * linear_segment_t::LENGTH));
}

// ----------------------------------------------------------------------------
void vdb::linear_object_state_pdu_t::read(byte_stream &stream)
{
    uint8_t
        segment_count = 0;

    stream.read(object_id);
    stream.read(referenced_object_id);
    stream.read(update);
    stream.read(force_id);
    stream.read(segment_count);
    stream.read(requestor_id);
    stream.read(receiver_id);
    stream.read(object_type);

    for(uint32_t i = 0; i < segment_count; ++i)
    {
        LOG_EXTRA_VERBOSE(
            "Reading linear segment %d/%d...",
            (i + 1),
            segment_count);

        linear_segment_t
            segment;

        segment.read(stream);

        segments.push_back(segment);
    }
}

// ----------------------------------------------------------------------------
void vdb::linear_object_state_pdu_t::write(byte_stream &stream) const
{
    const uint8_t
        segment_count = segments.size();

    stream.write(object_id);
    stream.write(referenced_object_id);
    stream.write(update);
    stream.write(force_id);
    stream.write(segment_count);
    stream.write(requestor_id);
    stream.write(receiver_id);
    stream.write(object_type);

    for(uint32_t i = 0; i < segments.size(); ++i)
    {
        segments[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
vdb::areal_object_state_pdu_t::areal_object_state_pdu_t(void) :
    update(0),
    force_id(0),
    modifications(0),
    specific_appearance(0),
    generic_appearance(0)
{

}

// ----------------------------------------------------------------------------
vdb::areal_object_state_pdu_t::~areal_object_state_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::areal_object_state_pdu_t::get_initator(void) const
{
    return &object_id;
}

// ----------------------------------------------------------------------------
bool vdb::areal_object_state_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (object_id.matches(*id_ptr)) : false);
}

// ----------------------------------------------------------------------------
void vdb::areal_object_state_pdu_t::clear(void)
{
    object_id.clear();
    referenced_object_id.clear();
    update = 0;
    force_id = 0;
    modifications = 0;
    object_type.clear();
    specific_appearance = 0;
    generic_appearance = 0;
    requestor_id.clear();
    receiver_id.clear();
    points.clear();
}

// ----------------------------------------------------------------------------
void vdb::areal_object_state_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    const domain_e
        domain = (domain_e)object_type.domain;
    const object_kind_e
        kind = (object_kind_e)object_type.kind;
    const object_geometry_e
        geometry = object_type.get_geometry();

    stream << prefix << "object_id "
           << object_id << std::endl
           << prefix << "referenced_object_id "
           << referenced_object_id << std::endl
           << prefix << "force_id "
           << enumerations::print(ENUM_FORCE_ID, force_id) << std::endl
           << prefix << "update "
           << (int)update << std::endl
           << prefix << "modifications "
           << (int)modifications << std::endl
           << prefix << "requestor_id "
           << requestor_id << std::endl
           << prefix << "receiver_id "
           << receiver_id << std::endl
           << prefix << "object_type "
           << object_type << std::endl
           << prefix << "object_type.domain "
           << enumerations::print(ENUM_DOMAIN, domain) << std::endl
           << prefix << "object_type.kind "
           << enumerations::print(ENUM_OBJECT_KIND, kind) << std::endl
           << prefix << "object_type.geometry "
           << enumerations::print(ENUM_OBJECT_GEOMETRY, geometry) << std::endl
           << prefix << "object_type.description '"
           << object_type.get_description() << "'" << std::endl
           << prefix << "specific_appearance "
           << binary::str(specific_appearance, true) << std::endl;

    object_state_appearance::print_specific(
        (prefix + "specific_appearance."),
        object_type,
        specific_appearance,
        stream);

    stream << prefix << "generic_appearance "
           << binary::str(generic_appearance, true) << std::endl;

    object_state_appearance::print_generic(
        (prefix + "generic_appearance."),
        generic_appearance,
        stream);

    stream << prefix << "points "
           << (int)points.size() << std::endl;

    for(uint32_t i = 0; i < points.size(); ++i)
    {
        stream << prefix << "points[" << i << "].gcc "
               << points[i] << std::endl
               << prefix << "points[" << i << "].gdc "
               << points[i].geodetic_str() << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::areal_object_state_pdu_t::print_summary(
    std::ostream &stream) const
{
    stream << "  " << object_id << "  " << (force_id_e)force_id
           << std::endl
           << "  " << object_type << "  " << object_type.get_description()
           << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::areal_object_state_pdu_t::length(void) const
{
    return (BASE_LENGTH + (points.size() * location24_t::LENGTH));
}

// ----------------------------------------------------------------------------
void vdb::areal_object_state_pdu_t::read(byte_stream &stream)
{
    uint16_t
        point_count = 0;

    stream.read(object_id);
    stream.read(referenced_object_id);
    stream.read(update);
    stream.read(force_id);
    stream.read(modifications);
    stream.read(object_type);
    stream.read(specific_appearance);
    stream.read(generic_appearance);
    stream.read(point_count);
    stream.read(requestor_id);
    stream.read(receiver_id);

    for(uint16_t i = 0; i < point_count; ++i)
    {
        location24_t
            point;

        point.read(stream);

        points.push_back(point);
    }
}

// ----------------------------------------------------------------------------
void vdb::areal_object_state_pdu_t::write(byte_stream &stream) const
{
    const uint16_t
        point_count = points.size();

    stream.write(object_id);
    stream.write(referenced_object_id);
    stream.write(update);
    stream.write(force_id);
    stream.write(modifications);
    stream.write(object_type);
    stream.write(specific_appearance);
    stream.write(generic_appearance);
    stream.write(point_count);
    stream.write(requestor_id);
    stream.write(receiver_id);

    for(uint16_t i = 0; i < point_count; ++i)
    {
        points[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::object_state_appearance::print_generic(
    const std::string &prefix,
    uint16_t value,
    std::ostream &stream)
{
    const uint32_t
        completion = (value & 0x00FF), // bits 7-0
        damage = binary::get_2_bits_16(9, value),
        predistributed = binary::get_1_bit_16(10, value),
        deactivated = binary::get_1_bit_16(11, value),
        smoking = binary::get_1_bit_16(12, value),
        flaming = binary::get_1_bit_16(13, value);

    stream << prefix << "completion(%) "
           << completion << std::endl
           << prefix << "damage "
           << enumerations::print(ENUM_OBJECT_DAMAGE, damage) << std::endl
           << prefix << "predistributed "
           << enumerations::print(ENUM_YES_NO, predistributed) << std::endl
           << prefix << "deactivated "
           << enumerations::print(ENUM_YES_NO, deactivated) << std::endl
           << prefix << "smoking "
           << enumerations::print(ENUM_YES_NO, smoking) << std::endl
           << prefix << "flaming "
           << enumerations::print(ENUM_YES_NO, flaming) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::object_state_appearance::print_specific(
    const std::string &prefix,
    const object_type_t &type,
    uint32_t value,
    std::ostream &stream)
{
    // TODO: vdb::object_state_appearance::print_specific()
}
