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
#include "vdb_entity_info_pdus.h"
#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_services.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
vdb::entity_state_pdu_t::entity_state_pdu_t(void) :
    force_id(0),
    appearance(0),
    capabilities(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_state_pdu_t::~entity_state_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::entity_state_pdu_t::get_initator(void) const
{
    return &entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::entity_state_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? (entity_id.matches(*id_ptr)) : false);
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::clear(void)
{
    entity_id.clear();
    force_id = 0;
    entity_type.clear();
    alternative_entity_type.clear();
    location.clear();
    velocity.clear();
    orientation.clear();
    appearance = 0;
    dead_reckoning.clear();
    marking.clear();
    capabilities = 0;
    variable_parameters.clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity_id "
           << entity_id << std::endl
           << prefix << "marking "
           << marking << std::endl
           << prefix << "force_id "
           << enumerations::print(ENUM_FORCE_ID, force_id) << std::endl
           << prefix << "entity_type "
           << entity_type << std::endl
           << prefix << "entity_type.description '"
           << entity_type.get_description() << "'" << std::endl
           << prefix << "alternative_entity_type "
           << alternative_entity_type << std::endl
           << prefix << "alternative_entity_type.description '"
           << alternative_entity_type.get_description() << "'" << std::endl
           << prefix << "location.gcc "
           << location << std::endl
           << prefix << "location.gdc "
           << location.geodetic_str() << std::endl
           << prefix << "velocity "
           << velocity << std::endl
           << prefix << "velocity(m/s) "
           << float_to_string(velocity.get_length()) << std::endl
           << prefix << "orientation "
           << orientation << std::endl;

    entity_state_appearance::print(*this, prefix, stream);

    dead_reckoning.print(prefix, stream);

    stream << prefix << "capabilities "
           << binary::str(capabilities, true) << std::endl;

    print_capabilities(prefix, stream);

    stream << prefix << "vpr_count "
           << (int)variable_parameters.records.size() << std::endl;

    if (variable_parameters.records.size() > 0)
    {
        variable_parameter_records_t::use_kind(entity_type.get_kind_e());
        variable_parameter_records_t::use_domain(entity_type.get_domain_e());

        variable_parameters.print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::print_capabilities(
    const std::string &prefix,
    std::ostream &stream) const
{
    const uint32_t
        ammunition_supply = binary::get_1_bit_32(0, capabilities),
        fuel_supply = binary::get_1_bit_32(1, capabilities),
        recovery = binary::get_1_bit_32(2, capabilities),
        repair = binary::get_1_bit_32(3, capabilities),
        ads_broadcast = binary::get_1_bit_32(4, capabilities),
        sling_load_carrier = binary::get_1_bit_32(5, capabilities),
        sling_loadable = binary::get_1_bit_32(6, capabilities),
        ied_presence = binary::get_1_bit_32(7, capabilities),
        task_organizable = binary::get_1_bit_32(8, capabilities);

    stream << prefix << "ammunition_supply "
           << (yes_no_e)ammunition_supply
           << " [" << ammunition_supply << "]" << std::endl
           << prefix << "fuel_supply "
           << (yes_no_e)fuel_supply
           << " [" << fuel_supply << "]" << std::endl
           << prefix << "recovery "
           << (yes_no_e)recovery
           << " [" << recovery << "]" << std::endl
           << prefix << "repair "
           << (yes_no_e)repair
           << " [" << repair << "]" << std::endl
           << prefix << "ads_broadcast "
           << (yes_no_e)ads_broadcast
           << " [" << ads_broadcast << "]" << std::endl
           << prefix << "sling_load_carrier "
           << (yes_no_e)sling_load_carrier
           << " [" << sling_load_carrier << "]" << std::endl
           << prefix << "sling_loadable "
           << (yes_no_e)sling_loadable
           << " [" << sling_loadable << "]" << std::endl
           << prefix << "ied_presence "
           << (yes_no_e)ied_presence
           << " [" << ied_presence << "]" << std::endl
           << prefix << "task_organizable "
           << (yes_no_e)task_organizable
           << " [" << task_organizable << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::print_summary(std::ostream &stream) const
{
    const uint32_t
        damage = binary::get_2_bits_32(4, appearance);

    stream << "  " << marking << "  " << (force_id_e)force_id << std::endl
           << "  " << entity_type << " '" << entity_type.get_description()
           << "'" << std::endl;

    if (damage > 0)
    {
        switch(entity_type.get_kind())
        {
            case ENT_KIND_PLATFORMS:
                stream << "  " << (severity_e)damage << " DAMAGE" << std::endl;
                break;
            case ENT_KIND_LIFEFORMS:
                stream << "  " << (lf_health_e)damage << std::endl;
                break;
        }
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::entity_state_pdu_t::length(void) const
{
    return (BASE_LENGTH + variable_parameters.length());
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::read(byte_stream &stream)
{
    uint8_t
        record_count = 0;

    clear();

    stream.read(entity_id); // 6 bytes
    stream.read(force_id); // 1 byte
    stream.read(record_count); // 1 byte
    stream.read(entity_type); // 8 bytes
    stream.read(alternative_entity_type); // 8 bytes
    stream.read(velocity); // 12 bytes
    stream.read(location); // 24 bytes
    stream.read(orientation); // 12 bytes
    stream.read(appearance); // 4 bytes
    stream.read(dead_reckoning); // 40 bytes
    stream.read(marking); // 12 bytes
    stream.read(capabilities); // 4 bytes

    variable_parameters.read(stream, record_count);

    id_mappings::set_marking(entity_id, marking);
}

// ----------------------------------------------------------------------------
void vdb::entity_state_pdu_t::write(byte_stream &stream) const
{
    const uint8_t
        record_count = variable_parameters.records.size();

    stream.write(entity_id); // 6 bytes
    stream.write(force_id); // 1 byte
    stream.write(record_count); // 1 byte
    stream.write(entity_type); // 8 bytes
    stream.write(alternative_entity_type); // 8 bytes
    stream.write(velocity); // 12 bytes
    stream.write(location); // 24 bytes
    stream.write(orientation); // 12 bytes
    stream.write(appearance); // 4 bytes
    stream.write(dead_reckoning); // 40 bytes
    stream.write(marking); // 12 bytes
    stream.write(capabilities); // 4 bytes

    variable_parameters.write(stream);
}

// ----------------------------------------------------------------------------
vdb::entity_state_update_pdu_t::entity_state_update_pdu_t(void) :
    padding(0),
    appearance(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_state_update_pdu_t::~entity_state_update_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::entity_state_update_pdu_t::get_initator(void) const
{
    return &entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::entity_state_update_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? entity_id.matches(*id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::entity_state_update_pdu_t::clear(void)
{
    entity_id.clear();
    padding = 0;
    velocity.clear();
    location.clear();
    orientation.clear();
    appearance = 0;
    variable_parameters.clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_state_update_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity_id ";

    entity_id.print_full(stream);

    stream << std::endl
           << prefix << "velocity "
           << velocity << std::endl
           << prefix << "velocity(m/s) "
           << float_to_string(velocity.get_length()) << std::endl
           << prefix << "location.gcc "
           << location << std::endl
           << prefix << "location.gdc "
           << location.geodetic_str() << std::endl
           << prefix << "orientation "
           << orientation << std::endl
           << prefix << "variable_parameters.count "
           << (int)variable_parameters.records.size() << std::endl;

    if (variable_parameters.records.size() > 0)
    {
        variable_parameters.print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_state_update_pdu_t::print_summary(
    std::ostream &stream) const
{
    stream << "  " << location << std::endl
           << "  " << velocity << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::entity_state_update_pdu_t::length(void) const
{
    return (BASE_LENGTH + variable_parameters.length());
}

// ----------------------------------------------------------------------------
void vdb::entity_state_update_pdu_t::read(byte_stream &stream)
{
    uint8_t
        record_count = 0;

    stream.read(entity_id);
    stream.read(padding);
    stream.read(record_count);
    stream.read(velocity);
    stream.read(location);
    stream.read(orientation);
    stream.read(appearance);

    variable_parameters.read(stream, record_count);
}

// ----------------------------------------------------------------------------
void vdb::entity_state_update_pdu_t::write(byte_stream &stream) const
{
    const uint8_t
        record_count = variable_parameters.records.size();

    stream.write(entity_id);
    stream.write(padding);
    stream.write(record_count);
    stream.write(velocity);
    stream.write(location);
    stream.write(orientation);
    stream.write(appearance);

    variable_parameters.write(stream);
}

// ----------------------------------------------------------------------------
vdb::collision_pdu_t::collision_pdu_t(void) :
    collision_type(0),
    padding(0),
    mass(0)
{

}

// ----------------------------------------------------------------------------
vdb::collision_pdu_t::~collision_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::collision_pdu_t::get_initator(void) const
{
    return &issuing_entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::collision_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        issuing_entity_id.matches(*id_ptr) or
        colliding_entity_id.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::collision_pdu_t::clear(void)
{
    issuing_entity_id.clear();
    colliding_entity_id.clear();
    event_id.clear();
    collision_type = 0;
    padding = 0;
    velocity.clear();
    mass = 0;
    location.clear();
}

// ----------------------------------------------------------------------------
void vdb::collision_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "issuing_entity_id ";

    issuing_entity_id.print_full(stream);

    stream << std::endl
           << prefix << "colliding_entity_id ";

    colliding_entity_id.print_full(stream);

    stream << std::endl
           << prefix << "event_id "
           << event_id << std::endl
           << prefix << "collision_type "
           << enumerations::print(ENUM_COLLISION, collision_type) << std::endl
           << prefix << "padding "
           << binary::str(padding) << std::endl
           << prefix << "velocity "
           << velocity << std::endl
           << prefix << "velocity(m/s) "
           << float_to_string(velocity.get_length()) << std::endl
           << prefix << "mass "
           << float_to_string(mass, 1, 4) << std::endl
           << prefix << "location "
           << location << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::collision_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << (collision_e)collision_type << std::endl
           << "  " << issuing_entity_id
           << "->" << colliding_entity_id << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::collision_pdu_t::read(byte_stream &stream)
{
    stream.read(issuing_entity_id);
    stream.read(colliding_entity_id);
    stream.read(event_id);
    stream.read(collision_type);
    stream.read(padding);
    stream.read(velocity);
    stream.read(mass);
    stream.read(location);
}

// ----------------------------------------------------------------------------
void vdb::collision_pdu_t::write(byte_stream &stream) const
{
    stream.write(issuing_entity_id);
    stream.write(colliding_entity_id);
    stream.write(event_id);
    stream.write(collision_type);
    stream.write(padding);
    stream.write(velocity);
    stream.write(mass);
    stream.write(location);
}

// ----------------------------------------------------------------------------
void vdb::entity_state_appearance::print(
    const entity_state_pdu_t &pdu,
    const std::string &prefix,
    std::ostream &stream)
{
    stream << prefix << "appearance "
           << binary::str(pdu.appearance, true) << std::endl;

    switch(pdu.entity_type.get_kind())
    {
        case ENT_KIND_PLATFORMS:
            switch(pdu.entity_type.get_domain())
            {
                case DOMAIN_AIR:
                    print_air_platform(pdu, prefix, stream);
                    break;

                case DOMAIN_LAND:
                    print_land_platform(pdu, prefix, stream);
                    break;

                default:
                    break;
            }
            break;

        case ENT_KIND_LIFEFORMS:
            print_lifeform(pdu, prefix, stream);
            break;

        default:
            break;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_state_appearance::print_lifeform(
    const entity_state_pdu_t &pdu,
    const std::string &prefix,
    std::ostream &stream)
{
    const uint32_t
        clothing = binary::get_1_bit_32(0, pdu.appearance),
        health = binary::get_2_bits_32(4, pdu.appearance),
        compliance = binary::get_4_bits_32(8, pdu.appearance),
        flash_lights = binary::get_1_bit_32(12, pdu.appearance),
        posture = binary::get_4_bits_32(19, pdu.appearance),
        frozen = binary::get_1_bit_32(21, pdu.appearance),
        mounted = binary::get_1_bit_32(22, pdu.appearance),
        deactivated = binary::get_1_bit_32(23, pdu.appearance),
        first_weapon = binary::get_2_bits_32(25, pdu.appearance),
        second_weapon = binary::get_2_bits_32(27, pdu.appearance),
        camouflage = binary::get_2_bits_32(29, pdu.appearance),
        concealed_stationary = binary::get_1_bit_32(30, pdu.appearance),
        concealed_movement = binary::get_1_bit_32(31, pdu.appearance);

    stream << prefix << "lifeform_appearance.clothing "
           << enumerations::print(ENUM_LF_CLOTH_TYPE, clothing) << std::endl
           << prefix << "lifeform_appearance.health "
           << enumerations::print(ENUM_LF_HEALTH, health) << std::endl
           << prefix << "lifeform_appearance.compliance "
           << enumerations::print(ENUM_LF_COMPLIANCE, compliance) << std::endl
           << prefix << "lifeform_appearance.flash_lights "
           << enumerations::print(ENUM_YES_NO, flash_lights) << std::endl
           << prefix << "lifeform_appearance.posture "
           << enumerations::print(ENUM_LF_POSTURE, posture) << std::endl
           << prefix << "lifeform_appearance.frozen "
           << enumerations::print(ENUM_YES_NO, frozen) << std::endl
           << prefix << "lifeform_appearance.mounted "
           << enumerations::print(ENUM_YES_NO, mounted) << std::endl
           << prefix << "lifeform_appearance.deactivated "
           << enumerations::print(ENUM_YES_NO, deactivated) << std::endl
           << prefix << "lifeform_appearance.first_weapon "
           << enumerations::print(ENUM_LF_WEAPON_STATE, first_weapon) << std::endl
           << prefix << "lifeform_appearance.second_weapon "
           << enumerations::print(ENUM_LF_WEAPON_STATE, second_weapon) << std::endl
           << prefix << "lifeform_appearance.camouflage "
           << enumerations::print(ENUM_LF_CAMOUFLAGE_TYPE, camouflage) << std::endl
           << prefix << "lifeform_appearance.concealed_stationary "
           << enumerations::print(ENUM_YES_NO, concealed_stationary) << std::endl
           << prefix << "lifeform_appearance.concealed_movement "
           << enumerations::print(ENUM_YES_NO, concealed_movement) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::entity_state_appearance::print_air_platform(
    const entity_state_pdu_t &pdu,
    const std::string &prefix,
    std::ostream &stream)
{
    const uint32_t
        camouflaged = binary::get_1_bit_32(0, pdu.appearance),
        propulsion_damage = binary::get_1_bit_32(1, pdu.appearance),
        firepower_damage = binary::get_1_bit_32(2, pdu.appearance),
        damage = binary::get_2_bits_32(4, pdu.appearance),
        smoke = binary::get_2_bits_32(6, pdu.appearance),
        landing_lights = binary::get_1_bit_32(12, pdu.appearance),
        navigation_lights = binary::get_1_bit_32(13, pdu.appearance),
        collision_lights = binary::get_1_bit_32(14, pdu.appearance),
        flaming = binary::get_1_bit_32(15, pdu.appearance),
        after_burner = binary::get_1_bit_32(16, pdu.appearance),
        frozen = binary::get_1_bit_32(21, pdu.appearance),
        power_plant = binary::get_1_bit_32(22, pdu.appearance),
        deactivated = binary::get_1_bit_32(23, pdu.appearance),
        formation_lights = binary::get_1_bit_32(24, pdu.appearance),
        spot_lights = binary::get_1_bit_32(28, pdu.appearance),
        interior_lights = binary::get_1_bit_32(29, pdu.appearance);

    stream << prefix << "platform_air_appearance.camouflaged "
           << enumerations::print(ENUM_YES_NO, camouflaged) << std::endl
           << prefix << "platform_air_appearance.propulsion_damage "
           << enumerations::print(ENUM_YES_NO, propulsion_damage) << std::endl
           << prefix << "platform_air_appearance.firepower_damage "
           << enumerations::print(ENUM_YES_NO, firepower_damage) << std::endl
           << prefix << "platform_air_appearance.damage "
           << enumerations::print(ENUM_SEVERITY, damage) << std::endl
           << prefix << "platform_air_appearance.smoke "
           << enumerations::print(ENUM_AIR_SMOKE, smoke) << std::endl
           << prefix << "platform_air_appearance.landing_lights "
           << enumerations::print(ENUM_ON_OFF, landing_lights) << std::endl
           << prefix << "platform_air_appearance.navigation_lights "
           << enumerations::print(ENUM_ON_OFF, navigation_lights) << std::endl
           << prefix << "platform_air_appearance.collision_lights "
           << enumerations::print(ENUM_ON_OFF, collision_lights) << std::endl
           << prefix << "platform_air_appearance.flaming "
           << enumerations::print(ENUM_YES_NO, flaming) << std::endl
           << prefix << "platform_air_appearance.after_burner "
           << enumerations::print(ENUM_ON_OFF, after_burner) << std::endl
           << prefix << "platform_air_appearance.frozen "
           << enumerations::print(ENUM_YES_NO, frozen) << std::endl
           << prefix << "platform_air_appearance.power_plant "
           << enumerations::print(ENUM_ON_OFF, power_plant) << std::endl
           << prefix << "platform_air_appearance.deactivated "
           << enumerations::print(ENUM_YES_NO, deactivated) << std::endl
           << prefix << "platform_air_appearance.formation_lights "
           << enumerations::print(ENUM_ON_OFF, formation_lights) << std::endl
           << prefix << "platform_air_appearance.spot_lights "
           << enumerations::print(ENUM_ON_OFF, spot_lights) << std::endl
           << prefix << "platform_air_appearance.interior_lights "
           << enumerations::print(ENUM_ON_OFF, interior_lights) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::entity_state_appearance::print_land_platform(
    const entity_state_pdu_t &pdu,
    const std::string &prefix,
    std::ostream &stream)
{
    const uint32_t
        camouflaged = binary::get_1_bit_32(0, pdu.appearance),
        mobility_damage = binary::get_1_bit_32(1, pdu.appearance),
        firepower_damage = binary::get_1_bit_32(2, pdu.appearance),
        damage = binary::get_2_bits_32(4, pdu.appearance),
        smoke = binary::get_2_bits_32(6, pdu.appearance),
        head_lights = binary::get_1_bit_32(12, pdu.appearance),
        tail_lights = binary::get_1_bit_32(13, pdu.appearance),
        brake_lights = binary::get_1_bit_32(14, pdu.appearance),
        flaming = binary::get_1_bit_32(15, pdu.appearance),
        launcher = binary::get_1_bit_32(16, pdu.appearance),
        frozen = binary::get_1_bit_32(21, pdu.appearance),
        power_plant = binary::get_1_bit_32(22, pdu.appearance),
        deactivated = binary::get_1_bit_32(23, pdu.appearance),
        tent = binary::get_1_bit_32(24, pdu.appearance),
        ramp = binary::get_1_bit_32(25, pdu.appearance),
        blackout_lights = binary::get_1_bit_32(26, pdu.appearance),
        blackout_brake_lights = binary::get_1_bit_32(27, pdu.appearance),
        spot_lights = binary::get_1_bit_32(28, pdu.appearance),
        interior_lights = binary::get_1_bit_32(29, pdu.appearance),
        surrendered = binary::get_1_bit_32(30, pdu.appearance);

    stream << prefix << "platform_land_appearance.camouflaged "
           << enumerations::print(ENUM_YES_NO, camouflaged) << std::endl
           << prefix << "platform_land_appearance.mobility_damage "
           << enumerations::print(ENUM_YES_NO, mobility_damage) << std::endl
           << prefix << "platform_land_appearance.firepower_damage "
           << enumerations::print(ENUM_YES_NO, firepower_damage) << std::endl
           << prefix << "platform_land_appearance.damage "
           << enumerations::print(ENUM_SEVERITY, damage) << std::endl
           << prefix << "platform_land_appearance.smoke "
           << enumerations::print(ENUM_LAND_SMOKE, smoke) << std::endl
           << prefix << "platform_land_appearance.head_lights "
           << enumerations::print(ENUM_ON_OFF, head_lights) << std::endl
           << prefix << "platform_land_appearance.tail_lights "
           << enumerations::print(ENUM_ON_OFF, tail_lights) << std::endl
           << prefix << "platform_land_appearance.brake_lights "
           << enumerations::print(ENUM_ON_OFF, brake_lights) << std::endl
           << prefix << "platform_land_appearance.flaming "
           << enumerations::print(ENUM_YES_NO, flaming) << std::endl
           << prefix << "platform_land_appearance.launcher "
           << enumerations::print(ENUM_ON_OFF, launcher) << std::endl
           << prefix << "platform_land_appearance.frozen "
           << enumerations::print(ENUM_YES_NO, frozen) << std::endl
           << prefix << "platform_land_appearance.power_plant "
           << enumerations::print(ENUM_ON_OFF, power_plant) << std::endl
           << prefix << "platform_land_appearance.deactivated "
           << enumerations::print(ENUM_YES_NO, deactivated) << std::endl
           << prefix << "platform_land_appearance.tent "
           << enumerations::print(ENUM_YES_NO, tent) << std::endl
           << prefix << "platform_land_appearance.ramp "
           << enumerations::print(ENUM_YES_NO, ramp) << std::endl
           << prefix << "platform_land_appearance.blackout_lights "
           << enumerations::print(ENUM_ON_OFF, blackout_lights) << std::endl
           << prefix << "platform_land_appearance.blackout_brake_lights "
           << enumerations::print(ENUM_ON_OFF, blackout_brake_lights) << std::endl
           << prefix << "platform_land_appearance.spot_lights "
           << enumerations::print(ENUM_ON_OFF, spot_lights) << std::endl
           << prefix << "platform_land_appearance.interior_lights "
           << enumerations::print(ENUM_ON_OFF, interior_lights) << std::endl
           << prefix << "platform_land_appearance.surrendered "
           << enumerations::print(ENUM_YES_NO, surrendered) << std::endl;
}
