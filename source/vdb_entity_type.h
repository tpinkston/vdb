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

#ifndef VDB_ENTITY_TYPE_H
#define VDB_ENTITY_TYPE_H

#include "vdb_enums.h"
#include "vdb_object.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class entity_type_t : public record_t
    {
      public:

        entity_type_t(void);
        entity_type_t(uint64_t value);
        entity_type_t(const uint16_t values[7]);
        entity_type_t(const entity_type_t &copy);
        virtual ~entity_type_t(void);

        bool valid(void) const;
        bool empty(void) const;

        ent_kind_e get_kind_e(void) const;
        domain_e get_domain_e(void) const;
        country_e get_country_e(void) const;

        uint8_t get_kind(void) const;
        uint8_t get_domain(void) const;
        uint16_t get_country(void) const;
        uint8_t get_category(void) const;
        uint8_t get_subcategory(void) const;
        uint8_t get_specific(void) const;
        uint8_t get_extra(void) const;

        void set_kind(uint8_t value);
        void set_domain(uint8_t value);
        void set_country(uint16_t value);
        void set_category(uint8_t value);
        void set_subcategory(uint8_t value);
        void set_specific(uint8_t value);
        void set_extra(uint8_t value);

        uint64_t get_value(void) const;
        void set_value(uint64_t);

        const std::string &get_name(void) const;
        const std::string &get_description(void) const;

        void get_values(uint16_t values[7]) const;

        void get_parent(entity_type_t &parent) const;
        bool get_valid_parent(entity_type_t &parent) const;

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        virtual bool operator==(const entity_type_t &) const;
        virtual bool operator!=(const entity_type_t &) const;
        virtual bool operator<(const entity_type_t &) const;
        virtual bool operator>(const entity_type_t &) const;

      private:

        uint8_t
            kind,
            domain;
        uint16_t
            country;
        uint8_t
            category,
            subcategory,
            specific,
            extra;
        mutable std::string
            name,
            description;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class entity_type_data
    {
      public:

        static void load(void);

        static void print(std::ostream &stream);

        static void convert_to_value(
            const uint8_t kind,
            const uint8_t domain,
            const uint16_t country,
            const uint8_t category,
            const uint8_t subcategory,
            const uint8_t specific,
            const uint8_t extra,
            uint64_t &value
        );

        static void convert_from_value(
            const uint64_t value,
            uint8_t &kind,
            uint8_t &domain,
            uint16_t &country,
            uint8_t &category,
            uint8_t &subcategory,
            uint8_t &specific,
            uint8_t &extra
        );

        static const std::string &get_name(uint64_t value);
        static const std::string &get_description(uint64_t value);

        static bool get_name(uint64_t value, std::string &name);
        static bool get_description(uint64_t value, std::string &description);

      private:

        static void add_all(void);

        static void add(
            const int kind,
            const int domain,
            const int country,
            const int category,
            const int subcategory,
            const int specific,
            const int extra,
            const char *name_ptr,
            const char *description_ptr
        );

        static std::map<uint64_t, std::string>
            names,
            descriptions;
        static bool
            loaded;
    };
}

// ----------------------------------------------------------------------------
inline uint64_t vdb::entity_type_t::get_value(void) const
{
    uint64_t
        value = 0;

    entity_type_data::convert_to_value(
        kind,
        domain,
        country,
        category,
        subcategory,
        specific,
        extra,
        value);

    return value;
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_value(uint64_t value)
{
    entity_type_data::convert_from_value(
        value,
        kind,
        domain,
        country,
        category,
        subcategory,
        specific,
        extra);

    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline vdb::ent_kind_e vdb::entity_type_t::get_kind_e(void) const
{
    return (ent_kind_e)kind;
}

// ----------------------------------------------------------------------------
inline vdb::domain_e vdb::entity_type_t::get_domain_e(void) const
{
    return (domain_e)domain;
}

// ----------------------------------------------------------------------------
inline vdb::country_e vdb::entity_type_t::get_country_e(void) const
{
    return (country_e)country;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_kind(void) const
{
    return kind;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_domain(void) const
{
    return domain;
}

// ----------------------------------------------------------------------------
inline uint16_t vdb::entity_type_t::get_country(void) const
{
    return country;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_category(void) const
{
    return category;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_subcategory(void) const
{
    return subcategory;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_specific(void) const
{
    return specific;
}

// ----------------------------------------------------------------------------
inline uint8_t vdb::entity_type_t::get_extra(void) const
{
    return extra;
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_kind(uint8_t value)
{
    kind = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_domain(uint8_t value)
{
    domain = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_country(uint16_t value)
{
    country = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_category(uint8_t value)
{
    category = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_subcategory(uint8_t value)
{
    subcategory = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_specific(uint8_t value)
{
    specific = value;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
inline void vdb::entity_type_t::set_extra(uint8_t value)
{
    extra = value;
    name.clear();
    description.clear();
}

#endif
