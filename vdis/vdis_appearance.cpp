#include "vdis_appearance.h"

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
