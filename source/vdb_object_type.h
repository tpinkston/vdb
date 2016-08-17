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

#ifndef VDB_OBJECT_TYPE_H
#define VDB_OBJECT_TYPE_H

#include "vdb_command.h"
#include "vdb_enums.h"
#include "vdb_object.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class object_type_t : public record_t
    {
      public:

        object_type_t(void);
        object_type_t(uint32_t value);
        object_type_t(const object_type_t &copy);
        virtual ~object_type_t(void);

        bool is_null(void) const;

        uint32_t get_value(void) const;
        void set_value(uint32_t);

        object_geometry_e get_geometry(void) const;

        const std::string &get_description(void) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        virtual bool operator==(const object_type_t &) const;
        virtual bool operator!=(const object_type_t &) const;
        virtual bool operator<(const object_type_t &) const;
        virtual bool operator>(const object_type_t &) const;

        uint8_t
            domain,
            kind,
            category,
            subcategory;

      private:

        mutable const std::string
            *description_ptr;
        mutable object_geometry_e
            geometry;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class object_type_data : public command
    {
      public:

        object_type_data(void);
        virtual ~object_type_data(void);

        virtual int run(void);

        static void load(void);

        static void convert_to_value(
            const uint8_t domain,
            const uint8_t kind,
            const uint8_t category,
            const uint8_t subcategory,
            uint32_t &value
        );

        static void convert_from_value(
            const uint32_t value,
            uint8_t &domain,
            uint8_t &kind,
            uint8_t &category,
            uint8_t &subcategory
        );

        static const std::string &get_description(uint32_t value);
        static object_geometry_e get_geometry(uint32_t value);

      private:

        static void print(std::ostream &stream);

        static void add_all(void);

        static void add(
            const int domain,
            const int kind,
            const int category,
            const int subcategory,
            const char *description_ptr,
            const char *geometry_ptr
        );

        static std::map<uint32_t, std::string>
            descriptions;
        static std::map<uint32_t, object_geometry_e>
            geometries;
        static bool
            loaded;
    };
}

// ----------------------------------------------------------------------------
inline uint32_t vdb::object_type_t::get_value(void) const
{
    uint32_t
        value = 0;

    object_type_data::convert_to_value(
        domain,
        kind,
        category,
        subcategory,
        value);

    return value;
}

// ----------------------------------------------------------------------------
inline void vdb::object_type_t::set_value(uint32_t value)
{
    object_type_data::convert_from_value(
        value,
        domain,
        kind,
        category,
        subcategory);
}

// ----------------------------------------------------------------------------
inline vdb::object_type_data::object_type_data(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::object_type_data::~object_type_data(void)
{

}

// ----------------------------------------------------------------------------
inline int vdb::object_type_data::run(void)
{
    print(std::cout);

    return 0;
}

#endif
