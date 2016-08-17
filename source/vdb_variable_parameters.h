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

#ifndef VDB_VARIABLE_PARAMETERS_H
#define VDB_VARIABLE_PARAMETERS_H

#include "vdb_ids.h"
#include "vdb_enums.h"
#include "vdb_object.h"
#include "vdb_spatial.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Base class for VPR contents.
    //
    class vpr_content_t : public record_t
    {
      public:

        vpr_content_t(void) { }
        vpr_content_t(const vpr_content_t &copy) { }
        virtual ~vpr_content_t(void) { }

        virtual uint32_t length(void) const { return LENGTH; }

        static const uint32_t
            LENGTH = 15; // bytes
    };

    // ------------------------------------------------------------------------
    // Encapsulates a single variable parameter record (VPR).
    //
    class vpr_t : public record_t
    {
      public:

        vpr_t(void);
        vpr_t(const vpr_t &copy);
        virtual ~vpr_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            type;
        vpr_content_t
            *content_ptr;

        static const uint32_t
            LENGTH = 16; // bytes
    };

    // ------------------------------------------------------------------------
    class vpr_default_content_t : public vpr_content_t
    {
      public:

        vpr_default_content_t(void);
        vpr_default_content_t(const vpr_default_content_t &copy);
        virtual ~vpr_default_content_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            data[LENGTH];
    };

    // ------------------------------------------------------------------------
    class articulated_part_t : public vpr_content_t
    {
      public:

        articulated_part_t(void);
        articulated_part_t(const articulated_part_t &copy);
        virtual ~articulated_part_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            change_indicator;
        uint16_t
            part_id;
        uint32_t
            part_type,
            part_metric;
        float32_t
            value;
        uint32_t
            padding;

        static const uint32_t
            MASK_5_BITS = 31; // 0x1F
    };

    // ------------------------------------------------------------------------
    class entity_association_t : public vpr_content_t
    {
      public:

        entity_association_t(void);
        entity_association_t(const entity_association_t &copy);
        virtual ~entity_association_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            change_indicator,
            association_status,
            association_type;
        id_t
            entity_id;
        uint16_t
            station;
        uint8_t
            connection,
            group_membership;
        uint16_t
            group_number;
    };

    // ------------------------------------------------------------------------
    class entity_offset_t : public vpr_content_t
    {
      public:

        entity_offset_t(void);
        entity_offset_t(const entity_offset_t &copy);
        virtual ~entity_offset_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            offset_type;
        uint16_t
            padding;
        location12_t
            position;
    };

    // ------------------------------------------------------------------------
    class ext_appearance_t : public vpr_content_t
    {
      public:

        ext_appearance_t(void);
        ext_appearance_t(const ext_appearance_t &copy);
        virtual ~ext_appearance_t(void);

        virtual void clear(void);

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            status;
        uint16_t
            equipment;
    };

    // ------------------------------------------------------------------------
    class ext_platform_appearance_t : public ext_appearance_t
    {
      public:

        ext_platform_appearance_t(void);
        ext_platform_appearance_t(const ext_platform_appearance_t &copy);
        virtual ~ext_platform_appearance_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            paint_scheme;
        uint16_t
            decal_scheme;
        uint8_t
            primary_condition,
            primary_color,
            secondary_condition,
            secondary_color;
        uint32_t
            lights;
        uint8_t
            thermal_indicators;
    };

    // ------------------------------------------------------------------------
    class ext_supply_appearance_t : public ext_appearance_t
    {
      public:

        ext_supply_appearance_t(void);
        ext_supply_appearance_t(const ext_supply_appearance_t &copy);
        virtual ~ext_supply_appearance_t(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    class ext_cultural_feature_appearance_t : public ext_appearance_t
    {
      public:

        ext_cultural_feature_appearance_t(void);
        ext_cultural_feature_appearance_t(const ext_cultural_feature_appearance_t &copy);
        virtual ~ext_cultural_feature_appearance_t(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Encapsulates a collection of variable parameter records.
    //
    class variable_parameter_records_t : public record_t
    {
      public:

        variable_parameter_records_t(void);
        variable_parameter_records_t(const variable_parameter_records_t &copy);
        virtual ~variable_parameter_records_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void read(byte_stream &stream, uint32_t);
        virtual void write(byte_stream &stream) const;

        std::vector<vpr_t>
            records;

        // --------------------------------------------------------------------
        // Print utility methods

        // The fields and values of some records depend on the entity kind
        // and domain.
        //
        static void use_kind(ent_kind_e value) { usable_kind = value; }
        static void use_domain(domain_e value) { usable_domain = value; }

        static void print_conditional_materials(
            const std::string &prefix,
            uint8_t value,
            std::ostream &stream
        );

        static void print_extended_equipment(
            const std::string &prefix,
            uint16_t value,
            std::ostream &stream
        );

        static void print_extended_lights(
            const std::string &prefix,
            uint32_t value,
            std::ostream &stream
        );

        static void print_extended_status(
            const std::string &prefix,
            uint8_t value,
            std::ostream &stream
        );

      private:

        static ent_kind_e
            usable_kind;
        static domain_e
            usable_domain;
    };
}

#endif
