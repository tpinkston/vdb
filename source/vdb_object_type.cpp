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

#include "vdb_enums.h"
#include "vdb_logger.h"
#include "vdb_object_type.h"

#include "vdb_byte_stream.h"
#include "vdb_string.h"

namespace
{
    const std::string
        description_undefined = "[description undefined]";
}

namespace vdb
{
    const uint32_t
        object_type_t::LENGTH = 4;
    std::map<uint32_t, std::string>
        object_type_data::descriptions;
    std::map<uint32_t, object_geometry_e>
        object_type_data::geometries;
    bool
        object_type_data::loaded = false;
}

// ----------------------------------------------------------------------------
vdb::object_type_t::object_type_t() :
    domain(0),
    kind(0),
    category(0),
    subcategory(0),
    description_ptr(0),
    geometry(OBJECT_GEOMETRY_UNKNOWN)
{

}

// ----------------------------------------------------------------------------
vdb::object_type_t::object_type_t(uint32_t value) :
    domain(0),
    kind(0),
    category(0),
    subcategory(0),
    description_ptr(0),
    geometry(OBJECT_GEOMETRY_UNKNOWN)
{
    set_value(value);
}

// ----------------------------------------------------------------------------
vdb::object_type_t::object_type_t(const object_type_t &copy) :
    domain(copy.domain),
    kind(copy.kind),
    category(copy.category),
    subcategory(copy.subcategory),
    description_ptr(copy.description_ptr),
    geometry(copy.geometry)
{

}

// ----------------------------------------------------------------------------
vdb::object_type_t::~object_type_t()
{
    // Do not delete 'description_ptr'

    clear();
}

// ----------------------------------------------------------------------------
bool vdb::object_type_t::is_null(void) const
{
    return (domain == 0) and
           (kind == 0) and
           (category == 0) and
           (subcategory == 0);
}

// ----------------------------------------------------------------------------
vdb::object_geometry_e vdb::object_type_t::get_geometry() const
{
    if (geometry == OBJECT_GEOMETRY_UNKNOWN)
    {
        geometry = object_type_data::get_geometry(get_value());
    }

    return geometry;
}

// ----------------------------------------------------------------------------
const std::string &vdb::object_type_t::get_description() const
{
    if (not description_ptr)
    {
        description_ptr = &(object_type_data::get_description(get_value()));
    }

    return *description_ptr;
}

// ----------------------------------------------------------------------------
void vdb::object_type_t::clear(void)
{
    domain = 0;
    kind = 0;
    category = 0;
    subcategory = 0;
    geometry = OBJECT_GEOMETRY_UNKNOWN;
    description_ptr = 0;
}

// ----------------------------------------------------------------------------
void vdb::object_type_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << (int)domain << "."
           << (int)kind << "."
           << (int)category << "."
           << (int)subcategory;
}

// ----------------------------------------------------------------------------
void vdb::object_type_t::read(byte_stream &stream)
{
    stream.read(domain);
    stream.read(kind);
    stream.read(category);
    stream.read(subcategory);
}

// ----------------------------------------------------------------------------
void vdb::object_type_t::write(byte_stream &stream) const
{
    stream.write(domain);
    stream.write(kind);
    stream.write(category);
    stream.write(subcategory);
}

// ----------------------------------------------------------------------------
bool vdb::object_type_t::operator==(const object_type_t &rhs) const
{
    return (domain == rhs.domain) and
           (kind == rhs.kind) and
           (category == rhs.category) and
           (subcategory == rhs.subcategory);
}

// ----------------------------------------------------------------------------
bool vdb::object_type_t::operator!=(const object_type_t &rhs) const
{
    return not (*this == rhs);
}

