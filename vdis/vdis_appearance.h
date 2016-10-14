#ifndef VDIS_ENTITY_APPEARANCE_H
#define VDIS_ENTITY_APPEARANCE_H

#include "vdis_byte_stream.h"

namespace vdis
{
    // ------------------------------------------------------------------------
    struct lifeform_appearance_bits_t
    {
        uint32_t            camouflaged:1;          // Bit 0
        uint32_t            unused1:2;              // Bits 1-2
        uint32_t            health:2;               // Bits 3-4
        uint32_t            compliance:4;           // Bits 5-8
        uint32_t            unused2:3;              // Bits 9-11
        uint32_t            flash_lights:1;         // Bit 12
        uint32_t            unused3:3;              // Bits 13-15
        uint32_t            posture:4;              // Bits 16-19
        uint32_t            unused4:1;              // Bits 20
        uint32_t            frozen:1;               // Bit 21
        uint32_t            mounted:1;              // Bit 22
        uint32_t            deactivated:1;          // Bit 23
        uint32_t            weapon1:2;              // Bits 24-25
        uint32_t            weapon2:2;              // Bits 26-27
        uint32_t            camouflage:2;           // Bits 28-29
        uint32_t            concealed_stationary:1; // Bits 30
        uint32_t            concealed_movement:1;   // Bits 31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    struct air_platform_appearance_bits_t
    {
        uint32_t            camouflaged:1;          // Bit 0
        uint32_t            propulsion_damage:1;    // Bit 1
        uint32_t            firepower_damage:1;     // Bit 2
        uint32_t            damage:2;               // Bits 3-4
        uint32_t            smoke:2;                // Bits 5-6
        uint32_t            trailing_effects:2;     // Bits 7-8
        uint32_t            canopy:3;               // Bits 9-11
        uint32_t            landing_lights:1;       // Bit 12
        uint32_t            navigation_lights:1;    // Bit 13
        uint32_t            collision_lights:1;     // Bit 14
        uint32_t            flaming:1;              // Bit 15
        uint32_t            after_burner:1;         // Bit 16
        uint32_t            unused1:4;              // Bits 17-20
        uint32_t            frozen:1;               // Bit 21
        uint32_t            power_plant:1;          // Bit 22
        uint32_t            deactivated:1;          // Bit 23
        uint32_t            formation_lights:1;     // Bit 24
        uint32_t            unused2:3;              // Bits 25-27
        uint32_t            spot_lights:1;          // Bit 28
        uint32_t            interior_lights:1;      // Bit 29
        uint32_t            unused3:2;              // Bits 30-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    struct land_platform_appearance_bits_t
    {
        uint32_t            camouflaged:1;          // Bit 0
        uint32_t            mobility_damage:1;      // Bit 1
        uint32_t            firepower_damage:1;     // Bit 2
        uint32_t            damage:2;               // Bits 3-4
        uint32_t            smoke:2;                // Bits 5-6
        uint32_t            trailing_effects:2;     // Bits 7-8
        uint32_t            hatch:3;                // Bits 9-11
        uint32_t            head_lights:1;          // Bit 12
        uint32_t            tail_lights:1;          // Bit 13
        uint32_t            brake_lights:1;         // Bit 14
        uint32_t            flaming:1;              // Bit 15
        uint32_t            launcher_raised:1;      // Bit 16
        uint32_t            unused1:2;              // Bits 17-18
        uint32_t            concealed:1;            // Bit 19
        uint32_t            unused2:1;              // Bit 20
        uint32_t            frozen:1;               // Bit 21
        uint32_t            power_plant:1;          // Bit 22
        uint32_t            deactivated:1;          // Bit 23
        uint32_t            tent_extended:1;        // Bit 24
        uint32_t            ramp_down:1;            // Bit 25
        uint32_t            blackout_lights:1;      // Bit 26
        uint32_t            blackout_brake_lights:1;// Bit 27
        uint32_t            spot_lights:1;          // Bit 28
        uint32_t            interior_lights:1;      // Bit 29
        uint32_t            surrendered:1;          // Bit 30
        uint32_t            cloaked:1;              // Bit 31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.2.2) for point log cribs, abatis, vehicle
    // defilades, infantry fighting positions.
    //
    struct point_appearance_v2_bits_t
    {
        uint32_t            breach:8;               // Bits 0-7
        uint32_t            unused:24;              // Bits 8-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.2.3) for air/ground bursts.
    //
    struct point_appearance_v3_bits_t
    {
        uint32_t            opacity:8;              // Bits 0-7
        uint32_t            size:8;                 // Bits 8-15
        uint32_t            height:8;               // Bits 16-23
        uint32_t            burst_count:6;          // Bits 24-29
        uint32_t            chemical:2;             // Bits 30-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.2.4) for point craters.
    //
    struct point_appearance_v4_bits_t
    {
        uint32_t            size:8;                 // Bits 0-7
        uint32_t            unused:24;              // Bits 8-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.2.5) for point ribbon bridges.
    //
    struct point_appearance_v5_bits_t
    {
        uint32_t            segment_count:8;        // Bits 0-7
        uint32_t            unused:24;              // Bits 8-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.3.1) for tank ditches and concertina
    // wires.
    //
    struct linear_appearance_v1_bits_t
    {
        uint32_t            breach:8;               // Bits 0-7
        uint32_t            unused:8;               // Bits 8-15
        uint32_t            breach_length:8;        // Bits 16-23
        uint32_t            breach_position:8;      // Bits 24-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.3.2) for exhaust smoke.
    //
    struct linear_appearance_v2_bits_t
    {
        uint32_t            opacity:8;              // Bits 0-7
        uint32_t            attached:1;             // Bit 8
        uint32_t            chemical:2;             // Bits 9-10
        uint32_t            unused:19;              // Bits 11-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.3.3) for minefield lane markers.
    //
    struct linear_appearance_v3_bits_t
    {
        uint32_t            visible_side:2;         // Bits 0-1
        uint32_t            unused:30;              // Bits 11-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    // SISO-REF-010 (Section 12.1.2.4.1) for minefields.
    //
    struct areal_appearance_v1_bits_t
    {
        uint32_t            breach:2;               // Bits 0-1
        uint32_t            unused:14;              // Bits 2-15
        uint32_t            mine_count:16;          // Bits 16-31

        void print(const string_t &prefix, std::ostream &out) const;
    };

    // ------------------------------------------------------------------------
    template<typename bits_t>
    union appearance_t
    {

        uint32_t            value;
        bits_t              bits;

        inline void clear(void)
        {
            value = 0;
        }

        inline void print(const string_t &prefix, std::ostream &out) const
        {
            bits.print(prefix, out);
        }

        inline void read(byte_stream_t &stream)
        {
            stream.read(value);
        }

        inline void write(byte_stream_t &stream)
        {
            stream.read(value);
        }
    };

    // Entity appearances
    //
    typedef appearance_t<lifeform_appearance_bits_t>
        lifeform_appearance_t;
    typedef appearance_t<air_platform_appearance_bits_t>
        air_platform_appearance_t;
    typedef appearance_t<land_platform_appearance_bits_t>
        land_platform_appearance_t;

    // Point object appearances
    //
    typedef appearance_t<point_appearance_v2_bits_t>
        point_appearance_v2_t;
    typedef appearance_t<point_appearance_v3_bits_t>
        point_appearance_v3_t;
    typedef appearance_t<point_appearance_v4_bits_t>
        point_appearance_v4_t;
    typedef appearance_t<point_appearance_v5_bits_t>
        point_appearance_v5_t;

    // Linear object appearances
    //
    typedef appearance_t<linear_appearance_v1_bits_t>
        linear_appearance_v1_t;
    typedef appearance_t<linear_appearance_v2_bits_t>
        linear_appearance_v2_t;
    typedef appearance_t<linear_appearance_v3_bits_t>
        linear_appearance_v3_t;

    // Areal object appearances
    //
    typedef appearance_t<areal_appearance_v1_bits_t>
        areal_appearance_v1_t;
}

#endif
