#include "vdis_appearance.h"

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
    if (camouflaged)
    {
        out << prefix << "camouflaged yes" << std::endl;
    }

    if (health)
    {
        out << prefix << "health " << (lf_health_e)health << std::endl;
    }

    if (compliance)
    {
        out << prefix << "compliance" << (lf_compliance_e)compliance << std::endl;
    }

    if (flash_lights)
    {
        out << prefix << "flash_lights yes" << std::endl;
    }

    if (posture)
    {
        out << prefix << "posture " << (lf_posture_e)posture << std::endl;
    }

    if (frozen)
    {
        out << prefix << "frozen yes" << std::endl;
    }

    if (mounted)
    {
        out << prefix << "mounted yes" << std::endl;
    }

    if (deactivated)
    {
        out << prefix << "deactivated yes" << std::endl;
    }

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

    if (concealed_stationary)
    {
        out << prefix << "concealed_stationary yes" << std::endl;
    }

    if (concealed_movement)
    {
        out << prefix << "concealed_movement yes" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::air_platform_appearance_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (camouflaged)
    {
        out << prefix << "camouflaged yes" << std::endl;
    }

    if (propulsion_damage)
    {
        out << prefix << "propulsion_damage yes" << std::endl;
    }

    if (firepower_damage)
    {
        out << prefix << "firepower_damage yes" << std::endl;
    }

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
        out << prefix << "trailing_effects " << (int)trailing_effects << std::endl; // TODO: CREATE ENUM
    }

    if (canopy)
    {
        out << prefix << "canopy " << (int)canopy << std::endl; // TODO: CREATE ENUM
    }

    if (landing_lights)
    {
        out << prefix << "landing_lights yes" << std::endl;
    }

    if (navigation_lights)
    {
        out << prefix << "navigation_lights yes" << std::endl;
    }

    if (collision_lights)
    {
        out << prefix << "collision_lights yes" << std::endl;
    }

    if (flaming)
    {
        out << prefix << "flaming yes" << std::endl;
    }

    if (after_burner)
    {
        out << prefix << "after_burner yes" << std::endl;
    }

    if (frozen)
    {
        out << prefix << "frozen yes" << std::endl;
    }

    if (power_plant)
    {
        out << prefix << "power_plant yes" << std::endl;
    }

    if (deactivated)
    {
        out << prefix << "deactivated yes" << std::endl;
    }

    if (formation_lights)
    {
        out << prefix << "formation_lights yes" << std::endl;
    }

    if (spot_lights)
    {
        out << prefix << "spot_lights yes" << std::endl;
    }

    if (interior_lights)
    {
        out << prefix << "interior_lights yes" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::land_platform_appearance_bits_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    if (camouflaged)
    {
        out << prefix << "camouflaged yes" << std::endl;
    }

    if (mobility_damage)
    {
        out << prefix << "propulsion_damage yes" << std::endl;
    }

    if (firepower_damage)
    {
        out << prefix << "firepower_damage yes" << std::endl;
    }

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
        out << prefix << "trailing_effects " << (int)trailing_effects << std::endl; // TODO: CREATE ENUM
    }

    if (hatch)
    {
        out << prefix << "hatch " << (int)hatch << std::endl; // TODO: CREATE ENUM
    }

    if (head_lights)
    {
        out << prefix << "landing_lights yes" << std::endl;
    }

    if (tail_lights)
    {
        out << prefix << "navigation_lights yes" << std::endl;
    }

    if (brake_lights)
    {
        out << prefix << "collision_lights yes" << std::endl;
    }

    if (flaming)
    {
        out << prefix << "flaming yes" << std::endl;
    }

    if (launcher_raised)
    {
        out << prefix << "after_burner yes" << std::endl;
    }

    if (concealed)
    {
        out << prefix << "after_burner yes" << std::endl;
    }

    if (frozen)
    {
        out << prefix << "frozen yes" << std::endl;
    }

    if (power_plant)
    {
        out << prefix << "power_plant yes" << std::endl;
    }

    if (deactivated)
    {
        out << prefix << "deactivated yes" << std::endl;
    }

    if (tent_extended)
    {
        out << prefix << "tent_extended yes" << std::endl;
    }

    if (ramp_down)
    {
        out << prefix << "ramp_down yes" << std::endl;
    }

    if (blackout_lights)
    {
        out << prefix << "blackout_lights yes" << std::endl;
    }

    if (blackout_brake_lights)
    {
        out << prefix << "blackout_brake_lights yes" << std::endl;
    }

    if (spot_lights)
    {
        out << prefix << "spot_lights yes" << std::endl;
    }

    if (interior_lights)
    {
        out << prefix << "interior_lights yes" << std::endl;
    }

    if (surrendered)
    {
        out << prefix << "surrendered yes" << std::endl;
    }

    if (cloaked)
    {
        out << prefix << "cloaked yes" << std::endl;
    }
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
