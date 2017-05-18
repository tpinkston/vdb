#ifndef VDIS_VARIABLE_PARAMETER_RECORDS_H
#define VDIS_VARIABLE_PARAMETER_RECORDS_H

#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_enums.h"

#define VP_RECORD_SIZE 16
#define VP_RECORD_DATA_SIZE 15

namespace vdis
{
    struct variable_parameter_record_t;

    // Returns array with length equal to 'count'
    //
    variable_parameter_record_t **read_variable_parameter_records(
        byte_stream_t &stream,
        uint32_t count
    );

    // ------------------------------------------------------------------------
    struct variable_parameter_content_t
    {
        virtual ~variable_parameter_content_t(void) { }

        virtual vp_record_type_e type(void) const = 0;

        virtual void print(const string_t &, std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;

        static void using_type(const entity_type_t *type_ptr);
    };

    // ------------------------------------------------------------------------
    struct variable_parameter_record_t
    {
        variable_parameter_content_t    *content_ptr;        // 15 bytes

        variable_parameter_record_t(void) : content_ptr(0) { }
        ~variable_parameter_record_t(void);

        inline vp_record_type_e type(void) const
        {
            return content_ptr ? content_ptr->type() : VP_RECORD_TYPE_END;
        }

        void clear(void);
        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct default_variable_content_t : variable_parameter_content_t
    {
        uint8_t                 record_type;                // 1 byte
        uint8_t                 buffer[VP_RECORD_DATA_SIZE];

        default_variable_content_t(uint8_t type);

        inline vp_record_type_e type(void) const
        {
            return (vp_record_type_e)record_type;
        }

        inline void clear(void)
        {
            record_type = (uint8_t)VP_RECORD_TYPE_END;
            std::memset(buffer, 0, VP_RECORD_DATA_SIZE);
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct articulated_part_t : variable_parameter_content_t
    {
        uint8_t                 change_indicator;       // 1 byte
        uint16_t                part_id;                // 2 bytes
        uint32_t                type_metric;            // 4 bytes
        float32_t               value;                  // 4 bytes
        uint32_t                padding;                // 4 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_ARTICULATED_PART;
        }

        inline articulated_parts_e part_type_enum(void) const
        {
            return (articulated_parts_e)(type_metric & ~0x1F);
        }

        inline articulated_parts_metric_e part_metric_enum(void) const
        {
            return (articulated_parts_metric_e)(type_metric & 0x1F);
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_association_t : variable_parameter_content_t
    {
        uint8_t                 change_indicator;       // 1 byte
        uint8_t                 association_status;     // 1 byte
        uint8_t                 association_type;       // 1 byte
        id_t                    entity_id;              // 6 bytes
        uint16_t                station;                // 2 bytes
        uint8_t                 connection_type;        // 1 byte
        uint8_t                 group_membership;       // 1 byte
        uint16_t                group_number;           // 2 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_ENTITY_ASSOC;
        }

        inline ent_assoc_status_e association_status_enum(void) const
        {
            return (ent_assoc_status_e)association_status;
        }

        inline phys_assoc_type_e association_type_enum(void) const
        {
            return (phys_assoc_type_e)association_type;
        }

        inline phys_conn_type_e connection_type_enum(void) const
        {
            return (phys_conn_type_e)connection_type;
        }

        inline grp_mem_type_e membership_enum(void) const
        {
            return (grp_mem_type_e)group_membership;
        }

        inline bool operator==(const entity_association_t &other) const
        {
            if (change_indicator != other.change_indicator)     return false;
            if (association_status != other.association_status) return false;
            if (association_type != other.association_type)     return false;
            if (entity_id != other.entity_id)                   return false;
            if (station != other.station)                       return false;
            if (connection_type != other.connection_type)       return false;
            if (group_membership != other.group_membership)     return false;
            if (group_number != other.group_number)             return false;
            return true;
        }

        inline bool operator!=(const entity_association_t &other) const
        {
            return (not (*this == other));
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_offset_t : variable_parameter_content_t
    {
        uint8_t                 offset_type;            // 1 byte
        uint16_t                padding;                // 2 bytes
        location12_t            position;               // 12 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_ENTITY_OFFSET;
        }

        inline offset_type_e offset_type_enum(void) const
        {
            return (offset_type_e)offset_type;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct high_fidelity_lights_t : variable_parameter_content_t
    {
        hifi_light_flags_t      flags;                  // 1 byte
        uint16_t                light_type;             // 2 bytes
        uint8_t                 part_id;                // 1 byte
        uint8_t                 padding;                // 1 byte
        uint8_t                 color;                  // 1 byte
        uint8_t                 beam_half_angle;        // 1 byte
        uint16_t                range_attenuation;      // 2 bytes
        uint16_t                intensity;              // 2 bytes
        uint8_t                 flash_rate;             // 1 byte
        uint8_t                 infrared_index;         // 1 byte
        uint8_t                 thermal_index;          // 1 byte
        uint8_t                 feather;                // 1 byte

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_HIFI_LIGHTS;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct legacy_extended_lifeform_appearance_t : variable_parameter_content_t
    {
        uint8_t                 clothing_scheme;        // 1 byte
        uint8_t                 primary_color;          // 1 byte
        uint8_t                 secondary_color;        // 1 byte
        uint32_t                equipment;              // 4 bytes
        uint8_t                 status;                 // 1 byte
        uint8_t                 unused1;                // 1 byte
        uint16_t                attributes;             // 2 bytes
        uint32_t                unused2;                // 4 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_EXT_LIFEFORM_APP;
        }

        inline lf_cloth_scheme_e cloth_scheme_enum(void) const
        {
            return (lf_cloth_scheme_e)clothing_scheme;
        }

        inline colors_e color_primary_enum(void) const
        {
            return (colors_e)primary_color;
        }

        inline colors_e color_secondary_enum(void) const
        {
            return (colors_e)secondary_color;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_appearance_t : variable_parameter_content_t
    {
        uint8_t                 status;                 // 1 byte
        uint16_t                equipment;              // 2 bytes

        void print(const string_t &, std::ostream &) const;
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

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_EXT_PLATFORM_APP;
        }

        inline pl_paint_scheme_e paint_scheme_enum(void) const
        {
            return (pl_paint_scheme_e)paint;
        }

        inline pl_decal_scheme_e decal_scheme_enum(void) const
        {
            return (pl_decal_scheme_e)decal;
        }

        inline colors_e color_primary_enum(void) const
        {
            return (colors_e)primary_color;
        }

        inline colors_e color_secondary_enum(void) const
        {
            return (colors_e)secondary_color;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_supply_appearance_t : extended_appearance_t
    {
        uint8_t                 padding[12];            // 12 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_EXT_SUPPLY_APP;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct extended_cultural_feature_appearance_t : extended_appearance_t
    {
        uint8_t                 padding[12];            // 12 bytes

        inline vp_record_type_e type(void) const
        {
            return VP_RECORD_TYPE_EXT_CULT_FEAT_APP;
        }

        void print(const string_t &, std::ostream &) const;
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

        inline present_domain_e present_domain_enum(void) const
        {
            return (present_domain_e)bits.present_domain;
        }

        inline disguise_status_e disguise_status_enum(void) const
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

        void print(const string_t &, std::ostream &) const;
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

        inline severity_e cleanliness_enum(void) const
        {
            return (severity_e)bits.cleanliness;
        }

        inline severity_e damage_enum(void) const
        {
            return (severity_e)bits.damage;
        }

        inline pl_cond_mtl_e material_enum(void) const
        {
            return (pl_cond_mtl_e)bits.material;
        }

        inline severity_e rust_enum(void) const
        {
            return (severity_e)bits.rust;
        }

        void print(const string_t &, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct extended_equipment_bits_t
    {
        uint16_t                unused1:5;                  // Bits 0-4
        uint16_t                sling_loaded:1;             // Bit 5
        uint16_t                unused2:3;                  // Bits 6-8
        uint16_t                ied:2;                      // Bits 9-10
        uint16_t                unused3:5;                  // Bits 11-15

        void print(const string_t &, std::ostream &) const;
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

        void print(const string_t &, std::ostream &) const;
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

        void print(const string_t &, std::ostream &) const;
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
