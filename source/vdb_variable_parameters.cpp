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
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_string.h"
#include "vdb_variable_parameters.h"

vdb::ent_kind_e
    vdb::variable_parameter_records_t::usable_kind = ENT_KIND_OTHER;
vdb::domain_e
    vdb::variable_parameter_records_t::usable_domain = DOMAIN_OTHER;

// ----------------------------------------------------------------------------
vdb::vpr_t::vpr_t(void) : type(0), content_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::vpr_t::vpr_t(const vpr_t &copy) : type(copy.type), content_ptr(0)
{
    #define COPY_VPR(T, V) new T(*static_cast<const T*>(V))

    if (copy.content_ptr)
    {
        switch((vp_record_type_e)type)
        {
            case VP_RECORD_TYPE_ARTICULATED_PART:
                content_ptr = COPY_VPR(
                    articulated_part_t,
                    copy.content_ptr);
                break;
            case VP_RECORD_TYPE_ENTITY_ASSOC:
                content_ptr = COPY_VPR(
                    entity_association_t,
                    copy.content_ptr);
                break;
            case VP_RECORD_TYPE_ENTITY_OFFSET:
                content_ptr = COPY_VPR(
                    entity_offset_t,
                    copy.content_ptr);
                break;
            case VP_RECORD_TYPE_EXT_PLATFORM_APP:
                content_ptr = COPY_VPR(
                    ext_platform_appearance_t,
                    copy.content_ptr);
                break;
            case VP_RECORD_TYPE_ATTACHED_PART:
            case VP_RECORD_TYPE_SEPARATION:
            case VP_RECORD_TYPE_ENTITY_TYPE:
            case VP_RECORD_TYPE_EXT_LIFEFORM_APP:
            case VP_RECORD_TYPE_HIFI_LIGHTS:
            case VP_RECORD_TYPE_CHEVRON_MARKING:
            case VP_RECORD_TYPE_HIFI_THERMAL:
            case VP_RECORD_TYPE_DEAD_RECKONING:
            case VP_RECORD_TYPE_ARMY_TASK_ORG:
            case VP_RECORD_TYPE_HEAD_GAZING_WPN_AIM:
            case VP_RECORD_TYPE_LIFEFORM_ACT_SEQ:
            case VP_RECORD_TYPE_LEGACY_EXT_LIFEFORM_APP:
            case VP_RECORD_TYPE_EXT_CULT_FEAT_APP:
            case VP_RECORD_TYPE_EXT_SUPPLY_APP:
            default:
                content_ptr = COPY_VPR(
                    vpr_default_content_t,
                    copy.content_ptr);
                break;
        }
    }

    #undef COPY_VPR
}

