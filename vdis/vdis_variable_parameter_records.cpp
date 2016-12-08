#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"
#include "vdis_variable_parameter_records.h"

namespace
{
    const vdis::entity_type_t *entity_type_ptr = 0;
}

// ----------------------------------------------------------------------------
vdis::variable_parameter_record_t **vdis::read_variable_parameter_records(
    byte_stream_t &stream,
    uint32_t count)
{
    variable_parameter_record_t **records = 0;

    if (count > 0)
    {
        records = new variable_parameter_record_t*[count];
    }

    for(uint32_t i = 0; i < count; ++i)
    {
        records[i] = new variable_parameter_record_t;

        LOG_EXTRA_VERBOSE(
            "Reading VP record %d/%d@%p with stream at index %d/%d...",
            (i + 1),
            count,
            records[i],
            stream.index(),
            stream.length());

        records[i]->read(stream);
    }

    return records;
}

// ----------------------------------------------------------------------------
void vdis::variable_parameter_content_t::using_type(
    const entity_type_t *type_ptr)
{
    entity_type_ptr = type_ptr;
}

// ----------------------------------------------------------------------------
vdis::variable_parameter_record_t::~variable_parameter_record_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::variable_parameter_record_t::clear(void)
{
    if (content_ptr)
    {
        delete content_ptr;
    }

    content_ptr = 0;
}

// ----------------------------------------------------------------------------
void vdis::variable_parameter_record_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "type " << type() << std::endl;

    if (content_ptr)
    {
        content_ptr->print(prefix, out);
    }
    else
    {
        out << prefix << "content null" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::variable_parameter_record_t::read(byte_stream_t &stream)
{
    if (stream.remaining_length() < VP_RECORD_SIZE)
    {
        LOG_ERROR(
            "Stream too short for VPR: %d bytes",
            stream.remaining_length());
    }
    else
    {
        // Read the record type (1 byte)
        //
        uint8_t type = 0;
        stream.read(type);

        LOG_EXTRA_VERBOSE(
            "Reading VP record %s",
            enumerations::get_name(ENUM_VP_RECORD_TYPE, type).c_str());

        switch(type)
        {
            case VP_RECORD_TYPE_ARTICULATED_PART:
            {
                content_ptr = new articulated_part_t;
                break;
            }
            case VP_RECORD_TYPE_ENTITY_ASSOC:
            {
                content_ptr = new entity_association_t;
                break;
            }
            case VP_RECORD_TYPE_ENTITY_OFFSET:
            {
                content_ptr = new entity_offset_t;
                break;
            }
            case VP_RECORD_TYPE_LEGACY_EXT_LIFEFORM_APP:
            {
                content_ptr = new legacy_extended_lifeform_appearance_t;
                break;
            }
            case VP_RECORD_TYPE_EXT_PLATFORM_APP:
            {
                content_ptr = new extended_platform_appearance_t;
                break;
            }
            case VP_RECORD_TYPE_EXT_SUPPLY_APP:
            {
                content_ptr = new extended_supply_appearance_t;
                break;
            }
            case VP_RECORD_TYPE_EXT_CULT_FEAT_APP:
            {
                content_ptr = new extended_cultural_feature_appearance_t;
                break;
            }
            default:
            {
                content_ptr = new default_variable_content_t(type);
                break;
            }
        }

        content_ptr->read(stream);
    }
}

// ----------------------------------------------------------------------------
void vdis::variable_parameter_record_t::write(byte_stream_t &stream)
{
    // TODO
}

// ----------------------------------------------------------------------------
vdis::default_variable_content_t::default_variable_content_t(uint8_t type) :
    record_type(type)
{

}

// ----------------------------------------------------------------------------
void vdis::default_variable_content_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    vdis::byte_buffer_t
        temp(buffer, VP_RECORD_DATA_SIZE, false);

    temp.print((prefix + "content."), out);
}

// ----------------------------------------------------------------------------
void vdis::default_variable_content_t::read(byte_stream_t &stream)
{
    stream.read(buffer, VP_RECORD_DATA_SIZE);
}