// ----------------------------------------------------------------------------
bool vdb::object_type_t::operator<(const object_type_t &rhs) const
{
    if (domain < rhs.domain)
    {
        return true;
    }
    else if (domain > rhs.domain)
    {
        return false;
    }

    if (kind < rhs.kind)
    {
        return true;
    }
    else if (kind > rhs.kind)
    {
         return false;
    }

    if (category < rhs.category)
    {
        return true;
    }
    else if (category > rhs.category)
    {
        return false;
    }

    if (subcategory < rhs.subcategory)
    {
        return true;
    }
    else if (subcategory > rhs.subcategory)
    {
        return false;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::object_type_t::operator>(const object_type_t &rhs) const
{
    return (*this != rhs) and not (*this < rhs);
}

// ----------------------------------------------------------------------------
void vdb::object_type_data::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE("Loaded %d objects types...", descriptions.size());
    }
}

// ----------------------------------------------------------------------------
void vdb::object_type_data::print(std::ostream &stream)
{
    std::map<uint32_t, std::string>::const_iterator
        description = descriptions.begin();
    std::map<uint32_t, object_geometry_e>::const_iterator
        geometry;
    object_type_t
        object_type;

    while(description != descriptions.end())
    {
        geometry = geometries.find(description->first);

        object_type.set_value(description->first);

        stream << object_type.str() << ",";

        if (geometry != geometries.end())
        {
            stream << (object_geometry_e)geometry->second;
        }
        else
        {
            stream << "unknown geometry";
        }

        stream << ",\"" << description->second << "\"" << std::endl;

        ++description;
    }
}

// ----------------------------------------------------------------------------
void vdb::object_type_data::convert_to_value(
    const uint8_t domain,
    const uint8_t kind,
    const uint8_t category,
    const uint8_t subcategory,
    uint32_t &value)
{
    value = 0;
    value = (uint8_t)(domain & 0xFF);
    value <<= 8;
    value |= (uint8_t)(kind & 0xFF);
    value <<= 8;
    value |= (uint8_t)(category & 0xFF);
    value <<= 8;
    value |= (uint8_t)(subcategory & 0xFF);
}

// ----------------------------------------------------------------------------
void vdb::object_type_data::convert_from_value(
    const uint32_t value,
    uint8_t &domain,
    uint8_t &kind,
    uint8_t &category,
    uint8_t &subcategory)
{
    uint32_t bits = value;

    subcategory = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    category = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    kind = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    domain = (uint8_t)(bits & 0xFF);
}

// ----------------------------------------------------------------------------
const std::string &vdb::object_type_data::get_description(uint32_t value)
{
    std::map<uint32_t, std::string>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        return search_itor->second;
    }

    return description_undefined;
}

// ----------------------------------------------------------------------------
vdb::object_geometry_e vdb::object_type_data::get_geometry(uint32_t value)
{
    std::map<uint32_t, object_geometry_e>::const_iterator
        search_itor = geometries.find(value);

    if (search_itor != geometries.end())
    {
        return search_itor->second;
    }

    return OBJECT_GEOMETRY_UNKNOWN;
}

// ----------------------------------------------------------------------------
void vdb::object_type_data::add(
    const int domain,
    const int kind,
    const int category,
    const int subcategory,
    const char *description_ptr,
    const char *geometry_ptr)
{
    uint32_t
        value;

    convert_to_value(
        domain,
        kind,
        category,
        subcategory,
        value);

    if (description_ptr)
    {
        descriptions[value] = description_ptr;
    }

    if (geometry_ptr)
    {
        std::string
            geometry = to_lowercase(geometry_ptr);

        if (geometry == "point")
        {
            geometries[value] = OBJECT_GEOMETRY_POINT;
        }
        else if (geometry == "linear")
        {
            geometries[value] = OBJECT_GEOMETRY_LINEAR;
        }
        else if (geometry == "areal")
        {
            geometries[value] = OBJECT_GEOMETRY_AREAL;
        }
        else
        {
            geometries[value] = OBJECT_GEOMETRY_UNKNOWN;
        }
    }
}