// ----------------------------------------------------------------------------
vdb::vpr_t::~vpr_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::vpr_t::clear(void)
{
    type = 0;

    if (content_ptr)
    {
        delete content_ptr;
        content_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::vpr_t::print(const std::string &prefix, std::ostream &stream) const
{
    stream << prefix << "type "
           << enumerations::print(ENUM_VP_RECORD_TYPE, type) << std::endl;

    if (content_ptr)
    {
        content_ptr->print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::vpr_t::read(byte_stream &stream)
{
    clear();

    if (stream.get_length_remaining() < LENGTH)
    {
        LOG_ERROR(
            "Cannot create VP record -  %d bytes remaining in buffer",
            stream.get_length_remaining());
    }
    else
    {
        stream.read(type);

        switch((vp_record_type_e)type)
        {
            case VP_RECORD_TYPE_ARTICULATED_PART:
                content_ptr = new articulated_part_t();
                break;
            case VP_RECORD_TYPE_ENTITY_ASSOC:
                content_ptr = new entity_association_t();
                break;
            case VP_RECORD_TYPE_ENTITY_OFFSET:
                content_ptr = new entity_offset_t();
                break;
            case VP_RECORD_TYPE_EXT_PLATFORM_APP:
                content_ptr = new ext_platform_appearance_t();
                break;
            case VP_RECORD_TYPE_EXT_CULT_FEAT_APP:
                content_ptr = new ext_cultural_feature_appearance_t();
                break;
            case VP_RECORD_TYPE_EXT_SUPPLY_APP:
                content_ptr = new ext_supply_appearance_t();
                break;
            default:
                content_ptr = new vpr_default_content_t();
        }

        stream.read(*content_ptr);
    }
}

// ----------------------------------------------------------------------------
void vdb::vpr_t::write(byte_stream &stream) const
{
    if (content_ptr)
    {
        stream.write(type);
        stream.write(*content_ptr);
    }
    else
    {
        LOG_ERROR("No VPR content to save for record type %d", type);
    }
}

// ----------------------------------------------------------------------------
vdb::vpr_default_content_t::vpr_default_content_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
vdb::vpr_default_content_t::vpr_default_content_t(
    const vpr_default_content_t &copy)
{
    std::memcpy(data, copy.data, LENGTH);
}

// ----------------------------------------------------------------------------
vdb::vpr_default_content_t::~vpr_default_content_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::vpr_default_content_t::clear(void)
{
    std::memset(data, 0, LENGTH);
}

// ----------------------------------------------------------------------------
void vdb::vpr_default_content_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    hexadecimal::stream(
        prefix,
        data,
        LENGTH,
        8,
        stream);
}

// ----------------------------------------------------------------------------
void vdb::vpr_default_content_t::read(byte_stream &stream)
{
    stream.read(data, LENGTH);
}

// ----------------------------------------------------------------------------
void vdb::vpr_default_content_t::write(byte_stream &stream) const
{
    stream.write(data, LENGTH);
}

// ----------------------------------------------------------------------------
vdb::articulated_part_t::articulated_part_t(void) :
    change_indicator(0),
    part_id(0),
    part_type(0),
    part_metric(0),
    value(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::articulated_part_t::articulated_part_t(
    const articulated_part_t &copy
) :
    change_indicator(copy.change_indicator),
    part_id(copy.part_id),
    part_type(copy.part_type),
    part_metric(copy.part_metric),
    value(copy.value),
    padding(copy.padding)
{

}

// ----------------------------------------------------------------------------
vdb::articulated_part_t::~articulated_part_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::articulated_part_t::clear(void)
{
    change_indicator = 0;
    part_id = 0;
    part_type = 0;
    part_metric = 0;
    value = 0;
}

// ----------------------------------------------------------------------------
void vdb::articulated_part_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "articulated_part.change_indicator "
           << (int)change_indicator << std::endl
           << prefix << "articulated_part.part_id "
           << part_id << std::endl
           << prefix << "articulated_part.part_type "
           << (articulated_parts_e)part_type
           << " [" << (int)part_type << "]" << std::endl
           << prefix << "articulated_part.part_metric "
           << (articulated_parts_metric_e)part_metric
           << " [" << (int)part_metric << "]" << std::endl
           << prefix << "articulated_part.value "
           << float_to_string(value, 1, 6) << std::endl
           << prefix << "articulated_part.padding "
           << padding << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::articulated_part_t::read(byte_stream &stream)
{
    uint32_t
        part = 0;

    stream.read(change_indicator);
    stream.read(part_id);
    stream.read(part);
    stream.read(value);
    stream.read(padding);

    part_type = (part & ~MASK_5_BITS);
    part_metric = (part & MASK_5_BITS);
}

// ----------------------------------------------------------------------------
void vdb::articulated_part_t::write(byte_stream &stream) const
{
    stream.write(change_indicator);
    stream.write(part_id);
    stream.write((uint32_t)(part_type & part_metric));
    stream.write(value);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
vdb::entity_association_t::entity_association_t(void) :
    change_indicator(0),
    association_status(0),
    association_type(0),
    station(0),
    connection(0),
    group_membership(0),
    group_number(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_association_t::entity_association_t(
    const entity_association_t &copy
) :
    change_indicator(copy.change_indicator),
    association_status(copy.association_status),
    association_type(copy.association_type),
    entity_id(copy.entity_id),
    station(copy.station),
    connection(copy.connection),
    group_membership(copy.group_membership),
    group_number(copy.group_number)
{

}

// ----------------------------------------------------------------------------
vdb::entity_association_t::~entity_association_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_association_t::clear(void)
{
    change_indicator = 0;
    association_status = 0;
    association_type = 0;
    entity_id.clear();
    station = 0;
    connection = 0;
    group_membership = 0;
    group_number = 0;
}

// ----------------------------------------------------------------------------
void vdb::entity_association_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity_association.change_indicator "
           << (int)change_indicator << std::endl
           << prefix << "entity_association.status "
           << (ent_assoc_status_e)association_status
           << " [" << (int)association_status << "]" << std::endl
           << prefix << "entity_association.type "
           << (phys_assoc_type_e)association_type
           << " [" << (int)association_type << "]" << std::endl
           << prefix << "entity_association.entity_id ";

    entity_id.print_full(stream);

    stream << std::endl
           << prefix << "entity_association.station "
           << station << std::endl
           << prefix << "entity_association.connection "
           << (phys_conn_type_e)connection
           << " [" << (int)connection << "]" << std::endl
           << prefix << "entity_association.group_membership "
           << (grp_mem_type_e)group_membership
           << " [" << (int)group_membership << "]" << std::endl
           << prefix << "entity_association.group_number "
           << group_number << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::entity_association_t::read(byte_stream &stream)
{
    stream.read(change_indicator);
    stream.read(association_status);
    stream.read(association_type);
    stream.read(entity_id);
    stream.read(station);
    stream.read(connection);
    stream.read(group_membership);
    stream.read(group_number);
}

// ----------------------------------------------------------------------------
void vdb::entity_association_t::write(byte_stream &stream) const
{
    stream.write(change_indicator);
    stream.write(association_status);
    stream.write(association_type);
    stream.write(entity_id);
    stream.write(station);
    stream.write(connection);
    stream.write(group_membership);
    stream.write(group_number);
}

// ----------------------------------------------------------------------------
vdb::entity_offset_t::entity_offset_t(void) : offset_type(0), padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_offset_t::entity_offset_t(
    const entity_offset_t &copy
) :
    offset_type(copy.offset_type),
    padding(copy.padding),
    position(copy.position)
{

}

// ----------------------------------------------------------------------------
vdb::entity_offset_t::~entity_offset_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_offset_t::clear(void)
{
    offset_type = 0;
    padding = 0;
    position.clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_offset_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity_offset.offset_type "
           << (offset_type_e)offset_type
           << " [" << (int)offset_type << "]" << std::endl
           << prefix << "entity_offset.position " << position << std::endl
           << prefix << "entity_offset.padding " << padding << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::entity_offset_t::read(byte_stream &stream)
{
    stream.read(offset_type);
    stream.read(padding);
    stream.read(position);
}

// ----------------------------------------------------------------------------
void vdb::entity_offset_t::write(byte_stream &stream) const
{
    stream.write(offset_type);
    stream.write(padding);
    stream.write(position);
}

// ----------------------------------------------------------------------------
vdb::ext_appearance_t::ext_appearance_t(void) :
    status(0),
    equipment(0)
{

}

// ----------------------------------------------------------------------------
vdb::ext_appearance_t::ext_appearance_t(const ext_appearance_t &copy
) :
    status(copy.status),
    equipment(copy.equipment)
{

}

// ----------------------------------------------------------------------------
vdb::ext_appearance_t::~ext_appearance_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::ext_appearance_t::clear(void)
{
    status = 0;
    equipment = 0;
}

// ----------------------------------------------------------------------------
void vdb::ext_appearance_t::read(byte_stream &stream)
{
    stream.skip(12);

    stream.read(status);
    stream.read(equipment);
}

// ----------------------------------------------------------------------------
void vdb::ext_appearance_t::write(byte_stream &stream) const
{
    stream.skip(12);

    stream.write(status);
    stream.write(equipment);
}

// ----------------------------------------------------------------------------
vdb::ext_platform_appearance_t::ext_platform_appearance_t(void) :
    paint_scheme(0),
    decal_scheme(0),
    primary_condition(0),
    primary_color(0),
    secondary_condition(0),
    secondary_color(0),
    lights(0),
    thermal_indicators(0)
{

}

// ----------------------------------------------------------------------------
vdb::ext_platform_appearance_t::ext_platform_appearance_t(
    const ext_platform_appearance_t &copy
) :
    ext_appearance_t(copy),
    paint_scheme(copy.paint_scheme),
    decal_scheme(copy.decal_scheme),
    primary_condition(copy.primary_condition),
    primary_color(copy.primary_color),
    secondary_condition(copy.secondary_condition),
    secondary_color(copy.secondary_color),
    lights(copy.lights),
    thermal_indicators(copy.thermal_indicators)
{

}

// ----------------------------------------------------------------------------
vdb::ext_platform_appearance_t::~ext_platform_appearance_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::ext_platform_appearance_t::clear(void)
{
    ext_appearance_t::clear();

    paint_scheme = 0;
    decal_scheme = 0;
    primary_condition = 0;
    primary_color = 0;
    secondary_condition = 0;
    secondary_color = 0;
    lights = 0;
    thermal_indicators = 0;
    status = 0;
    equipment = 0;
}

// ----------------------------------------------------------------------------
void vdb::ext_platform_appearance_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "ext_platform_appearance.paint_scheme "
           << (pl_paint_scheme_e)paint_scheme
           << " [" << (int)paint_scheme << "]" << std::endl
           << prefix << "ext_platform_appearance.decal_scheme "
           << (pl_decal_scheme_e)decal_scheme
           << " [" << (int)decal_scheme << "]" << std::endl
           << prefix << "ext_platform_appearance.primary_condition "
           << binary::str(primary_condition) << std::endl;

    variable_parameter_records_t::print_conditional_materials(
        (prefix + "ext_platform_appearance.primary_condition."),
        primary_condition,
        stream);

    stream << prefix << "ext_platform_appearance.primary_color "
           << (colors_e)primary_color << std::endl
           << prefix << "ext_platform_appearance.secondary_condition "
           << binary::str(secondary_condition) << std::endl;

    variable_parameter_records_t::print_conditional_materials(
        (prefix + "ext_platform_appearance.secondary_condition."),
        secondary_condition,
        stream);

    stream << prefix << "ext_platform_appearance.secondary_color "
           << (colors_e)secondary_color << std::endl
           << prefix << "ext_platform_appearance.lights "
           << binary::str(lights, true) << std::endl;

    variable_parameter_records_t::print_extended_lights(
        (prefix + "ext_platform_appearance.lights."),
        lights,
        stream);

    stream << prefix << "ext_platform_appearance.thermal_indicators "
           << binary::str(thermal_indicators) << std::endl
           << prefix << "ext_platform_appearance.status "
           << binary::str(status) << std::endl;

    variable_parameter_records_t::print_extended_status(
        (prefix + "ext_platform_appearance.status."),
        status,
        stream);

    stream << prefix << "ext_platform_appearance.equipment "
           << binary::str(equipment, true) << std::endl;

    variable_parameter_records_t::print_extended_equipment(
        (prefix + "ext_platform_appearance.equipment."),
        equipment,
        stream);
}

// ----------------------------------------------------------------------------
void vdb::ext_platform_appearance_t::read(byte_stream &stream)
{
    stream.read(paint_scheme);
    stream.read(decal_scheme);
    stream.read(primary_condition);
    stream.read(primary_color);
    stream.read(secondary_condition);
    stream.read(secondary_color);
    stream.read(lights);
    stream.read(thermal_indicators);
    stream.read(status);
    stream.read(equipment);
}

// ----------------------------------------------------------------------------
void vdb::ext_platform_appearance_t::write(byte_stream &stream) const
{
    stream.write(paint_scheme);
    stream.write(decal_scheme);
    stream.write(primary_condition);
    stream.write(primary_color);
    stream.write(secondary_condition);
    stream.write(secondary_color);
    stream.write(lights);
    stream.write(thermal_indicators);
    stream.write(status);
    stream.write(equipment);
}

// ----------------------------------------------------------------------------
vdb::ext_supply_appearance_t::ext_supply_appearance_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::ext_supply_appearance_t::ext_supply_appearance_t(
    const ext_supply_appearance_t &copy
) :
    ext_appearance_t(copy)
{

}

// ----------------------------------------------------------------------------
vdb::ext_supply_appearance_t::~ext_supply_appearance_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::ext_supply_appearance_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "ext_supply_appearance.status "
           << binary::str(status) << std::endl;

    variable_parameter_records_t::print_extended_status(
        (prefix + "ext_supply_appearance.status."),
        status,
        stream);

    stream << prefix << "ext_supply_appearance.equipment "
           << binary::str(equipment, true) << std::endl;

    variable_parameter_records_t::print_extended_equipment(
        (prefix + "ext_supply_appearance.equipment."),
        equipment,
        stream);
}

// ----------------------------------------------------------------------------
vdb::ext_cultural_feature_appearance_t::ext_cultural_feature_appearance_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::ext_cultural_feature_appearance_t::ext_cultural_feature_appearance_t(
    const ext_cultural_feature_appearance_t &copy
) :
    ext_appearance_t(copy)
{

}

// ----------------------------------------------------------------------------
vdb::ext_cultural_feature_appearance_t::~ext_cultural_feature_appearance_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::ext_cultural_feature_appearance_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "ext_cultural_feature_appearance.status "
           << binary::str(status) << std::endl;

    variable_parameter_records_t::print_extended_status(
        (prefix + "ext_cultural_feature_appearance.status."),
        status,
        stream);

    stream << prefix << "ext_cultural_feature_appearance.equipment "
           << binary::str(equipment, true) << std::endl;

    variable_parameter_records_t::print_extended_equipment(
        (prefix + "ext_cultural_feature_appearance.equipment."),
        equipment,
        stream);
}

// ----------------------------------------------------------------------------
vdb::variable_parameter_records_t::variable_parameter_records_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::variable_parameter_records_t::variable_parameter_records_t(
    const variable_parameter_records_t &copy)
{
    for(size_t i = 0, size = copy.records.size(); i < size; ++i)
    {
        records.push_back(copy.records[i]);
    }
}

// ----------------------------------------------------------------------------
vdb::variable_parameter_records_t::~variable_parameter_records_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::clear(void)
{
    records.clear();
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].print(
            (prefix + "vpr[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::variable_parameter_records_t::length(void) const
{
    // All VP records are the same size.
    //
    return (records.size() * vpr_t::LENGTH);
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::read(byte_stream &stream)
{
    LOG_ERROR("Cannot read variable parameter records without record count!");
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::read(
    byte_stream &stream,
    uint32_t record_count)
{
    for(uint32_t i = 0; (i < record_count) and stream(); ++i)
    {
        vpr_t
            record;

        record.read(stream);

        records.push_back(record);
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::write(
    byte_stream &stream) const
{
    for(uint32_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::print_conditional_materials(
    const std::string &prefix,
    uint8_t value,
    std::ostream &stream)
{
    if (value != 0)
    {
        const uint32_t
            rust = binary::get_1_bit_8(7, value),
            material = binary::get_1_bit_8(5, value),
            damage = binary::get_1_bit_8(3, value),
            cleanliness = binary::get_1_bit_8(1, value);

        stream << prefix << "rust " << (severity_e)rust
               << " [" << rust << "]" << std::endl
               << prefix << "material " << (pl_cond_mtl_e)rust
               << " [" << material << "]" << std::endl
               << prefix << "damage " << (severity_e)rust
               << " [" << damage << "]" << std::endl
               << prefix << "cleanliness " << (severity_e)rust
               << " [" << cleanliness << "]" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::print_extended_equipment(
    const std::string &prefix,
    uint16_t value,
    std::ostream &stream)
{
    if (usable_kind == ENT_KIND_SUPPLIES)
    {
        const uint32_t
            sling_loaded = binary::get_1_bit_16(5, value),
            ied = binary::get_2_bits_16(10, value);

        stream << prefix << "sling_loaded "
               << (yes_no_e)sling_loaded
               << " [" << sling_loaded << "]" << std::endl
               << prefix << "ied "
               << (ied_presence_e)ied
               << " [" << ied << "]" << std::endl;
    }

    if (usable_kind == ENT_KIND_CULTURAL_FEATURES)
    {
        const uint32_t
            sling_loaded = binary::get_1_bit_16(5, value),
            ied = binary::get_2_bits_16(10, value);

        stream << prefix << "sling_loaded "
               << (yes_no_e)sling_loaded
               << " [" << sling_loaded << "]" << std::endl
               << prefix << "ied "
               << (ied_presence_e)ied
               << " [" << ied << "]" << std::endl;
    }

    if ((usable_kind == ENT_KIND_PLATFORMS) and (usable_domain == DOMAIN_AIR))
    {
        const uint32_t
            auxiliary_power_plant2 = binary::get_1_bit_16(0, value),
            auxiliary_power_plant3 = binary::get_1_bit_16(1, value),
            auxiliary_power_plant4 = binary::get_1_bit_16(2, value),
            sling_load = binary::get_2_bits_16(4, value),
            sling_loaded = binary::get_1_bit_16(5, value),
            sling_damage = binary::get_2_bits_16(7, value),
            sa_server = binary::get_1_bit_16(8, value),
            ied = binary::get_2_bits_16(10, value),
            hoist = binary::get_2_bits_16(12, value),
            right_door_open = binary::get_1_bit_16(13, value),
            left_door_open = binary::get_1_bit_16(14, value),
            atir_countermeasures_active = binary::get_1_bit_16(15, value);

        stream << prefix << "auxiliary_power_plant2 "
               << (on_off_e)auxiliary_power_plant2
               << " [" << auxiliary_power_plant2 << "]" << std::endl
               << prefix << "auxiliary_power_plant3 "
               << (on_off_e)auxiliary_power_plant3
               << " [" << auxiliary_power_plant3 << "]" << std::endl
               << prefix << "auxiliary_power_plant4 "
               << (on_off_e)auxiliary_power_plant4
               << " [" << auxiliary_power_plant4 << "]" << std::endl
               << prefix << "sling_load "
               << (air_eq_slingload_e)sling_load
               << " [" << sling_load << "]" << std::endl
               << prefix << "sling_loaded "
               << (yes_no_e)sling_loaded
               << " [" << sling_loaded << "]" << std::endl
               << prefix << "sling_damage "
               << (sling_damage_e)sling_damage
               << " [" << sling_damage << "]" << std::endl
               << prefix << "sa_server "
               << (presence_e)sa_server
               << " [" << sa_server << "]" << std::endl
               << prefix << "ied "
               << (ied_presence_e)ied
               << " [" << ied << "]" << std::endl
               << prefix << "hoist "
               << (hoist_status_e)hoist
               << " [" << hoist << "]" << std::endl
               << prefix << "right_door_open "
               << (yes_no_e)right_door_open
               << " [" << right_door_open << "]" << std::endl
               << prefix << "left_door_open "
               << (yes_no_e)left_door_open
               << " [" << left_door_open << "]" << std::endl
               << prefix << "atir_countermeasures_active "
               << (yes_no_e)atir_countermeasures_active
               << " [" << atir_countermeasures_active << "]" << std::endl;
    }

    if ((usable_kind == ENT_KIND_PLATFORMS) and (usable_domain == DOMAIN_LAND))
    {
        const uint32_t
            spoiler = binary::get_1_bit_16(0, value),
            cargo_racks = binary::get_1_bit_16(1, value),
            auxiliary_power_plant = binary::get_1_bit_16(2, value),
            emergency_lights = binary::get_1_bit_16(3, value),
            sling_loaded = binary::get_1_bit_16(5, value),
            crew_antenna = binary::get_1_bit_16(6, value),
            sa_server = binary::get_1_bit_16(8, value),
            ied = binary::get_2_bits_16(10, value);

        stream << prefix << "spoiler "
               << (presence_e)spoiler
               << " [" << spoiler << "]" << std::endl
               << prefix << "cargo_racks "
               << (presence_e)cargo_racks
               << " [" << cargo_racks << "]" << std::endl
               << prefix << "auxiliary_power_plant4 "
               << (on_off_e)auxiliary_power_plant
               << " [" << auxiliary_power_plant << "]" << std::endl
               << prefix << "emergency_lights "
               << (on_off_e)emergency_lights
               << " [" << emergency_lights << "]" << std::endl
               << prefix << "sling_loaded "
               << (yes_no_e)sling_loaded
               << " [" << sling_loaded << "]" << std::endl
               << prefix << "crew_antenna "
               << (presence_e)crew_antenna
               << " [" << crew_antenna << "]" << std::endl
               << prefix << "sa_server "
               << (presence_e)sa_server
               << " [" << sa_server << "]" << std::endl
               << prefix << "ied "
               << (ied_presence_e)ied
               << " [" << ied << "]" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::print_extended_lights(
    const std::string &prefix,
    uint32_t value,
    std::ostream &stream)
{
    if ((usable_kind == ENT_KIND_PLATFORMS) and (usable_domain == DOMAIN_AIR))
    {
        const uint32_t
            formation_intensity = binary::get_3_bits_32(2, value),
            red_anticollision = binary::get_1_bit_32(3, value),
            spot_search = binary::get_1_bit_32(4, value),
            nvg_search1 = binary::get_1_bit_32(5, value),
            nvg_search2 = binary::get_1_bit_32(6, value),
            extended_search1 = binary::get_1_bit_32(7, value),
            extended_search2 = binary::get_1_bit_32(8, value),
            aft_navigation = binary::get_1_bit_32(9, value),
            extended_landing = binary::get_1_bit_32(10, value);

        stream << prefix << "formation_intensity "
               << formation_intensity << std::endl
               << prefix << "red_anticollision "
               << (on_off_e)red_anticollision
               << " [" << red_anticollision << "]" << std::endl
               << prefix << "spot_search "
               << (on_off_e)spot_search
               << " [" << spot_search << "]" << std::endl
               << prefix << "nvg_search1 "
               << (on_off_e)nvg_search1
               << " [" << nvg_search1 << "]" << std::endl
               << prefix << "nvg_search2 "
               << (on_off_e)nvg_search2
               << " [" << nvg_search2 << "]" << std::endl
               << prefix << "extended_search1 "
               << (on_off_e)extended_search1
               << " [" << extended_search1 << "]" << std::endl
               << prefix << "extended_search1 "
               << (on_off_e)extended_search2
               << " [" << extended_search2 << "]" << std::endl
               << prefix << "aft_navigation "
               << (on_off_e)aft_navigation
               << " [" << aft_navigation << "]" << std::endl
               << prefix << "extended_landing "
               << (on_off_e)extended_landing
               << " [" << extended_landing << "]" << std::endl;
    }

    if ((usable_kind == ENT_KIND_PLATFORMS) and (usable_domain == DOMAIN_LAND))
    {
        const uint32_t
            left_signal = binary::get_1_bit_32(0, value),
            right_signal = binary::get_1_bit_32(1, value),
            high_beams = binary::get_1_bit_32(2, value),
            spot_search = binary::get_1_bit_32(3, value),
            running = binary::get_1_bit_32(4, value),
            fog = binary::get_1_bit_32(5, value),
            emergency = binary::get_1_bit_32(6, value),
            parking = binary::get_1_bit_32(7, value),
            reverse = binary::get_1_bit_32(8, value),
            auxiliary_high_beams = binary::get_1_bit_32(9, value),
            rear_interior = binary::get_1_bit_32(10, value),
            emergency_vehicle = binary::get_1_bit_32(11, value);

        stream << prefix << "left_signal "
               << (on_off_e)left_signal
               << " [" << left_signal << "]" << std::endl
               << prefix << "right_signal "
               << (on_off_e)right_signal
               << " [" << right_signal << "]" << std::endl
               << prefix << "high_beams "
               << (on_off_e)high_beams
               << " [" << high_beams << "]" << std::endl
               << prefix << "spot_search "
               << (on_off_e)spot_search
               << " [" << spot_search << "]" << std::endl
               << prefix << "running "
               << (on_off_e)running
               << " [" << running << "]" << std::endl
               << prefix << "fog "
               << (on_off_e)fog
               << " [" << fog << "]" << std::endl
               << prefix << "emergency "
               << (on_off_e)emergency
               << " [" << emergency << "]" << std::endl
               << prefix << "parking "
               << (on_off_e)parking
               << " [" << parking << "]" << std::endl
               << prefix << "reverse "
               << (on_off_e)reverse
               << " [" << reverse << "]" << std::endl
               << prefix << "auxiliary_high_beams "
               << (on_off_e)auxiliary_high_beams
               << " [" << auxiliary_high_beams << "]" << std::endl
               << prefix << "rear_interior "
               << (on_off_e)rear_interior
               << " [" << rear_interior << "]" << std::endl
               << prefix << "emergency_vehicle "
               << (on_off_e)emergency_vehicle
               << " [" << emergency_vehicle << "]" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_parameter_records_t::print_extended_status(
    const std::string &prefix,
    uint8_t value,
    std::ostream &stream)
{
    const uint32_t
        present_domain = binary::get_4_bits_8(3, value),
        disguise = binary::get_3_bits_8(6, value),
        invincible = binary::get_1_bit_8(7, value);

    stream << prefix << "present_domain " << (present_domain_e)present_domain
           << " [" << present_domain << "]" << std::endl
           << prefix << "disguise " << (disguise_status_e)disguise
           << " [" << disguise << "]" << std::endl
           << prefix << "invincible " << (yes_no_e)invincible
           << std::endl;
}
