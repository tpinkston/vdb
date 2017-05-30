#include "vdis_appearance.h"

namespace
{
    // ------------------------------------------------------------------------
    void print_flag(
        const std::string &prefix,
        const char *name,
        int value,
        std::ostream &out)
    {
        if (value)
        {
            out << prefix << name << " yes" << std::endl;
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::print_appearance(
    const std::string &prefix,
    vdis::entity_type_t type,
    uint32_t appearance,
    std::ostream &out)
{
    if (type.kind_enum() == ENT_KIND_LIFEFORMS)
    {
        lifeform_appearance_t
            entity_appearance;

        entity_appearance.value = appearance;
        entity_appearance.print(prefix + "lifeform_appearance.", out);
    }
    else if (type.kind_enum() == ENT_KIND_PLATFORMS)
    {
        if (type.domain_enum() == DOMAIN_AIR)
        {
            air_platform_appearance_t
                entity_appearance;

            entity_appearance.value = appearance;
            entity_appearance.print(prefix + "air_platform_appearance.", out);
        }
        else if (type.domain_enum() == DOMAIN_LAND)
        {
            land_platform_appearance_t
                entity_appearance;

            entity_appearance.value = appearance;
            entity_appearance.print(prefix + "land_platform_appearance.", out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::lifeform_appearance_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    print_flag(prefix, "camouflaged", camouflaged, out);

    if (health)
    {
        out << prefix << "health " << (lf_health_e)health << std::endl;
    }

    if (compliance)
    {
        out << prefix << "compliance" << (lf_compliance_e)compliance << std::endl;
    }

    print_flag(prefix, "flash_lights", flash_lights, out);

    if (posture)
    {
        out << prefix << "posture " << (lf_posture_e)posture << std::endl;
    }

    print_flag(prefix, "frozen", frozen, out);
    print_flag(prefix, "mounted", mounted, out);
    print_flag(prefix, "deactivated", deactivated, out);

    if (weapon1)
    {
        out << prefix << "weapon1 " << (lf_weapon_state_e)weapon1 << std::endl;
    }

    if (weapon2)
    {
        out << prefix << "weapon2 " << (lf_weapon_state_e)weapon2 << std::endl;
    }

    if (camouflage)
    {
        out << prefix << "camouflage " << (lf_camouflage_type_e)camouflage << std::endl;
    }

    print_flag(prefix, "concealed_stationary", concealed_stationary, out);
    print_flag(prefix, "concealed_movement", concealed_movement, out);
}

// ----------------------------------------------------------------------------
void vdis::air_platform_appearance_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    print_flag(prefix, "camouflaged", camouflaged, out);
    print_flag(prefix, "propulsion_damage", propulsion_damage, out);
    print_flag(prefix, "firepower_damage", firepower_damage, out);

    if (damage)
    {
        out << prefix << "damage " << (severity_e)damage << std::endl;
    }

    if (smoke)
    {
        out << prefix << "smoke " << (air_smoke_e)smoke << std::endl;
    }

    if (trailing_effects)
    {
        // TODO: CREATE ENUM
        out << prefix << "trailing_effects " << (int)trailing_effects << std::endl;
    }

    print_flag(prefix, "canopy", canopy, out);
    print_flag(prefix, "landing_lights", landing_lights, out);
    print_flag(prefix, "navigation_lights", navigation_lights, out);
    print_flag(prefix, "collision_lights", collision_lights, out);
    print_flag(prefix, "flaming", flaming, out);
    print_flag(prefix, "after_burner", after_burner, out);
    print_flag(prefix, "lower_collision_light", lower_collision_light, out);
    print_flag(prefix, "upper_collision_light", upper_collision_light, out);
    print_flag(prefix, "night_collision", night_collision, out);
    print_flag(prefix, "blinking", blinking, out);
    print_flag(prefix, "frozen", frozen, out);
    print_flag(prefix, "power_plant", power_plant, out);
    print_flag(prefix, "deactivated", deactivated, out);
    print_flag(prefix, "formation_lights", formation_lights, out);
    print_flag(prefix, "spot_light", spot_light, out);
    print_flag(prefix, "interior_lights", interior_lights, out);
}

// ----------------------------------------------------------------------------
void vdis::land_platform_appearance_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    print_flag(prefix, "camouflaged", camouflaged, out);
    print_flag(prefix, "mobility_damage", mobility_damage, out);
    print_flag(prefix, "firepower_damage", firepower_damage, out);

    if (smoke)
    {
        out << prefix << "smoke " << (air_smoke_e)smoke << std::endl;
    }

    if (trailing_effects)
    {
        // TODO: CREATE ENUM
        out << prefix << "trailing_effects " << (int)trailing_effects << std::endl;
    }

    if (hatch)
    {
        // TODO: CREATE ENUM
        out << prefix << "hatch " << (int)hatch << std::endl;
    }

    print_flag(prefix, "head_lights", head_lights, out);
    print_flag(prefix, "tail_lights", tail_lights, out);
    print_flag(prefix, "brake_lights", brake_lights, out);
    print_flag(prefix, "flaming", flaming, out);
    print_flag(prefix, "launcher_raised", launcher_raised, out);

    if (camouflage_type)
    {
        // TODO: CREATE ENUM
        out << prefix << "camouflage_type " << (int)camouflage_type << std::endl;
    }

    print_flag(prefix, "concealed", concealed, out);
    print_flag(prefix, "frozen", frozen, out);
    print_flag(prefix, "power_plant", power_plant, out);
    print_flag(prefix, "deactivated", deactivated, out);
    print_flag(prefix, "tent_extended", tent_extended, out);
    print_flag(prefix, "ramp_down", ramp_down, out);
    print_flag(prefix, "blackout_lights", blackout_lights, out);
    print_flag(prefix, "blackout_brake_lights", blackout_brake_lights, out);
    print_flag(prefix, "spot_lights", spot_lights, out);
    print_flag(prefix, "interior_lights", interior_lights, out);
    print_flag(prefix, "surrendered", surrendered, out);
    print_flag(prefix, "cloaked", cloaked, out);
}

// ----------------------------------------------------------------------------
void vdis::point_appearance_v2_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "breach ";

    switch(breach)
    {
        case 0:
            out << "none(0[0x00])" << std::endl;
            break;
        case 1:
            out << "breached(1[0x01])" << std::endl;
            break;
        case 2:
            out << "cleared(2[0x02])" << std::endl;
            break;
        default:
            out << (int)breach << std::endl;
    }

    out << prefix << "unused(30 bits) "
        << to_bin_string((uint32_t)unused, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::point_appearance_v3_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "opacity " << (int)opacity << std::endl
        << prefix << "size " << (int)size << std::endl
        << prefix << "height " << (int)height << std::endl
        << prefix << "burst_count " << (int)burst_count << std::endl
        << prefix << "chemical ";

    switch(chemical)
    {
        case 0:
            out << "other(0[0x00])" << std::endl;
            break;
        case 1:
            out << "hydrochloric(1[0x01])" << std::endl;
            break;
        case 2:
            out << "white_phosphorous(2[0x02])" << std::endl;
            break;
        case 3:
            out << "red_phosphorous(3[0x03])" << std::endl;
            break;
        default:
            out << (int)chemical << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::point_appearance_v4_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "diameter " << (int)diameter << std::endl
        << prefix << "unused(24 bits) "
        << to_bin_string((uint32_t)unused, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::point_appearance_v5_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "segment_count " << (int)segment_count << std::endl
        << prefix << "unused(24 bits) "
        << to_bin_string((uint32_t)unused, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::linear_appearance_v1_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "breach ";

    switch(breach)
    {
        case 0:
            out << "none(0[0x00])" << std::endl;
            break;
        case 1:
            out << "slight(1[0x01])" << std::endl;
            break;
        case 2:
            out << "moderate(2[0x02])" << std::endl;
            break;
        case 3:
            out << "cleared(3[0x03])" << std::endl;
            break;
        default:
            out << (int)breach << std::endl;
    }

    out << prefix << "unused(14 bits) "
        << to_bin_string((uint16_t)unused, true) << std::endl
        << prefix << "breach_length " << (int)breach_length << std::endl
        << prefix << "breach_position " << (int)breach_position << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::linear_appearance_v2_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "opacity " << (int)opacity << std::endl
        << prefix << "attached " << (yes_no_e)attached << std::endl
        << prefix << "chemical ";

    switch(chemical)
    {
        case 0:
            out << "other(0[0x00])" << std::endl;
            break;
        case 1:
            out << "hydrochloric(1[0x01])" << std::endl;
            break;
        case 2:
            out << "white_phosphorous(2[0x02])" << std::endl;
            break;
        case 3:
            out << "red_phosphorous(3[0x03])" << std::endl;
            break;
        default:
            out << (int)chemical << std::endl;
    }

    out << prefix << "unused(21 bits) "
        << to_bin_string((uint32_t)unused, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::linear_appearance_v3_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "visible_side ";

    switch(visible_side)
    {
        case 0:
            out << "left(0[0x00])" << std::endl;
            break;
        case 1:
            out << "right(1[0x01])" << std::endl;
            break;
        case 2:
            out << "left_and_right(2[0x02])" << std::endl;
            break;
        default:
            out << (int)visible_side << std::endl;
    }

    out << prefix << "unused(30 bits) "
        << to_bin_string((uint32_t)unused, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::areal_appearance_v1_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "breach ";

    switch(breach)
    {
        case 0:
            out << "none" << std::endl;
            break;
        case 1:
            out << "breached" << std::endl;
            break;
        case 2:
            out << "cleared" << std::endl;
            break;
        default:
            out << (int)breach << std::endl;
    }

    out << prefix << "unused(14 bits) "
        << to_bin_string((uint16_t)unused) << std::endl
        << prefix << "mine_count " << (int)mine_count << std::endl;
}