// ----------------------------------------------------------------------------
void vdis::default_variable_content_t::write(byte_stream_t &stream)
{
    stream.write(buffer, VP_RECORD_DATA_SIZE);
}

// ----------------------------------------------------------------------------
void vdis::articulated_part_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "articulated_part.change_indicator "
        << (int)change_indicator << std::endl
        << prefix << "articulated_part.part_id "
        << part_id << std::endl
        << prefix << "articulated_part.type "
        << part_type_enum() << std::endl
        << prefix << "articulated_part.metric "
        << part_metric_enum() << std::endl
        << prefix << "articulated_part.value "
        << to_string(value) << std::endl
        << prefix << "articulated_part.padding "
        << to_hex_string(padding) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::articulated_part_t::read(byte_stream_t &stream)
{
    stream.read(change_indicator);
    stream.read(part_id);
    stream.read(type_metric);
    stream.read(value);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::articulated_part_t::write(byte_stream_t &stream)
{
    stream.write(change_indicator);
    stream.write(part_id);
    stream.write(type_metric);
    stream.write(value);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::entity_association_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "entity_association.change_indicator "
        << (int)change_indicator << std::endl
        << prefix << "entity_association.status "
        << association_status_enum() << std::endl
        << prefix << "entity_association.type "
        << association_type_enum() << std::endl
        << prefix << "entity_association.entity_id "
        << entity_id << std::endl
        << prefix << "entity_association.station "
        << (int)station << std::endl
        << prefix << "entity_association.connection_type "
        << connection_type_enum() << std::endl
        << prefix << "entity_association.group_membership "
        << membership_enum() << std::endl
        << prefix << "entity_association.group_number "
        << (int)group_number << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::entity_association_t::read(byte_stream_t &stream)
{
    stream.read(change_indicator);
    stream.read(association_status);
    stream.read(association_type);
    entity_id.read(stream);
    stream.read(station);
    stream.read(connection_type);
    stream.read(group_membership);
    stream.read(group_number);
}

// ----------------------------------------------------------------------------
void vdis::entity_association_t::write(byte_stream_t &stream)
{
    stream.write(change_indicator);
    stream.write(association_status);
    stream.write(association_type);
    entity_id.write(stream);
    stream.write(station);
    stream.write(connection_type);
    stream.write(group_membership);
    stream.write(group_number);
}

// ----------------------------------------------------------------------------
void vdis::entity_offset_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "entity_offset.offset_type "
        << offset_type_enum() << std::endl
        << prefix << "entity_offset.padding "
        << (int)padding << std::endl
        << prefix << "entity_offset.position "
        << position << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::entity_offset_t::read(byte_stream_t &stream)
{
    stream.read(offset_type);
    stream.read(padding);
    position.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::entity_offset_t::write(byte_stream_t &stream)
{
    stream.write(offset_type);
    stream.write(padding);
    position.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::legacy_extended_lifeform_appearance_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "legacy_ext_lifeform_app.paint_scheme "
        << cloth_scheme_enum() << std::endl
        << prefix << "legacy_ext_lifeform_app.primary_color "
        << color_primary_enum() << std::endl
        << prefix << "legacy_ext_lifeform_app.secondary_color "
        << color_secondary_enum() << std::endl
        << prefix << "legacy_ext_lifeform_app.equipment "
        << (int)equipment << std::endl // TODO
        << prefix << "legacy_ext_lifeform_app.status "
        << (int)status << std::endl // TODO
        << prefix << "legacy_ext_lifeform_app.unused1 "
        << to_hex_string(unused1) << std::endl
        << prefix << "legacy_ext_lifeform_app.attributes "
        << (int)attributes << std::endl // TODO
        << prefix << "legacy_ext_lifeform_app.unused2 "
        << to_hex_string(unused2) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::legacy_extended_lifeform_appearance_t::read(byte_stream_t &stream)
{
    stream.read(clothing_scheme);
    stream.read(primary_color);
    stream.read(secondary_color);
    stream.read(equipment);
    stream.read(status);
    stream.read(unused1);
    stream.read(attributes);
    stream.read(unused2);
}

// ----------------------------------------------------------------------------
void vdis::legacy_extended_lifeform_appearance_t::write(byte_stream_t &stream)
{
    stream.write(clothing_scheme);
    stream.write(primary_color);
    stream.write(secondary_color);
    stream.write(equipment);
    stream.write(status);
    stream.write(unused1);
    stream.write(attributes);
    stream.write(unused2);
}

// ----------------------------------------------------------------------------
void vdis::extended_appearance_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    vdis::extended_status_t
        default_status;
    vdis::domain_e
        domain = vdis::DOMAIN_OTHER;
    string_t
        domain_name;

    if (entity_type_ptr)
    {
        domain = (vdis::domain_e)entity_type_ptr->domain;
    }

    domain_name = vdis::enumerations::get_name(ENUM_DOMAIN, domain);

    default_status.value = status;
    default_status.print(prefix, out);

    switch(domain)
    {
        case vdis::DOMAIN_AIR:
        {
            vdis::extended_equipment_t<vdis::air_platform_extended_equipment_bits_t>
                air_equipment;

            air_equipment.value = equipment;
            air_equipment.bits.print(prefix, out);
            break;
        }
        case vdis::DOMAIN_LAND:
        {
            vdis::extended_equipment_t<vdis::land_platform_extended_equipment_bits_t>
                land_equipment;

            land_equipment.value = equipment;
            land_equipment.bits.print(prefix, out);
            break;
        }
        default:
        {
            vdis::extended_equipment_t<vdis::extended_equipment_bits_t>
                default_equipment;

            default_equipment.value = equipment;
            default_equipment.bits.print(prefix, out);
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::extended_platform_appearance_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (paint != 0)
    {
        out << prefix << "ext_platform_app.paint_scheme "
            << paint_scheme_enum() << std::endl;
    }

    if (decal != 0)
    {
        out << prefix << "ext_platform_app.decal_scheme "
            << decal_scheme_enum() << std::endl;
    }

    if (primary_condition != 0)
    {
        conditional_materials_t
            materials;

        materials.value = primary_condition;
        materials.print(
            (prefix + "ext_platform_app.primary_condition "),
            out);
    }

    if (primary_color != 0)
    {
        out << prefix << "ext_platform_app.primary_color "
            << color_primary_enum() << std::endl;
    }

    if (secondary_condition != 0)
    {
        conditional_materials_t
            materials;

        materials.value = secondary_condition;
        materials.print(
            (prefix + "ext_platform_app.primary_condition "),
            out);
    }

    if (secondary_color != 0)
    {
        out << prefix << "ext_platform_app.secondary_color "
            << color_secondary_enum() << std::endl;
    }

    if (lights != 0)
    {
        // TODO: ext_platform_appearance.lights
        out << prefix << "ext_platform_app.lights "
            << to_bin_string(lights, true) << std::endl;
    }

    if (thermal_indicators != 0)
    {
        out << prefix << "ext_platform_app.thermal_indicators "
            << to_bin_string(thermal_indicators) << std::endl;
    }

    extended_appearance_t::print((prefix + "ext_platform_app."), out);
}

// ----------------------------------------------------------------------------
void vdis::extended_platform_appearance_t::read(byte_stream_t &stream)
{
    stream.read(paint);
    stream.read(decal);
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
void vdis::extended_platform_appearance_t::write(byte_stream_t &stream)
{
    stream.write(paint);
    stream.write(decal);
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
void vdis::extended_supply_appearance_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    extended_appearance_t::print((prefix + "ext_supply_app."), out);
}

// ----------------------------------------------------------------------------
void vdis::extended_supply_appearance_t::read(byte_stream_t &stream)
{
    stream.read(padding, 12);
    stream.read(status);
    stream.read(equipment);
}

// ----------------------------------------------------------------------------
void vdis::extended_supply_appearance_t::write(byte_stream_t &stream)
{
    stream.write(padding, 12);
    stream.write(status);
    stream.write(equipment);
}

// ----------------------------------------------------------------------------
void vdis::extended_cultural_feature_appearance_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    extended_appearance_t::print((prefix + "ext_cult_feat_app."), out);
}

// ----------------------------------------------------------------------------
void vdis::extended_cultural_feature_appearance_t::read(byte_stream_t &stream)
{
    stream.read(padding, 12);
    stream.read(status);
    stream.read(equipment);
}

// ----------------------------------------------------------------------------
void vdis::extended_cultural_feature_appearance_t::write(byte_stream_t &stream)
{
    stream.write(padding, 12);
    stream.write(status);
    stream.write(equipment);
}

// ----------------------------------------------------------------------------
void vdis::extended_status_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "status.value " << to_bin_string(value) << std::endl;

    if (bits.present_domain != 0)
    {
        out << prefix << "status.present_domain "
            << present_domain_enum() << std::endl;
    }

    if (bits.disguise != 0)
    {
        out << prefix << "status.disguise "
            << disguise_status_enum() << std::endl;
    }

    if (bits.invincible != 0)
    {
        out << prefix << "status.invincible "
            << (invincible() ? "true" : "false") << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::conditional_materials_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (bits.cleanliness != 0)
    {
        out << prefix << "cleanliness " << cleanliness_enum() << std::endl;
    }

    if (bits.damage != 0)
    {
        out << prefix << "damage " << damage_enum() << std::endl;
    }

    if (bits.material != 0)
    {
        out << prefix << "material " << material_enum() << std::endl;
    }

    if (bits.rust != 0)
    {
        out << prefix << "rust " << rust_enum() << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::extended_equipment_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (sling_loaded != 0)
    {
        out << prefix << "equipment.sling_loaded " << "yes" << std::endl;
    }

    if (ied != 0)
    {
        out << prefix << "equipment.ied " << "yes" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::air_platform_extended_equipment_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (aux_power_plant2 != 0)
    {
        out << prefix << "aux_power_plant2 " << "yes" << std::endl;
    }

    if (aux_power_plant3 != 0)
    {
        out << prefix << "aux_power_plant3 " << "yes" << std::endl;
    }

    if (aux_power_plant4 != 0)
    {
        out << prefix << "aux_power_plant4 " << "yes" << std::endl;
    }

    if (sling_load != 0)
    {
        out << prefix << "sling_load "
            << (air_eq_slingload_e)sling_load << std::endl;
    }

    if (sling_loaded != 0)
    {
        out << prefix << "sling_loaded " << "yes" << std::endl;
    }

    if (sling_damage != 0)
    {
        out << prefix << "sling_damage "
            << (sling_damage_e)sling_damage << std::endl;
    }

    if (sa_server != 0)
    {
        out << prefix << "sa_server " << "yes" << std::endl;
    }

    if (ied != 0)
    {
        out << prefix << "ied " << (ied_presence_e)ied << std::endl;
    }

    if (hoist != 0)
    {
        out << prefix << "hoist " << (hoist_status_e)hoist << std::endl;
    }

    if (right_door_open != 0)
    {
        out << prefix << "right_door_open " << "yes" << std::endl;
    }

    if (left_door_open != 0)
    {
        out << prefix << "left_door_open " << "yes" << std::endl;
    }

    if (countermeasures_active != 0)
    {
        out << prefix << "countermeasures_active " << "yes" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::land_platform_extended_equipment_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (spoiler != 0)
    {
        out << prefix << "spoiler " << "yes" << std::endl;
    }

    if (cargo_racks != 0)
    {
        out << prefix << "cargo_racks " << "yes" << std::endl;
    }

    if (aux_power_plant != 0)
    {
        out << prefix << "aux_power_plant " << "yes" << std::endl;
    }

    if (emergency_lights != 0)
    {
        out << prefix << "emergency_lights " << "yes" << std::endl;
    }

    if (sling_loaded != 0)
    {
        out << prefix << "sling_loaded " << "yes" << std::endl;
    }

    if (crew_antenna != 0)
    {
        out << prefix << "crew_antenna " << "yes" << std::endl;
    }

    if (sa_server != 0)
    {
        out << prefix << "sa_server " << "yes" << std::endl;
    }

    if (ied != 0)
    {
        out << prefix << "ied " << (ied_presence_e)ied << std::endl;
    }
}
