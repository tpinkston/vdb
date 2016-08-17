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
#include "vdb_services.h"
#include "vdb_string.h"
#include "vdb_warfare_pdus.h"

// ----------------------------------------------------------------------------
vdb::burst_descriptor_t::burst_descriptor_t() :
    warhead(0),
    fuse(0),
    quantity(0),
    rate(0)
{

}

// ----------------------------------------------------------------------------
vdb::burst_descriptor_t::burst_descriptor_t(
    const burst_descriptor_t &copy
) :
    munition(copy.munition),
    warhead(copy.warhead),
    fuse(copy.fuse),
    quantity(copy.quantity),
    rate(copy.rate)
{

}

// ----------------------------------------------------------------------------
vdb::burst_descriptor_t::~burst_descriptor_t()
{

}

// ----------------------------------------------------------------------------
void vdb::burst_descriptor_t::clear(void)
{
    munition.clear();
    warhead = 0;
    fuse = 0;
    quantity = 0;
    rate = 0;
}

// ----------------------------------------------------------------------------
void vdb::burst_descriptor_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "burst_descriptor.munition "
           << munition << std::endl
           << prefix << "burst_descriptor.munition.description "
           << munition.get_description() << std::endl
           << prefix << "burst_descriptor.warhead "
           << enumerations::print(ENUM_WARHEAD_BURST_DESC, warhead) << std::endl
           << prefix << "burst_descriptor.fuse "
           << enumerations::print(ENUM_FUSE_BURST_DESC, fuse) << std::endl
           << prefix << "burst_descriptor.quantity "
           << (int)quantity << std::endl
           << prefix << "burst_descriptor.warhead "
           << (int)rate << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::burst_descriptor_t::read(byte_stream &stream)
{
    stream.read(munition);
    stream.read(warhead);
    stream.read(fuse);
    stream.read(quantity);
    stream.read(rate);
}

// ----------------------------------------------------------------------------
void vdb::burst_descriptor_t::write(byte_stream &stream) const
{
    stream.write(munition);
    stream.write(warhead);
    stream.write(fuse);
    stream.write(quantity);
    stream.write(rate);
}

// ----------------------------------------------------------------------------
vdb::fire_pdu_t::fire_pdu_t(void) : fire_mission_index(0), range(0.0f)
{

}

// ----------------------------------------------------------------------------
vdb::fire_pdu_t::~fire_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::fire_pdu_t::get_initator(void) const
{
    return &shooter;
}

