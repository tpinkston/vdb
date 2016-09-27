#ifndef VDIS_VP_RECORDS_H
#define VDIS_VP_RECORDS_H

#include <cstdint>
#include <string>

#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_enums.h"

#define VP_RECORD_SIZE 16
#define VP_RECORD_DATA_SIZE 15

namespace vdis
{
    // ------------------------------------------------------------------------
    struct vp_record_content_t
    {
        virtual ~vp_record_content_t(void) { }

        virtual void print(const std::string &, std::ostream &) const = 0;

        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;

        static void using_type(const entity_type_t *type_ptr);
    };

    // ------------------------------------------------------------------------
    struct vp_record_t
    {
        uint8_t                 type;                   // 1 byte
        vp_record_content_t    *content_ptr;            // 15 bytes

        ~vp_record_t(void);

        inline vp_record_type_e record_type(void) const
        {
            return (vdis::vp_record_type_e)type;
        }

        void clear(void);

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static vp_record_t **read(byte_stream_t &stream, uint32_t count);
    };

    // ------------------------------------------------------------------------
    struct default_variable_content_t : vp_record_content_t
    {
        uint8_t                 buffer[VP_RECORD_DATA_SIZE];

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct articulated_part_t : vp_record_content_t
    {
        uint8_t                 change_indicator;       // 1 byte
        uint16_t                part_id;                // 2 bytes
        uint32_t                type_metric;            // 4 bytes
        float32_t               value;                  // 4 bytes
        uint32_t                padding;                // 4 bytes

        inline articulated_parts_e type(void) const
        {
            return (articulated_parts_e)(type_metric & ~0x1F);
        }

        inline articulated_parts_metric_e metric(void) const
        {
            return (articulated_parts_metric_e)(type_metric & 0x1F);
        }

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_association_t : vp_record_content_t
    {
        uint8_t                 change_indicator;       // 1 byte
        uint8_t                 status;                 // 1 byte
        uint8_t                 type;                   // 1 byte
        entity_id_t             entity_id;              // 6 bytes
        uint16_t                station;                // 2 bytes
        uint8_t                 connection_type;        // 1 byte
        uint8_t                 group_membership;       // 1 byte
        uint16_t                group_number;           // 2 bytes

        inline ent_assoc_status_e association_status(void) const
        {
            return (ent_assoc_status_e)status;
        }

        inline phys_assoc_type_e association_type(void) const
        {
            return (phys_assoc_type_e)type;
        }

        inline phys_conn_type_e connection(void) const
        {
            return (phys_conn_type_e)connection_type;
        }

        inline grp_mem_type_e membership(void) const
        {
            return (grp_mem_type_e)group_membership;
        }

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_offset_t : vp_record_content_t
    {
        uint8_t                 type;                   // 1 byte
        uint16_t                padding;                // 2 bytes
        location12_t            position;               // 12 bytes

        inline offset_type_e offset_type(void) const
        {
            return (offset_type_e)type;
        }

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct legacy_extended_lifeform_appearance_t : vp_record_content_t
    {
        uint8_t                 clothing_scheme;        // 1 byte
        uint8_t                 primary_color;          // 1 byte
        uint8_t                 secondary_color;        // 1 byte
        uint32_t                equipment;              // 4 bytes
        uint8_t                 status;                 // 1 byte
        uint8_t                 unused1;                // 1 byte
        uint16_t                attributes;             // 2 bytes
        uint32_t                unused2;                // 4 bytes

        inline lf_cloth_scheme_e cloth_scheme(void) const
        {
            return (lf_cloth_scheme_e)clothing_scheme;
        }

        inline colors_e color_primary(void) const
        {
            return (colors_e)primary_color;
        }

        inline colors_e color_secondary(void) const
        {
            return (colors_e)secondary_color;
        }

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_appearance_t : vp_record_content_t
    {
        uint8_t                 status;                 // 1 byte
        uint16_t                equipment;              // 2 bytes

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct extended_platform_appearance_t : public extended_appearance_t
    {
        uint8_t                 paint;                  // 1 byte
        uint16_t                decal;                  // 2 bytes
        uint8_t                 primary_condition;      // 1 byte
        uint8_t                 primary_color;          // 1 byte
        uint8_t                 secondary_condition;    // 1 byte
        uint8_t                 secondary_color;        // 1 byte
        uint32_t                lights;                 // 4 bytes
        uint8_t                 thermal_indicators;     // 1 byte

        inline pl_paint_scheme_e paint_scheme(void) const
        {
            return (pl_paint_scheme_e)paint;
        }

        inline pl_decal_scheme_e decal_scheme(void) const
        {
            return (pl_decal_scheme_e)decal;
        }

        inline colors_e color_primary(void) const
        {
            return (colors_e)primary_color;
        }

        inline colors_e color_secondary(void) const
        {
            return (colors_e)secondary_color;
        }

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_supply_appearance_t : extended_appearance_t
    {
        uint8_t                 padding[12];            // 12 bytes

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_cultural_feature_appearance_t : extended_appearance_t
    {
        uint8_t                 padding[12];            // 12 bytes

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union extended_status_t
    {
        struct bits_t
        {
            uint8_t             present_domain:4;       // Bits 3-0
            uint8_t             disguise:3;             // Bits 6-4
            uint8_t             invincible:1;           // Bit 7
        };

        uint8_t                 value;
        bits_t                  bits;

        inline present_domain_e present_domain(void) const
        {
            return (present_domain_e)bits.present_domain;
        }

        inline disguise_status_e disguise_status(void) const
        {
            return (disguise_status_e)bits.disguise;
        }

        inline bool invincible(void) const
        {
            return (bits.invincible != 0);
        }

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    union conditional_materials_t
    {
        struct bits_t
        {
            uint8_t             cleanliness:2;              // Bits 0-1
            uint8_t             damage:2;                   // Bit2 2-3
            uint8_t             material:2;                 // Bits 4-5
            uint8_t             rust:2;                     // Bits 6-7
        };

        uint8_t                 value;
        bits_t                  bits;

        inline severity_e cleanliness(void) const
        {
            return (severity_e)bits.cleanliness;
        }

        inline severity_e damage(void) const
        {
            return (severity_e)bits.damage;
        }

        inline pl_cond_mtl_e material(void) const
        {
            return (pl_cond_mtl_e)bits.material;
        }

        inline severity_e rust(void) const
        {
            return (severity_e)bits.rust;
        }

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct extended_equipment_bits_t
    {
        uint16_t                unused1:5;                  // Bits 0-4
        uint16_t                sling_loaded:1;             // Bit 5
        uint16_t                unused2:3;                  // Bits 6-8
        uint16_t                ied:2;                      // Bits 9-10
        uint16_t                unused3:5;                  // Bits 11-15

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct air_platform_extended_equipment_bits_t
    {
        uint16_t                aux_power_plant2:1;         // Bit 0
        uint16_t                aux_power_plant3:1;         // Bit 1
        uint16_t                aux_power_plant4:1;         // Bit 2
        uint16_t                sling_load:2;               // Bits 3-4
        uint16_t                sling_loaded:1;             // Bit 5
        uint16_t                sling_damage:2;             // Bits 6-7
        uint16_t                sa_server:1;                // Bit 8
        uint16_t                ied:2;                      // Bits 9-10
        uint16_t                hoist:2;                    // Bits 11-12
        uint16_t                right_door_open:1;          // Bit 13
        uint16_t                left_door_open:1;           // Bit 14
        uint16_t                countermeasures_active:1;   // Bit 15

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct land_platform_extended_equipment_bits_t
    {
        uint16_t                spoiler:1;                  // Bit 0
        uint16_t                cargo_racks:1;              // Bit 1
        uint16_t                aux_power_plant:1;          // Bit 2
        uint16_t                emergency_lights:1;         // Bit 3
        uint16_t                unused1:1;                  // Bit 4
        uint16_t                sling_loaded:1;             // Bit 5
        uint16_t                crew_antenna:1;             // Bits 6
        uint16_t                unused2:1;                  // Bits 7
        uint16_t                sa_server:1;                // Bit 8
        uint16_t                ied:2;                      // Bits 9-10
        uint16_t                unused3:5;                  // Bits 11-15

        void print(const std::string &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    template<typename bits_t>
    union extended_equipment_t
    {
        uint16_t                value;
        bits_t                  bits;

        void clear(void)
        {
            value = 0;
        }
    };
}

#endif