// ----------------------------------------------------------------------------
bool vdb::fire_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (shooter == *id_ptr) or (target == *id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::fire_pdu_t::clear(void)
{
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
void vdb::fire_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "shooter ";

    shooter.print_full(stream);

    stream << std::endl
           << prefix << "target ";

    target.print_full(stream);

    stream << std::endl
           << prefix << "munition "
           << munition << std::endl
           << prefix << "event " << event << std::endl
           << prefix << "fire_mission_index "
           << fire_mission_index << std::endl
           << prefix << "world_location.gcc "
           << world_location << std::endl
           << prefix << "world_location.gdc "
           << world_location.geodetic_str() << std::endl;

    burst_descriptor.print(prefix, stream);

    stream << prefix << "velocity "
           << velocity << std::endl
           << prefix << "velocity(m/s) "
           << float_to_string(velocity.get_length()) << std::endl
           << prefix << "range(m) "
           << float_to_string(range) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::fire_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << event << ":  " << shooter << "->" << target << std::endl
           << "  " << burst_descriptor.munition
           << " '" << burst_descriptor.munition.get_description() << "'"
           << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::fire_pdu_t::read(byte_stream &stream)
{
    stream.read(shooter);
    stream.read(target);
    stream.read(munition);
    stream.read(event);
    stream.read(fire_mission_index);
    stream.read(world_location);
    stream.read(burst_descriptor);
    stream.read(velocity);
    stream.read(range);
}

// ----------------------------------------------------------------------------
void vdb::fire_pdu_t::write(byte_stream &stream) const
{
    stream.write(shooter);
    stream.write(target);
    stream.write(munition);
    stream.write(event);
    stream.write(fire_mission_index);
    stream.write(world_location);
    stream.write(burst_descriptor);
    stream.write(velocity);
    stream.write(range);
}

// ----------------------------------------------------------------------------
vdb::detonation_pdu_t::detonation_pdu_t(void) :
    detonation_result(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::detonation_pdu_t::~detonation_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::detonation_pdu_t::get_initator(void) const
{
    return &shooter;
}

// ----------------------------------------------------------------------------
bool vdb::detonation_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (shooter == *id_ptr) or (target == *id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::detonation_pdu_t::clear(void)
{
    shooter.clear();
    target.clear();
    munition.clear();
    event.clear();
    velocity.clear();
    world_location.clear();
    burst_descriptor.clear();
    entity_location.clear();
    detonation_result = 0;
    padding = 0;
    variable_parameters.clear();
}

// ----------------------------------------------------------------------------
void vdb::detonation_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "shooter ";

    shooter.print_full(stream);

    stream << std::endl << prefix << "target ";

    target.print_full(stream);

    stream << std::endl << prefix << "munition "
           << munition << std::endl
           << prefix << "event "
           << event << std::endl
           << prefix << "velocity "
           << velocity << std::endl
           << prefix << "velocity(m/s) "
           << float_to_string(velocity.get_length()) << std::endl
           << prefix << "world_location.gcc "
           << world_location << std::endl
           << prefix << "world_location.gdc "
           << world_location.geodetic_str() << std::endl;

    burst_descriptor.print((prefix + ""), stream);

    stream << prefix << "entity_location "
           << entity_location << std::endl
           << prefix << "detonation_result "
           << enumerations::print(ENUM_DETONATION_RESULT, detonation_result)
           << std::endl
           << prefix << "padding "
           << (int)padding << std::endl
           << prefix << "variable_parameters.count "
           << (int)variable_parameters.records.size() << std::endl;

    if (variable_parameters.records.size()> 0)
    {
        variable_parameters.print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::detonation_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << event << ":  " << shooter << "->" << target << std::endl
           << "  " << burst_descriptor.munition
           << " '" << burst_descriptor.munition.get_description() << "'"
           << std::endl
           << "  " << (detonation_result_e)detonation_result << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::detonation_pdu_t::length(void) const
{
    return (BASE_LENGTH + variable_parameters.length());
}

// ----------------------------------------------------------------------------
void vdb::detonation_pdu_t::read(byte_stream &stream)
{
    uint8_t
        record_count = 0;

    stream.read(shooter);
    stream.read(target);
    stream.read(munition);
    stream.read(event);
    stream.read(velocity);
    stream.read(world_location);
    stream.read(burst_descriptor);
    stream.read(entity_location);
    stream.read(detonation_result);
    stream.read(record_count);
    stream.read(padding);

    variable_parameters.read(stream, record_count);
}

// ----------------------------------------------------------------------------
void vdb::detonation_pdu_t::write(byte_stream &stream) const
{
    stream.write(shooter);
    stream.write(target);
    stream.write(munition);
    stream.write(event);
    stream.write(velocity);
    stream.write(world_location);
    stream.write(burst_descriptor);
    stream.write(entity_location);
    stream.write(detonation_result);
    stream.write((uint8_t)variable_parameters.records.size());
    stream.write(padding);
    stream.write(variable_parameters);
}

// ----------------------------------------------------------------------------
vdb::entity_damage_status_pdu_t::entity_damage_status_pdu_t(void) : padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_damage_status_pdu_t::~entity_damage_status_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::entity_damage_status_pdu_t::get_initator(void) const
{
    return &entity;
}

// ----------------------------------------------------------------------------
bool vdb::entity_damage_status_pdu_t::contains(const id_t *id_ptr) const
{
    if (id_ptr)
    {
        return (entity == *id_ptr);
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_damage_status_pdu_t::clear(void)
{
    entity.clear();
    padding = 0;
    description_records.clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_damage_status_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity ";

    entity.print_full(stream);

    stream << std::endl
           << prefix << "padding "
           << hexadecimal::str(padding, true) << std::endl
           << prefix << "description_records "
           << description_records.records.size() << std::endl;

    description_records.print(prefix, stream);
}

// ----------------------------------------------------------------------------
void vdb::entity_damage_status_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << entity << std::endl;

    description_records.print_summary(stream);
}

// ----------------------------------------------------------------------------
uint32_t vdb::entity_damage_status_pdu_t::length(void) const
{
    return (BASE_LENGTH + description_records.length());
}

// ----------------------------------------------------------------------------
void vdb::entity_damage_status_pdu_t::read(byte_stream &stream)
{
    stream.read(entity);
    stream.read(padding);
    stream.read(description_records);
}

// ----------------------------------------------------------------------------
void vdb::entity_damage_status_pdu_t::write(byte_stream &stream) const
{
    stream.write(entity);
    stream.write(padding);
    stream.write(description_records);
}
